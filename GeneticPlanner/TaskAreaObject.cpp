#include "TaskAreaObject.h"

#include <QtDebug>
#include <QMenu>

TaskAreaObject::TaskAreaObject(QWeakPointer<TaskArea> area, QColor fillColor, QObject *parent) :
    PolygonObject(QPolygon(), fillColor, parent), _area(area)
{
    QSharedPointer<TaskArea> strong = area.toStrongRef();
    if (strong.isNull())
    {
        qWarning() << this << "got null task area";
        this->deleteLater();
        return;
    }

    TaskArea * rawArea = strong.data();
    connect(rawArea,
            SIGNAL(changed()),
            this,
            SLOT(handleAreaChanged()));
    connect(rawArea,
            SIGNAL(destroyed()),
            this,
            SLOT(handleAreaDestroyed()));
    connect(this,
            SIGNAL(polygonChanged(QPolygonF)),
            rawArea,
            SLOT(setPolygon(QPolygonF)));

    this->setGeoPoly(strong->polygon());
}

//protected
void TaskAreaObject::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QMenu menu(event->widget());
    menu.addAction("Edit Tasks",this,SLOT(spawnConfigurationWidget()));
    menu.exec(event->screenPos());
}

//private slot
void TaskAreaObject::spawnConfigurationWidget()
{
    qDebug() << "Spawned";
}

//private slot
void TaskAreaObject::handleAreaChanged()
{
    QSharedPointer<TaskArea> strong = _area.toStrongRef();
    if (strong.isNull())
    {
        this->deleteLater();
        return;
    }

    QPolygonF poly = strong->polygon();
    this->setGeoPoly(poly);
}

void TaskAreaObject::handleAreaDestroyed()
{
    this->deleteLater();
}
