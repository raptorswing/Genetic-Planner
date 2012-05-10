#include "TaskAreaObject.h"

#include <QtDebug>
#include <QMenu>

TaskAreaObject::TaskAreaObject(QPolygonF geoPoly, QColor fillColor, QObject *parent) :
    PolygonObject(geoPoly, fillColor, parent)
{
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
