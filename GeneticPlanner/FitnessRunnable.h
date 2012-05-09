#ifndef FITNESSRUNNABLE_H
#define FITNESSRUNNABLE_H

#include <QRunnable>

#include "PlanningProblem.h"
#include "Individual.h"

class FitnessRunnable : public QRunnable
{
public:
    FitnessRunnable(PlanningProblem * problem,Individual * individual);

    virtual void run();

    qreal result() const;

private:
    PlanningProblem * _problem;
    Individual * _individual;
    qreal _result;
};

#endif // FITNESSRUNNABLE_H
