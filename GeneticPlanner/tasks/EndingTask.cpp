#include "EndingTask.h"

#include <QVector3D>

#include "guts/Conversions.h"

EndingTask::EndingTask(const QPolygonF& geoPoly) :
    PathTask(geoPoly)
{
}

EndingTask::EndingTask(QDataStream &stream)
{
    stream >> _area;
}

qreal EndingTask::performance(const QList<Position> &positions)
{
    qreal goalScore = 0.0;
    const QPointF goalPoint = this->geoPoly().boundingRect().center();
    const QPointF geoPos = positions.last().lonLat();
    QVector3D enuPos = Conversions::lla2enu(geoPos.y(),geoPos.x(),_alt,
                                            goalPoint.y(),goalPoint.x(),_alt);
    const qreal stdDev = 30.0;
    qreal dist = enuPos.length();

    if (this->geoPoly().containsPoint(geoPos,Qt::OddEvenFill))
        goalScore += 500;
    else
        goalScore += 10*PathTask::normal(dist,stdDev);

    return qMin<qreal>(500.0,goalScore);
}

QString EndingTask::taskType() const
{
    return "Ending";
}

QSharedPointer<PathTask> EndingTask::copy() const
{
    return QSharedPointer<PathTask>(new EndingTask(_area));
}

void EndingTask::serialize(QDataStream &stream)
{
    stream << _area;
}

bool EndingTask::shortnessRewardApplies() const
{
    return true;
}

void EndingTask::setEndingArea(const QPolygonF& area)
{
    _area = area;
}
