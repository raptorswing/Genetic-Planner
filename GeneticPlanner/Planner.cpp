#include "Planner.h"

#include <QList>
#include <QMap>
#include <QtDebug>
#include <QDateTime>
#include <QThreadPool>
#include <cmath>

#include "FitnessRunnable.h"
#include "guts/Conversions.h"

const qreal PI = 3.1415926535897932384626433;
const qreal SQRT2PI = sqrt(2.0*PI);

Planner::Planner()
{
    qsrand(QDateTime::currentDateTime().toTime_t());
}

Individual Planner::plan(int generations)
{
    QMap<qreal, Individual *> genePool;

    const int mutantCount = 150;
    const int breedCount = 15;

    //Must be at least mutantCount
    const int generationSize = mutantCount + breedCount;

    qreal desiredBest = 2000;
    qreal bestSoFar = 0.0;
    for (int g = 0; true ; g++)
    {
        //Breed the survivors
        QList<Individual *> newIndividuals;
        QList<qreal> fitnesses = genePool.keys();
        for (int i = 0; i < breedCount && genePool.size() > 2; i++)
        {
            const int indexA = qrand() % fitnesses.size();
            const int indexB = qrand() % fitnesses.size();
            const qreal keyA = fitnesses[indexA];
            const qreal keyB = fitnesses[indexB];
            Individual * A = genePool.value(keyA);
            Individual * B = genePool.value(keyB);
            Individual * newIndividual = new Individual(*A,*B);
            /*
            qreal fitness = this->fitness(*newIndividual);
            newIndividual->setUtility(fitness);
            genePool.insertMulti(newIndividual->computedUtility(),newIndividual);
            */
            newIndividuals.append(newIndividual);
        }

        //Create some mutants
        //while (genePool.size() < generationSize)
        for (int i = 0; i < generationSize; i++)
        {
            Individual * newIndividual = new Individual();
            /*
            qreal fitness = this->fitness(*newIndividual);
            newIndividual->setUtility(fitness);
            genePool.insertMulti(newIndividual->computedUtility(),newIndividual);
            */
            newIndividuals.append(newIndividual);
        }

        //Rank all the new individuals
        QThreadPool * pool = QThreadPool::globalInstance();
        QList<FitnessRunnable  *> runnables;
        foreach(Individual * ind, newIndividuals)
        {
            FitnessRunnable * runnable = new FitnessRunnable(this,ind);
            runnables.append(runnable);
            pool->start(runnable);
        }
        pool->waitForDone();
        for (int i = 0; i < newIndividuals.size(); i++)
        {
            qreal fitness = runnables[i]->result();
            Individual * ind = newIndividuals[i];
            ind->setUtility(fitness);
            genePool.insertMulti(fitness,ind);
            delete runnables[i];
        }
        runnables.clear();
        newIndividuals.clear();

        //Find the best individual
        qreal bestFitness = genePool.keys().last();
        if (bestFitness > desiredBest)
        {
            qDebug() << bestFitness << ">" << desiredBest;
            qDebug() << "Finished in" << g << "generations";
            break;
        }
        if (bestFitness > bestSoFar)
            bestSoFar = bestFitness;

        if (g % 100 == 0)
        {
            qDebug() << "Generation" << g << "has" << genePool.size() << "individuals, best has fitness" << bestSoFar;
            qsrand(QDateTime::currentDateTime().toTime_t());
        }

        //Eliminate half
        for (int i = 0; i < genePool.size()/2; i++)
        {
            Individual * removed = genePool.take(genePool.keys().first());
            delete removed;
        }
    }
    Individual toRet = *genePool[genePool.keys().last()];

    while (genePool.size() > 0)
    {
        Individual * removed = genePool.take(genePool.keys().first());
        delete removed;
    }

    return toRet;
}

