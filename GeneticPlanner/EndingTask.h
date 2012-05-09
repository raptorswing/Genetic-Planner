#ifndef ENDINGTASK_H
#define ENDINGTASK_H

#include "PathTask.h"

class EndingTask : public PathTask
{
public:
    EndingTask(const QPointF& endingPos, qreal withinDistance=30.0);

    virtual qreal performance(const QList<QPointF>& positions);

private:
    QPointF _endingPos;
    qreal _stdDev;
};

#endif // ENDINGTASK_H
