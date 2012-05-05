#ifndef PLANNINGCONTROLWIDGET_H
#define PLANNINGCONTROLWIDGET_H

#include <QWidget>

namespace Ui {
class PlanningControlWidget;
}

class PlanningControlWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit PlanningControlWidget(QWidget *parent = 0);
    ~PlanningControlWidget();
    
private slots:
    void on_resetButton_clicked();

    void on_planResumeButton_clicked();

    void on_desiredFitnessSpinBox_valueChanged(double arg1);

private:
    Ui::PlanningControlWidget *ui;
};

#endif // PLANNINGCONTROLWIDGET_H