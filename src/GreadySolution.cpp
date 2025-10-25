#include "GreadySolution.h"
#include <CVRProblem.h>
#include <iostream>

GreadySolution::GreadySolution(CVRProblem& problem,int firstCity) : Solution(problem.GetDimension()-1)
{
    int matrixSize = 0;
    int capacity = 0;
    Location* LocationArray;

    double** matrix = problem.GetData(matrixSize,capacity,LocationArray);
    if(firstCity == 0 || firstCity >= matrixSize)
    {
        int best = 1;
        for (int i = 2; i < matrixSize; i++)
        {
            if(matrix[0][i]< best)
            {
                best = i;
            }
        }
        firstCity = best;
    }

    int* solution = new int[matrixSize-1];
    bool* taken = new bool[matrixSize-1];
    for(int i =0;i<matrixSize-1;i++)
    {
        solution[i] = -1;
        taken[i] = false;
    }

    int currentCity = 0;
    int currentCapacity = 0;

    solution[0] = firstCity;
    taken[firstCity-1] = true;
    currentCity = firstCity;
    currentCapacity = LocationArray[currentCity].demand;

    for(int s =1;s<matrixSize-1;s++)
    {
        int bestNextCity = -1;
        double bestDistance = -1;
        bool isBestReturning = false;
        for(int comparedCity = 1;comparedCity<matrixSize;comparedCity++)
        {
            bool isCurrentReturning = LocationArray[comparedCity].demand +currentCapacity>capacity;
            if(taken[comparedCity-1])
                continue;

            double comparedDistance;
            if(isCurrentReturning)
            {
                 comparedDistance = matrix[currentCity][0];
                 comparedDistance += matrix[0][comparedCity];
            }
            else
            {
                comparedDistance = matrix[currentCity][comparedCity];
            }

            if(bestDistance == -1)
            {
                bestNextCity = comparedCity;
                bestDistance = comparedDistance;
                isBestReturning = isCurrentReturning;
                continue;
            }

            if(bestDistance>comparedDistance)
            {
                bestNextCity = comparedCity;
                bestDistance = comparedDistance;
                isBestReturning = isCurrentReturning;
            }
        }
        taken[bestNextCity-1] = true;
        solution[s] = bestNextCity;
        currentCity = bestNextCity;

        if(isBestReturning)
            currentCapacity = 0;
        else
            currentCapacity+=LocationArray[currentCity].demand;
    }

    //Cleanup
    delete[] taken;
    for (int i = 0; i < matrixSize; i++)
    {
        delete[] matrix[i];
    }
    delete[] matrix;
    delete[] LocationArray;

    /*for (int i = 0; i < matrixSize-1; i++)
    {
        std::cout<<"Gready S"<<i<<":"<<solution[i]<<std::endl;;
    }*/
    Representation = solution;
    Size = matrixSize-1;
    Value = problem.EstimateSolution(*this);
}
GreadySolution::GreadySolution(CVRProblem& problem) : GreadySolution(problem,0)
{

}
