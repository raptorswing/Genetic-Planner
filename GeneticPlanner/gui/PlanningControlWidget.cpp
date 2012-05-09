#include "PlanningControlWidget.h"
#include "ui_PlanningControlWidget.h"

PlanningControlWidget::PlanningControlWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlanningControlWidget)
{
    ui->setupUi(this);
}

PlanningControlWidget::~PlanningControlWidget()
{
    delete ui;
}

void PlanningControlWidget::setIsPlanningRunning(bool running)
{
    this->ui->planResumeButton->setChecked(running);
}

//private slot
void PlanningControlWidget::on_resetButton_clicked()
{
    this->planningClearRequested();
}

//private slot
void PlanningControlWidget::on_planResumeButton_toggled()
{
    if (this->ui->planResumeButton->isChecked())
        this->planningStartRequested(this->ui->desiredFitnessSpinBox->value());
    else
        this->planningPauseRequested();
}

//private slot
void PlanningControlWidget::on_desiredFitnessSpinBox_valueChanged(double arg1)
{
    Q_UNUSED(arg1)
}
