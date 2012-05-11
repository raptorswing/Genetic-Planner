#ifndef PLANNINGPROBLEMDISPLAYADAPTER_H
#define PLANNINGPROBLEMDISPLAYADAPTER_H

#include <QObject>
#include <QPointF>
#include <QPointer>

#include "MapGraphicsScene.h"
#include "PlanningProblem.h"

class PlanningProblemDisplayAdapter : public QObject
{
    Q_OBJECT
public:
    explicit PlanningProblemDisplayAdapter(const PlanningProblem& problem, MapGraphicsScene * mgScene, QObject *parent = 0);

    void setPlanningProblem(const PlanningProblem& nProblem);
    PlanningProblem planningProblem() const;
    
signals:
    
public slots:
    void setStartPosition(const QPointF& geoPos, const qreal& altitude);
    void removeStartPosition();
    void setEndPosition(const QPointF& geoPos, const qreal& altitude);
    void removeEndPosition();

private slots:
    void handleStartPositionObjectPosChanged();
    void handleEndPositionObjectPosChanged();

private:
    PlanningProblem _problem;
    MapGraphicsScene * _mgScene;

    QPointer<MapGraphicsObject> _startPositionObject;
    QPointer<MapGraphicsObject> _endPositionObject;
    
};

#endif // PLANNINGPROBLEMDISPLAYADAPTER_H
