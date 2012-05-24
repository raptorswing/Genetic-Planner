#include "PlanningProblem.h"

#include "tasks/FlyThroughTask.h"
#include "tasks/EndingTask.h"
#include "tasks/NoFlyTask.h"

#include <QtDebug>

PlanningProblem::PlanningProblem()
{
    _startIsDefined = false;
    _endIsDefined = false;
}

PlanningProblem::~PlanningProblem()
{

}

bool PlanningProblem::isReady() const
{
    //return (this->isStartingDefined());
    return false;
}

qreal PlanningProblem::fitness(QSharedPointer<Individual> individual)
{
    if (!this->isReady())
        return 0.0;

    qreal toRet = 0.0;

    return toRet;
}

QSet<QSharedPointer<TaskArea> > PlanningProblem::areas() const
{
    return _areas;
}

bool PlanningProblem::isStartDefined() const
{
    return _startIsDefined;
}

Position PlanningProblem::startingPosition() const
{
    return _startPos;
}

bool PlanningProblem::isEndDefined() const
{
    return _endIsDefined;
}

Position PlanningProblem::endingPosition() const
{
    return _endPos;
}

//public slot
void PlanningProblem::addArea(QSharedPointer<TaskArea> area)
{
    if (area.isNull())
        return;

    TaskArea * rawArea = area.data();
    connect(rawArea,
            SIGNAL(changed()),
            this,
            SIGNAL(changed()));

    _areas.insert(area);
    this->areaAdded(area);
    this->changed();
}

//public slot
void PlanningProblem::removeArea(QSharedPointer<TaskArea> area)
{
    if (area.isNull())
        return;

    _areas.remove(area);
    this->changed();
}

//public slot
void PlanningProblem::setStartPosition(const Position &pos)
{
    _startPos = pos;
    _startIsDefined = true;

    this->startPositionChanged(pos);
    this->changed();
}

//public slot
void PlanningProblem::clearStartPosition()
{
    _startIsDefined = false;
    this->startPositionRemoved();
    this->changed();
    qDebug() << "Start Area Cleared";
}

//public slot
void PlanningProblem::setEndPosition(const Position &pos)
{
    _endPos = pos;
    _endIsDefined = true;
    this->endPositionChanged(pos);
    this->changed();
}

//public slot
void PlanningProblem::clearEndPosition()
{
    _endIsDefined = false;
    this->endPositionRemoved();
    this->changed();
    qDebug() << "End Area Cleared";
}

//private
PlanningProblem::PlanningProblem(const PlanningProblem &other)
{
    Q_UNUSED(other)
    qWarning() << "Warning: disabled copy constructor called";
}

//private
PlanningProblem &PlanningProblem::operator =(const PlanningProblem &other)
{
    Q_UNUSED(other)
    qDebug() << "Warning: disabled assignment operator used";
    return *this;
}

QDataStream & operator<< (QDataStream& stream, const PlanningProblem& problem)
{
    //Areas
    {
        QSet<QSharedPointer<TaskArea> > areas = problem.areas();
        int numAreas = areas.size();

        stream << numAreas;
        foreach(QSharedPointer<TaskArea> area, areas)
            stream << *area.data();
    }

    //Starting position
    {
        stream << problem.isStartDefined();
        stream << problem.startingPosition();
    }

    //Ending position
    {
        stream << problem.isEndDefined();
        stream << problem.endingPosition();
    }

    return stream;
}

QDataStream & operator>> (QDataStream& stream, PlanningProblem& problem)
{
    //Areas
    {
        int numAreas;
        stream >> numAreas;

        for (int i = 0; i < numAreas; i++)
        {
            QSharedPointer<TaskArea> area(new TaskArea(QPolygonF()));
            stream >> *area;
            problem.addArea(area);
        }
    }

    //Starting Position
    {
        bool isDefined;
        Position pos;
        stream >> isDefined;
        stream >> pos;

        if (isDefined)
            problem.setStartPosition(pos);
        else
            problem.clearStartPosition();
    }

    //Ending Position
    {
        bool isDefined;
        Position pos;
        stream >> isDefined;
        stream >> pos;

        if (isDefined)
            problem.setEndPosition(pos);
        else
            problem.clearEndPosition();
    }

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

