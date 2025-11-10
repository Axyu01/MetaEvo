#include "SAAlgorithm.h"
#include <cmath>
#include <iostream>
#include <algorithm>

SAAlgorithm::SAAlgorithm(CVRProblem& problem)
    : problem(&problem)
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
    int size = s->Size;
    Solution* neighbor = new Solution(*s);

    int i = rand() % size;
    int j = rand() % size;

    //swap
    int temp = neighbor->Representation[i];
    neighbor->Representation[i] = neighbor->Representation[j];
    neighbor->Representation[j] = temp;

    neighbor->Value = problem->EstimateSolution(*neighbor);
    return neighbor;
}

double SAAlgorithm::AcceptanceProbability(double oldVal, double newVal, double T)
{
    if (newVal < oldVal)
        return 1.0; // zawsze akceptuj lepsze
    return 1/(1+std::exp((newVal - oldVal) / T));
}

void SAAlgorithm::IterateWithLogging(SolutionsLogger& logger, int logInterval)
{
    if (!solution)
        Init();

    std::vector<Solution*> logBuffer(1);
    double T = startTemp;
    int iteration = 0;

    while (T > minTemp)
    {
        for (int i = 0; i < iterationsPerTemp; i++)
        {
            Solution* candidates[n];
            Solution* bestCandidate = nullptr;
            for(int s = 0;s<n;s++)
            {
                Solution* candidate = Neighbor(solution);
                candidates[s] = candidate;

                if(bestCandidate == nullptr || candidate->Value < bestCandidate->Value)
                    bestCandidate = candidate;
            }

            double ap = AcceptanceProbability(solution->Value, bestCandidate->Value, T);
            // Accept
            if (bestCandidate->Value < solution->Value || (double)rand()/RAND_MAX < ap)
            {
                if(solution)
                    delete solution;
                solution = new Solution(*bestCandidate);
            }
            // Update best
            if (bestCandidate->Value < bestSolution->Value)
            {
                if(bestSolution)
                    delete bestSolution;
                bestSolution = new Solution(*bestCandidate);
            }
            for(int s = 0;s<n;s++)
            {
                delete candidates[s];
            }
            iteration++;

            // logowanie co n iteracji
            if (iteration % logInterval == 0)
            {
                logBuffer[0] = solution;
                logger.Log(logBuffer, std::to_string(iteration));
            }
        }

        T *= alpha; // cooling
    }
    logBuffer[0] = bestSolution;
    logger.Log(logBuffer, std::to_string(++iteration));
}

void SAAlgorithm::PrintStatus(double T, int iter, double bestVal)
{
    std::cout << "[SA] Iter: " << iter
              << " | Temp: " << T
              << " | Best: " << bestVal
              << std::endl;
}
