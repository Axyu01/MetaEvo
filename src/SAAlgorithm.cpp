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

    // Prosty operator SWAP
    int i = rng() % n;
    int j = rng() % n;
    std::swap(neighbor->Representation[i], neighbor->Representation[j]);

    neighbor->Value = problem->EstimateSolution(*neighbor);
    return neighbor;
}

double SAAlgorithm::AcceptanceProbability(double oldVal, double newVal, double T)
{
    if (newVal < oldVal)
        return 1.0; // zawsze akceptuj lepsze
    return std::exp((oldVal - newVal) / T);
}

void SAAlgorithm::IterateWithLogging(SolutionsLogger& logger, int logInterval)
{
    if (!solution)
        Init();

    std::vector<Solution*> logBuffer(1);
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

            // Akceptacja kandydata
            if (candidate->Value < solution->Value || uniform(rng) < ap)
            {
                CopySolution(solution, candidate);
            }

            // Aktualizacja najlepszego
            if (candidate->Value < bestSolution->Value)
            {
                CopySolution(bestSolution, candidate);
                noImprove = 0;
            }
            else
            {
                noImprove++;
            }

            delete candidate;
            iteration++;

            // logowanie co n iteracji
            if (iteration % logInterval == 0)
            {
                logBuffer[0] = solution;
                logger.Log(logBuffer, std::to_string(iteration));
            }

            if (iteration % 50000 == 0)
                PrintStatus(T, iteration, bestSolution->Value);
        }

        T *= alpha; // schładzanie
    }

    std::cout << "[SA] Finished. Best value: " << bestSolution->Value << std::endl;
}

void SAAlgorithm::CopySolution(Solution*& dest, const Solution* src)
{
    if (dest)
        delete dest;
    dest = new Solution(*src);
}

void SAAlgorithm::PrintStatus(double T, int iter, double bestVal)
{
    std::cout << "[SA] Iter: " << iter
              << " | Temp: " << T
              << " | Best: " << bestVal
              << std::endl;
}
