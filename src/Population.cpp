#include "Population.h"
#include "cstdlib"
#include <algorithm>
#include <iostream>
Population::Population(PopParameters params)
{
    Params = params;
    //ctor
}

Population::~Population()
{
    //dtor
}
void Population::Init()
{
    //Reserve memory
    if(Params.init_population_size >= Params.selection_population_size*2)
        Solutions.reserve(Params.init_population_size);
    else
        Solutions.reserve(Params.init_population_size*2);

    Solutions.clear();
    for(int i=0;i<Params.init_population_size;i++)
    {
        Solution* s = Params.randSolutionCreator();
        Solutions.push_back(s);
    }
}
void Population::Eval()
{
    for(Solution* s : Solutions)
    {
        s->Value = Params.evaluator(*s);
    }
}
void Population::Sort()
{
    // Sort ascending by Value (smallest first)
    std::sort(Solutions.begin(), Solutions.end(),
              [](const Solution* a, const Solution* b) {
                  return a->Value < b->Value;
              });
}
void Population::Select()
{
    int desiredPop = Params.selection_population_size;
    int popSize = Solutions.size();

    if (popSize <= desiredPop)
        return;

    Sort();
    // Remove the excess elements at the end
    for(int i = desiredPop;i<Solutions.size();i++)
    {
        delete Solutions[i];
    }
    Solutions.erase(Solutions.begin() + desiredPop, Solutions.end());
}
void Population::Cross()
{
    std::vector<Solution*> newChildren;
    newChildren.reserve(Solutions.size()); // optional, to reduce reallocations

    for (Solution* parent1 : Solutions)
    {
        // Probability check using rand()
        double prob = static_cast<double>(rand()) / RAND_MAX;
        if (prob < Params.crossover_chance)
        {
            // Pick a random second parent
            size_t idx = rand() % Solutions.size();
            Solution* parent2 = Solutions[idx];

            // Generate child
            Solution* child = Params.crossOperator(*parent1, *parent2);
            newChildren.push_back(child);
        }
    }

    // Add all new children to the population
    Solutions.insert(Solutions.end(),
                     std::make_move_iterator(newChildren.begin()),
                     std::make_move_iterator(newChildren.end()));
}
void Population::Mutate()
{
    for(Solution* s : Solutions)
    {
        if((double)rand()/RAND_MAX<=Params.mutation_chance)
        {
            Params.mutationOperator(*s);
        }
    }
}

