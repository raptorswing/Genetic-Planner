#ifndef PATHTASK_H
#define PATHTASK_H

#include <QList>
#include <QPointF>
#include <QtGlobal>
#include <QSharedPointer>

class PathTask
{
public:
    PathTask();
    virtual ~PathTask();

    virtual qreal performance(const QList<QPointF>& positions)=0;
    virtual QString taskType() const=0;

    virtual QSharedPointer<PathTask> copy() const=0;

protected:
    static qreal normal(qreal x, qreal stdDev,qreal scaleFactor=1000.0);
};

#endif // PATHTASK_H
