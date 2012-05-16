#include "PlanningControlWidget.h"
#include "ui_PlanningControlWidget.h"

PlanningControlWidget::PlanningControlWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlanningControlWidget)
{
    ui->setupUi(this);
    this->setPlanningState(Stopped);
}

PlanningControlWidget::~PlanningControlWidget()
{
    delete ui;
}

//public slot
void PlanningControlWidget::setPlanningState(PlanningStatus status)
{
    this->ui->planButton->setEnabled((status == Stopped || status == Paused));
    this->ui->pauseButton->setEnabled((status == Running));
    this->ui->resetButton->setEnabled((status == Paused || status == Running));

    if (status == Stopped)
    {
        this->ui->currentFitnessDisplay->setValue(0.0);
        this->ui->currentIterationSpinbox->setValue(0);
    }
}

void PlanningControlWidget::setPlanningProgress(int iteration, qreal fitness)
{
    this->ui->currentIterationSpinbox->setValue(iteration);
    this->ui->currentFitnessDisplay->setValue(fitness);

    if (this->ui->stopAtFitnessCheckBox->isChecked() && fitness >= this->ui->stopAtFitnessSpinBox->value())
        this->planningPauseRequested();
    else if (this->ui->stopAtIterationCheckBox->isChecked() && iteration >= this->ui->stopAtIterationSpinBox->value())
        this->planningPauseRequested();
}

//public slot
void PlanningControlWidget::setIsPaused()
{
    this->setPlanningState(Paused);
}

//public slot
void PlanningControlWidget::setIsStopped()
{
    this->setPlanningState(Stopped);
}

//public slot
void PlanningControlWidget::setIsRunning()
{
    this->setPlanningState(Running);
}

//private slot
void PlanningControlWidget::on_resetButton_clicked()
{
    this->planningClearRequested();
}

//private slot
void PlanningControlWidget::on_planButton_clicked()
{
    this->planningStartRequested();
}

//private slot
void PlanningControlWidget::on_pauseButton_clicked()
{
    this->planningPauseRequested();
}
