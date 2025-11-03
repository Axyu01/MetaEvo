#ifndef POPULATION_H
#define POPULATION_H
#include <Solution.h>
#include <vector>

struct PopParameters
{
    int init_population_size = 100;
    int selection_population_size = 30;
    int elitism_count = 1;
    double mutation_chance;
    double crossover_chance;
    void (*mutationOperator)(Solution& s,double mutChance);
    Solution* (*crossOperator)(Solution& s1,Solution& s2);
    Solution* (*randSolutionCreator)();
    double (*evaluator)(Solution& s);
    double BEST_W = 1;
    double WORST_W = 40;
    double interpolation_strength = 1;
    bool useRulate = true;
};
class Population
{
    public:
        Population(PopParameters params);
        ~Population();
        //Initialize
        void Init();
        //Evaluate
        void Eval();
        //Select
        void Select();
        //Mutate
        void Mutate();
        //Crossover
        void Cross();

        void Sort();

        PopParameters Params;
        std::vector<Solution*> Solutions;
        std::vector<Solution*> Elites;

    protected:

    private:
        //SelectionOperator(Method?)
        //MutationOperator
        //CrossOperator
        //CrossMethod?
};
//template <typename T,typename U>
/*class Solution
{
    int* genome;
    int genomeSize;
    double estimation;
};*/

#endif // POPULATION_H
