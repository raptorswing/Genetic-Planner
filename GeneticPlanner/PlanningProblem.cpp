#include "PlanningProblem.h"

PlanningProblem::PlanningProblem()
{
}

PlanningProblem::PlanningProblem(const UAVParameters &uavParams, const SensorDefinition &sensorParams)
{
    this->setUAVSettings(uavParams);
    this->setSensorSettings(sensorParams);
}

UAVParameters PlanningProblem::uavSettings() const
{
    return _uavSettings;
}

SensorDefinition PlanningProblem::sensorSettings() const
{
    return _sensorSettings;
}

void PlanningProblem::setUAVSettings(const UAVParameters &uavParams)
{
    _uavSettings = uavParams;
}

void PlanningProblem::setSensorSettings(const SensorDefinition &sensorParams)
{
    _sensorSettings = sensorParams;
}
