#ifndef TASKAREA_H
#define TASKAREA_H

#include <QPolygonF>
#include <QSharedPointer>
#include <QSet>
#include <QObject>
#include <QDataStream>

#include "tasks/PathTask.h"

class TaskArea : public QObject
{
    Q_OBJECT
public:
    TaskArea(const QPolygonF& geoPoly);
    ~TaskArea();

    QPolygonF polygon() const;

    void addTask(QSharedPointer<PathTask> task);
    void removeTask(QSharedPointer<PathTask> task);
    QSet<QSharedPointer<PathTask> > tasks() const;

public slots:
    void setPolygon(const QPolygonF& geoPoly);

signals:
    void changed();


private:
    //Dis-allow copying!
    TaskArea(const TaskArea& other);
    TaskArea& operator =(const TaskArea& other);

    QPolygonF _geoPoly;
    QSet<QSharedPointer<PathTask> > _tasks;
};

QDataStream& operator <<(QDataStream& stream, const TaskArea& area);
QDataStream& operator >>(QDataStream& stream, TaskArea& area);

#endif // TASKAREA_H
