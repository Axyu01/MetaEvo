#ifndef CROSSOPS_H
#define CROSSOPS_H
#include <Solution.h>

class CrossOps
{
    public:
        //CrossOps();
        //virtual ~CrossOps();
        static Solution* OrderedCrossover(Solution& s1,Solution& s2);
        static Solution* CycleCrossover(Solution& s1, Solution& s2);
        static Solution* PMXCrossover(Solution& s1, Solution& s2);

    protected:

    private:
};

#endif // CROSSOPS_H
