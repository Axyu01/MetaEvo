#include "CVRProblem.h"
#include "Solution.h"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <math.h>
#include <algorithm> // std::min
#include <limits>    // std::numeric_limits
using namespace std;
double** CVRProblem::GetMatrixCopy()
{
    double** copyOfMatrix = new double*[_dimension];

    for (int i = 0; i < _dimension; i++)
    {
        copyOfMatrix[i] = new double[_dimension];
        for (int j = 0; j < _dimension; j++)
        {
            copyOfMatrix[i][j] = _distanceMatrix[i][j];
        }
    }

    return copyOfMatrix;
}
Location* CVRProblem::GetLocations()
{
    Location* LocationArray = new Location[_dimension];

    for (int i = 0; i < _dimension; i++)
    {
        LocationArray[i].demand = _LocationArray[i].demand;
        LocationArray[i].id = _LocationArray[i].id;
        LocationArray[i].x = _LocationArray[i].x;
        LocationArray[i].y = _LocationArray[i].y;
    }

    return LocationArray;
}
int CVRProblem::GetCapacity()
{
    return _capacity;
}
int CVRProblem::GetDimension()
{
    return _dimension;
}
double** CVRProblem::GetData(int& matrixSize,int& capacity,Location*& LocationArray)
{
    matrixSize = _dimension;
    capacity = _capacity;
    if(_distanceMatrix == nullptr)
        return nullptr;

    double** copyOfMatrix = GetMatrixCopy();

    LocationArray = GetLocations();

    return copyOfMatrix;
}
double CVRProblem::EstimateSolution(int* solutionArray,int arraySize)
{
    if(arraySize < 0)
        return 0;

    //int arraySize = _dimension-1;
    double estimation = 0;
    int load = 0;
    int currentNode;
    int previousNode = 0;

    for(int i = 0; i<arraySize; i++)
    {
        currentNode = solutionArray[i];
        int demand = 0;

        if(currentNode == 0)
            load = 0;
        else
            demand = _LocationArray[currentNode].demand;

        if(load + demand > _capacity)
        {
            load = demand;
            estimation += _distanceMatrix[previousNode][0];
            //cout <<"Going from: "<<previousNode <<" to: "<<0<<endl;
            estimation += _distanceMatrix[0][currentNode];
            //cout <<"Going from: "<<0 <<" to: "<<currentNode<<endl;
        }
        else
        {
            load+=demand;
            estimation += _distanceMatrix[previousNode][currentNode];
            //cout <<"Going from: "<< previousNode <<" to: "<<currentNode<<endl;
        }

        previousNode = currentNode;
    }
    estimation += _distanceMatrix[currentNode][0];
    //cout <<"Going from: "<< currentNode <<" to: "<<0<<endl;
    return estimation;
}
double CVRProblem::EstimateSolution(Solution& solution)
{
    return EstimateSolution(solution.Representation,solution.Size);
}
CVRProblem::CVRProblem(int numOfLocations)
    : _dimension(numOfLocations)
{
    initMatrix(numOfLocations);
    initLocations(numOfLocations);
}
CVRProblem::CVRProblem(std::string path)
{
    //cout << "CVRProblem::Constructor::PreLoaded"<< endl;
    LoadFromFile(path);
    //cout << "CVRProblem::Constructor::PostLoaded"<< endl;
}
CVRProblem::~CVRProblem()
{
    DestroyMatrix(_distanceMatrix,_dimension);
    DestroyLocations(_LocationArray);
}
void CVRProblem::Print()
{
    //Sprawdzenie danych
    cout << "Capacity: " << _capacity << endl;
    cout << "Dimension: " << _dimension << endl;
    cout << "Matrix:\n";
    for (int i =0; i<_dimension; i++)
    {
        for (int j =0; j<_dimension; j++)
        {
            cout <<" | "<< _distanceMatrix[i][j];
        }
        cout << " | " << endl;
    }
}
bool CVRProblem::LoadFromFile(std::string path)
{
    cout << "Loading File: "<< path<< endl;
    ifstream file(path);
    if (!file.is_open())
    {
        cerr << "Nie mozna otworzyc pliku!" << endl;
        return false;
    }

    string line;
    bool inLocationSection = false;
    bool inDemandSection = false;
    int nodeIterator = 0;

    while (getline(file, line))
    {
        if (line.find("DIMENSION") != string::npos)
        {
            string dummy;
            stringstream ss(line);
            ss >> dummy >> dummy >> _dimension;
            //cout << "Read Dimension"<< _dimension<< endl;
            initLocations(_dimension);
        }
        if (line.find("CAPACITY") != string::npos)
        {
            string dummy;
            stringstream ss(line);
            ss >> dummy >> dummy >> _capacity;
            //cout << "Read Capacity: " << _capacity << endl;
        }
        else if (line.find("NODE_COORD_SECTION") != string::npos)
        {
            //cout << "Readings Nodes/Cords..."<< endl;
            nodeIterator = 0;
            inLocationSection = true;
            inDemandSection = false;
            continue;
        }
        else if (line.find("DEMAND_SECTION") != string::npos)
        {
            //cout << "Readings Nodes Demands..."<< endl;
            nodeIterator = 0;
            inLocationSection = false;
            inDemandSection = true;
            continue;
        }
        else if (line.find("DEPOT_SECTION") != string::npos)
        {
            //cout << "Leaving..."<< endl;
            break;
        }

        if (inLocationSection)
        {
            //cout << "Reading location["<< nodeIterator+1<<"]"<< endl;
            stringstream ss(line);
            Location* location = &_LocationArray[nodeIterator++];
            ss >> location->id >> location->x >> location->y;
            location->demand = 0;
        }

        if (inDemandSection)
        {
            stringstream ss(line);
            int id, demand;
            ss >> id >> demand;
            //cout << "Reading demand for ["<< id <<"]"<< endl;
            if (id - 1 < _dimension)
                _LocationArray[id-1].demand = demand;
        }
    }

    initMatrix(_LocationArray,_dimension);

    file.close();

    return true;
}
double CVRProblem::distance(Location l1,Location l2)
{
    double xDist = l1.x - l2.x;
    double yDist = l1.y - l2.y;
    return sqrt(pow(xDist,2)+ pow(yDist,2));
}
//Initialization and Destruction
void CVRProblem::initMatrix(int matrixSize)
{
    if(_distanceMatrix != nullptr)
    {
        DestroyMatrix(_distanceMatrix,_dimension);
    }
    _distanceMatrix = new double*[matrixSize];
    for (int i = 0; i < matrixSize; i++)
    {
        _distanceMatrix[i] = new double[matrixSize];
        for (int j = 0; j < matrixSize; j++)
        {
            int randomInt = rand() % 100; // random distance 0–99
            _distanceMatrix[i][j] = randomInt;
            //std::cout << randomInt << " ";
        }
        //std::cout << std::endl;
    }
}
void CVRProblem::initMatrix(Location* locations,int matrixSize)
{
    initMatrix(matrixSize);
    for (int i = 0; i < matrixSize; i++)
    {
        for (int j = 0; j < matrixSize; j++)
        {
            _distanceMatrix[i][j] = distance(locations[i],locations[j]);
        }
    }
}
void CVRProblem::DestroyMatrix(double**& matrix,int matrixSize)
{
    for (int i = 0; i < matrixSize; i++)
    {
        delete[] matrix[i];
    }
    delete[] matrix;
    matrix = nullptr;
}
void CVRProblem::initLocations(int numOfLocations)
{
    if(_LocationArray != nullptr)
        DestroyLocations(_LocationArray);
    _LocationArray = new Location[numOfLocations];
}
void CVRProblem::DestroyLocations(Location*& LocationsArray)
{
    delete[] LocationsArray;
    LocationsArray = nullptr;
}
