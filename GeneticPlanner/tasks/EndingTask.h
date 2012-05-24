#ifndef ENDINGTASK_H
#define ENDINGTASK_H

#include "PathTask.h"

class EndingTask : public PathTask
{
public:
    EndingTask(const QPointF& endingPos, const qreal& endingAlt, qreal withinDistance=15.0);
    EndingTask(QDataStream& stream);

    virtual qreal performance(const QList<QPointF>& positions);

    virtual QString taskType() const;

    virtual QSharedPointer<PathTask> copy() const;

    virtual void serialize(QDataStream& stream);

    void setEndingPos(QPointF endingPos);

private:
    QPointF _endingPos;
    qreal _endingAlt;
    qreal _stdDev;
};

#endif // ENDINGTASK_H
