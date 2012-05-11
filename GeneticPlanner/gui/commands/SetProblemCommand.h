#ifndef SETPROBLEMCOMMAND_H
#define SETPROBLEMCOMMAND_H

#include <QPointer>

#include "PlanningProblem.h"
//#include "gui/PlanningProblemDisplayAdapter.h"
class PlanningProblemDisplayAdapter;
#include "gui/MWCommand.h"

class SetProblemCommand : public MWCommand
{
public:
    SetProblemCommand(const PlanningProblem& newProblem, PlanningProblemDisplayAdapter * adapter);
    SetProblemCommand(const PlanningProblem& newProblem, const PlanningProblem& oldProblem, PlanningProblemDisplayAdapter * adapter);

    virtual void exec();
    virtual void unExec();

private:
    PlanningProblem _newProblem;
    PlanningProblemDisplayAdapter * _adapter;

    bool _oldProblemSet;
    PlanningProblem _oldProblem;
};

#endif // SETPROBLEMCOMMAND_H
