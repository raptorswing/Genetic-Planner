#include "PathTask.h"

#include <cmath>

const qreal PI = 3.1415926535897932384626433;
const qreal SQRT2PI = sqrt(2.0*PI);

PathTask::PathTask()
{
}

PathTask::~PathTask()
{
}

//protected static
qreal PathTask::normal(qreal x, qreal stdDev, qreal scaleFactor)
{
    qreal expPart = exp(-0.5 * pow(x / stdDev, 2.0) / scaleFactor);
    qreal otherPart = stdDev * SQRT2PI;
    return (1.0 / otherPart) * expPart;
}
