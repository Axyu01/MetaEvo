#include "Tests.h"
#include "CVRProblem.h"
#include "SolutionsLogger.h"
#include "stdlib.h"
#include "iostream"
#include "GreadySolution.h"
#include <string>
#include "EvoAlgorithm.h"
#include "CrossOps.h"
#include "MutationOps.h"
#include "vector"

// =====================================================
// Definition of static member (required for linking)
// =====================================================
CVRProblem* Tests::_problem = nullptr;

// =====================================================
// Helper functions
// =====================================================

Solution* _randSol()
{
    int dim = Tests::_problem->GetDimension();
    Solution* s = new Solution(dim - 1);
    s->RandomizeCVRP(dim);
    return s;
}

double _estimate(Solution& s)
{
    return Tests::_problem->EstimateSolution(s);
}

// =====================================================
// Random tests
// =====================================================

void Tests::TestRandom()
{
    CVRProblem problem1("CVRP_files/A-n32-k5.vrp");
    CVRProblem problem2("CVRP_files/A-n37-k6.vrp");
    CVRProblem problem3("CVRP_files/A-n39-k5.vrp");
    CVRProblem problem4("CVRP_files/A-n45-k6.vrp");
    CVRProblem problem5("CVRP_files/A-n48-k7.vrp");
    CVRProblem problem6("CVRP_files/A-n54-k7.vrp");
    CVRProblem problem7("CVRP_files/A-n60-k9.vrp");

    TestRandom(problem1, "out/CVRP_files/random/A-n32-k5");
    TestRandom(problem2, "out/CVRP_files/random/A-n37-k6");
    TestRandom(problem3, "out/CVRP_files/random/A-n39-k5");
    TestRandom(problem4, "out/CVRP_files/random/A-n45-k6");
    TestRandom(problem5, "out/CVRP_files/random/A-n48-k7");
    TestRandom(problem6, "out/CVRP_files/random/A-n54-k7");
    TestRandom(problem7, "out/CVRP_files/random/A-n60-k9");
}

void Tests::TestRandom(CVRProblem& problem, std::string dir)
{
    //SolutionsLogger bestLogger(dir + "best.csv");
    //SolutionsLogger worstLogger(dir + "worst.csv");
    SolutionsLogger logger(dir + ".csv");
    std::vector<Solution*> forLogger;
    forLogger.push_back(nullptr);

    std::cout << "Random test for: " << dir << std::endl;

    int dimension = problem.GetDimension();
    Solution* best = nullptr;
    Solution* worst = nullptr;
    Solution s(dimension);
    double sum = 0;

    for (int i = 0; i < 10000; i++)
    {
        s.RandomizeCVRP(dimension);
        s.Value = problem.EstimateSolution(s);
        sum += s.Value;

        if (best == nullptr || best->Value > s.Value)
        {
            delete best;
            best = new Solution(s);
        }
        if (worst == nullptr || worst->Value < s.Value)
        {
            delete worst;
            worst = new Solution(s);
        }

        forLogger[0] = &s;
        logger.Log(forLogger,"");
    }

    best->Print("Best");
    worst->Print("Worst");
    std::cout << "Average: " << sum / 10000 << std::endl;

    delete best;
    delete worst;
}

// =====================================================
// Greedy tests
// =====================================================

void Tests::TestGready()
{
    CVRProblem problem1("CVRP_files//A-n32-k5.vrp");
    CVRProblem problem2("CVRP_files/A-n37-k6.vrp");
    CVRProblem problem3("CVRP_files/A-n39-k5.vrp");
    CVRProblem problem4("CVRP_files/A-n45-k6.vrp");
    CVRProblem problem5("CVRP_files/A-n48-k7.vrp");
    CVRProblem problem6("CVRP_files/A-n54-k7.vrp");
    CVRProblem problem7("CVRP_files/A-n60-k9.vrp");

    TestGready(problem1, "out/CVRP_files/gready/A-n32-k5");
    TestGready(problem2, "out/CVRP_files/gready/A-n37-k6");
    TestGready(problem3, "out/CVRP_files/gready/A-n39-k5");
    TestGready(problem4, "out/CVRP_files/gready/A-n45-k6");
    TestGready(problem5, "out/CVRP_files/gready/A-n48-k7");
    TestGready(problem6, "out/CVRP_files/gready/A-n54-k7");
    TestGready(problem7, "out/CVRP_files/gready/A-n60-k9");
}

