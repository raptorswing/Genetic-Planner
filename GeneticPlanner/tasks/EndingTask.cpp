#include "EndingTask.h"

#include <QVector3D>

#include "guts/Conversions.h"

EndingTask::EndingTask(const QPointF &endingPos, const qreal& endingAlt, qreal withinDistance) :
    _endingPos(endingPos), _endingAlt(endingAlt), _stdDev(withinDistance)
{
}

EndingTask::EndingTask(QDataStream &stream)
{
    stream >> _endingPos;
    stream >> _endingAlt;
    stream >> _stdDev;
}

qreal EndingTask::performance(const QList<Position> &positions)
{
    qreal goalScore = 0.0;
    const QPointF goalPoint = _endingPos;
    const QPointF geoPos = positions.last().lonLat();
    QVector3D enuPos = Conversions::lla2enu(geoPos.y(),geoPos.x(),_endingAlt,
                                            goalPoint.y(),goalPoint.x(),_endingAlt);
    qreal dist = enuPos.length();

    if (dist < _stdDev)
        goalScore += 500;
    else
        goalScore += 10*PathTask::normal(dist,_stdDev);

    return qMin<qreal>(500.0,goalScore);
}

QString EndingTask::taskType() const
{
    return "Ending";
}

QSharedPointer<PathTask> EndingTask::copy() const
{
    return QSharedPointer<PathTask>(new EndingTask(_endingPos,
                                                   _endingAlt,
                                                   _stdDev));
}

void EndingTask::serialize(QDataStream &stream)
{
    stream << _endingPos;
    stream << _endingAlt;
    stream << _stdDev;
}

void EndingTask::setEndingPos(QPointF endingPos)
{
    _endingPos = endingPos;
}
