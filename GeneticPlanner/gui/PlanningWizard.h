#ifndef PLANNINGWIZARD_H
#define PLANNINGWIZARD_H

#include <QWizard>

#include "UAVParameters.h"
#include "SensorDefinition.h"
#include "PlanningProblem.h"

namespace Ui {
class PlanningWizard;
}

class PlanningWizard : public QWizard
{
    Q_OBJECT
    
public:
    explicit PlanningWizard(QWidget *parent = 0);
    ~PlanningWizard();

private slots:
    void handlePageChanged(int newPageID);
    void handleFinished(int result);
    
private:
    Ui::PlanningWizard *ui;

    PlanningProblem _problem;
};

#endif // PLANNINGWIZARD_H
