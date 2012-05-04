#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "MapGraphicsScene.h"
#include "MapGraphicsView.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;

    MapGraphicsScene * _scene;
    MapGraphicsView * _view;
};

#endif // MAINWINDOW_H
