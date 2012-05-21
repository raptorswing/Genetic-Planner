#ifndef FLYTHROUGHTASK_H
#define FLYTHROUGHTASK_H

#include <QPolygonF>

#include "PathTask.h"

class FlyThroughTask : public PathTask
{
public:
    FlyThroughTask(QPolygonF area, qreal flyThroughAlt);
    FlyThroughTask(QDataStream& stream);
    virtual ~FlyThroughTask();

    virtual qreal performance(const QList<QPointF>& positions);

    virtual QSharedPointer<PathTask> copy() const;

    virtual QString taskType() const;

    virtual void serialize(QDataStream& stream);

    QPolygonF area() const;
    qreal alt() const;

private:
    QPolygonF _area;
    qreal _flyThroughAlt;
};

#endif // FLYTHROUGHTASK_H
