#ifndef MUTATIONOPS_H
#define MUTATIONOPS_H
#include <Solution.h>


class MutationOps
{
    public:
        //MutationOps();
        //virtual ~MutationOps();
        static void Swap(Solution& s,double mutChance);
        static void Inverse(Solution& s,double mutChance);

    protected:

    private:
};

#endif // MUTATIONOPS_H
