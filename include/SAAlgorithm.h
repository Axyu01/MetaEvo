#ifndef SAALGORITHM_H
#define SAALGORITHM_H

#include "Solution.h"
#include "CVRProblem.h"
#include "SolutionsLogger.h"
#include <random>
#include <string>

class SAAlgorithm
{
public:
    SAAlgorithm(CVRProblem& problem);
    virtual ~SAAlgorithm();

    CVRProblem* problem;
    Solution* solution;
    Solution* bestSolution;

    // === SA Parameters ===
    double startTemp = 1000.0;
    double minTemp = 1e-3;
    double alpha = 0.995;
    int iterationsPerTemp = 1000;
    int maxNoImprove = 2000;

    // === RNG ===
    std::mt19937 rng;
    std::uniform_real_distribution<double> uniform;

    // === Main methods ===
    void Init();
    void Iterate();
    void IterateWithLogging(SolutionsLogger& logger, int logInterval = 1000);

private:
    // === Helper methods ===
    Solution* Neighbor(Solution* s);       // generates neighborhood (with swap)
    double AcceptanceProbability(double oldVal, double newVal, double T);
    void CopySolution(Solution*& dest, const Solution* src);
    void PrintStatus(double T, int iter, double bestVal)
};

#endif // SAALGORITHM_H