qreal Planner::fitness(Individual &individual) const
{
    if (individual.isUtilityComputed())
        return individual.computedUtility();
    qreal toRet = 0.0;

    //QList<Individual::YawAction> actions = individual.yawActions();

    QList<QPointF> geoPositions = individual.generateGeoPoints(QPointF(-111.649253,40.249707));


    //Kiwanis Park
    {
        QPointF goalPoint(-111.639990,40.246501);
        qreal stdDev = 30;
        quint64 withinStdDevCount = 0;
        quint64 goalWithinStdDev = 1;
        qreal goalScore = 0.0;
        foreach(QPointF geoPos, geoPositions)
        {
            QVector3D enuPos = Conversions::lla2enu(geoPos.y(),geoPos.x(),1409,
                                                    goalPoint.y(),goalPoint.x(),1409);
            qreal dist = enuPos.length();

            if (dist < stdDev)
            {
                if (++withinStdDevCount >= goalWithinStdDev)
                {
                    goalScore += 500;
                    break;
                }
            }
            goalScore += 10*Planner::normal(dist,stdDev,1000);
        }

        goalScore = qMin<qreal>(500.0,goalScore);
        toRet += goalScore;
    }


    //Apartments
    {
        QPointF goalPoint(-111.645151,40.241617);
        qreal stdDev = 30;
        quint64 withinStdDevCount = 0;
        quint64 goalWithinStdDev = 1;
        qreal goalScore = 0.0;
        foreach(QPointF geoPos, geoPositions)
        {
            QVector3D enuPos = Conversions::lla2enu(geoPos.y(),geoPos.x(),1409,
                                                    goalPoint.y(),goalPoint.x(),1409);
            qreal dist = enuPos.length();

            if (dist < stdDev)
            {
                if (++withinStdDevCount >= goalWithinStdDev)
                {
                    goalScore += 500;
                    break;
                }
            }
            goalScore += 10*Planner::normal(dist,stdDev,1000);
        }

        goalScore = qMin<qreal>(500.0,goalScore);
        toRet += goalScore;
    }

    //JFSB
    {
        QPointF goalPoint(-111.651234,40.248422);
        qreal stdDev = 30;
        quint64 withinStdDevCount = 0;
        quint64 goalWithinStdDev = 1;
        qreal goalScore = 0.0;
        foreach(QPointF geoPos, geoPositions)
        {
            QVector3D enuPos = Conversions::lla2enu(geoPos.y(),geoPos.x(),1409,
                                                    goalPoint.y(),goalPoint.x(),1409);
            qreal dist = enuPos.length();

            if (dist < stdDev)
            {
                if (++withinStdDevCount >= goalWithinStdDev)
                {
                    goalScore += 500;
                    break;
                }
            }
            goalScore += 10*Planner::normal(dist,stdDev,1000);
        }

        goalScore = qMin<qreal>(500.0,goalScore);
        toRet += goalScore;
    }

    //End where we began maybe
    {
        //QPointF goalPoint = geoPositions.first();
        QPointF goalPoint(-111.645151,40.241617);
        qreal stdDev = 30;
        qreal goalScore = 0.0;
        QPointF geoPos = geoPositions.last();
        QVector3D enuPos = Conversions::lla2enu(geoPos.y(),geoPos.x(),1409,
                                                goalPoint.y(),goalPoint.x(),1409);
        qreal dist = enuPos.length();

        if (dist < stdDev)
            goalScore += 500;
        else
            goalScore += 10*Planner::normal(dist,stdDev, 1000);

        goalScore = qMin<qreal>(500.0,goalScore);
        toRet += goalScore;
    }

    //Punish long paths
    {
        qreal shortReward = 10* toRet / geoPositions.length();
        toRet += shortReward;
    }

    individual.setUtility(toRet);
    return toRet;
}

qreal Planner::normal(qreal x, qreal stdDev, qreal scaleFactor)
{
    qreal expPart = exp(-0.5 * pow(x / stdDev, 2.0) / scaleFactor);
    qreal otherPart = stdDev * SQRT2PI;
    return (1.0 / otherPart) * expPart;
}
