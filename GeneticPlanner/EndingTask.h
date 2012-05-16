#ifndef ENDINGTASK_H
#define ENDINGTASK_H

#include "PathTask.h"

class EndingTask : public PathTask
{
public:
    EndingTask(const QPointF& endingPos, const qreal& endingAlt, qreal withinDistance=30.0);

    virtual qreal performance(const QList<QPointF>& positions);

    virtual QString taskType() const;

    virtual QSharedPointer<PathTask> copy() const;

    void setEndingPos(QPointF endingPos);

private:
    QPointF _endingPos;
    qreal _endingAlt;
    qreal _stdDev;
};

#endif // ENDINGTASK_H
