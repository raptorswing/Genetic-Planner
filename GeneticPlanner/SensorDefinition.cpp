#include "SensorDefinition.h"

#include <QtDebug>

const quint32 ARBITRARY_DIRECTIONAL_SERIALIZATION = 47;
const quint32 ARBITRARY_OMNIDIRECTIONAL_SERIALIZATION = 23;

SensorDefinition::SensorDefinition()
{
    this->setType(SensorDefinition::Directional);
    this->setUsableDistance(750.0);
    this->setAngleOfViewDegrees(25.0);
    this->setGimballed(true);
    this->setName("Gimballed 100mm camera");
}

SensorDefinition::SensorDefinition(SensorDefinition::SensorType type, qreal usableDistance, qreal angleOfViewDegrees, bool gimballed, QString name)
{
    this->setType(type);
    this->setUsableDistance(usableDistance);
    this->setAngleOfViewDegrees(angleOfViewDegrees);
    this->setGimballed(gimballed);
    this->setName(name);
}

SensorDefinition::~SensorDefinition()
{
}

SensorDefinition::SensorType SensorDefinition::type() const
{
    return _type;
}

void SensorDefinition::setType(SensorDefinition::SensorType nType)
{
    if (nType != Directional
            && nType != OmniDirectional)
        return;
    _type = nType;
}

qreal SensorDefinition::usableDistance() const
{
    return _usableDistance;
}

void SensorDefinition::setUsableDistance(qreal nDist)
{
    _usableDistance = qMax<qreal>(1.0, nDist);
}

qreal SensorDefinition::angleOfViewDegrees() const
{
    return _angleOfViewDegrees;
}

void SensorDefinition::setAngleOfViewDegrees(qreal nDegrees)
{
    _angleOfViewDegrees = qMax<qreal>(1.0,qMin<qreal>(180.0,nDegrees));
}

bool SensorDefinition::gimballed() const
{
    return _gimballed;
}

void SensorDefinition::setGimballed(bool nGimballed)
{
    _gimballed = nGimballed;
}

QString SensorDefinition::name() const
{
    return _name;
}

void SensorDefinition::setName(QString nName)
{
    _name = nName;
}

bool SensorDefinition::operator ==(const SensorDefinition &other) const
{
    return (other.type() == this->type()
            && other.usableDistance() == this->usableDistance()
            && other.angleOfViewDegrees() == this->angleOfViewDegrees()
            && other.gimballed() == this->gimballed()
            && other.name() == this->name());
}

QDataStream & operator<< (QDataStream& stream, const SensorDefinition& params)
{
    stream << params.type();
    stream << params.usableDistance();
    stream << params.angleOfViewDegrees();
    stream << params.gimballed();
    stream << params.name();
    return stream;
}

QDataStream & operator>> (QDataStream& stream, SensorDefinition& params)
{
    SensorDefinition::SensorType type;
    qreal usableDistance;
    qreal angleOfViewDegrees;
    bool gimballed;
    QString name;

    stream >> type;
    stream >> usableDistance;
    stream >> angleOfViewDegrees;
    stream >> gimballed;
    stream >> name;

    params.setType(type);
    params.setUsableDistance(usableDistance);
    params.setAngleOfViewDegrees(angleOfViewDegrees);
    params.setGimballed(gimballed);
    params.setName(name);

    return stream;
}

QDataStream & operator<< (QDataStream& stream, const SensorDefinition::SensorType& type)
{
    quint32 toWrite;

    if (type == SensorDefinition::Directional)
        toWrite = ARBITRARY_DIRECTIONAL_SERIALIZATION;
    else if (type == SensorDefinition::OmniDirectional)
        toWrite = ARBITRARY_OMNIDIRECTIONAL_SERIALIZATION;
    else
        toWrite = ARBITRARY_DIRECTIONAL_SERIALIZATION;

    stream << toWrite;

    return stream;
}

QDataStream & operator>> (QDataStream& stream, SensorDefinition::SensorType& type)
{
    quint32 read;
    stream >> read;

    if (read == ARBITRARY_DIRECTIONAL_SERIALIZATION)
        type = SensorDefinition::Directional;
    else if (read == ARBITRARY_OMNIDIRECTIONAL_SERIALIZATION)
        type = SensorDefinition::OmniDirectional;
    else
        type = SensorDefinition::Directional;

    return stream;
}
