#include "MutationOps.h"
#include <cstdlib>
#include <iostream>

void MutationOps::Swap(Solution& s,double mutChance)
{
    int size = s.Size;
    if(size <2)
        return;

    int* representation = s.Representation;
    int repetitions = rand()%((int)(size*mutChance));
    for(int i=0;i<repetitions;i++)
    {
        int firstPos = rand()%size;
        int secondPos = firstPos;
        while(firstPos == secondPos)
        {
            secondPos = rand()%size;
        }

        //swap
        int temp = representation[firstPos];
        representation[firstPos] = representation[secondPos];
        representation[secondPos] = temp;
    }

}
void MutationOps::Inverse(Solution& s,double mutChance)
{
    int size = s.Size;
    if(size <2)
        return;

    int* representation = s.Representation;

    int smallerPos = rand()%size;
    int biggerPos = smallerPos;

    while(smallerPos == biggerPos)
    {
        biggerPos = rand()%size;
    }
    if(smallerPos > biggerPos)
    {
        int temp = smallerPos;
        smallerPos = biggerPos;
        biggerPos = temp;
    }

    int* segment = new int[biggerPos-smallerPos+1];

    for(int i = smallerPos;i<biggerPos+1;i++)
    {
        segment[i-smallerPos] = representation[i];
    }

    int decrement = biggerPos-smallerPos;

    for(int i = smallerPos;i<biggerPos+1;i++)
    {
        representation[i] = segment[decrement--];
    }

    delete[] segment;
}
