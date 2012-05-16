#include "FitnessRunnable.h"

#include "PlanningProblem.h"
#include "Individual.h"

FitnessRunnable::FitnessRunnable(PlanningProblem * problem,QSharedPointer<Individual> individual) :
    _problem(problem), _individual(individual)
{
    this->setAutoDelete(false);
}

void FitnessRunnable::run()
{
    _result = _problem->fitness(_individual);
}

qreal FitnessRunnable::result() const
{
    return _result;
}
