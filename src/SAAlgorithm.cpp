#include "SAAlgorithm.h"
#include <cmath>
#include <iostream>
#include <algorithm>

SAAlgorithm::SAAlgorithm(CVRProblem& problem)
    : problem(&problem), rng(std::random_device{}()), uniform(0.0, 1.0)
{
    solution = nullptr;
    bestSolution = nullptr;
}

SAAlgorithm::~SAAlgorithm()
{
    delete solution;
    delete bestSolution;
}

void SAAlgorithm::Init()
{
    int dim = problem->GetDimension();
    solution = new Solution(dim - 1);
    solution->RandomizeCVRP(dim);
    solution->Value = problem->EstimateSolution(*solution);

    bestSolution = new Solution(*solution);

    std::cout << "[SA] Initialized with value: " << solution->Value << std::endl;
}

Solution* SAAlgorithm::Neighbor(Solution* s)
{
    int n = s->Size;
    Solution* neighbor = new Solution(*s);

    // === prosty operator SWAP dla sąsiedztwa ===
    int i = rng() % n;
    int j = rng() % n;
    std::swap(neighbor->Representation[i], neighbor->Representation[j]);

    neighbor->Value = problem->EstimateSolution(*neighbor);
    return neighbor;
}

double SAAlgorithm::AcceptanceProbability(double oldVal, double newVal, double T)
{
    if (newVal < oldVal)
        return 1.0; // lepsze rozwiązanie – zawsze akceptujemy
    return std::exp((oldVal - newVal) / T); // gorsze – z pewnym prawdopodobieństwem
}

void SAAlgorithm::Iterate()
{
    if (!solution)
        Init();

    double T = startTemp;
    int noImprove = 0;
    int iteration = 0;

    std::cout << "[SA] Starting iterations..." << std::endl;

    while (T > minTemp && noImprove < maxNoImprove)
    {
        for (int i = 0; i < iterationsPerTemp; i++)
        {
            Solution* candidate = Neighbor(solution);
            double ap = AcceptanceProbability(solution->Value, candidate->Value, T);

            if (candidate->Value < solution->Value || uniform(rng) < ap)
            {
                // Akceptujemy nowego sąsiada
                delete solution;
                solution = new Solution(*candidate);
            }

            // Aktualizacja najlepszego rozwiązania
            if (candidate->Value < bestSolution->Value)
            {
                delete bestSolution;
                bestSolution = new Solution(*candidate);
                noImprove = 0;
            }
            else
            {
                noImprove++;
            }

            delete candidate;
            iteration++;

            if (iteration % 100000 == 0)
                PrintStatus(T, iteration, bestSolution->Value);
        }

        // Schładzanie
        T *= alpha;
    }

    std::cout << "[SA] Finished. Best value: " << bestSolution->Value << std::endl;
}

void SAAlgorithm::LoopUntil(double minTemperature)
{
    startTemp = 1000.0;
    minTemp = minTemperature;
    Iterate();
}

void SAAlgorithm::CopySolution(Solution*& dest, const Solution* src)
{
    if (dest)
        delete dest;
    dest = new Solution(*src);
}

void SAAlgorithm::PrintStatus(double T, int iter, double bestVal)
{
    std::cout << "[SA] Iteration: " << iter
              << " | Temp: " << T
              << " | Best: " << bestVal
              << std::endl;
}
