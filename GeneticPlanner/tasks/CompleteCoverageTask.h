#ifndef COMPLETECOVERAGETASK_H
#define COMPLETECOVERAGETASK_H

#include "PathTask.h"

#include <QVector3D>

class CompleteCoverageTask : public PathTask
{
    Q_OBJECT
public:
    explicit CompleteCoverageTask(const QPolygonF & geoPoly);
    CompleteCoverageTask(QDataStream& stream);

    virtual qreal performance(const QList<Position>& positions);
    virtual QString taskType() const;

    virtual QSharedPointer<PathTask> copy() const;

    virtual void serialize(QDataStream& stream);

    virtual bool shortnessRewardApplies() const;
    
signals:
    
public slots:  
};

uint qHash(const QVector3D & vec);

#endif // COMPLETECOVERAGETASK_H
