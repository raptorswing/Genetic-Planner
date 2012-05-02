#include "FitnessRunnable.h"

#include "Planner.h"
#include "Individual.h"

FitnessRunnable::FitnessRunnable(Planner * planner,Individual *individual) :
    _planner(planner), _individual(individual)
{
    this->setAutoDelete(false);
}

void FitnessRunnable::run()
{
    _result = _planner->fitness(*_individual);
}

qreal FitnessRunnable::result() const
{
    return _result;
}
