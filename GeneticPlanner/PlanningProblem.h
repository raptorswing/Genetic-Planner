#ifndef PLANNINGPROBLEM_H
#define PLANNINGPROBLEM_H

#include <QPointF>
#include <QList>
#include <QPointer>
#include <QObject>

#include "UAVParameters.h"
#include "SensorDefinition.h"
#include "Individual.h"
#include "PathTask.h"
#include "PolygonObject.h"

class PlanningProblem : public QObject
{
    Q_OBJECT
public:
    PlanningProblem(QObject * parent = 0);
    PlanningProblem(const UAVParameters& uavParams,
                    const SensorDefinition& sensorParams,
                    QObject * parent = 0);

    bool isReady() const;

    qreal fitness(const Individual& individual);

    UAVParameters uavSettings() const;
    SensorDefinition sensorSettings() const;

    bool isStartingDefined() const;
    QPointF startingPos() const;
    qreal startingAlt() const;

    bool isEndingDefined() const;
    QPointF endingPos() const;
    qreal endingAlt() const;

    void setUAVSettings(const UAVParameters& uavParams);
    void setSensorSettings(const SensorDefinition& sensorParams);

    void setStartingPos(QPointF startingPos,qreal startingAlt=1423);
    void setEndingPos(QPointF endingPos, qreal endingAlt=1423);

    void addTask(PathTask * pathTask);

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
    PathTask * _endingTask;
};

#endif // PLANNINGPROBLEM_H
