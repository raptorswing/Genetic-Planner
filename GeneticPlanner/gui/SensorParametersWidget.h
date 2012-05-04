#ifndef SENSORPARAMETERSWIDGET_H
#define SENSORPARAMETERSWIDGET_H

#include <QWidget>

#include "SensorDefinition.h"

namespace Ui {
class SensorParametersWidget;
}

class SensorParametersWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit SensorParametersWidget(QWidget *parent = 0);
    explicit SensorParametersWidget(SensorDefinition sensorStuff, QWidget * parent=0);
    ~SensorParametersWidget();

signals:
    void applied(SensorDefinition results);
    void cancelled();
    
private slots:
    void handleAnyValueChanged();

    void loadGUIFromCurrent();

    void on_directionalityComboBox_currentIndexChanged(int index);

    void on_maxDistanceSpinBox_valueChanged(double arg1);

    void on_angleOfViewSpinBox_valueChanged(double arg1);

    void on_gimballedCheckBox_toggled(bool checked);

    void on_nameLineEdit_editingFinished();

    void on_loadButton_clicked();

    void on_saveButton_clicked();

    void on_resetButton_clicked();

    void on_cancelButton_clicked();

    void on_applyButton_clicked();

private:
    Ui::SensorParametersWidget *ui;

    SensorDefinition _original;
    SensorDefinition _current;
};

#endif // SENSORPARAMETERSWIDGET_H
