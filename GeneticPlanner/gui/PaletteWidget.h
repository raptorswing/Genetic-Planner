#ifndef PALETTEWIDGET_H
#define PALETTEWIDGET_H

#include <QWidget>

namespace Ui {
class PaletteWidget;
}

class PaletteWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit PaletteWidget(QWidget *parent = 0);
    ~PaletteWidget();

signals:
    void addStartPointRequested();
    void addTaskAreaRequested();
    
private slots:
    void on_startPointButton_clicked();

    void on_taskAreaButton_clicked();

private:
    Ui::PaletteWidget *ui;
};

#endif // PALETTEWIDGET_H
