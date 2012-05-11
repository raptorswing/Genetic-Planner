#include "SetProblemCommand.h"

SetProblemCommand::SetProblemCommand(const PlanningProblem& newProblem, PlanningProblemDisplayAdapter * adapter) :
    _newProblem(newProblem), _adapter(adapter)
{
}

void SetProblemCommand::exec()
{
    PlanningProblem oldProblem = _adapter->planningProblem();
    _adapter->setPlanningProblem(_newProblem);
    _newProblem = oldProblem;
}

void SetProblemCommand::unExec()
{
    this->exec();
}
