#ifndef SOLUTION_H
#define SOLUTION_H
#include <string>

class Solution
{
    public:
        Solution(int size);
        Solution(const Solution& s);
        virtual ~Solution();
        void Randomize(int min,int max);
        void RandomizeCVRP(int dimension);
        void Print();
        void Print(std::string note);

        int* Representation;
        int Size;
        double Value;
        //static double CompareLogic();

    protected:

    private:

};

#endif // SOLUTION_H
