#ifndef CVRPROBLEM_H
#define CVRPROBLEM_H
#include <string>
#include <Solution.h>

struct Location {
    int id;
    double x, y;
    int demand;
};

class CVRProblem
{
    public:
        CVRProblem(int);
        CVRProblem(std::string path);
        ~CVRProblem();
        bool LoadFromFile(std::string path);
        void Print();
        double EstimateSolution(int* solutionArray,int arraySize);
        double EstimateSolution(Solution& solution);
        double** GetData(int& matrixSize,int& capacity,Location*& LocationArray);
        double** GetMatrixCopy();
        Location* GetLocations();
        int GetCapacity();
        int GetDimension();
        static void DestroyLocations(Location*& LocationsArray);
        static void DestroyMatrix(double**& matrix,int matrixSize);

    protected:

    private:
            void initMatrix(int matrixSize);
            void initMatrix(Location* locations,int matrixSize);

            void initLocations(int numOfLocations);

            double distance(Location l1,Location l2);
            double** _distanceMatrix = nullptr;
            Location* _LocationArray = nullptr;
            int _capacity;
            int _dimension;
};

#endif // CVRPROBLEM_H
