#include "TaskArea.h"

#include <QtDebug>

TaskArea::TaskArea(const QPolygonF &geoPoly) :
    _geoPoly(geoPoly)
{
}

TaskArea::~TaskArea()
{
}

QPolygonF TaskArea::polygon() const
{
    return _geoPoly;
}

void TaskArea::addTask(QSharedPointer<PathTask> task)
{
    if (task.isNull())
        return;

    PathTask * rawTask = task.data();
    connect(rawTask,
            SIGNAL(changed()),
            this,
            SIGNAL(changed()));

    _tasks.insert(task);
    this->changed();
}

void TaskArea::removeTask(QSharedPointer<PathTask> task)
{
    if (task.isNull() || !_tasks.contains(task))
        return;

    _tasks.remove(task);
    this->changed();
}

QSet<QSharedPointer<PathTask> > TaskArea::tasks() const
{
    return _tasks;
}

//public slot
void TaskArea::setPolygon(const QPolygonF &geoPoly)
{
    if (geoPoly == _geoPoly)
        return;

    _geoPoly = geoPoly;
    this->changed();
}

//private
TaskArea::TaskArea(const TaskArea &other)
{
    Q_UNUSED(other)
}

//private
TaskArea &TaskArea::operator =(const TaskArea &other)
{
    Q_UNUSED(other)
    return *this;
}


QDataStream& operator <<(QDataStream& stream, const TaskArea& area)
{
    stream << area.polygon();
    return stream;
}

QDataStream& operator >>(QDataStream& stream, TaskArea& area)
{
    QPolygonF poly;
    stream >> poly;

    area.setPolygon(poly);

    return stream;
}
