#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPointer>
#include <QSet>

#include "MapGraphicsScene.h"
#include "MapGraphicsView.h"
#include "CircleObject.h"

#include "PlanningProblem.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void handlePlanningControlStart(qreal desiredFitness);
    void handlePlanningControlReset();
    void handleStartPointAddRequested();
    void handleEndPointAddRequested();
    void handleTaskAreaAddRequested();
    void handleStartPositionMarkerPosChanged();
    void handleEndPositionMarkerPosChanged();
    
private:
    Ui::MainWindow *ui;

    MapGraphicsScene * _scene;
    MapGraphicsView * _view;

    PlanningProblem _problem;

    QPointer<CircleObject> _startPositionMarker;
    QPointer<CircleObject> _endPositionMarker;
    QSet<MapGraphicsObject * > _pathObjects;
};

#endif // MAINWINDOW_H
