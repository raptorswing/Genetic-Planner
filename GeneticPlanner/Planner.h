#ifndef PLANNER_H
#define PLANNER_H

#include "Individual.h"

class Planner
{
public:
    Planner();

    Individual plan(int generations=100);

    qreal fitness(Individual &individual) const;

    static qreal normal(qreal x, qreal stdDev,qreal scaleFactor=1.0);
};

#endif // PLANNER_H
