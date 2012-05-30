#ifndef PATHTASK_H
#define PATHTASK_H

#include <QList>
#include <QPointF>
#include <QtGlobal>
#include <QSharedPointer>
#include <QDataStream>
#include <QObject>
#include <QPolygonF>

#include "Position.h"

class PathTask : public QObject
{
    Q_OBJECT
public:
    PathTask(const QPolygonF& geoPoly = QPolygonF());
    virtual ~PathTask();

    virtual qreal performance(const QList<Position>& positions)=0;
    virtual QString taskType() const=0;

    virtual QSharedPointer<PathTask> copy() const=0;

    virtual void serialize(QDataStream& stream)=0;

    virtual bool shortnessRewardApplies() const;

    QPolygonF geoPoly() const;

signals:
    void changed();

protected:
    static qreal normal(qreal x, qreal stdDev,qreal scaleFactor=1000.0);

public slots:
    void setGeoPoly(const QPolygonF& newPoly);

private:
    QPolygonF _geoPoly;
};

#endif // PATHTASK_H
