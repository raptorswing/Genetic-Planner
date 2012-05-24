#ifndef POSE_H
#define POSE_H

#include "Position.h"

#include <QQuaternion>
#include <QDataStream>
#include <QtDebug>

class Pose
{
public:
    Pose();
    Pose(const Position& pos, const QQuaternion& quat);
    Pose(const Pose& other);
    ~Pose();

    bool operator ==(const Pose& other);
    bool operator !=(const Pose& other);
    Pose& operator=(const Pose& other);

    Position position() const;
    QQuaternion orientation() const;

    void setPosition(const Position& pos);
    void setOrientation(const QQuaternion& quat);

private:
    Position _position;
    QQuaternion _orientation;
};

//Non-member method for streaming to qDebug
QDebug operator<<(QDebug dbg, const Pose& pose);

//Non-member methods for serializing and de-serializing
QDataStream& operator<<(QDataStream& stream, const Pose& pose);
QDataStream& operator>>(QDataStream& stream, Pose& pose);

#endif // POSE_H
