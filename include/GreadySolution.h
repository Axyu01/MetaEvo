#ifndef GREADYSOLUTION_H
#define GREADYSOLUTION_H
#include <CVRProblem.h>
#include<Solution.h>


class GreadySolution : public Solution
{
    public:
        //static int* Solve(CVRProblem& problem);
        //static int* Solve(CVRProblem& problem,int firstCity);
        GreadySolution(CVRProblem& problem);
        GreadySolution(CVRProblem& problem,int firstCity);
        //~GreadySolution();

    protected:

    private:

};

#endif // GREADYSOLUTION_H
