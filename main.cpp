#include <iostream>
#include <CVRProblem.h>
#include <GreadySolution.h>
#include <Solution.h>
#include <cstdlib>
#include <time.h>
#include <EvoAlgorithm.h>
#include <Population.h>
#include <CrossOps.h>
#include <MutationOps.h>
#include <SolutionsLogger.h>
#include <vector>
#include <Tests.h>

using namespace std;
CVRProblem* problem;
Solution* randSol()
{
    int dim = problem->GetDimension();
    Solution* s = new Solution(dim-1);
    //Solution* s = new Solution((dim-1)*2-1);
    s->RandomizeCVRP(dim);
    return s;
}
double estimate(Solution& s)
{
    return problem->EstimateSolution(s);
}
int main()
{
    //auto SEED = 1760195673;//
    auto SEED = time(NULL);
    srand(SEED);
    cout << "SEED:"<<SEED << endl;
    problem = new CVRProblem("CVRP_files/A-n60-k9.vrp");

    //problem->LoadFromFile("CVRP_files/toy.vrp");
    //problem->Print();
    //int optimalSolution[] = {1,4,0,3,2,5};// for toy.vrp

    Solution* randomSolution = randSol();
    randomSolution->Value = problem->EstimateSolution(*randomSolution);
    randomSolution->Print("Random Solution");

    GreadySolution* greadySolution = new GreadySolution(*problem);
    greadySolution->Print("Gready Solution");
    /*
    PopParameters params;
    params.crossOperator = CrossOps::CycleCrossover;
    params.mutationOperator = MutationOps::Inverse;
    params.mutation_chance = 0.3;
    params.crossover_chance = 1;
    params.init_population_size = 2000;
    params.selection_population_size = 500;
    params.randSolutionCreator = randSol;
    params.evaluator = estimate;
    EvoAlgorithm* alg = new EvoAlgorithm(params);
    Population* population = alg->_population;
    alg->Start();
    for(int i=0;i<100;i++)
    {
        alg->Loop();
    }
    population->Sort();
    population->Solutions[0]->Print("Best Evo");


    SolutionsLogger* logger = new SolutionsLogger("test.csv");
    logger->Log(population->Solutions,"end population");
    delete alg;
    delete logger;
    */
    Tests::TestSA();
    //Tests::TestRandom();
    //Tests::TestGready();
    //Tests::TestEvo();


    return 0;
}
