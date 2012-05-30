#include "FlyThroughTask.h"

#include "guts/Conversions.h"

FlyThroughTask::FlyThroughTask(QPolygonF area, qreal flyThroughAlt) :
    PathTask(area), _flyThroughAlt(flyThroughAlt)
{
}

FlyThroughTask::FlyThroughTask(QDataStream &stream)
{
    QPolygonF area;
    stream >> area;
    this->setGeoPoly(area);
    stream >> _flyThroughAlt;
}

FlyThroughTask::~FlyThroughTask()
{
}

qreal FlyThroughTask::performance(const QList<Position> &positions)
{
    qreal goalScore = 0.0;

    const QPointF goalPoint = this->geoPoly().boundingRect().center();
    const qreal stdDev = 30.0;

    qreal closestDist = 5000000;
    foreach(const Position geoPos, positions)
    {

        QVector3D enuPos = Conversions::lla2enu(geoPos.latitude(),geoPos.longitude(),_flyThroughAlt,
                                                goalPoint.y(),goalPoint.x(),_flyThroughAlt);
        qreal dist = enuPos.length();
        if (dist < closestDist)
            closestDist = dist;

        if (this->geoPoly().containsPoint(geoPos.lonLat(),Qt::OddEvenFill))
            return 500.0;
    }

    goalScore += 10*PathTask::normal(closestDist,stdDev);

    goalScore = qMin<qreal>(500.0,goalScore);

    return goalScore;
}

QSharedPointer<PathTask> FlyThroughTask::copy() const
{
    return QSharedPointer<PathTask>(new FlyThroughTask(this->geoPoly(),
                                                       _flyThroughAlt));
}

QString FlyThroughTask::taskType() const
{
    return "FlyThrough";
}

void FlyThroughTask::serialize(QDataStream &stream)
{
    stream << this->geoPoly();
    stream << _flyThroughAlt;
}

qreal FlyThroughTask::alt() const
{
    return _flyThroughAlt;
}
