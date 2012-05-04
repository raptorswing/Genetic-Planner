#ifndef UAVPARAMETERS_H
#define UAVPARAMETERS_H

#include <QtGlobal>
#include <QDataStream>

class UAVParameters
{
public:
    /*!
     \brief Creates an object with the default UAV parameters

    */
    UAVParameters();

    UAVParameters(const qreal& speed,
                  const qreal& maxTurnRate,
                  const qreal& ceiling,
                  const qreal& maxFlightTime,
                  const qreal& maxClimbRate);

    /*!
     \brief Returns the desired "cruising" speed of the UAV in meters/second

     \return qreal
    */
    qreal speed() const;

    /*!
     \brief Returns the maximum turning rate of the UAV in degrees/second

     \return qreal
    */
    qreal maxTurningRate() const;

    /*!
     \brief Returns the maximum operating altitude of the UAV in meters above sea level.

     \return qreal
    */
    qreal ceiling() const;

    /*!
     \brief Returns the maximum length of the UAV's flight in minutes.

     \return qreal
    */
    qreal maxFlightTime() const;

    /*!
     \brief Returns the maximum speed at which the UAV can climb in meters/second

     \return qreal
    */
    qreal maxClimbRate() const;

    void setSpeed(qreal nSpeed);
    void setMaxTurnRate(qreal nMaxTurnRate);
    void setCeiling(qreal nCeiling);
    void setMaxFlightTime(qreal nMaxFlightTime);
    void setMaxClimbRate(qreal nMaxClimbRate);

    bool operator==(const UAVParameters& other) const;

private:
    void init(qreal speed, qreal maxTurnRate, qreal ceiling, qreal maxFlightTime, qreal maxClimbRate);

    qreal _speed;
    qreal _maxTurnRate;
    qreal _ceiling;
    qreal _maxFlightTime;
    qreal _maxClimbRate;
};

QDataStream & operator<< (QDataStream& stream, const UAVParameters& params);
QDataStream & operator>> (QDataStream& stream, UAVParameters& params);

#endif // UAVPARAMETERS_H
