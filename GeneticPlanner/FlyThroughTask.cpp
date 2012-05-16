#include "FlyThroughTask.h"

#include "guts/Conversions.h"

FlyThroughTask::FlyThroughTask(QPointF flyThroughPoint, qreal flyThroughAlt, qreal withinDistance) :
    _flyThroughPoint(flyThroughPoint), _flyThroughAlt(flyThroughAlt), _stdDev(withinDistance)
{
}

FlyThroughTask::~FlyThroughTask()
{
}

qreal FlyThroughTask::performance(const QList<QPointF> &positions)
{
    qreal goalScore = 0.0;

    const QPointF goalPoint = _flyThroughPoint;
    const qreal stdDev = _stdDev;

    quint64 withinStdDevCount = 0;
    quint64 goalWithinStdDev = 1;

    foreach(QPointF geoPos, positions)
    {
        QVector3D enuPos = Conversions::lla2enu(geoPos.y(),geoPos.x(),_flyThroughAlt,
                                                goalPoint.y(),goalPoint.x(),_flyThroughAlt);
        qreal dist = enuPos.length();

        if (dist < stdDev)
        {
            if (++withinStdDevCount >= goalWithinStdDev)
            {
                goalScore += 500;
                break;
            }
        }
        goalScore += 10*PathTask::normal(dist,stdDev);
    }

    goalScore = qMin<qreal>(500.0,goalScore);

    return goalScore;
}

QSharedPointer<PathTask> FlyThroughTask::copy() const
{
    return QSharedPointer<PathTask>(new FlyThroughTask(_flyThroughPoint,
                                                       _flyThroughAlt,
                                                       _stdDev));
}

QString FlyThroughTask::taskType() const
{
    return "FlyThrough";
}
