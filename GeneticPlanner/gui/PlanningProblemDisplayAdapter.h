#ifndef PLANNINGPROBLEMDISPLAYADAPTER_H
#define PLANNINGPROBLEMDISPLAYADAPTER_H

#include <QObject>
#include <QPointF>
#include <QPointer>
#include <QHash>

#include "MapGraphicsScene.h"
#include "PlanningProblem.h"
class TaskAreaObject;

class PlanningProblemDisplayAdapter : public QObject
{
    Q_OBJECT
public:
    explicit PlanningProblemDisplayAdapter(const PlanningProblem& problem, MapGraphicsScene * mgScene, QObject *parent = 0);

    void setPlanningProblem(const PlanningProblem& nProblem);
    PlanningProblem planningProblem() const;
    
signals:
    void problemHasChanged(const PlanningProblem& problem);
    
public slots:
    void setStartPosition(const QPointF& geoPos, const qreal& altitude);
    void removeStartPosition();
    void setEndPosition(const QPointF& geoPos, const qreal& altitude);
    void removeEndPosition();

    void addArea(const QPointF& center);
    void addArea(const QPolygonF& poly);

private slots:
    void handleStartPositionObjectPosChanged();
    void handleEndPositionObjectPosChanged();
    void handleAreaChanged();
    void handleAreaDestroyed();

private:
    void clear();
    PlanningProblem _problem;
    MapGraphicsScene * _mgScene;

    QPointer<MapGraphicsObject> _startPositionObject;
    QPointer<MapGraphicsObject> _endPositionObject;

    QHash<TaskAreaObject *, QPolygonF> _areas;
    
};

#endif // PLANNINGPROBLEMDISPLAYADAPTER_H
