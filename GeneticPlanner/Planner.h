#ifndef PLANNER_H
#define PLANNER_H

#include "Individual.h"
#include "PlanningProblem.h"

class Planner
{
public:
    Planner();

    Individual plan(PlanningProblem * problem, int generations=1000);
};

#endif // PLANNER_H
