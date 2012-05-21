#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QtDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QDataStream>

#include "tileSources/CompositeTileSource.h"
#include "tileSources/OSMTileSource.h"
#include "guts/CompositeTileSourceConfigurationWidget.h"
#include "CircleObject.h"
#include "TaskAreaObject.h"
#include "UAVParametersWidget.h"
#include "SensorParametersWidget.h"
#include "PlanningControlWidget.h"
#include "Planner.h"
#include "PlanningWizard.h"
#include "NoFlyTask.h"
#include "gui/commands/SetProblemCommand.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Setup MapGraphicsScene and View
    _scene = new MapGraphicsScene(this);
    _view = new MapGraphicsView(_scene,this);
    this->setCentralWidget(_view);

    _adapter = new PlanningProblemDisplayAdapter(PlanningProblem(), _scene, this);
    _planner = new Planner(_adapter->planningProblem(),this);

    //Setup tile sources for the MapGraphicsView
    QSharedPointer<CompositeTileSource> composite(new CompositeTileSource());
    QSharedPointer<MapTileSource> osm(new OSMTileSource());
    QSharedPointer<MapTileSource> mqSat(new OSMTileSource(OSMTileSource::MapQuestAerialTiles));
    composite->addSourceBottom(osm,0.75);
    composite->addSourceBottom(mqSat);
    _view->setTileSource(composite);

    //Zoom into BYU campus by default
    /*
    QPointF place(-111.649253,40.249707);
    _view->setZoomLevel(15);
    _view->centerOn(place);
    */

    //Provide our "map layers" dock widget with the composite tile source to be configured
    this->ui->mapLayersWidget->setComposite(composite);

    //When the user clicks "start planning" on the control widget, we want to start planning
    connect(this->ui->planningControlWidget,
            SIGNAL(planningStartRequested()),
            this,
            SLOT(handlePlanningControlStartRequested()));

    //When the user clicks "pause"
    connect(this->ui->planningControlWidget,
            SIGNAL(planningPauseRequested()),
            this,
            SLOT(handlePlanningControlPauseRequested()));

    //User clicks "reset" on the planning control widget
    connect(this->ui->planningControlWidget,
            SIGNAL(planningClearRequested()),
            this,
            SLOT(handlePlanningControlResetRequested()));

    //When the user uses the palette to request adding an end point
    connect(this->ui->paletteWidget,
            SIGNAL(addEndPointRequested()),
            this,
            SLOT(handleEndPointAddRequested()));

    //When the user uses the palette to request adding a start point
    connect(this->ui->paletteWidget,
            SIGNAL(addStartPointRequested()),
            this,
            SLOT(handleStartPointAddRequested()));

    //When the user uses the palette to request adding a task area
    connect(this->ui->paletteWidget,
            SIGNAL(addTaskAreaRequested()),
            this,
            SLOT(handleTaskAreaAddRequested()));

    //Update the state of the undo/redo buttons based on the state of the stacks when we do commands
    connect(this,
            SIGNAL(MWCommandExecuted()),
            this,
            SLOT(handleMWCommandExecuted()));

    connect(_planner,
            SIGNAL(planningStarted()),
            this->ui->planningControlWidget,
            SLOT(setIsRunning()));
    connect(_planner,
            SIGNAL(planningPaused()),
            this->ui->planningControlWidget,
            SLOT(setIsPaused()));
    connect(_planner,
            SIGNAL(planningCleared()),
            this->ui->planningControlWidget,
            SLOT(setIsStopped()));
    connect(_planner,
            SIGNAL(iterationFinished(int,qreal)),
            this->ui->planningControlWidget,
            SLOT(setPlanningProgress(int,qreal)));
    connect(_adapter,
            SIGNAL(problemHasChanged(const PlanningProblem&)),
            _planner,
            SLOT(clearPlanning()));
    connect(_adapter,
            SIGNAL(problemHasChanged(PlanningProblem)),
            _planner,
            SLOT(setProblem(PlanningProblem)));
    connect(_planner,
            SIGNAL(planningPaused()),
            this,
            SLOT(handlePlanningPaused()));
    connect(_planner,
            SIGNAL(planningCleared()),
            this,
            SLOT(handlePlanningCleared()));

    //Spawn a helpful wizard!
    /*
    PlanningWizard * wizard = new PlanningWizard(this);
    wizard->show();
    */

    //Maximize ourselves
    this->showMaximized();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//private slot
void MainWindow::handlePlanningControlStartRequested()
{
    if (!_adapter->planningProblem().isReady())
    {
        QMessageBox::information(this,
                                 "Can't plan - Problem not defined",
                                 "You need to provide more information to the planner. Have you defined a start position?");
        return;
    }
    qDebug() << "Start requested";
    _planner->startPlanning();
}

//private slot
void MainWindow::handlePlanningControlPauseRequested()
{
    qDebug() << "Pause requested";
    _planner->pausePlanning();
}

//private slot
void MainWindow::handlePlanningControlResetRequested()
{
    qDebug() << "Reset requested";
    _planner->clearPlanning();
}

//private slot
void MainWindow::handleStartPointAddRequested()
{    
    _adapter->setStartPosition(_view->center(),1500);
}

