#ifndef PLANNINGPROBLEM_H
#define PLANNINGPROBLEM_H

#include <QPointF>
#include <QList>
#include <QPointer>
#include <QDataStream>

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
    ~PlanningProblem();

    bool isReady() const;

    qreal fitness(const Individual& individual);

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

    void addTask(PathTask * pathTask,bool secondary=false);

private:
    UAVParameters _uavSettings;
    SensorDefinition _sensorSettings;

    bool _isStartingDefined;
    QPointF _startingPos;
    qreal _startingAlt;

    bool _isEndingDefined;
    QPointF _endingPos;
    qreal _endingAlt;

    QList<PathTask *> _tasks;
    QList<PathTask *> _secondaryTasks;
    EndingTask * _endingTask;
};

QDataStream & operator<< (QDataStream& stream, const PlanningProblem& problem);
QDataStream & operator>> (QDataStream& stream, PlanningProblem& problem);

#endif // PLANNINGPROBLEM_H
