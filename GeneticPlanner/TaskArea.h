#ifndef TASKAREA_H
#define TASKAREA_H

#include <QPolygonF>
#include <QSharedPointer>
#include <QList>

#include "PathTask.h"

class TaskArea
{
public:
    TaskArea(const QPolygonF& geoPoly);
    virtual ~TaskArea();

    void setPolygon(const QPolygonF& geoPoly);
    QPolygonF polygon() const;

    void addTask(QSharedPointer<PathTask> task);
    void removeTask(QSharedPointer<PathTask> task);
    QList<QSharedPointer<PathTask> > tasks() const;


private:
    QPolygonF _geoPoly;
    QList<QSharedPointer<PathTask> > _tasks;
};

#endif // TASKAREA_H
