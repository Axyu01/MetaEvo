#include "CrossOps.h"
#include "CrossOps.h"
#include <algorithm>
#include <cstdlib>
#include <vector>

Solution* CrossOps::OrderedCrossover(Solution& s1, Solution& s2)
{
    int size = s1.Size;
    if (size != s2.Size)
        return new Solution(s1);

    int* parent1 = s1.Representation;
    int* parent2 = s2.Representation;
    int* child = new int[size];

    // Initialize with -1
    for (int i = 0; i < size; i++)
        child[i] = -1;

    // Count how many zeros exist in total
    int zeroCount = 0;
    for (int i = 0; i < size; i++)
        if (parent1[i] == 0) zeroCount++;

    // 1️⃣ Choose crossover section
    int start = rand() % size;
    int end = rand() % size;
    if (start > end) std::swap(start, end);

    // 2️⃣ Copy slice from parent1
    for (int i = start; i <= end; i++)
        child[i] = parent1[i];

    // 3️⃣ Fill remaining from parent2 in order, avoiding duplicates
    int insertPos = (end + 1) % size;

    for (int i = 0; i < size; i++)
    {
        int gene = parent2[(end + 1 + i) % size];
        bool exists = false;

        // Check if this gene already exists (but allow multiple zeros)
        if (gene != 0)
        {
            for (int j = start; j <= end; j++)
                if (child[j] == gene) { exists = true; break; }
        }

        if (!exists)
        {
            // Insert
            child[insertPos] = gene;
            insertPos = (insertPos + 1) % size;
        }
    }

    // 4️⃣ Normalize zero count (ensure correct number)
    int currentZeroCount = 0;
    for (int i = 0; i < size; i++)
        if (child[i] == 0) currentZeroCount++;

    if (currentZeroCount > zeroCount)
    {
        // too many zeros, replace extras with unused customers
        std::vector<int> used;
        for (int i = 0; i < size; i++)
            if (child[i] != 0)
                used.push_back(child[i]);

        std::vector<int> all;
        for (int i = 0; i < size; i++)
            if (parent1[i] != 0 && std::find(used.begin(), used.end(), parent1[i]) == used.end())
                all.push_back(parent1[i]);

        int replaceIdx = 0;
        for (int i = 0; i < size && currentZeroCount > zeroCount && replaceIdx < (int)all.size(); i++)
        {
            if (child[i] == 0)
            {
                child[i] = all[replaceIdx++];
                currentZeroCount--;
            }
        }
    }

    // 5️⃣ Create offspring
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

    // Initialize child with -1
    for (int i = 0; i < size; i++)
        child[i] = -1;

    bool* visited = new bool[size];
    for (int i = 0; i < size; i++)
        visited[i] = false;

    int cycle = 0;

    while (true)
    {
        // Find first unvisited index
        int start = -1;
        for (int i = 0; i < size; i++)
        {
            if (!visited[i])
            {
                start = i;
                break;
            }
        }
        if (start == -1)
            break; // all visited

        // Build a cycle starting from 'start'
        int current = start;
        do
        {
            visited[current] = true;
            current = -1;

            // find where parent2’s value at this position appears in parent1
            int value = parent2[start];
            for (int i = 0; i < size; i++)
            {
                if (parent1[i] == value)
                {
                    current = i;
                    break;
                }
            }

            // Update start for next link in cycle
            start = current;

        } while (current != -1 && !visited[current]);

        // Assign genes for this cycle
        for (int i = 0; i < size; i++)
        {
            if (visited[i])
            {
                if (cycle % 2 == 0)
                    child[i] = parent1[i];
                else
                    child[i] = parent2[i];
            }
        }

        cycle++;
    }

    // Fill any remaining (shouldn’t happen, but safety)
    for (int i = 0; i < size; i++)
        if (child[i] == -1)
            child[i] = parent1[i];

    delete[] visited;

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

    int zeroId1 = -1;
    int zeroId2 = -1;

    for (int i = 0; i < size; i++)
    {
        if (s1.Representation[i] == 0) parent1[i] = zeroId1--;
        else parent1[i] = s1.Representation[i];

        if (s2.Representation[i] == 0) parent2[i] = zeroId2--;
        else parent2[i] = s2.Representation[i];

        child[i] = -1;
    }

    int start = rand() % size;
    int end = rand() % size;
    if (start > end) std::swap(start, end);

    for (int i = start; i <= end; i++)
        child[i] = parent1[i];

    for (int i = start; i <= end; i++)
    {
        int val1 = parent1[i];
        int val2 = parent2[i];

        bool exists = false;
        for (int j = 0; j < size; j++)
        {
            if (child[j] == val2)
            {
                exists = true;
                break;
            }
        }
        if (exists) continue;

        int pos = i;
        while (true)
        {
            int conflictVal = parent2[pos];
            pos = -1;

            for (int j = 0; j < size; j++)
            {
                if (parent1[j] == conflictVal)
                {
                    pos = j;
                    break;
                }
            }

            if (pos == -1 || child[pos] == -1)
            {
                child[pos] = val2;
                break;
            }
        }
    }

    for (int i = 0; i < size; i++)
    {
        if (child[i] == -1)
        {
            for (int j = 0; j < size; j++)
            {
                int gene = parent2[j];

                bool exists = false;
                for (int k = 0; k < size; k++)
                {
                    if (child[k] == gene)
                    {
                        exists = true;
                        break;
                    }
                }
                if (exists) continue;

                child[i] = gene;
                break;
            }
        }
    }

    for (int i = 0; i < size; i++)
    {
        if (child[i] < 0)
            child[i] = 0;
    }

    Solution* offspring = new Solution(size);
    for (int i = 0; i < size; i++)
        offspring->Representation[i] = child[i];

    delete[] parent1;
    delete[] parent2;
    delete[] child;

    return offspring;
}

