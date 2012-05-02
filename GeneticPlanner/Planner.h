#ifndef PLANNER_H
#define PLANNER_H

#include "Individual.h"

class Planner
{
public:
    Planner();

    Individual plan(int generations=100);

    qreal fitness(const Individual& individual) const;
};

#endif // PLANNER_H