//private slot
void MainWindow::handleEndPointAddRequested()
{
    _adapter->setEndPosition(_view->center(),1500);
}

//private slot
void MainWindow::handleTaskAreaAddRequested()
{
    _adapter->addArea(_view->center());
}

//private slot
void MainWindow::handlePlanningPaused()
{
    //Get the path that is currently best
    QSharedPointer<Individual> currentBest = _planner->getCurrentBest();

    //Preview it
    foreach(MapGraphicsObject * oldPreviewObj, _pathPreviewObjects)
        oldPreviewObj->deleteLater();
    _pathPreviewObjects.clear();

    foreach(QPointF geoPos, currentBest->generateGeoPoints(_planner->problem().startingPos()))
    {
        CircleObject * circle = new CircleObject(4.0,
                                                 false,
                                                 Qt::yellow);
        circle->setPos(geoPos);
        _pathPreviewObjects.insert(circle);
        _scene->addObject(circle);
    }
}

void MainWindow::handlePlanningCleared()
{
    foreach(MapGraphicsObject * oldPreviewObj, _pathPreviewObjects)
        oldPreviewObj->deleteLater();
    _pathPreviewObjects.clear();
}

//private slot
void MainWindow::undo()
{
    if (_undoStack.isEmpty())
        return;

    QSharedPointer<MWCommand> last = _undoStack.pop();
    last->unExec();
    _redoStack.push(last);
    this->MWCommandExecuted();
}

//private slot
void MainWindow::redo()
{
    if (_redoStack.isEmpty())
        return;

    QSharedPointer<MWCommand> last = _redoStack.pop();
    last->exec();
    _undoStack.push(last);
    this->MWCommandExecuted();
}

//private slot
void MainWindow::handleMWCommandExecuted()
{
    this->ui->actionUndo->setEnabled(!_undoStack.isEmpty());
    this->ui->actionRedo->setEnabled(!_redoStack.isEmpty());
}

//private slot
void MainWindow::on_actionExit_triggered()
{
    this->close();
}

//private slot
void MainWindow::on_actionUndo_triggered()
{
    this->undo();
}

//private slot
void MainWindow::on_actionRedo_triggered()
{
    this->redo();
}

//private slot
void MainWindow::on_actionNew_triggered()
{
    //Another great place to ask them if they want to save their current doc
    _adapter->setPlanningProblem(PlanningProblem());

    _openFile.clear();
}

//private slot
void MainWindow::on_actionOpen_triggered()
{
    QString fileToReadPath = QFileDialog::getOpenFileName(this,
                                                          "Select Problem File",
                                                          QString(),
                                                          "*.planningprob");
    if (fileToReadPath.isEmpty())
        return;

    QFile fp(fileToReadPath);
    if (!fp.exists())
    {
        QMessageBox::warning(this,
                             "Non-existant file",
                             "Failed to locate " + fp.fileName() + " for reading");
        return;
    }

    if (!fp.open(QFile::ReadOnly))
    {
        QMessageBox::warning(this,
                             "Failed to open file",
                             "Failed to open " + fp.fileName() + " for reading");
        return;
    }

    QDataStream stream(&fp);
    PlanningProblem toRead;
    stream >> toRead;

    _adapter->setPlanningProblem(toRead);

    _openFile = fileToReadPath;

    QMessageBox::information(this,
                             "Success",
                             "Loaded file successfully");

    if (_adapter->planningProblem().isStartingDefined())
    {
        if (_view->zoomLevel() < 10)
            _view->setZoomLevel(15);
        _view->centerOn(_adapter->planningProblem().startingPos());
    }
}

//private slot
void MainWindow::on_actionSave_Planning_Problem_triggered()
{
    if (_openFile.isEmpty())
    {
        this->on_actionSave_As_triggered();
        return;
    }

    QFile fp(_openFile);
    if (!fp.open(QFile::WriteOnly | QFile::Truncate))
    {
        QMessageBox::warning(this,
                             "Failed to open save file",
                             "Failed to open " + fp.fileName() + " for writing. File not saved");
        return;
    }

    QDataStream stream(&fp);
    stream << _adapter->planningProblem();

    QMessageBox::information(this,
                             "Success",
                             "Saved file successfully");
}

//private slot
void MainWindow::on_actionSave_As_triggered()
{
    QString destFile = QFileDialog::getSaveFileName(this,
                                                    "Select save file",
                                                    QString(),
                                                    "*.planningprob");
    if (destFile.isEmpty())
        return;
    _openFile = destFile;
    this->on_actionSave_Planning_Problem_triggered();
}

//private slot
void MainWindow::on_actionClose_triggered()
{
    //This would be a great place to ask if they want to save
    _adapter->setPlanningProblem(PlanningProblem());
    _openFile.clear();
}

//private slot
void MainWindow::on_actionExport_Solution_triggered()
{

}

//private
void MainWindow::doCommand(QSharedPointer<MWCommand> todo)
{
    if (todo.isNull())
        return;

    //Anything in the redo stack is now invalidated
    _redoStack.clear();

    todo->exec();
    _undoStack.push(todo);
    this->MWCommandExecuted();
}
