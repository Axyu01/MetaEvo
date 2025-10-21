#ifndef TESTS_H
#define TESTS_H
#include <string>
#include "CVRProblem.h"


class Tests
{
    public:
        //Tests();
        //virtual ~Tests();
        static void TestRandom();
        static void TestRandom(CVRProblem& problem,std::string dir);
        static void TestGready();
        static void TestGready(CVRProblem& problem,std::string dir);

    protected:

    private:
};

#endif // TESTS_H
