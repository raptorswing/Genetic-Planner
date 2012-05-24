#include "ProblemModelAdapter.h"

#include "TaskAreaObject.h"

const qreal START_END_MARKER_RADIUS = 6.0;

ProblemModelAdapter::ProblemModelAdapter(MapGraphicsScene *scene, QObject *parent) :
    QObject(parent), _scene(scene)
{
}

//public slot
void ProblemModelAdapter::setProblem(QWeakPointer<PlanningProblem> problem)
{
    /*
      We have to do this with the start and end markers to prevent and annoying save/load bug
      where the markers disappear and reappear upong loading over and over
    */
    if (!_startMarker.isNull())
    {
        disconnect(_startMarker.data(),
                   SIGNAL(destroyed()),
                   this,
                   SLOT(handleStartMarkerDestroyed()));
        _startMarker->deleteLater();
        _startMarker = 0;
    }

    if (!_endMarker.isNull())
    {
        disconnect(_endMarker.data(),
                   SIGNAL(destroyed()),
                   this,
                   SLOT(handleEndMarkerDestroyed()));
        _endMarker->deleteLater();
        _endMarker = 0;
    }

    _problem = problem;
    if (problem.isNull())
        return;

    PlanningProblem * raw = problem.data();


    connect(raw,
            SIGNAL(startPositionChanged(Position)),
            this,
            SLOT(handleStartPositionChanged(Position)));
    connect(raw,
            SIGNAL(startPositionRemoved()),
            this,
            SLOT(handleStartPositionRemoved()));

    connect(raw,
            SIGNAL(endPositionChanged(Position)),
            this,
            SLOT(handleEndPositionChanged(Position)));
    connect(raw,
            SIGNAL(endPositionRemoved()),
            this,
            SLOT(handleEndPositionRemoved()));

    connect(raw,
            SIGNAL(areaAdded(QSharedPointer<TaskArea>)),
            this,
            SLOT(handleAreaAdded(QSharedPointer<TaskArea>)));
}

//private slot
void ProblemModelAdapter::handleStartPositionChanged(const Position &pos)
{
    if (_startMarker.isNull())
    {
        _startMarker = new CircleObject(START_END_MARKER_RADIUS,
                                        true,
                                        Qt::green);
        _scene->addObject(_startMarker);
        connect(_startMarker.data(),
                SIGNAL(posChanged()),
                this,
                SLOT(handleStartMarkerMoved()));
        connect(_startMarker.data(),
                SIGNAL(destroyed()),
                this,
                SLOT(handleStartMarkerDestroyed()));
    }
    _startMarker->setPos(pos.lonLat());
}

//private slot
void ProblemModelAdapter::handleStartPositionRemoved()
{
    if (_startMarker.isNull())
        return;
    _startMarker->deleteLater();
    _startMarker = 0;
}

//private slot
void ProblemModelAdapter::handleEndPositionChanged(const Position &pos)
{
    if (_endMarker.isNull())
    {
        _endMarker = new CircleObject(START_END_MARKER_RADIUS,
                                      true,
                                      Qt::red);
        _scene->addObject(_endMarker);
        connect(_endMarker.data(),
                SIGNAL(posChanged()),
                this,
                SLOT(handleEndMarkerMoved()));
        connect(_endMarker.data(),
                SIGNAL(destroyed()),
                this,
                SLOT(handleEndMarkerDestroyed()));
    }
    _endMarker->setPos(pos.lonLat());
}

//private slot
void ProblemModelAdapter::handleEndPositionRemoved()
{
    if (_endMarker.isNull())
        return;
    _endMarker->deleteLater();
    _endMarker = 0;
}

//private slot
void ProblemModelAdapter::handleStartMarkerMoved()
{
    if (_startMarker.isNull())
        return;

    QPointF geoPos = _startMarker->pos();
    Position pos(geoPos,1500);

    QSharedPointer<PlanningProblem> strongProblem = _problem.toStrongRef();
    if (strongProblem.isNull())
        return;

    strongProblem->setStartPosition(pos);
}

//private slot
void ProblemModelAdapter::handleStartMarkerDestroyed()
{
    QSharedPointer<PlanningProblem> strongProblem = _problem.toStrongRef();
    if (strongProblem.isNull())
        return;

    strongProblem->clearStartPosition();
}

//private slot
void ProblemModelAdapter::handleEndMarkerMoved()
{
    if (_endMarker.isNull())
        return;

    QPointF geoPos = _endMarker->pos();
    Position pos(geoPos,1500);

    QSharedPointer<PlanningProblem> strongProblem = _problem.toStrongRef();
    if (strongProblem.isNull())
        return;

    strongProblem->setEndPosition(pos);
}

//private slot
void ProblemModelAdapter::handleEndMarkerDestroyed()
{
    QSharedPointer<PlanningProblem> strongProblem = _problem.toStrongRef();
    if (strongProblem.isNull())
        return;

    strongProblem->clearEndPosition();
}

void ProblemModelAdapter::handleAreaAdded(QSharedPointer<TaskArea> area)
{
    TaskAreaObject * obj = new TaskAreaObject(area.toWeakRef());
    _scene->addObject(obj);
}
