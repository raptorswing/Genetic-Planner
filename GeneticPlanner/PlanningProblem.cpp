#include "PlanningProblem.h"

#include "FlyThroughTask.h"
#include "EndingTask.h"
#include "NoFlyTask.h"

#include <QtDebug>

PlanningProblem::PlanningProblem()
{
    _isStartingDefined = false;
    _isEndingDefined = false;
}

PlanningProblem::PlanningProblem(const UAVParameters &uavParams, const SensorDefinition &sensorParams)
{
    this->setUAVSettings(uavParams);
    this->setSensorSettings(sensorParams);

    _isStartingDefined = false;
    _isEndingDefined = false;
}

PlanningProblem::PlanningProblem(const PlanningProblem &other)
{
    _isStartingDefined = false;
    _isEndingDefined = false;


    //Starting and end position
    if (other.isStartingDefined())
        this->setStartingPos(other.startingPos(),other.startingAlt());

    //The ending point will define the ending task for us, too
    if (other.isEndingDefined())
        this->setEndingPos(other.endingPos(),other.endingAlt());

    //UAV and sensor settings
    this->setUAVSettings(other.uavSettings());
    this->setSensorSettings(other.sensorSettings());

    //Copy Tasks
    foreach(QSharedPointer<PathTask> task, other.tasks())
        this->addTask(task->copy());
    foreach(QSharedPointer<PathTask> task, other.secondaryTasks())
        this->addTask(task->copy(),true);

    //Areas
    _areas = other.areas();
}

PlanningProblem::~PlanningProblem()
{

}

bool PlanningProblem::isReady() const
{
    return (this->isStartingDefined());
}

qreal PlanningProblem::fitness(QSharedPointer<Individual> individual)
{
    if (!this->isReady())
        return 0.0;

    qreal toRet = 0.0;

    //Get the geo points of the path and run them all through our tasks!
    QList<QPointF> geoPositions = individual->generateGeoPoints(this->startingPos());

    foreach(QSharedPointer<PathTask> task, _tasks)
    {
        qreal temp = task->performance(geoPositions);
        toRet += temp;
    }

    //Do the "ending task" if applicable
    if (_endingTask)
        toRet += _endingTask->performance(geoPositions);

    //Punish long paths
    {
        qreal shortReward = 10 * toRet / geoPositions.length();
        toRet += shortReward;
    }

    /*
      We add reward for secondary tasks after the "long path punishment" to avoid amplifying reward from
      simply not flying somewhere.
    */
    foreach(QSharedPointer<PathTask> task, _secondaryTasks)
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
    _endingTask.clear();
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

    if (_endingTask.isNull())
        _endingTask = QSharedPointer<EndingTask>(new EndingTask(endingPos,15.0));
    else
        _endingTask->setEndingPos(endingPos);
}

void PlanningProblem::addTask(QSharedPointer<PathTask> pathTask, bool secondary)
{
    if (!pathTask)
        return;

    if (!secondary)
        _tasks.append(pathTask);
    else
        _secondaryTasks.append(pathTask);
}

QList<QSharedPointer<PathTask> > PlanningProblem::tasks() const
{
    return _tasks;
}

QList<QSharedPointer<PathTask> > PlanningProblem::secondaryTasks() const
{
    return _secondaryTasks;
}

void PlanningProblem::addArea(const TaskArea &area)
{
    _areas.insert(area);
    if (!_areas.contains(area))
        qWarning() << "Insertion of area failed";
}

void PlanningProblem::setAreas(const QSet<TaskArea> &toSet)
{
    _areas = toSet;
}

void PlanningProblem::removeArea(const TaskArea &toRemove)
{
    if (!_areas.remove(toRemove))
        qWarning() << this << "failed to remove area. Size is now" << _areas.size();
}

QSet<TaskArea> PlanningProblem::areas() const
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

    //stream << problem.areas();

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

    /*
    QSet<TaskArea> areas;
    stream >> areas;
    problem.setAreas(areas);
    */

    return stream;
}

QDataStream & operator<< (QDataStream& stream, const QSharedPointer<PathTask>& problem)
{
    QString type = problem->taskType();

    stream << type;
    problem->serialize(stream);

    return stream;
}

QDataStream & operator>> (QDataStream& stream, QSharedPointer<PathTask>& problem)
{
    QString type;
    stream >> type;

    if (type == "FlyThrough")
        problem = QSharedPointer<PathTask>(new FlyThroughTask(stream));
    else if (type == "NoFly")
        problem = QSharedPointer<PathTask>(new NoFlyTask(stream));
    else if (type == "Ending")
        problem = QSharedPointer<PathTask>(new EndingTask(stream));
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

bool operator==(const TaskArea& A, const TaskArea& B)
{
    if (A.polygon() != B.polygon())
        return false;

    QList<QSharedPointer<PathTask> > aTasks = A.tasks();
    QList<QSharedPointer<PathTask> > bTasks = B.tasks();
    if (aTasks.size() != bTasks.size())
        return false;

    for (int i = 0; i < aTasks.size(); i++)
    {
        QSharedPointer<PathTask> task = aTasks[i];
        if (!bTasks.contains(task))
            return false;
    }
    return true;
}

bool operator!=(const TaskArea& A, const TaskArea& B)
{
    return !(A == B);
}

uint qHash(const TaskArea& area)
{
    uint toRet = 0;

    QList<QSharedPointer<PathTask> > tasks = area.tasks();
    foreach(QSharedPointer<PathTask> task, tasks)
        toRet += qHash(task->taskType());

    toRet ^= qHash(area.polygon());

    return toRet;
}
