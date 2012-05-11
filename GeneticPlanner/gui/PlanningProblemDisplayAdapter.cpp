#include "PlanningProblemDisplayAdapter.h"

#include "CircleObject.h"

#include <QtDebug>

PlanningProblemDisplayAdapter::PlanningProblemDisplayAdapter(const PlanningProblem &problem, MapGraphicsScene * mgScene, QObject *parent) :
    QObject(parent), _problem(problem), _mgScene(mgScene)
{
}

void PlanningProblemDisplayAdapter::setPlanningProblem(const PlanningProblem &nProblem)
{
    //Clear all old map GUI objects before setting up the new object
    if (!_startPositionObject.isNull())
        _startPositionObject->deleteLater();
    if (!_endPositionObject.isNull())
        _endPositionObject->deleteLater();

    _problem = nProblem;

    if (_problem.isStartingDefined())
        this->setStartPosition(_problem.startingPos(),_problem.startingAlt());
    if (_problem.isEndingDefined())
        this->setEndPosition(_problem.endingPos(),_problem.endingAlt());
}

PlanningProblem PlanningProblemDisplayAdapter::planningProblem() const
{
    return _problem;
}

void PlanningProblemDisplayAdapter::setStartPosition(const QPointF &geoPos, const qreal &altitude)
{
    CircleObject * obj = new CircleObject(6.0,true,Qt::green);
    obj->setPos(geoPos);
    _mgScene->addObject(obj);
    _startPositionObject = obj;

    connect(obj,
            SIGNAL(posChanged()),
            this,
            SLOT(handleStartPositionObjectPosChanged()));
    connect(obj,
            SIGNAL(destroyed()),
            this,
            SLOT(removeStartPosition()));

    _problem.setStartingPos(geoPos,altitude);
}

void PlanningProblemDisplayAdapter::removeStartPosition()
{
    if (!_startPositionObject.isNull())
        _startPositionObject->deleteLater();
    _problem.clearStartingPos();
}

void PlanningProblemDisplayAdapter::setEndPosition(const QPointF &geoPos, const qreal &altitude)
{
    CircleObject * obj = new CircleObject(6.0, true, Qt::red);
    obj->setPos(geoPos);
    _mgScene->addObject(obj);
    _endPositionObject = obj;

    connect(obj,
            SIGNAL(posChanged()),
            this,
            SLOT(handleEndPositionObjectPosChanged()));
    connect(obj,
            SIGNAL(destroyed()),
            this,
            SLOT(removeEndPosition()));

    _problem.setEndingPos(geoPos,altitude);
}

void PlanningProblemDisplayAdapter::removeEndPosition()
{
    if (!_endPositionObject.isNull())
        _endPositionObject->deleteLater();
    _problem.clearEndingPos();
}

//private slot
void PlanningProblemDisplayAdapter::handleStartPositionObjectPosChanged()
{
    const QPointF& pos = _startPositionObject->pos();
    const qreal& alt = _problem.startingAlt();
    _problem.setStartingPos(pos,alt);
}

//private slot
void PlanningProblemDisplayAdapter::handleEndPositionObjectPosChanged()
{
    const QPointF& pos = _endPositionObject->pos();
    const qreal& alt = _problem.endingAlt();
    _problem.setEndingPos(pos,alt);
}
