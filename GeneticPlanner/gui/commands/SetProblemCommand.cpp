#include "SetProblemCommand.h"

#include "gui/PlanningProblemDisplayAdapter.h"

SetProblemCommand::SetProblemCommand(const PlanningProblem& newProblem, PlanningProblemDisplayAdapter * adapter) :
    _newProblem(newProblem), _oldProblemSet(false), _adapter(adapter)
{
}

SetProblemCommand::SetProblemCommand(const PlanningProblem &newProblem, const PlanningProblem &oldProblem, PlanningProblemDisplayAdapter *adapter) :
    _newProblem(newProblem), _oldProblemSet(true), _oldProblem(oldProblem), _adapter(adapter)
{
}

void SetProblemCommand::exec()
{
    if (!_oldProblemSet)
        _oldProblem = _adapter->planningProblem();

    _adapter->setPlanningProblem(_newProblem);
}

void SetProblemCommand::unExec()
{
    _adapter->setPlanningProblem(_oldProblem);
}
