#include "CrossOps.h"
#include "CrossOps.h"
#include <algorithm>
#include <cstdlib>
#include <vector>
#include <iostream>

Solution* CrossOps::OrderedCrossover(Solution& s1, Solution& s2)
{
    int size = s1.Size;
    if (size != s2.Size)
        return new Solution(s1);

    int* parent1 = s1.Representation;
    int* parent2 = s2.Representation;
    int* child = new int[size];

    for (int i = 0; i < size; i++)
        child[i] = -1;

    //Encode zeros
    int zeroEncoder = -1;
    for (int i = 0; i < size; i++)
        if (parent1[i] == 0)
            parent1[i] = zeroEncoder--;

    zeroEncoder = -1;
    for (int i = 0; i < size; i++)
        if (parent2[i] == 0)
            parent2[i] = zeroEncoder--;

    int start = rand() % size;
    int end = rand() % size;
    if (start > end)
    {
        int temp = end;
        end = start;
        start = temp;
    }
    //Copy desired fragment
    for (int i = start; i <= end; i++)
        child[i] = parent1[i];

    //Fill remaining genes
    int insertPos = 0;
    for (int i = 0; i < size; i++)
    {
        if(i == start)//Skip copied part
        {
            i = end;
            continue;
        }
        int gene;
        bool exists = true;
        while(exists)
        {
            exists = false;
            gene = parent2[insertPos];
            for (int j = start; j <= end; j++)
                    if (child[j] == gene) { exists = true; break; }

            if (!exists)
            {
                child[i] = gene;
            }
            insertPos = (insertPos + 1) % size;
        }
    }

    //Decode zeros
    for (int i = 0; i < size; i++)
        if (parent1[i] < 0)
            parent1[i] = 0;
    for (int i = 0; i < size; i++)
        if (parent2[i] < 0)
            parent2[i] = 0;
    for (int i = 0; i < size; i++)
        if (child[i] < 0)
            child[i] = 0;


    //Create offspring
    Solution* offspring = new Solution(size);
    for (int i = 0; i < size; i++)
        offspring->Representation[i] = child[i];

    delete[] child;
    return offspring;
}
Solution* CrossOps::CycleCrossover(Solution& s1, Solution& s2)
{
    int size = s1.Size;
    if (size != s2.Size)
        return new Solution(s1);

    int* parent1 = s1.Representation;
    int* parent2 = s2.Representation;
    int* child = new int[size];

    //Encode zeros
    int zeroEncoder = -1;
    for (int i = 0; i < size; i++)
        if (parent1[i] == 0)
            parent1[i] = zeroEncoder--;

    zeroEncoder = -1;
    for (int i = 0; i < size; i++)
        if (parent2[i] == 0)
            parent2[i] = zeroEncoder--;

    // Initialize child with parent1
    for (int i = 0; i < size; i++)
        child[i] = parent1[i];

    bool* visited = new bool[size];
    for (int i = 0; i < size; i++)
        visited[i] = false;

    int start = rand() % size;
    int index = start;

    do
    {
        int fromVal = parent1[index];
        int toVal = parent2[index];
        for (int i = 0; i < size; i++)
        {
            if(parent1[i] == toVal)
            {

                index = i;
                visited[index] = true;
                break;
            }
        }
    }
    while (index != start);

    //Fill unvisited with second parent
    for (int i = 0; i < size; i++)
        if (visited[i] == false)
            child[i] = parent2[i];

    delete visited;
    //Decode zeros
    for (int i = 0; i < size; i++)
        if (parent1[i] < 0)
            parent1[i] = 0;
    for (int i = 0; i < size; i++)
        if (parent2[i] < 0)
            parent2[i] = 0;
    for (int i = 0; i < size; i++)
        if (child[i] < 0)
            child[i] = 0;

    //Create offspring
    Solution* offspring = new Solution(size);
    for (int i = 0; i < size; i++)
        offspring->Representation[i] = child[i];

    delete[] child;
    return offspring;
}
Solution* CrossOps::PMXCrossover(Solution& s1, Solution& s2)
{
    int size = s1.Size;
    if (size != s2.Size)
        return new Solution(s1);

    int* parent1 = new int[size];
    int* parent2 = new int[size];
    int* child   = new int[size];

    int zeroEncoder1 = -1;
    int zeroEncoder2 = -1;

    for (int i = 0; i < size; i++)
    {
        if (s1.Representation[i] == 0) parent1[i] = zeroEncoder1--;
        else parent1[i] = s1.Representation[i];

        if (s2.Representation[i] == 0) parent2[i] = zeroEncoder2--;
        else parent2[i] = s2.Representation[i];

        child[i] = -1;
    }

    int start = rand() % size;
    int end = rand() % size;
    if (start > end)
    {
        int temp = start;
        start = end;
        end = temp;
    }

    for (int i = start; i <= end; i++)
        child[i] = parent1[i];

    //Perform mapping
    for (int i = 0; i < size; i++)
    {
        //skip map part
        if(i == start)
        {
            i = end;
            continue;
        }

        child[i] = parent2[i];

        bool mapping = true;
        while(mapping)
        {
            mapping = false;
            for (int j = start; j <= end; j++)
            {
                int fromVal = parent1[j];
                int toVal = parent2[j];

                if (child[i] == fromVal)
                {
                    child[i] = toVal;
                    mapping = true;
                    break;
                }
            }
        }
    }
    //Decode zeros
    for (int i = 0; i < size; i++)
        if (parent1[i] < 0)
            parent1[i] = 0;
    for (int i = 0; i < size; i++)
        if (parent2[i] < 0)
            parent2[i] = 0;
    for (int i = 0; i < size; i++)
        if (child[i] < 0)
            child[i] = 0;

    Solution* offspring = new Solution(size);
    for (int i = 0; i < size; i++)
        offspring->Representation[i] = child[i];

    delete[] parent1;
    delete[] parent2;
    delete[] child;

    return offspring;
}

