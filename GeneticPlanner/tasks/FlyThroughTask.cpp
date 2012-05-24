#include "FlyThroughTask.h"

#include "guts/Conversions.h"

FlyThroughTask::FlyThroughTask(QPolygonF area, qreal flyThroughAlt) :
    _area(area), _flyThroughAlt(flyThroughAlt)
{
}

FlyThroughTask::FlyThroughTask(QDataStream &stream)
{
    stream >> _area;
    stream >> _flyThroughAlt;
}

FlyThroughTask::~FlyThroughTask()
{
}

qreal FlyThroughTask::performance(const QList<QPointF> &positions)
{
    qreal goalScore = 0.0;

    const QPointF goalPoint = _area.boundingRect().center();
    const qreal stdDev = 30.0;

    qreal closestDist = 5000000;
    foreach(const QPointF geoPos, positions)
    {

        QVector3D enuPos = Conversions::lla2enu(geoPos.y(),geoPos.x(),_flyThroughAlt,
                                                goalPoint.y(),goalPoint.x(),_flyThroughAlt);
        qreal dist = enuPos.length();
        if (dist < closestDist)
            closestDist = dist;

        if (_area.containsPoint(geoPos,Qt::OddEvenFill))
            return 500.0;
    }

    goalScore += 10*PathTask::normal(closestDist,stdDev);

    goalScore = qMin<qreal>(500.0,goalScore);

    return goalScore;
}

QSharedPointer<PathTask> FlyThroughTask::copy() const
{
    return QSharedPointer<PathTask>(new FlyThroughTask(_area,
                                                       _flyThroughAlt));
}

QString FlyThroughTask::taskType() const
{
    return "FlyThrough";
}

void FlyThroughTask::serialize(QDataStream &stream)
{
    stream << _area;
    stream << _flyThroughAlt;
}

QPolygonF FlyThroughTask::area() const
{
    return _area;
}

qreal FlyThroughTask::alt() const
{
    return _flyThroughAlt;
}
