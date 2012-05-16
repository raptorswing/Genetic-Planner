#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPointer>
#include <QStack>
#include <QSharedPointer>

#include "MapGraphicsScene.h"
#include "MapGraphicsView.h"
#include "CircleObject.h"
#include "PlanningProblemDisplayAdapter.h"
#include "MWCommand.h"
#include "Planner.h"

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
    void handlePlanningControlStartRequested();
    void handlePlanningControlPauseRequested();
    void handlePlanningControlResetRequested();
    void handleStartPointAddRequested();
    void handleEndPointAddRequested();
    void handleTaskAreaAddRequested();

    void undo();
    void redo();

    void handleMWCommandExecuted();
    
    void on_actionExit_triggered();

    void on_actionUndo_triggered();

    void on_actionRedo_triggered();

    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_Planning_Problem_triggered();

    void on_actionSave_As_triggered();

    void on_actionClose_triggered();

    void on_actionExport_Solution_triggered();

private:
    Ui::MainWindow *ui;

    MapGraphicsScene * _scene;
    MapGraphicsView * _view;

    //This guy maps the "model" of the PlanningProblem to the "view" of the MapGraphics scene and view
    PlanningProblemDisplayAdapter * _adapter;

    Planner * _planner;


    //Stuff for undo/redo
    void doCommand(QSharedPointer<MWCommand> todo);
    QStack<QSharedPointer<MWCommand> > _undoStack;
    QStack<QSharedPointer<MWCommand> > _redoStack;

    QString _openFile;
};

#endif // MAINWINDOW_H
