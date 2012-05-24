#ifndef PLANNINGPROBLEM_H
#define PLANNINGPROBLEM_H

#include <QPointF>
#include <QList>
#include <QPointer>
#include <QDataStream>
#include <QSet>
#include <QSharedPointer>
#include <QObject>

#include "UAVParameters.h"
#include "SensorDefinition.h"
#include "Individual.h"
#include "tasks/PathTask.h"
#include "PolygonObject.h"
#include "tasks/EndingTask.h"
#include "TaskArea.h"
#include "Position.h"

class PlanningProblem : public QObject
{
    Q_OBJECT
public:
    PlanningProblem();
    ~PlanningProblem();

    bool isReady() const;

    qreal fitness(QSharedPointer<Individual> individual);

    QSet<QSharedPointer<TaskArea> > areas() const;

    bool isStartDefined() const;
    Position startingPosition() const;

    bool isEndDefined() const;
    Position endingPosition() const;

public slots:
    void addArea(QSharedPointer<TaskArea> area);
    void removeArea(QSharedPointer<TaskArea> area);

    void setStartPosition(const Position& pos);
    void clearStartPosition();

    void setEndPosition(const Position& pos);
    void clearEndPosition();

signals:
    void changed();

    void startPositionChanged(const Position& pos);
    void endPositionChanged(const Position& pos);

    void startPositionRemoved();
    void endPositionRemoved();

    void areaAdded(QSharedPointer<TaskArea> area);

private:
    PlanningProblem(const PlanningProblem& other);
    PlanningProblem& operator=(const PlanningProblem& other);

    QSet<QSharedPointer<TaskArea> > _areas;

    Position _startPos;
    bool _startIsDefined;

    Position _endPos;
    bool _endIsDefined;
};

QDataStream & operator<< (QDataStream& stream, const PlanningProblem& problem);
QDataStream & operator>> (QDataStream& stream, PlanningProblem& problem);
QDataStream & operator<< (QDataStream& stream, const QSharedPointer<PathTask>& problem);
QDataStream & operator>> (QDataStream& stream, QSharedPointer<PathTask>& problem);


bool operator==(const TaskArea& A, const TaskArea& B);
bool operator!=(const TaskArea& A, const TaskArea& B);
uint qHash(const TaskArea& area);

#endif // PLANNINGPROBLEM_H
