#include "TaskArea.h"

TaskArea::TaskArea(const QPolygonF &geoPoly) :
    _geoPoly(geoPoly)
{
}

TaskArea::~TaskArea()
{
}

void TaskArea::setPolygon(const QPolygonF &geoPoly)
{
    _geoPoly = geoPoly;
}

QPolygonF TaskArea::polygon() const
{
    return _geoPoly;
}

void TaskArea::addTask(QSharedPointer<PathTask> task)
{
    _tasks.append(task);
}

void TaskArea::removeTask(QSharedPointer<PathTask> task)
{
    _tasks.removeAll(task);
}

QList<QSharedPointer<PathTask> > TaskArea::tasks() const
{
    return _tasks;
}
