#ifndef SENSORDEFINITION_H
#define SENSORDEFINITION_H

#include <QtGlobal>
#include <QDataStream>

class SensorDefinition
{
public:
    enum SensorType
    {
        Directional,
        OmniDirectional
    };

public:
    SensorDefinition();
    SensorDefinition(SensorType type, qreal usableDistance, qreal angleOfViewDegrees = 50.0, bool gimballed = false, QString name = QString("Unknown Sensor"));
    virtual ~SensorDefinition();

    SensorType type() const;
    void setType(SensorType);

    qreal usableDistance() const;
    void setUsableDistance(qreal);

    qreal angleOfViewDegrees() const;
    void setAngleOfViewDegrees(qreal);

    bool gimballed() const;
    void setGimballed(bool);

    QString name() const;
    void setName(QString nName);

    bool operator==(const SensorDefinition& other) const;

private:
    SensorType _type;
    qreal _usableDistance;
    qreal _angleOfViewDegrees;
    bool _gimballed;
    QString _name;
};

QDataStream & operator<< (QDataStream& stream, const SensorDefinition& params);
QDataStream & operator>> (QDataStream& stream, SensorDefinition& params);

QDataStream & operator<< (QDataStream& stream, const SensorDefinition::SensorType& type);
QDataStream & operator>> (QDataStream& stream, SensorDefinition::SensorType& type);

#endif // SENSORDEFINITION_H
