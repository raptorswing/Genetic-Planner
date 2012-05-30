#include "NoFlyTask.h"

#include <QVector3D>

#include "guts/Conversions.h"

NoFlyTask::NoFlyTask(QPolygonF geoPoly) :
    PathTask(geoPoly)
{
}

NoFlyTask::NoFlyTask(QDataStream &stream)
{
    QPolygonF poly;
    stream >> poly;
    this->setGeoPoly(poly);
}

qreal NoFlyTask::performance(const QList<Position> &positions)
{
    qreal goalScore = 500.0;

    foreach(Position geoPos, positions)
    {
        if (this->geoPoly().containsPoint(geoPos.lonLat(),Qt::OddEvenFill))
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

QSharedPointer<PathTask> NoFlyTask::copy() const
{
    return QSharedPointer<PathTask>(new NoFlyTask(this->geoPoly()));
}

void NoFlyTask::serialize(QDataStream &stream)
{
    stream << this->geoPoly();
}

bool NoFlyTask::shortnessRewardApplies() const
{
    return false;
}
