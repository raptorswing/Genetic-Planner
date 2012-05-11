#include "EndingTask.h"

#include <QVector3D>

#include "guts/Conversions.h"

EndingTask::EndingTask(const QPointF &endingPos, qreal withinDistance) :
    _endingPos(endingPos), _stdDev(withinDistance)
{
}

qreal EndingTask::performance(const QList<QPointF> &positions)
{
    qreal goalScore = 0.0;
    const QPointF goalPoint = _endingPos;
    const QPointF geoPos = positions.last();
    QVector3D enuPos = Conversions::lla2enu(geoPos.y(),geoPos.x(),1409,
                                            goalPoint.y(),goalPoint.x(),1409);
    qreal dist = enuPos.length();

    if (dist < _stdDev)
        goalScore += 500;
    else
        goalScore += 10*PathTask::normal(dist,_stdDev);

    return qMin<qreal>(500.0,goalScore);
}

void EndingTask::setEndingPos(QPointF endingPos)
{
    _endingPos = endingPos;
}
