#ifndef SETPROBLEMCOMMAND_H
#define SETPROBLEMCOMMAND_H

#include <QPointer>

#include "PlanningProblem.h"
#include "gui/PlanningProblemDisplayAdapter.h"
#include "gui/MWCommand.h"

class SetProblemCommand : public MWCommand
{
public:
    SetProblemCommand(const PlanningProblem& newProblem, PlanningProblemDisplayAdapter * adapter);

    virtual void exec();
    virtual void unExec();

private:
    PlanningProblem _newProblem;
    PlanningProblemDisplayAdapter * _adapter;
};

#endif // SETPROBLEMCOMMAND_H
