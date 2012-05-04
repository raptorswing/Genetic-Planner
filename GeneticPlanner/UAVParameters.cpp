#include "UAVParameters.h"

UAVParameters::UAVParameters() :
    _speed(13.0), _maxTurnRate(20.0), _ceiling(1600.0), _maxFlightTime(60.0), _maxClimbRate(1.0)
{
}

UAVParameters::UAVParameters(const qreal &speed,
                             const qreal &maxTurnRate,
                             const qreal &ceiling,
                             const qreal &maxFlightTime,
                             const qreal &maxClimbRate)
{
    this->init(speed,
               maxTurnRate,
               ceiling,
               maxFlightTime,
               maxClimbRate);
}

qreal UAVParameters::speed() const
{
    return _speed;
}

qreal UAVParameters::maxTurningRate() const
{
    return _maxTurnRate;
}

qreal UAVParameters::ceiling() const
{
    return _ceiling;
}

qreal UAVParameters::maxFlightTime() const
{
    return _maxFlightTime;
}

qreal UAVParameters::maxClimbRate() const
{
    return _maxClimbRate;
}

void UAVParameters::setSpeed(qreal nSpeed)
{
    _speed = qMax<qreal>(1.0,nSpeed);
}

void UAVParameters::setMaxTurnRate(qreal nMaxTurnRate)
{
    _maxTurnRate = qMax<qreal>(1.0,nMaxTurnRate);
}

void UAVParameters::setCeiling(qreal nCeiling)
{
    _ceiling = qMax<qreal>(100.0,nCeiling);
}

void UAVParameters::setMaxFlightTime(qreal nMaxFlightTime)
{
    _maxFlightTime = qMax<qreal>(1.0,nMaxFlightTime);
}

void UAVParameters::setMaxClimbRate(qreal nMaxClimbRate)
{
    _maxClimbRate = qMax<qreal>(0.1,nMaxClimbRate);
}

bool UAVParameters::operator==(const UAVParameters &other) const
{
    return (other.speed() == this->speed()
            && other.maxTurningRate() == this->maxTurningRate()
            && other.ceiling() == this->ceiling()
            && other.maxFlightTime() == this->maxFlightTime()
            && other.maxClimbRate() == this->maxClimbRate());
}

//private
void UAVParameters::init(qreal speed, qreal maxTurnRate, qreal ceiling, qreal maxFlightTime, qreal maxClimbRate)
{
    this->setSpeed(speed);
    this->setMaxTurnRate(maxTurnRate);
    this->setCeiling(ceiling);
    this->setMaxFlightTime(maxFlightTime);
    this->setMaxClimbRate(maxClimbRate);
}

QDataStream & operator<< (QDataStream& stream, const UAVParameters& params)
{
    stream << params.speed();
    stream << params.maxTurningRate();
    stream << params.ceiling();
    stream << params.maxFlightTime();
    stream << params.maxClimbRate();

    return stream;
}

QDataStream & operator>> (QDataStream& stream, UAVParameters& params)
{
    qreal speed;
    qreal maxTurningRate;
    qreal ceiling;
    qreal maxFlightTime;
    qreal maxClimbRate;

    stream >> speed;
    stream >> maxTurningRate;
    stream >> ceiling;
    stream >> maxFlightTime;
    stream >> maxClimbRate;

    params.setSpeed(speed);
    params.setMaxTurnRate(maxTurningRate);
    params.setCeiling(ceiling);
    params.setMaxFlightTime(maxFlightTime);
    params.setMaxClimbRate(maxClimbRate);

    return stream;
}
