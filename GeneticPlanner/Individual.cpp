#include "Individual.h"
#include "guts/Conversions.h"

#include <cmath>
#include <QtDebug>

const int ACTION_DISTANCE_MAX = 100.0;
const int ACTION_DISTANCE_MIN = 10.0;
const int TURNING_RATE_MAX_DEGREES_PER_SECOND = 5.0;
const int SPEED_METERS_PER_SECOND = 15.0;

const qreal PI = 3.1415926535897932384626433;

Individual::Individual()
{
    int numberOfActions = (qrand() % 100) + 1;
    for (int i = 0; i < numberOfActions; i++)
        this->appendYawAction(Individual::randomYawAction());
}

Individual::Individual(const Individual &A, const Individual &B)
{
    int aSize = A._yawActions.size();
    int bSize = B._yawActions.size();
    int numActions = qRound((aSize + bSize)/2.0);

    for (int i = 0; i < numActions; i++)
    {
        //If actions are available from both parents, choose randomly
        if (aSize > i && bSize > i)
        {
            if (qrand() % 2)
                this->appendYawAction(A._yawActions[i]);
            else
                this->appendYawAction(B._yawActions[i]);
        }
        else if (aSize > i)
            this->appendYawAction(A._yawActions[i]);
        else if (bSize > i)
            this->appendYawAction(B._yawActions[i]);
    }
}

Individual::Individual(int numberOfActions)
{
    for (int i = 0; i < numberOfActions; i++)
        this->appendYawAction(Individual::randomYawAction());
}

Individual::~Individual()
{
}

QList<Individual::YawAction> Individual::yawActions() const
{
    return _yawActions;
}

void Individual::appendYawAction(const Individual::YawAction & action)
{
    _yawActions.append(action);
}

QList<QPointF> Individual::generateGeoPoints(const QPointF &startingPos) const
{
    QList<QPointF> toRet;
    toRet.append(startingPos);

    qreal directionRad = 0.0;
    /*
    const qreal maxRadsPerSecond = TURNING_RATE_MAX_DEGREES_PER_SECOND * PI / 180.0;
    const qreal maxRadsPerMeter = maxRadsPerSecond / SPEED_METERS_PER_SECOND;
    */


    foreach(YawAction action, this->yawActions())
    {
        //We proceed in one-meter intervals
        for (int i = 0; i < action.distance; i++)
        {
            QVector3D enuPos(0,0,0);
            QPointF geoPos = toRet.last();

            //If the action is a turn, we have to change our direction
            if (!action.isStraight)
            {
                qreal deltaRadPerMeter = action.turningRateRadsPerSecond / SPEED_METERS_PER_SECOND;
                directionRad += deltaRadPerMeter;
            }
            //Use the direction and project ahead a little bit
            enuPos.setX(cos(directionRad));
            enuPos.setY(sin(directionRad));
            QPointF newGeoPos = Conversions::enu2lla(enuPos.x(),
                                                     enuPos.y(),
                                                     enuPos.z(),
                                                     geoPos.y(),
                                                     geoPos.x(),
                                                     0.0).lonlat;
            toRet.append(newGeoPos);
        }
    }

    return toRet;
}

//private static
Individual::YawAction Individual::randomYawAction()
{
    YawAction toRet;

    if (qrand() %  2)
        toRet.isStraight = true;
    else
    {
        toRet.isStraight = false;
        toRet.turningRateRadsPerSecond = ((qrand() % TURNING_RATE_MAX_DEGREES_PER_SECOND) + 1) * PI/180.0;
        if (qrand() % 2)
            toRet.turningRateRadsPerSecond *= -1;
    }

    toRet.distance = (qrand() % (ACTION_DISTANCE_MAX - ACTION_DISTANCE_MIN)) + ACTION_DISTANCE_MIN;

    return toRet;
}
