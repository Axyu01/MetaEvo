#include "Population.h"
#include "cstdlib"
#include <algorithm>
#include <iostream>
#include <math.h>
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
    for(int i=0; i<Params.init_population_size; i++)
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
              [](const Solution* a, const Solution* b)
    {
        return a->Value < b->Value;
    });
}
void Population::Select()
{
    bool useRulate = Params.useRulate;

    int desiredPop = Params.selection_population_size;
    int popSize = Solutions.size();

    if (popSize <= desiredPop)
        return;
    Sort();
    if(useRulate == true)
    {
        Sort();
        //Reevaluate
        double BEST_W = Params.BEST_W;
        double WORST_W = Params.WORST_W;
        double interpolation_strength = Params.interpolation_strength;

        int elitismCount = std::min(Params.elitism_count, (int)Solutions.size());

        for(int i=0; i<elitismCount; i++)
        {
            Solutions[i]->Value = 0;//protecting the elites
        }
        for(int i=elitismCount; i<popSize; i++)
        {
            double interpolation =(double)i/popSize;
            interpolation = std::pow(interpolation,interpolation_strength);
            Solutions[i]->Value = BEST_W +(WORST_W-BEST_W)*interpolation;
        }
        //Rulate
        double maxEstim = 0;

        for(int i=0; i<popSize; i++)
        {
            maxEstim += Solutions[i]->Value;
        }

        for(int i=0; i<popSize-desiredPop; i++)
        {
            double eliminationValue = 0;
            while(eliminationValue == 0)
            {
                double maxPercentage = static_cast<double>(rand()) / RAND_MAX;
                eliminationValue = maxPercentage * maxEstim;
            }
            double currentValueSum = 0;
            int eliminatedSolution = 0;

            double sValue =0;
            for(int s=0; s<popSize-desiredPop; s++)
            {
                sValue = Solutions[s]->Value;
                //Check if solution is chosen to eliminate
                if(currentValueSum<= eliminationValue && eliminationValue <=currentValueSum+sValue)
                {
                    eliminatedSolution = s;
                    break;
                }
                else
                {
                    currentValueSum+=sValue;
                }
            }
            //Swap with last to allow elimination at the end
            int lastSolution = popSize-i-1;
            Solution* temp = Solutions[lastSolution];
            Solutions[lastSolution] = Solutions[eliminatedSolution];
            Solutions[eliminatedSolution] = temp;

            //Update maxEstim to narrow the elimination field
            maxEstim -= sValue;
        }
    }


    // Remove the excess elements at the end
    for(int i = desiredPop; i<Solutions.size(); i++)
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
    int elitismCount = std::min(Params.elitism_count, (int)Solutions.size());

    for (int i = elitismCount; i < Solutions.size(); ++i)
    {
        Solution* s = Solutions[i];
        if ((double)rand() / RAND_MAX <= Params.mutation_chance)
        {
            Params.mutationOperator(*s, Params.mutation_chance);
        }
    }
}

