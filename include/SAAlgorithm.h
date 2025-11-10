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

    double startTemp = 1000.0;
    double minTemp = 1e-3;
    double alpha = 0.995;
    int n = 1;
    int iterationsPerTemp = 1000;

    void Init();
    void Iterate();
    void IterateWithLogging(SolutionsLogger& logger, int logInterval = 1000);

private:
    Solution* Neighbor(Solution* s);
    double AcceptanceProbability(double oldVal, double newVal, double T);
    void PrintStatus(double T, int iter, double bestVal);
};

#endif // SAALGORITHM_H
