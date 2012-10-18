#include "PaletteWidget.h"
#include "ui_PaletteWidget.h"

PaletteWidget::PaletteWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PaletteWidget)
{
    ui->setupUi(this);
}

PaletteWidget::~PaletteWidget()
{
    delete ui;
}

void PaletteWidget::on_startPointButton_clicked()
{
    this->addStartPointRequested();
}

void PaletteWidget::on_taskAreaButton_clicked()
{
    this->addTaskAreaRequested();
}
