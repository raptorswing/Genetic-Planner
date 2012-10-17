#include "TaskArea.h"

#include "tasks/NoFlyTask.h"
#include "tasks/FlyThroughTask.h"

#include <QtDebug>

TaskArea::TaskArea(const QPolygonF &geoPoly) :
    _geoPoly(geoPoly)
{
}

TaskArea::~TaskArea()
{
}

QPolygonF TaskArea::geoPoly() const
{
    return _geoPoly;
}

void TaskArea::addTask(QSharedPointer<PathTask> task)
{
    if (task.isNull())
        return;

    PathTask * rawTask = task.data();
    connect(rawTask,
            SIGNAL(taskAreaChanged()),
            this,
            SIGNAL(taskAreaChanged()));
    connect(this,
            SIGNAL(geoPolyChanged(QPolygonF)),
            rawTask,
            SLOT(setGeoPoly(QPolygonF)));

    _tasks.insert(task);
    this->taskAreaChanged();
}

void TaskArea::removeTask(QSharedPointer<PathTask> task)
{
    if (task.isNull() || !_tasks.contains(task))
        return;

    _tasks.remove(task);
    this->taskAreaChanged();
}

QSet<QSharedPointer<PathTask> > TaskArea::tasks() const
{
    return _tasks;
}

int TaskArea::numTasks() const
{
    return _tasks.size();
}

//public slot
void TaskArea::setGeoPoly(const QPolygonF &geoPoly)
{
    if (geoPoly == _geoPoly)
        return;

    _geoPoly = geoPoly;
    this->geoPolyChanged(geoPoly);
    this->taskAreaChanged();
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
    stream << area.geoPoly();

    stream << area.numTasks();
    QSet<QSharedPointer<PathTask> > tasks = area.tasks();
    foreach(QSharedPointer<PathTask> task, tasks)
    {
        stream << task->taskType();
        task->serialize(stream);
    }

    return stream;
}

QDataStream& operator >>(QDataStream& stream, TaskArea& area)
{
    QPolygonF poly;
    stream >> poly;
    area.setGeoPoly(poly);

    int numTasks;
    stream >> numTasks;
    for (int i = 0; i < numTasks; i++)
    {
        QString type;
        stream >> type;
        QSharedPointer<PathTask> task;
        if (type == "NoFly")
            task = QSharedPointer<PathTask>(new NoFlyTask(stream));
        else if (type == "FlyThrough")
            task = QSharedPointer<PathTask>(new FlyThroughTask(stream));
        else
            qWarning() << "Unknown task type" << type;
        area.addTask(task);
    }

    return stream;
}
