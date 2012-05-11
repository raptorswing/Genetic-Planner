#include "NoFlyTask.h"

#include <QVector3D>

#include "guts/Conversions.h"

NoFlyTask::NoFlyTask(PolygonObject *polyObj) :
    _polyObj(polyObj)
{
}

qreal NoFlyTask::performance(const QList<QPointF> &positions)
{
    if (_polyObj.isNull())
        return 500.0;

    qreal goalScore = 500.0;
    //const qreal stdDev = 100.0;

    //QPointF goalPoint =_polyObj->pos();

    foreach(QPointF geoPos, positions)
    {
        /*
        QVector3D enuPos = Conversions::lla2enu(geoPos.y(),geoPos.x(),1409,
                                                goalPoint.y(),goalPoint.x(),1409);
        qreal dist = enuPos.length();
        goalScore -= 10*PathTask::normal(dist,stdDev);
        */

        if (_polyObj->geoPoly().containsPoint(geoPos,Qt::OddEvenFill))
        {
            goalScore -= 10;
            if (goalScore <= 0)
                break;
            //break;
        }
    }

    goalScore = qMax<qreal>(0.0,qMin<qreal>(500.0,goalScore));

    return goalScore;
}
