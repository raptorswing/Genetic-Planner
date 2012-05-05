#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QtDebug>

#include "tileSources/CompositeTileSource.h"
#include "tileSources/OSMTileSource.h"
#include "guts/CompositeTileSourceConfigurationWidget.h"
#include "CircleObject.h"
#include "UAVParametersWidget.h"
#include "SensorParametersWidget.h"
#include "PlanningControlWidget.h"
#include "Planner.h"
#include "PlanningWizard.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Setup MapGraphicsScene and View
    _scene = new MapGraphicsScene(this);
    _view = new MapGraphicsView(_scene,this);
    this->setCentralWidget(_view);

    //Setup tile sources for the MapGraphicsView
    QSharedPointer<CompositeTileSource> composite(new CompositeTileSource());
    QSharedPointer<MapTileSource> osm(new OSMTileSource());
    QSharedPointer<MapTileSource> mqSat(new OSMTileSource(OSMTileSource::MapQuestAerialTiles));
    composite->addSourceBottom(osm,0.75);
    composite->addSourceBottom(mqSat);
    _view->setTileSource(composite);

    //Zoom into BYU campus by default
    QPointF place(-111.649253,40.249707);
    _view->setZoomLevel(15);
    _view->centerOn(place);

    //Create a helpful dock widget to allow the user to tweak map layers
    CompositeTileSourceConfigurationWidget * layerAdjust = new CompositeTileSourceConfigurationWidget(composite,this->ui->mapLayersDockWidget);
    this->ui->mapLayersDockWidget->setWidget(layerAdjust);
    delete this->ui->dockWidgetContents;

    //Create a helpful dock widget to allow the user to start/stop planning
    PlanningControlWidget * planControl = new PlanningControlWidget(this->ui->planningControlDockWidget);
    this->ui->planningControlDockWidget->setWidget(planControl);
    delete this->ui->dockWidgetContents_5;

    //Spawn a helpful wizard!
    PlanningWizard * wizard = new PlanningWizard(this);
    wizard->show();

    //Maximize ourselves
    this->showMaximized();
}

MainWindow::~MainWindow()
{
    delete ui;
}
