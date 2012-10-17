#ifndef TASKAREAOBJECTEDITWIDGET_H
#define TASKAREAOBJECTEDITWIDGET_H

#include <QWidget>
#include <QPointer>

#include "TaskAreaObject.h"

namespace Ui {
class TaskAreaObjectEditWidget;
}

class TaskAreaObjectEditWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit TaskAreaObjectEditWidget(QPointer<TaskAreaObject> taskAreaObj = 0, QWidget *parent = 0);
    ~TaskAreaObjectEditWidget();
    
private slots:
    void on_addNoFlyButton_clicked();

    void on_AddFlyThruButton_clicked();

    void on_addEndingButton_clicked();

    void on_pushButton_clicked();

private:
    Ui::TaskAreaObjectEditWidget *ui;

    QPointer<TaskAreaObject> _taskAreaObj;
};

#endif // TASKAREAOBJECTEDITWIDGET_H
