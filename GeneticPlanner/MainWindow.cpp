#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QtDebug>

#include "tileSources/CompositeTileSource.h"
#include "tileSources/OSMTileSource.h"
#include "guts/CompositeTileSourceConfigurationWidget.h"
#include "CircleObject.h"

#include "Planner.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);



    _scene = new MapGraphicsScene(this);
    _view = new MapGraphicsView(_scene,this);
    this->setCentralWidget(_view);

    QSharedPointer<CompositeTileSource> composite(new CompositeTileSource());
    QSharedPointer<MapTileSource> osm(new OSMTileSource());
    QSharedPointer<MapTileSource> mqSat(new OSMTileSource(OSMTileSource::MapQuestAerialTiles));
    composite->addSourceBottom(osm,0.75);
    composite->addSourceBottom(mqSat);
    _view->setTileSource(composite);

    CompositeTileSourceConfigurationWidget * dockContents = new CompositeTileSourceConfigurationWidget(composite,
                                                                                                       this->ui->mapLayersDockWidget);
    this->ui->mapLayersDockWidget->setWidget(dockContents);
    delete this->ui->dockWidgetContents;

    QPointF place(-111.649253,40.249707);
    _view->setZoomLevel(15);
    _view->centerOn(place);


    /*
    Planner p;
    Individual best = p.plan(1000);

    qDebug() << "Result has length" << best.yawActions().size() << "and fitness" << p.fitness(best);
    QList<QPointF> geoPath = best.generateGeoPoints(place);


    foreach(QPointF pos, geoPath)
    {
        CircleObject * circle = new CircleObject(4.0,false,Qt::red);
        circle->setPos(pos);
        _scene->addObject(circle);
        //qDebug() << QString::number(pos.x(),'g',10) << QString::number(pos.y(),'g',10);
    }
    */
}

MainWindow::~MainWindow()
{
    delete ui;
}
