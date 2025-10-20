#ifndef EVOALGORITHM_H
#define EVOALGORITHM_H
#include <Population.h>

class EvoAlgorithm
{
    public:
        EvoAlgorithm(PopParameters params);
        virtual ~EvoAlgorithm();
        void Start();
        void Loop();
        Population* _population;

    protected:

    private:

};

#endif // EVOALGORITHM_H
