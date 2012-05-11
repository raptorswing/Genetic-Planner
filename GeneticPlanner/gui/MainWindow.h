#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPointer>
#include <QSet>
#include <QStack>
#include <QSharedPointer>

#include "MapGraphicsScene.h"
#include "MapGraphicsView.h"
#include "CircleObject.h"

#include "PlanningProblem.h"
#include "MWCommand.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void MWCommandExecuted();

private slots:
    void handlePlanningControlStart(qreal desiredFitness);
    void handlePlanningControlReset();
    void handleStartPointAddRequested();
    void handleEndPointAddRequested();
    void handleTaskAreaAddRequested();
    void handleStartPositionMarkerPosChanged();
    void handleEndPositionMarkerPosChanged();

    void undo();
    void redo();

    void handleMWCommandExecuted();
    
    void on_actionExit_triggered();

    void on_actionUndo_triggered();

    void on_actionRedo_triggered();

private:
    Ui::MainWindow *ui;

    MapGraphicsScene * _scene;
    MapGraphicsView * _view;

    PlanningProblem _problem;

    QPointer<CircleObject> _startPositionMarker;
    QPointer<CircleObject> _endPositionMarker;
    QSet<MapGraphicsObject * > _pathObjects;

    void doCommand(QSharedPointer<MWCommand> todo);
    QStack<QSharedPointer<MWCommand> > _undoStack;
    QStack<QSharedPointer<MWCommand> > _redoStack;
};

#endif // MAINWINDOW_H
