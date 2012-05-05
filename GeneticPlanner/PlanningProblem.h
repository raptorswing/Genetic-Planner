#ifndef PLANNINGPROBLEM_H
#define PLANNINGPROBLEM_H

#include "UAVParameters.h"
#include "SensorDefinition.h"

class PlanningProblem
{
public:
    PlanningProblem();
    PlanningProblem(const UAVParameters& uavParams,
                    const SensorDefinition& sensorParams);

    UAVParameters uavSettings() const;
    SensorDefinition sensorSettings() const;

    void setUAVSettings(const UAVParameters& uavParams);
    void setSensorSettings(const SensorDefinition& sensorParams);

private:
    UAVParameters _uavSettings;
    SensorDefinition _sensorSettings;
};

#endif // PLANNINGPROBLEM_H
