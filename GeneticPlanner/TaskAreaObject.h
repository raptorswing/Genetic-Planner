#ifndef TASKAREAOBJECT_H
#define TASKAREAOBJECT_H

#include "PolygonObject.h"

class TaskAreaObject : public PolygonObject
{
    Q_OBJECT
public:
    explicit TaskAreaObject(QPolygonF geoPoly,QColor fillColor = QColor(200,200,200,200), QObject *parent=0);

protected:
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent * event);
    
signals:
    
public slots:

private slots:
    void spawnConfigurationWidget();
    
};

#endif // TASKAREAOBJECT_H
