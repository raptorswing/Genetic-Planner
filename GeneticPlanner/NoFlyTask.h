#ifndef NOFLYTASK_H
#define NOFLYTASK_H

#include <QPointer>

#include "PathTask.h"
#include "PolygonObject.h"

class NoFlyTask : public PathTask
{
public:
    NoFlyTask(PolygonObject * polyObj);

    virtual qreal performance(const QList<QPointF>& positions);

private:
    QPointer<PolygonObject> _polyObj;
};

#endif // NOFLYTASK_H
