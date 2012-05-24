#ifndef FITNESSRUNNABLE_H
#define FITNESSRUNNABLE_H

#include <QRunnable>
#include <QSharedPointer>

#include "PlanningProblem.h"
#include "Individual.h"

class FitnessRunnable : public QRunnable
{
public:
    FitnessRunnable(QSharedPointer<PlanningProblem> problem, QSharedPointer<Individual> individual);

    virtual void run();

    qreal result() const;

private:
    QSharedPointer<PlanningProblem> _problem;
    QSharedPointer<Individual> _individual;
    qreal _result;
};

#endif // FITNESSRUNNABLE_H
