#include "Solution.h"
#include "cstdlib"
#include "iostream"
Solution::Solution(int size)
{
    Size = size;
    Value = 0;
    Representation = new int[size];
}
Solution::Solution(const Solution& s)
{
    Size = s.Size;
    Value = s.Value;
    Representation = new int[Size];
    int* copiedRepresentation = s.Representation;
    for(int i=0;i<Size;i++)
    {
        Representation[i] = copiedRepresentation[i];
    }
}
Solution::~Solution()
{
    delete[] Representation;
}
void Solution::Randomize(int min,int max)
{
    if(min > max)
        return;

    for(int i=0;i<Size;i++)
    {
        int randNum = rand()%(max-min+1)+min;
        Representation[i] = randNum;
    }
}
void Solution::RandomizeCVRP(int dimension)
{
    if(Size<dimension-1)
        return;

    for(int i=0;i<Size;i++)
    {
        Representation[i] = 0;
    }

    for(int i=1;i<dimension;)
    {
        int randPos = rand()% Size;
        if(Representation[randPos] == 0)
        {
            Representation[randPos] = i++;
        }
    }
}
void Solution::Print(std::string note)
{
    bool newRoad = false;
    std::cout <<"Solution:|";
    for(int i=0;i<Size;i++)
    {
        std::cout << Representation[i] <<"|";
    }
    if(note != "")
        std::cout <<" ("<<note<<")";
    std::cout <<" Value:["<<Value<<"]";
    std::cout <<std::endl;
}
void Solution::Print()
{
    Print("");
}
