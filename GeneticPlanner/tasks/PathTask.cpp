#include "PathTask.h"

#include <cmath>

const qreal PI = 3.1415926535897932384626433;
const qreal SQRT2PI = sqrt(2.0*PI);

PathTask::PathTask(const QPolygonF &geoPoly) : _geoPoly(geoPoly)
{
}

PathTask::~PathTask()
{
}

//virtual
bool PathTask::shortnessRewardApplies() const
{
    return true;
}

QPolygonF PathTask::geoPoly() const
{
    return _geoPoly;
}

//protected static
qreal PathTask::normal(qreal x, qreal stdDev, qreal scaleFactor)
{
    qreal expPart = exp(-0.5 * pow(x / stdDev, 2.0) / scaleFactor);
    qreal otherPart = stdDev * SQRT2PI;
    return (1.0 / otherPart) * expPart;
}

//public slot
void PathTask::setGeoPoly(const QPolygonF &newPoly)
{
    _geoPoly = newPoly;
    this->changed();
}
