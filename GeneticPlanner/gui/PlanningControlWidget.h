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
    enum PlanningStatus
    {
        Stopped,
        Running,
        Paused
    };

public:
    explicit PlanningControlWidget(QWidget *parent = 0);
    ~PlanningControlWidget();

public slots:
    void setPlanningState(PlanningStatus);
    void setPlanningProgress(int iteration, qreal fitness);
    void setIsPaused();
    void setIsStopped();
    void setIsRunning();

signals:
    void planningStartRequested();
    void planningPauseRequested();
    void planningClearRequested();
    
private slots:
    void on_resetButton_clicked();

    void on_planButton_clicked();

    void on_pauseButton_clicked();

private:
    Ui::PlanningControlWidget *ui;
};

#endif // PLANNINGCONTROLWIDGET_H
