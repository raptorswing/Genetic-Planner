#ifndef PLANNINGPROBLEM_H
#define PLANNINGPROBLEM_H

#include <QPointF>
#include <QList>
#include <QPointer>
#include <QDataStream>
#include <QSet>
#include <QSharedPointer>

#include "UAVParameters.h"
#include "SensorDefinition.h"
#include "Individual.h"
#include "PathTask.h"
#include "PolygonObject.h"
#include "EndingTask.h"

class PlanningProblem
{
public:
    PlanningProblem();
    PlanningProblem(const UAVParameters& uavParams,
                    const SensorDefinition& sensorParams);
    PlanningProblem(const PlanningProblem& other);
    ~PlanningProblem();

    bool isReady() const;

    qreal fitness(QSharedPointer<Individual> individual);

    UAVParameters uavSettings() const;
    SensorDefinition sensorSettings() const;

    bool isStartingDefined() const;
    QPointF startingPos() const;
    qreal startingAlt() const;
    void clearStartingPos();

    bool isEndingDefined() const;
    QPointF endingPos() const;
    qreal endingAlt() const;
    void clearEndingPos();

    void setUAVSettings(const UAVParameters& uavParams);
    void setSensorSettings(const SensorDefinition& sensorParams);

    void setStartingPos(QPointF startingPos,qreal startingAlt=1423);
    void setEndingPos(QPointF endingPos, qreal endingAlt=1423);

    void addTask(QSharedPointer<PathTask> pathTask,bool secondary=false);
    QList<QSharedPointer<PathTask> > tasks() const;
    QList<QSharedPointer<PathTask> > secondaryTasks() const;

    void addArea(const QPolygonF& geoPoly);
    void setAreas(const QSet<QPolygonF>& toSet);
    void removeArea(const QPolygonF& toRemove);
    QSet<QPolygonF> areas() const;

private:
    UAVParameters _uavSettings;
    SensorDefinition _sensorSettings;

    bool _isStartingDefined;
    QPointF _startingPos;
    qreal _startingAlt;

    bool _isEndingDefined;
    QPointF _endingPos;
    qreal _endingAlt;

    QList<QSharedPointer<PathTask> > _tasks;
    QList<QSharedPointer<PathTask> > _secondaryTasks;
    QSharedPointer<EndingTask> _endingTask;

    QSet<QPolygonF> _areas;
};

QDataStream & operator<< (QDataStream& stream, const PlanningProblem& problem);
QDataStream & operator>> (QDataStream& stream, PlanningProblem& problem);
QDataStream & operator<< (QDataStream& stream, const QSharedPointer<PathTask>& problem);
QDataStream & operator>> (QDataStream& stream, QSharedPointer<PathTask>& problem);

bool operator==(const QPolygonF& A, const QPolygonF& B);
bool operator!=(const QPolygonF& A, const QPolygonF& B);
uint qHash(const QPolygonF& poly);

#endif // PLANNINGPROBLEM_H
