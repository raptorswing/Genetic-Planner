#include "Planner.h"

#include <QList>
#include <QMap>
#include <QtDebug>
#include <QDateTime>
#include <QThreadPool>
#include <cmath>
#include <QHostInfo>

#include "FitnessRunnable.h"
#include "guts/Conversions.h"

const qreal PI = 3.1415926535897932384626433;
const qreal SQRT2PI = sqrt(2.0*PI);

Planner::Planner(QSharedPointer<PlanningProblem> problem, QObject *parent) :
    QObject(parent)
{
    qsrand(QDateTime::currentDateTime().toTime_t() ^ qHash(QHostInfo::localHostName()));

    _mutantCount = 150;
    _breedCount = 15;
    _keepCount = 25;
    _bestFitnessSoFar = 0.0;
    _iterationCounter = 0;

    _iterationTimer = new QTimer(this);
    connect(_iterationTimer,
            SIGNAL(timeout()),
            this,
            SLOT(doIteration()));

    this->setProblem(problem);
}

Planner::~Planner()
{
}

QSharedPointer<Individual> Planner::getCurrentBest() const
{
    if (_genePool.isEmpty())
        return QSharedPointer<Individual>(new Individual());

    const QSharedPointer<Individual> best = _genePool.value(_genePool.keys().last());

    return best;
}

qreal Planner::getBestFitnessSoFar() const
{
    return _bestFitnessSoFar;
}

QSharedPointer<PlanningProblem> Planner::problem() const
{
    return _problem;
}

//public slot
void Planner::setProblem(QSharedPointer<PlanningProblem> problem)
{
    this->clearPlanning();
    _problem = problem;
    this->newProblemSet(problem);

    connect(problem.data(),
            SIGNAL(changed()),
            this,
            SLOT(clearPlanning()));
}

//public slot
void Planner::startPlanning()
{
    _iterationTimer->start(0);
    this->planningStarted();
}

//public slot
void Planner::pausePlanning()
{
    _iterationTimer->stop();
    this->planningPaused();
}

//public slot
void Planner::clearPlanning()
{
    _iterationTimer->stop();
    _bestFitnessSoFar = 0.0;
    _iterationCounter = 0;
    _genePool.clear();
    this->planningCleared();
}

//public slot
void Planner::doIteration()
{
    //Breed the survivors
    QList<QSharedPointer<Individual> > newIndividuals;
    QList<qreal> fitnesses = _genePool.keys();
    for (int i = 0; i < _breedCount && _genePool.size() > 2; i++)
    {
        const int indexA = qrand() % fitnesses.size();
        const int indexB = qrand() % fitnesses.size();
        const qreal keyA = fitnesses[indexA];
        const qreal keyB = fitnesses[indexB];
        QSharedPointer<Individual> A = _genePool.value(keyA);
        QSharedPointer<Individual> B = _genePool.value(keyB);
        QSharedPointer<Individual> newIndividual(new Individual(*A,*B));
        newIndividuals.append(newIndividual);
    }

    //Create some mutants
    //while (_genePool.size() < generationSize)
    for (int i = 0; i < _mutantCount; i++)
    {
        QSharedPointer<Individual> newIndividual(new Individual());
        newIndividuals.append(newIndividual);
    }


    for (int i = 0; i < 5 && _genePool.size() > 0; i++)
    {
        QSharedPointer<Individual> newIndividual(new Individual(*_genePool.values()[qrand() % _genePool.size()], 2));
        newIndividuals.append(newIndividual);
    }


    //Rank all the new individuals
    QThreadPool * pool = QThreadPool::globalInstance();
    QList<FitnessRunnable  *> runnables;
    foreach(QSharedPointer<Individual> ind, newIndividuals)
    {
        FitnessRunnable * runnable = new FitnessRunnable(_problem,ind);
        runnables.append(runnable);
        pool->start(runnable);
    }
    pool->waitForDone();
    for (int i = 0; i < newIndividuals.size(); i++)
    {
        qreal fitness = runnables[i]->result();
        QSharedPointer<Individual> ind = newIndividuals[i];
        ind->setUtility(fitness);
        _genePool.insertMulti(fitness,ind);
        delete runnables[i];
    }
    runnables.clear();
    newIndividuals.clear();

    //Find the best individual
    qreal bestFitness = _genePool.keys().last();

    //If the best individual in this generation is the best so far, record it
    if (bestFitness > _bestFitnessSoFar)
        _bestFitnessSoFar = bestFitness;

    //Eliminate half
    for (int i = 0; i < _genePool.size() - _keepCount; i++)
        _genePool.take(_genePool.keys().first());

    this->iterationFinished(++_iterationCounter,
                            _bestFitnessSoFar);
}
