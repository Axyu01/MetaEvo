#include "EvoAlgorithm.h"
#include "iostream"
EvoAlgorithm::EvoAlgorithm(PopParameters params)
{
    _population = new Population(params);
}

EvoAlgorithm::~EvoAlgorithm()
{
    delete _population;
}
void EvoAlgorithm::Start()
{
    _population->Init();
    _population->Eval();
}
void EvoAlgorithm::Loop()
{
    //std::cout<< "Select"<<std::endl;
    _population->Select();
    //std::cout<< "Cross"<<std::endl;
    _population->Cross();
    //std::cout<< "Mutate"<<std::endl;
    _population->Mutate();
    //std::cout<< "Eval"<<std::endl;
    _population->Eval();
}
