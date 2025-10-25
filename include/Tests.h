#ifndef TESTS_H
#define TESTS_H
#include <string>
#include "CVRProblem.h"
#include <Population.h>


class Tests
{
    public:
        //Tests();
        //virtual ~Tests();
        static void TestRandom();
        static void TestRandom(CVRProblem& problem,std::string dir);
        static void TestGready();
        static void TestGready(CVRProblem& problem,std::string dir);
        static void TestEvo();
        static void TestEvo(CVRProblem& problem,std::string dir,PopParameters popParams,int iterations);
        static CVRProblem* _problem;

    protected:

    private:
        //Solution* _randSol();
        //double _estimate(Solution& s);
};

#endif // TESTS_H
