#ifndef TASKAREALISTMODEL_H
#define TASKAREALISTMODEL_H

#include <QAbstractListModel>
#include <QWeakPointer>

#include "TaskArea.h"

class TaskAreaListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit TaskAreaListModel(QWeakPointer<TaskArea> area, QObject * parent = 0);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

private:
    QWeakPointer<TaskArea> _area;
    
signals:
    
public slots:
    
};

#endif // TASKAREALISTMODEL_H
