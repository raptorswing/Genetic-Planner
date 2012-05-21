#include "PlanningProblemDisplayAdapter.h"

#include "CircleObject.h"
#include "TaskAreaObject.h"

#include <QtDebug>
#include <QHashIterator>

PlanningProblemDisplayAdapter::PlanningProblemDisplayAdapter(const PlanningProblem &problem, MapGraphicsScene * mgScene, QObject *parent) :
    QObject(parent), _problem(problem), _mgScene(mgScene)
{
}

void PlanningProblemDisplayAdapter::setPlanningProblem(const PlanningProblem &nProblem)
{
    this->clear();

    _problem = nProblem;

    if (_problem.isStartingDefined())
        this->setStartPosition(_problem.startingPos(),_problem.startingAlt());
    if (_problem.isEndingDefined())
        this->setEndPosition(_problem.endingPos(),_problem.endingAlt());

    QSet<QPolygonF> areas = _problem.areas();
    foreach (QPolygonF area, areas)
        this->addArea(area);

    this->problemHasChanged(_problem);
}

PlanningProblem PlanningProblemDisplayAdapter::planningProblem() const
{
    return _problem;
}

void PlanningProblemDisplayAdapter::setStartPosition(const QPointF &geoPos, const qreal &altitude)
{
    if (_startPositionObject.isNull())
    {
        CircleObject * obj = new CircleObject(6.0,true,Qt::green);
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
    }

    _startPositionObject->setPos(geoPos);
    _problem.setStartingPos(geoPos,altitude);

    this->problemHasChanged(_problem);
}

void PlanningProblemDisplayAdapter::removeStartPosition()
{
    if (!_startPositionObject.isNull())
        _startPositionObject->deleteLater();
    _problem.clearStartingPos();

    this->problemHasChanged(_problem);
}

void PlanningProblemDisplayAdapter::setEndPosition(const QPointF &geoPos, const qreal &altitude)
{
    if (_endPositionObject.isNull())
    {
        CircleObject * obj = new CircleObject(6.0, true, Qt::red);
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
    }
    _endPositionObject->setPos(geoPos);
    _problem.setEndingPos(geoPos,altitude);

    this->problemHasChanged(_problem);
}

void PlanningProblemDisplayAdapter::removeEndPosition()
{
    if (!_endPositionObject.isNull())
        _endPositionObject->deleteLater();
    _problem.clearEndingPos();

    this->problemHasChanged(_problem);
}

void PlanningProblemDisplayAdapter::addArea(const QPointF &center)
{
    const qreal degrees = 0.001;
    QPointF topLeft = center + QPointF(-1*degrees,degrees);
    QPointF topRight = center + QPointF(degrees,degrees);
    QPointF bottomLeft = center + QPointF(-1*degrees,-1*degrees);
    QPointF bottomRight = center + QPointF(degrees,-1*degrees);
    QPolygonF geoPoly;
    geoPoly << topLeft << topRight << bottomRight << bottomLeft;

    TaskAreaObject * obj = new TaskAreaObject(geoPoly);
    connect(obj,
            SIGNAL(posChanged()),
            this,
            SLOT(handleAreaChanged()));
    connect(obj,
            SIGNAL(destroyed()),
            this,
            SLOT(handleAreaDestroyed()));
    if (!_problem.areas().contains(obj->geoPoly()))
        _problem.addArea(obj->geoPoly());
    _areas.insert(obj,obj->geoPoly());
    _mgScene->addObject(obj);

    this->problemHasChanged(_problem);
}

void PlanningProblemDisplayAdapter::addArea(const QPolygonF &poly)
{
    TaskAreaObject * obj = new TaskAreaObject(poly);
    connect(obj,
            SIGNAL(posChanged()),
            this,
            SLOT(handleAreaChanged()));
    connect(obj,
            SIGNAL(destroyed()),
            this,
            SLOT(handleAreaDestroyed()));
    if (!_problem.areas().contains(obj->geoPoly()))
        _problem.addArea(obj->geoPoly());
    _areas.insert(obj,obj->geoPoly());
    _mgScene->addObject(obj);

    this->problemHasChanged(_problem);
}

//private slot
void PlanningProblemDisplayAdapter::handleStartPositionObjectPosChanged()
{
    const QPointF& pos = _startPositionObject->pos();
    const qreal& alt = _problem.startingAlt();
    _problem.setStartingPos(pos,alt);

    this->problemHasChanged(_problem);
}

//private slot
void PlanningProblemDisplayAdapter::handleEndPositionObjectPosChanged()
{
    const QPointF& pos = _endPositionObject->pos();
    const qreal& alt = _problem.endingAlt();
    _problem.setEndingPos(pos,alt);

    this->problemHasChanged(_problem);
}

//private slot
void PlanningProblemDisplayAdapter::handleAreaChanged()
{
    QObject * sender = QObject::sender();
    if (!sender)
        return;

    TaskAreaObject * poly = qobject_cast<TaskAreaObject *>(sender);
    if (!poly)
        return;

    if (!_areas.contains(poly))
    {
        qWarning() << this << "area mapping doesn't contain" << poly;
        return;
    }

    QPolygonF oldGeoPoly = _areas.value(poly);
    _problem.removeArea(oldGeoPoly);
    _problem.addArea(poly->geoPoly());
    _areas.insert(poly,poly->geoPoly());

    this->problemHasChanged(_problem);
}

//private slot
void PlanningProblemDisplayAdapter::handleAreaDestroyed()
{
    QObject * sender = QObject::sender();
    if (!sender)
        return;

    /*
    Don't use this pointer for anything substantive as everything above the QObject level
    has already been deleted by this point.
    */
    TaskAreaObject * dangerPolyPointer = (TaskAreaObject *)sender;
    QPolygonF oldGeoPoly = _areas.take(dangerPolyPointer);
    _problem.removeArea(oldGeoPoly);

    this->problemHasChanged(_problem);
}

//private
void PlanningProblemDisplayAdapter::clear()
{
    //Clear all old map GUI objects before setting up the new object
    if (!_startPositionObject.isNull())
    {
        disconnect(_startPositionObject.data(),
                   SIGNAL(destroyed()),
                   this,
                   SLOT(removeStartPosition()));
        _startPositionObject->deleteLater();
        _startPositionObject = 0;
    }

    if (!_endPositionObject.isNull())
    {
        disconnect(_endPositionObject.data(),
                   SIGNAL(destroyed()),
                   this,
                   SLOT(removeEndPosition()));
        _endPositionObject->deleteLater();
        _endPositionObject = 0;
    }

    QHashIterator<TaskAreaObject *,QPolygonF> areaIter(_areas);
    while (areaIter.hasNext())
    {
        areaIter.next();
        TaskAreaObject * poly = areaIter.key();
        disconnect(poly,
                   SIGNAL(destroyed()),
                   this,
                   SLOT(handleAreaDestroyed()));
        poly->deleteLater();
    }
    _areas.clear();

    this->problemHasChanged(_problem);
}
