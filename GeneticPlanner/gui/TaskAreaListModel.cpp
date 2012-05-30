#include "TaskAreaListModel.h"

TaskAreaListModel::TaskAreaListModel(QWeakPointer<TaskArea> area, QObject * parent) :
    QAbstractListModel(parent), _area(area)
{
}

int TaskAreaListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    QSharedPointer<TaskArea> strong = _area.toStrongRef();
    if (strong.isNull())
        return 0;

    return strong->numTasks();
}

QVariant TaskAreaListModel::data(const QModelIndex &index, int role) const
{
    QSharedPointer<TaskArea> strong = _area.toStrongRef();
    if (strong.isNull())
        return QVariant("Null area");

    if (!index.isValid())
        return QVariant("Invalid index");

    if (index.row() >= strong->numTasks())
        return QVariant("Row out of bounds");

    QList<QSharedPointer<PathTask> > tasks = strong->tasks().toList();
    if (role == Qt::DisplayRole)
    {
        return QVariant(tasks[index.row()]->taskType());
    }
    else
        return QVariant();
}

Qt::ItemFlags TaskAreaListModel::flags(const QModelIndex &index) const
{
    QSharedPointer<TaskArea> strong = _area.toStrongRef();

    if (!index.isValid() || strong.isNull() || index.row() >= strong->numTasks())
        return Qt::NoItemFlags;
    return (Qt::ItemIsSelectable| Qt::ItemIsEnabled);
}
