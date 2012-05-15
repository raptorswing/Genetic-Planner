#include "NoFlyTask.h"

#include <QVector3D>

#include "guts/Conversions.h"

NoFlyTask::NoFlyTask(QPolygonF geoPoly) :
    _geoPoly(geoPoly)
{
}

qreal NoFlyTask::performance(const QList<QPointF> &positions)
{
    qreal goalScore = 500.0;

    foreach(QPointF geoPos, positions)
    {
        if (_geoPoly.containsPoint(geoPos,Qt::OddEvenFill))
        {
            goalScore -= 10;
            if (goalScore <= 0)
                break;
        }
    }

    goalScore = qMax<qreal>(0.0,qMin<qreal>(500.0,goalScore));

    return goalScore;
}

QString NoFlyTask::taskType() const
{
    return "NoFly";
}
