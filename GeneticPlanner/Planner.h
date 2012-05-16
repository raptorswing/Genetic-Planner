#ifndef PLANNER_H
#define PLANNER_H

#include "Individual.h"
#include "PlanningProblem.h"

#include <QMap>
#include <QSharedPointer>
#include <QObject>
#include <QTimer>

class Planner : public QObject
{
    Q_OBJECT

signals:
    void planningStarted();
    void planningPaused();
    void planningFinished();
    void iterationFinished(int iterationNumber, qreal bestFitness);

public:
    Planner(const PlanningProblem& problem, QObject * parent=0);
    ~Planner();

    QSharedPointer<Individual> getCurrentBest() const;

    qreal getBestFitnessSoFar() const;

    void setProblem(const PlanningProblem& problem);

public slots:
    void startPlanning();
    void pausePlanning();
    void clearPlanning();
    void doIteration();

private:
    PlanningProblem _problem;

    QMap<qreal, QSharedPointer<Individual> > _genePool;

    int _mutantCount;
    int _breedCount;
    int _keepCount;

    qreal _bestFitnessSoFar;
    int _iterationCounter;

    QTimer * _iterationTimer;
};

#endif // PLANNER_H
