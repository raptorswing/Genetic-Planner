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

//private slot
void PlanningControlWidget::on_resetButton_clicked()
{

}

//private slot
void PlanningControlWidget::on_planResumeButton_clicked()
{

}

//private slot
void PlanningControlWidget::on_desiredFitnessSpinBox_valueChanged(double arg1)
{

}
