#ifndef TASKAREAOBJECT_H
#define TASKAREAOBJECT_H

#include "PolygonObject.h"
#include "TaskArea.h"

#include <QWeakPointer>

class TaskAreaObject : public PolygonObject
{
    Q_OBJECT
public:
    explicit TaskAreaObject(QWeakPointer<TaskArea> taskArea,
                            QColor fillColor = QColor(200,200,200,200),
                            QObject *parent=0);

    QWeakPointer<TaskArea> taskArea() const;

private:
    QWeakPointer<TaskArea> _area;


protected:
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent * event);
    
signals:
    
public slots:

private slots:
    void spawnConfigurationWidget();
    void handleAreaChanged();
    void handleAreaDestroyed();
    
};

#endif // TASKAREAOBJECT_H
