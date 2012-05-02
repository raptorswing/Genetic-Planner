#ifndef FITNESSRUNNABLE_H
#define FITNESSRUNNABLE_H

#include <QRunnable>

class Planner;
#include "Individual.h"

class FitnessRunnable : public QRunnable
{
public:
    FitnessRunnable(Planner * planner,Individual * individual);

    virtual void run();

    qreal result() const;

private:
    Planner * _planner;
    Individual * _individual;
    qreal _result;
};

#endif // FITNESSRUNNABLE_H
