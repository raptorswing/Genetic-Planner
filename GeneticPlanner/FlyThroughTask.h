#ifndef FLYTHROUGHTASK_H
#define FLYTHROUGHTASK_H

#include <QPointF>

#include "PathTask.h"

class FlyThroughTask : public PathTask
{
public:
    FlyThroughTask(QPointF flyThroughPoint, qreal flyThroughAlt, qreal withinDistance = 30.0);
    virtual ~FlyThroughTask();

    virtual qreal performance(const QList<QPointF>& positions);

private:
    QPointF _flyThroughPoint;
    qreal _flyThroughAlt;
    qreal _stdDev;
};

#endif // FLYTHROUGHTASK_H
