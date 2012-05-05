#include "UAVParametersWidget.h"
#include "ui_UAVParametersWidget.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QDataStream>

UAVParametersWidget::UAVParametersWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UAVParametersWidget)
{
    ui->setupUi(this);

    this->loadGUIFromCurrent();
}

UAVParametersWidget::UAVParametersWidget(const UAVParameters &params, QWidget *parent) :
    QWidget(parent), _original(params), _current(params)
{
    this->loadGUIFromCurrent();
}

UAVParametersWidget::~UAVParametersWidget()
{
    delete ui;
}

void UAVParametersWidget::setOriginal(const UAVParameters & orig)
{
    _original = orig;
    this->loadGUIFromCurrent();
}

void UAVParametersWidget::setCurrent(const UAVParameters & curr)
{
    _current = curr;
    this->loadGUIFromCurrent();
}

void UAVParametersWidget::setValues(const UAVParameters &original, const UAVParameters &current)
{
    _original = original;
    _current = current;
    this->loadGUIFromCurrent();
}

UAVParameters UAVParametersWidget::current() const
{
    return _current;
}

void UAVParametersWidget::setHideCancelAndApply(bool hide)
{
    this->ui->applyButton->setHidden(hide);
    this->ui->cancelButton->setHidden(hide);
}

//private slot
void UAVParametersWidget::handleAnyValueChanged()
{
    bool changed = true;
    if (_original == _current)
        changed = false;

    this->ui->applyButton->setEnabled(changed);
}

//private slot
void UAVParametersWidget::loadGUIFromCurrent()
{
    this->ui->speedSpinBox->setValue(_current.speed());
    this->ui->maxTurnSpinBox->setValue(_current.maxTurningRate());
    this->ui->ceilingSpinBox->setValue(_current.ceiling());
    this->ui->maxFlightDurationSpinBox->setValue(_current.maxFlightTime());
    this->ui->maxClimbRateSpinBox->setValue(_current.maxClimbRate());

    this->handleAnyValueChanged();
}

//private slot
void UAVParametersWidget::on_applyButton_clicked()
{
    _original = _current;
    this->handleAnyValueChanged();
    this->applied(_current);

    if (!this->parent())
        this->close();
}

//private slot
void UAVParametersWidget::on_cancelButton_clicked()
{
    _current = _original;
    this->loadGUIFromCurrent();
    this->cancelled();

    if (!this->parent())
        this->close();
}

//private slot
void UAVParametersWidget::on_resetButton_clicked()
{
    _current = UAVParameters();
    this->loadGUIFromCurrent();
}

//private slot
void UAVParametersWidget::on_saveButton_clicked()
{
    QString destinationFilePath = QFileDialog::getSaveFileName(this,
                                                               "Select output file",
                                                               QString(),
                                                               "*.uavparams");
    if (destinationFilePath.isEmpty())
        return;

    QFile fp(destinationFilePath);

    //Try to open the file for writing
    if (!fp.open(QFile::WriteOnly | QFile::Truncate))
    {
        QMessageBox::warning(this,
                             "Failed to open file",
                             "Failed to open " + fp.fileName() + ". Error: " + fp.errorString());
        return;
    }

    QDataStream stream(&fp);
    stream << _current;

    QMessageBox::information(this,
                             "Written",
                             "UAV parameters written to file successfully");

}

//private slot
void UAVParametersWidget::on_loadButton_clicked()
{
    QString sourceFilePath = QFileDialog::getOpenFileName(this,
                                                          "Select input file",
                                                          QString(),
                                                          "*.uavparams");
    if (sourceFilePath.isEmpty())
        return;

    QFile fp(sourceFilePath);
    if (!fp.exists())
    {
        QMessageBox::warning(this,
                             "File does not exist",
                             "File does not exist. Could not load parameters.");
        return;
    }

    if (!fp.open(QFile::ReadOnly))
    {
        QMessageBox::warning(this,
                             "Failed to open file",
                             "Failed to open parameter file for reading");
        return;
    }

    QDataStream stream(&fp);
    stream >> _current;
    QMessageBox::information(this,
                             "UAV Parameters Loaded",
                             "Loaded UAV parameters successfully.");
    this->loadGUIFromCurrent();
}

//private slot
void UAVParametersWidget::on_speedSpinBox_valueChanged(double arg1)
{
    _current.setSpeed(arg1);
    this->handleAnyValueChanged();
}

//private slot
void UAVParametersWidget::on_maxTurnSpinBox_valueChanged(double arg1)
{
    _current.setMaxTurnRate(arg1);
    this->handleAnyValueChanged();
}

//private slot
void UAVParametersWidget::on_ceilingSpinBox_valueChanged(double arg1)
{
    _current.setCeiling(arg1);
    this->handleAnyValueChanged();
}

//private slot
void UAVParametersWidget::on_maxFlightDurationSpinBox_valueChanged(double arg1)
{
    _current.setMaxFlightTime(arg1);
    this->handleAnyValueChanged();
}

//private slot
void UAVParametersWidget::on_maxClimbRateSpinBox_valueChanged(double arg1)
{
    _current.setMaxClimbRate(arg1);
    this->handleAnyValueChanged();
}
