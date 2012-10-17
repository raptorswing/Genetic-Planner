#include "TaskAreaObjectEditWidget.h"
#include "ui_TaskAreaObjectEditWidget.h"

#include "TaskAreaListModel.h"
#include "tasks/NoFlyTask.h"
#include "tasks/FlyThroughTask.h"
#include "tasks/EndingTask.h"
#include "tasks/CompleteCoverageTask.h"

TaskAreaObjectEditWidget::TaskAreaObjectEditWidget(QPointer<TaskAreaObject> taskAreaObj, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TaskAreaObjectEditWidget),
    _taskAreaObj(taskAreaObj)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);

    if (taskAreaObj.isNull())
    {
        this->deleteLater();
        return;
    }

    connect(taskAreaObj.data(),
            SIGNAL(destroyed()),
            this,
            SLOT(deleteLater()));

    TaskAreaListModel * model = new TaskAreaListModel(taskAreaObj->taskArea(),this);
    this->ui->taskListView->setModel(model);
}

TaskAreaObjectEditWidget::~TaskAreaObjectEditWidget()
{
    delete ui;
}

//private slot
void TaskAreaObjectEditWidget::on_addNoFlyButton_clicked()
{
    if (_taskAreaObj.isNull())
        return;

    QWeakPointer<TaskArea> weak = _taskAreaObj->taskArea();
    QSharedPointer<TaskArea> strong = weak.toStrongRef();
    if (strong.isNull())
        return;

    QSharedPointer<PathTask> task(new NoFlyTask(strong->geoPoly()));
    strong->addTask(task);
}

//private slot
void TaskAreaObjectEditWidget::on_AddFlyThruButton_clicked()
{
    if (_taskAreaObj.isNull())
        return;

    QWeakPointer<TaskArea> weak = _taskAreaObj->taskArea();
    QSharedPointer<TaskArea> strong = weak.toStrongRef();
    if (strong.isNull())
        return;

    QSharedPointer<PathTask> task(new FlyThroughTask(strong->geoPoly(),1500));
    strong->addTask(task);
}

//private slot
void TaskAreaObjectEditWidget::on_addEndingButton_clicked()
{
    if (_taskAreaObj.isNull())
        return;

    QWeakPointer<TaskArea> weak = _taskAreaObj->taskArea();
    QSharedPointer<TaskArea> strong = weak.toStrongRef();
    if (strong.isNull())
        return;

    QSharedPointer<PathTask> task(new EndingTask(strong->geoPoly()));
    strong->addTask(task);
}

//private slot
void TaskAreaObjectEditWidget::on_pushButton_clicked()
{
    if (_taskAreaObj.isNull())
        return;

    QWeakPointer<TaskArea> weak = _taskAreaObj->taskArea();
    QSharedPointer<TaskArea> strong = weak.toStrongRef();
    if (strong.isNull())
        return;

    QSharedPointer<PathTask> task(new CompleteCoverageTask(strong->geoPoly()));
    strong->addTask(task);
}
