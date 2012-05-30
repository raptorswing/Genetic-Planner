#include "PlanningWizard.h"
#include "ui_PlanningWizard.h"

#include <QtDebug>

PlanningWizard::PlanningWizard(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::PlanningWizard)
{
    ui->setupUi(this);

    /*
    this->ui->uavParams->setValues(_problem.uavSettings(),
                                   _problem.uavSettings());
    this->ui->sensorParams->setValues(_problem.sensorSettings(),
                                      _problem.sensorSettings());
                                      */
    this->ui->uavParams->setHideCancelAndApply(true);
    this->ui->sensorParams->setHideCancelAndApply(true);

    connect(this,
            SIGNAL(currentIdChanged(int)),
            this,
            SLOT(handlePageChanged(int)));

    connect(this,
            SIGNAL(finished(int)),
            this,
            SLOT(handleFinished(int)));
}

PlanningWizard::~PlanningWizard()
{
    delete ui;
}

//private slot
void PlanningWizard::handlePageChanged(int newPageID)
{
    Q_UNUSED(newPageID)
    //If the user advances to the next page, use the UAV values they set even if they didn't "apply" them.
    /*
    if (newPageID == 1)
        _problem.setUAVSettings(this->ui->uavParams->current());
        */
}

//private slot
void PlanningWizard::handleFinished(int result)
{
    Q_UNUSED(result)
    /*
    if (result == 1)
        _problem.setSensorSettings(this->ui->sensorParams->current());
        */
}
