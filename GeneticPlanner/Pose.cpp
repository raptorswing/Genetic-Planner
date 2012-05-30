#include "Pose.h"

Pose::Pose()
{
}

Pose::Pose(const Position &pos, const QQuaternion &quat)
{
    _position = pos;
    _orientation = quat;
}

Pose::Pose(const Pose &other)
{
    _position = other._position;
    _orientation = other._orientation;
}

Pose::~Pose()
{
}

bool Pose::operator ==(const Pose &other)
{
    return ((this->_position == other._position)
            && (this->_orientation == other._orientation));
}

bool Pose::operator !=(const Pose &other)
{
    return !(*this == other);
}

Pose &Pose::operator =(const Pose &other)
{
    if (&other != this)
    {
        this->_position = other._position;
        this->_orientation = other._orientation;
    }
    return *this;
}

Position Pose::position() const
{
    return _position;
}

QQuaternion Pose::orientation() const
{
    return _orientation;
}

void Pose::setPosition(const Position &pos)
{
    _position = pos;
}

void Pose::setOrientation(const QQuaternion &quat)
{
    _orientation = quat;
}

//Non-member method for streaming to qDebug
QDebug operator<<(QDebug dbg, const Pose& pose)
{
    dbg.nospace() << pose.position() << pose.orientation();

    return dbg.space();
}

//Non-member methods for serializing and de-serializing
QDataStream& operator<<(QDataStream& stream, const Pose& pose)
{
    stream << pose.position() << pose.orientation();
    return stream;
}

QDataStream& operator>>(QDataStream& stream, Pose& pose)
{
    Position pos;
    QQuaternion orientation;

    stream >> pos;
    stream >> orientation;

    pose.setPosition(pos);
    pose.setOrientation(orientation);

    return stream;
}
