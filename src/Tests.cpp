#include "Tests.h"
#include "CVRProblem.h"
#include "SolutionsLogger.h"
#include "stdlib.h"
#include "iostream"
#include "GreadySolution.h"
#include <string>
//Tests
void Tests::TestRandom()
{
    CVRProblem problem1("CVRP_files/A-n32-k5.vrp");
    CVRProblem problem2("CVRP_files/A-n37-k6.vrp");
    CVRProblem problem3("CVRP_files/A-n39-k5.vrp");
    CVRProblem problem4("CVRP_files/A-n45-k6.vrp");
    CVRProblem problem5("CVRP_files/A-n48-k7.vrp");
    CVRProblem problem6("CVRP_files/A-n54-k7.vrp");
    CVRProblem problem7("CVRP_files/A-n60-k9.vrp");

    TestRandom(problem1, "out/CVRP_files/A-n32-k5");
    TestRandom(problem2, "out/CVRP_files/A-n37-k6");
    TestRandom(problem3, "out/CVRP_files/A-n39-k5");
    TestRandom(problem4, "out/CVRP_files/A-n45-k6");
    TestRandom(problem5, "out/CVRP_files/A-n48-k7");
    TestRandom(problem6, "out/CVRP_files/A-n54-k7");
    TestRandom(problem7, "out/CVRP_files/A-n60-k9");
}
void Tests::TestRandom(CVRProblem& problem,std::string dir)
{
    std::cout << "random test for:" << dir << std::endl;

    int dimension = problem.GetDimension();
    //SolutionsLogger bestLogger(dir+"/best.csv");
    //SolutionsLogger worstLogger(dir+"/worst.csv");
    //SolutionsLogger allLogger(dir+"/all.csv");
    //Solution s(dimension*2-1);
    Solution* best = nullptr;
    Solution* worst = nullptr;
    Solution s(dimension);
    double sum = 0;
    for(int i=0;i<10000;i++)
    {
        s.RandomizeCVRP(dimension);
        s.Value = problem.EstimateSolution(s);
        sum+= s.Value;
        if(best == nullptr)
            best = new Solution(s);
        else if(best->Value > s.Value)
        {
            delete best;
            best = new Solution(s);
        }
        if(worst == nullptr)
            worst = new Solution(s);
        else if(worst->Value < s.Value)
        {
            delete worst;
            worst = new Solution(s);
        }
    }
    best->Print("Best");
    worst->Print("Worst");
    std::cout<<"Avarage:"<<sum/10000<<std::endl;
    delete best;
    delete worst;
}
void Tests::TestGready()
{
    std::cout<< "XDDDD"<<std::to_string(12345)<<std::endl;
    CVRProblem problem1("CVRP_files/A-n32-k5.vrp");
    CVRProblem problem2("CVRP_files/A-n37-k6.vrp");
    CVRProblem problem3("CVRP_files/A-n39-k5.vrp");
    CVRProblem problem4("CVRP_files/A-n45-k6.vrp");
    CVRProblem problem5("CVRP_files/A-n48-k7.vrp");
    CVRProblem problem6("CVRP_files/A-n54-k7.vrp");
    CVRProblem problem7("CVRP_files/A-n60-k9.vrp");

    TestGready(problem1, "out/CVRP_files/A-n32-k5");
    TestGready(problem2, "out/CVRP_files/A-n37-k6");
    TestGready(problem3, "out/CVRP_files/A-n39-k5");
    TestGready(problem4, "out/CVRP_files/A-n45-k6");
    TestGready(problem5, "out/CVRP_files/A-n48-k7");
    TestGready(problem6, "out/CVRP_files/A-n54-k7");
    TestGready(problem7, "out/CVRP_files/A-n60-k9");
}
void Tests::TestGready(CVRProblem& problem,std::string dir)
{
    std::cout << "gready test for:" << dir << std::endl;

    int dimension = problem.GetDimension();
    //SolutionsLogger bestLogger(dir+"/best.csv");
    //SolutionsLogger worstLogger(dir+"/worst.csv");
    //SolutionsLogger allLogger(dir+"/all.csv");
    //Solution s(dimension*2-1);
    Solution* best = nullptr;
    Solution* worst = nullptr;
    double sum = 0;
    for(int i=1;i<dimension;i++)
    {
        GreadySolution s(problem,i);
        s.Value = problem.EstimateSolution(s);
        sum+= s.Value;
        if(best == nullptr)
            best = new Solution(s);
        else if(best->Value > s.Value)
        {
            delete best;
            best = new Solution(s);
        }
        if(worst == nullptr)
            worst = new Solution(s);
        else if(worst->Value < s.Value)
        {
            delete worst;
            worst = new Solution(s);
        }
    }
    best->Print("Best");
    worst->Print("Worst");
    std::cout<<"Avarage:"<<sum/dimension-1<<std::endl;
    delete best;
    delete worst;
}
