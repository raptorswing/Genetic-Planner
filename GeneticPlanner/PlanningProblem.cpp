#include "PlanningProblem.h"

#include "FlyThroughTask.h"
#include "EndingTask.h"

#include <QtDebug>

PlanningProblem::PlanningProblem()
{
    _isStartingDefined = false;
    _isEndingDefined = false;
    _endingTask = 0;
}

PlanningProblem::PlanningProblem(const UAVParameters &uavParams, const SensorDefinition &sensorParams)
{
    this->setUAVSettings(uavParams);
    this->setSensorSettings(sensorParams);

    _isStartingDefined = false;
    _isEndingDefined = false;
    _endingTask = 0;
}

PlanningProblem::~PlanningProblem()
{
    foreach(PathTask * task, _tasks)
        delete task;
    _tasks.clear();

    foreach(PathTask * task, _secondaryTasks)
        delete task;
    _secondaryTasks.clear();
}

bool PlanningProblem::isReady() const
{
    return (this->isStartingDefined());
}

qreal PlanningProblem::fitness(const Individual &individual)
{
    if (!this->isReady())
        return 0.0;

    qreal toRet = 0.0;

    //Get the geo points of the path and run them all through our tasks!
    QList<QPointF> geoPositions = individual.generateGeoPoints(this->startingPos());

    foreach(PathTask * task, _tasks)
        toRet += task->performance(geoPositions);

    //Do the "ending task" if applicable
    if (_endingTask)
    {
        toRet += _endingTask->performance(geoPositions);
    }

    //Punish long paths
    {
        qreal shortReward = 10 * toRet / geoPositions.length();
        toRet += shortReward;
    }

    /*
      We add reward for secondary tasks after the "long path punishment" to avoid amplifying reward from
      simply not flying somewhere.
    */
    foreach(PathTask * task, _secondaryTasks)
        toRet += task->performance(geoPositions);

    return toRet;
}

UAVParameters PlanningProblem::uavSettings() const
{
    return _uavSettings;
}

SensorDefinition PlanningProblem::sensorSettings() const
{
    return _sensorSettings;
}

bool PlanningProblem::isStartingDefined() const
{
    return _isStartingDefined;
}

QPointF PlanningProblem::startingPos() const
{
    return _startingPos;
}

qreal PlanningProblem::startingAlt() const
{
    return _startingAlt;
}

void PlanningProblem::clearStartingPos()
{
    _isStartingDefined = false;
}

bool PlanningProblem::isEndingDefined() const
{
    return _isEndingDefined;
}

QPointF PlanningProblem::endingPos() const
{
    return _endingPos;
}

qreal PlanningProblem::endingAlt() const
{
    return _endingAlt;
}

void PlanningProblem::clearEndingPos()
{
    _isEndingDefined = false;
}

void PlanningProblem::setUAVSettings(const UAVParameters &uavParams)
{
    _uavSettings = uavParams;
}

void PlanningProblem::setSensorSettings(const SensorDefinition &sensorParams)
{
    _sensorSettings = sensorParams;
}

void PlanningProblem::setStartingPos(QPointF startingPos, qreal startingAlt)
{
    _startingPos = startingPos;
    _startingAlt = startingAlt;
    _isStartingDefined = true;
}

void PlanningProblem::setEndingPos(QPointF endingPos, qreal endingAlt)
{
    _endingPos = endingPos;
    _endingAlt = endingAlt;
    _isEndingDefined = true;

    if (!_endingTask)
        _endingTask = new EndingTask(endingPos,15.0);
    else
        _endingTask->setEndingPos(endingPos);
}

void PlanningProblem::addTask(PathTask *pathTask, bool secondary)
{
    if (!pathTask)
        return;

    if (!secondary)
        _tasks.append(pathTask);
    else
        _secondaryTasks.append(pathTask);
}

void PlanningProblem::addArea(const QPolygonF &geoPoly)
{
    _areas.insert(geoPoly);
    if (!_areas.contains(geoPoly))
        qWarning() << "Insertion of" << geoPoly << "failed";
}

void PlanningProblem::setAreas(const QSet<QPolygonF> &toSet)
{
    _areas = toSet;
}

void PlanningProblem::removeArea(const QPolygonF &toRemove)
{
    if (!_areas.remove(toRemove))
        qWarning() << this << "failed to remove area" << toRemove << ". Size is now" << _areas.size();
}

QSet<QPolygonF> PlanningProblem::areas() const
{
    return _areas;
}

QDataStream & operator<< (QDataStream& stream, const PlanningProblem& problem)
{
    stream << problem.uavSettings();
    stream << problem.sensorSettings();

    stream << problem.isStartingDefined();
    stream << problem.startingPos();
    stream << problem.startingAlt();

    stream << problem.isEndingDefined();
    stream << problem.endingPos();
    stream << problem.endingAlt();

    stream << problem.areas();


    return stream;
}

QDataStream & operator>> (QDataStream& stream, PlanningProblem& problem)
{
    UAVParameters uavSettings;
    stream >> uavSettings;

    SensorDefinition sensorSettings;
    stream >> sensorSettings;

    bool isStartingDefined;
    stream >> isStartingDefined;

    QPointF startingPos;
    stream >> startingPos;

    qreal startingAlt;
    stream >> startingAlt;

    bool isEndingDefined;
    stream >> isEndingDefined;

    QPointF endingPos;
    stream >> endingPos;

    qreal endingAlt;
    stream >> endingAlt;

    problem.setUAVSettings(uavSettings);
    problem.setSensorSettings(sensorSettings);

    if (isStartingDefined)
        problem.setStartingPos(startingPos,startingAlt);
    else
        problem.clearStartingPos();

    if (isEndingDefined)
        problem.setEndingPos(endingPos,endingAlt);
    else
        problem.clearEndingPos();

    QSet<QPolygonF> areas;
    stream >> areas;
    problem.setAreas(areas);


    return stream;
}

bool operator==(const QPolygonF& A, const QPolygonF& B)
{
    if (A.size() != B.size())
        return false;

    for (int i = 0; i < A.size(); i++)
    {
        QPointF a = A[i];
        QPointF b = B[i];
        if (a != b)
            return false;
    }

    return true;
}

bool operator!=(const QPolygonF& A, const QPolygonF& B)
{
    return !(A == B);
}

uint qHash(const QPolygonF& poly)
{
    uint toRet = 0;

    for (int i = 0; i < poly.size(); i++)
    {
        const QPointF& pos = poly.at(i);
        toRet ^= qRound(pos.x() + pos.y());
    }
    return toRet;
}
