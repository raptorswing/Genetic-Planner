#include "TaskAreaObject.h"
#include "gui/TaskAreaObjectEditWidget.h"

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
            SLOT(setGeoPoly(QPolygonF)));

    this->setGeoPoly(strong->geoPoly());

    //We call this on construction to initialize things even though nothing has really changed
    this->handleAreaChanged();
}

QWeakPointer<TaskArea> TaskAreaObject::taskArea() const
{
    return _area;
}

//protected
void TaskAreaObject::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QMenu menu(event->widget());
    menu.addAction("Edit Tasks",this,SLOT(spawnConfigurationWidget()));
    menu.addAction("Delete",this,SLOT(deleteLater()));
    menu.exec(event->screenPos());
}

//private slot
void TaskAreaObject::spawnConfigurationWidget()
{
    TaskAreaObjectEditWidget * config = new TaskAreaObjectEditWidget(this);
    config->show();
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

    QPolygonF poly = strong->geoPoly();
    this->setGeoPoly(poly);

    //Check out the tasks
    QColor fillColor(255,255,255,100);
    if (strong->numTasks() >= 1)
    {
        fillColor = Qt::gray;
        foreach(QSharedPointer<PathTask> task, strong->tasks())
        {
            if (task->taskType() == "NoFly")
            {
                fillColor = QColor(255,0,0,100);
                break;
            }
            else if (task->taskType() == "FlyThrough")
            {
                fillColor = QColor(0,0,255,100);
                break;
            }
        }
    }
    this->setFillColor(fillColor);
}

void TaskAreaObject::handleAreaDestroyed()
{
    this->deleteLater();
}
