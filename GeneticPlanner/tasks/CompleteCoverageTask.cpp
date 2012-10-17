#include "CompleteCoverageTask.h"

#include <QList>
#include <QSet>

#include "guts/Conversions.h"

CompleteCoverageTask::CompleteCoverageTask(const QPolygonF & geoPoly) :
    PathTask(geoPoly)
{
}

CompleteCoverageTask::CompleteCoverageTask(QDataStream &stream)
{
    QPolygonF geoPoly;
    stream >> geoPoly;
    this->setGeoPoly(geoPoly);
}

qreal CompleteCoverageTask::performance(const QList<Position> &positions)
{
    //This function is real slow. I should optimize it in the future. Quad trees or binning?
    //This function is also assuming flat earth...

    const qreal deltaDegrees = 0.001;
    const QRectF boundingBox = this->geoPoly().boundingRect();
    QSet<QVector3D> toCheck;
    for (qreal x = boundingBox.left(); x < boundingBox.right(); x += deltaDegrees)
    {
        for (qreal y = boundingBox.top(); y < boundingBox.bottom(); y += deltaDegrees)
        {
            if (this->geoPoly().containsPoint(QPoint(x,y), Qt::OddEvenFill))
                continue;

            toCheck.insert(Conversions::lla2xyz(y, x, 1500));
        }
    }


    const qreal perHit = 500 / (qreal)toCheck.size();
    qreal partials = 0.0;
    const qreal maxPartial = perHit / (positions.size() * toCheck.size());
    QSet<QVector3D> hits;
    foreach(const Position & planePos, positions)
    {
        //Annoying conversion necessary...
        Conversions::Position convPos;
        convPos.altitude = planePos.altitude();
        convPos.lonlat = planePos.lonLat();

        QVector3D pos = Conversions::lla2xyz(convPos);
        foreach(const QVector3D & sensorPoint, toCheck)
        {
            const qreal dist = (pos - sensorPoint).length();
            if (dist < 10.0)
                hits.insert(sensorPoint);
            else if (dist < 20.0)
                partials += qMin<qreal>(maxPartial, 10*PathTask::normal(dist,20.0));
        }
    }
    return hits.size() * perHit + partials;
}

QString CompleteCoverageTask::taskType() const
{
    return "CompleteCoverageTask";
}

QSharedPointer<PathTask> CompleteCoverageTask::copy() const
{
    return QSharedPointer<PathTask>(new CompleteCoverageTask(this->geoPoly()));
}

void CompleteCoverageTask::serialize(QDataStream &stream)
{
}

bool CompleteCoverageTask::shortnessRewardApplies() const
{
    return true;
}

//non-member
uint qHash(const QVector3D & vec)
{
    return vec.x() * vec.y() * vec.z();
}
