#ifndef ENDINGTASK_H
#define ENDINGTASK_H

#include "PathTask.h"

class EndingTask : public PathTask
{
public:
    EndingTask(const QPolygonF &geoPoly);
    EndingTask(QDataStream& stream);

    virtual qreal performance(const QList<Position>& positions);

    virtual QString taskType() const;

    virtual QSharedPointer<PathTask> copy() const;

    virtual void serialize(QDataStream& stream);

    virtual bool shortnessRewardApplies() const;

    void setEndingArea(const QPolygonF &area);

private:
    QPolygonF _area;
    qreal _alt;
};

#endif // ENDINGTASK_H