void Tests::TestGready(CVRProblem& problem, std::string dir)
{
    std::cout << "Greedy test for: " << dir << std::endl;
    SolutionsLogger logger(dir + ".csv");
    std::vector<Solution*> forLogger;
    forLogger.push_back(nullptr);

    int dimension = problem.GetDimension();
    Solution* best = nullptr;
    Solution* worst = nullptr;
    double sum = 0;

    for (int i = 1; i < dimension; i++)
    {
        GreadySolution s(problem, i);
        s.Value = problem.EstimateSolution(s);
        sum += s.Value;
        forLogger[0] = &s;
        logger.Log(forLogger,"");

        if (best == nullptr || best->Value > s.Value)
        {
            delete best;
            best = new Solution(s);
        }
        if (worst == nullptr || worst->Value < s.Value)
        {
            delete worst;
            worst = new Solution(s);
        }
    }

    best->Print("Best");
    worst->Print("Worst");
    std::cout << "Average: " << sum / (dimension - 1) << std::endl;

    delete best;
    delete worst;
}

// =====================================================
// Evolutionary algorithm tests
// =====================================================

void Tests::TestEvo()
{
    CVRProblem problem1("CVRP_files/A-n32-k5.vrp");
    CVRProblem problem2("CVRP_files/A-n37-k6.vrp");
    CVRProblem problem3("CVRP_files/A-n39-k5.vrp");
    CVRProblem problem4("CVRP_files/A-n45-k6.vrp");
    CVRProblem problem5("CVRP_files/A-n48-k7.vrp");
    CVRProblem problem6("CVRP_files/A-n54-k7.vrp");
    CVRProblem problem7("CVRP_files/A-n60-k9.vrp");

    PopParameters params;
    params.crossOperator = CrossOps::CycleCrossover;
    params.mutationOperator = MutationOps::Inverse;
    params.mutation_chance = 0.3;
    params.crossover_chance = 1.0;
    params.init_population_size = 500;
    params.selection_population_size = 100;
    params.randSolutionCreator = _randSol;
    params.evaluator = _estimate;

    TestEvo(problem1, "out/CVRP_files/evo/A-n32-k5", params, 100);
    TestEvo(problem2, "out/CVRP_files/evo/A-n37-k6", params, 100);
    TestEvo(problem3, "out/CVRP_files/evo/A-n39-k5", params, 100);
    TestEvo(problem4, "out/CVRP_files/evo/A-n45-k6", params, 100);
    TestEvo(problem5, "out/CVRP_files/evo/A-n48-k7", params, 100);
    TestEvo(problem6, "out/CVRP_files/evo/A-n54-k7", params, 100);
    TestEvo(problem7, "out/CVRP_files/evo/A-n60-k9", params, 100);
}

void Tests::TestEvo(CVRProblem& problem, std::string dir, PopParameters popParams, int iterations)
{
    std::cout << "Evolutionary test for: " << dir << std::endl;

    // ✅ Ensure the static problem pointer is set correctly
    _problem = &problem;

    for(int trial = 0;trial<10;trial++)
    {
        SolutionsLogger logger(dir + "_trial_" + std::to_string(trial+1)+".csv");
        EvoAlgorithm evo(popParams);
        evo.Start();

        for (int i = 0; i < iterations; i++)
        {
            evo.Loop();
            logger.Log(evo._population->Solutions,std::to_string(i));
        }

        evo._population->Sort();
        evo._population->Solutions[0]->Print("Best Evolutionary Solution");
    }
}
