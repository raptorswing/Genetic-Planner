#include "SensorParametersWidget.h"
#include "ui_SensorParametersWidget.h"

#include <QtDebug>
#include <QFile>
#include <QMessageBox>
#include <QFileDialog>

SensorParametersWidget::SensorParametersWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SensorParametersWidget)
{
    ui->setupUi(this);
    this->loadGUIFromCurrent();
}

SensorParametersWidget::SensorParametersWidget(SensorDefinition sensorStuff, QWidget *parent) :
    QWidget(parent), _current(sensorStuff), _original(sensorStuff)
{
    this->loadGUIFromCurrent();
}

SensorParametersWidget::~SensorParametersWidget()
{
    delete ui;
}

void SensorParametersWidget::setOriginal(const SensorDefinition &orig)
{
    _original = orig;
    this->loadGUIFromCurrent();
}

void SensorParametersWidget::setCurrent(const SensorDefinition &curr)
{
    _current = curr;
    this->loadGUIFromCurrent();
}

void SensorParametersWidget::setValues(const SensorDefinition &original, const SensorDefinition &current)
{
    _original = original;
    _current = current;
    this->loadGUIFromCurrent();
}

SensorDefinition SensorParametersWidget::current() const
{
    return _current;
}

void SensorParametersWidget::setHideCancelAndApply(bool hide)
{
    this->ui->applyButton->setHidden(hide);
    this->ui->cancelButton->setHidden(hide);
}

//private slot
void SensorParametersWidget::handleAnyValueChanged()
{
    bool changed = true;
    if (_original == _current)
        changed = false;

    this->ui->applyButton->setEnabled(changed);
}

//private slot
void SensorParametersWidget::loadGUIFromCurrent()
{
    if (_current.type() == SensorDefinition::Directional)
        this->ui->directionalityComboBox->setCurrentIndex(0);
    else
        this->ui->directionalityComboBox->setCurrentIndex(1);

    this->ui->maxDistanceSpinBox->setValue(_current.usableDistance());
    this->ui->angleOfViewSpinBox->setValue(_current.angleOfViewDegrees());
    this->ui->gimballedCheckBox->setChecked(_current.gimballed());
    this->ui->nameLineEdit->setText(_current.name());

    this->handleAnyValueChanged();
}

//private slot
void SensorParametersWidget::on_directionalityComboBox_currentIndexChanged(int index)
{
    if (index == 0)
        _current.setType(SensorDefinition::Directional);
    else
        _current.setType(SensorDefinition::OmniDirectional);

    this->handleAnyValueChanged();
}

//private slot
void SensorParametersWidget::on_maxDistanceSpinBox_valueChanged(double arg1)
{
    _current.setUsableDistance(arg1);
    this->handleAnyValueChanged();
}

//private slot
void SensorParametersWidget::on_angleOfViewSpinBox_valueChanged(double arg1)
{
    _current.setAngleOfViewDegrees(arg1);
    this->handleAnyValueChanged();
}

//private slot
void SensorParametersWidget::on_gimballedCheckBox_toggled(bool checked)
{
    _current.setGimballed(checked);
    this->handleAnyValueChanged();
}

//private slot
void SensorParametersWidget::on_nameLineEdit_editingFinished()
{
    _current.setName(this->ui->nameLineEdit->text());
    this->handleAnyValueChanged();
}

//private slot
void SensorParametersWidget::on_loadButton_clicked()
{
    QString sourceFilePath = QFileDialog::getOpenFileName(this,
                                                          "Select input file",
                                                          QString(),
                                                          "*.sensorparams");
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
                             "Sensor Parameters Loaded",
                             "Loaded Sensor parameters successfully.");
    this->loadGUIFromCurrent();
}

//private slot
void SensorParametersWidget::on_saveButton_clicked()
{
    QString destinationFilePath = QFileDialog::getSaveFileName(this,
                                                               "Select output file",
                                                               QString(),
                                                               "*.sensorparams");
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
                             "Sensor parameters written to file successfully");
}

//private slot
void SensorParametersWidget::on_resetButton_clicked()
{
    _current = SensorDefinition();
    this->loadGUIFromCurrent();
}

//private slot
void SensorParametersWidget::on_cancelButton_clicked()
{
    _current = _original;
    this->cancelled();
    this->loadGUIFromCurrent();

    if (!this->parent())
        this->close();
}

//private slot
void SensorParametersWidget::on_applyButton_clicked()
{
    _original = _current;
    this->applied(_current);
    this->handleAnyValueChanged();

    if (!this->parent())
        this->close();
}
