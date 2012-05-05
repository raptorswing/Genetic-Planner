#ifndef UAVPARAMETERSWIDGET_H
#define UAVPARAMETERSWIDGET_H

#include <QWidget>

#include "UAVParameters.h"

namespace Ui {
class UAVParametersWidget;
}

class UAVParametersWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit UAVParametersWidget(QWidget *parent = 0);
    explicit UAVParametersWidget(const UAVParameters& params, QWidget * parent = 0);
    ~UAVParametersWidget();

    void setOriginal(const UAVParameters&);
    void setCurrent(const UAVParameters&);
    void setValues(const UAVParameters& original, const UAVParameters& current);
    UAVParameters current() const;

    void setHideCancelAndApply(bool);

signals:
    void applied(UAVParameters results);
    void cancelled();
    
private slots:
    void handleAnyValueChanged();

    void loadGUIFromCurrent();

    void on_applyButton_clicked();

    void on_cancelButton_clicked();

    void on_resetButton_clicked();

    void on_saveButton_clicked();

    void on_loadButton_clicked();

    void on_speedSpinBox_valueChanged(double arg1);

    void on_maxTurnSpinBox_valueChanged(double arg1);

    void on_ceilingSpinBox_valueChanged(double arg1);

    void on_maxFlightDurationSpinBox_valueChanged(double arg1);

    void on_maxClimbRateSpinBox_valueChanged(double arg1);

private:
    Ui::UAVParametersWidget *ui;

    UAVParameters _original;
    UAVParameters _current;
};

#endif // UAVPARAMETERSWIDGET_H
