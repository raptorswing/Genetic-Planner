#ifndef NOFLYTASK_H
#define NOFLYTASK_H

#include <QPolygonF>

#include "PathTask.h"
#include "PolygonObject.h"

class NoFlyTask : public PathTask
{
public:
    NoFlyTask(QPolygonF geoPoly);

    virtual qreal performance(const QList<QPointF>& positions);

    virtual QString taskType() const;

    virtual QSharedPointer<PathTask> copy() const;

private:
    QPolygonF _geoPoly;
};

#endif // NOFLYTASK_H
