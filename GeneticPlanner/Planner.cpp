#include "Planner.h"

#include <QList>
#include <QMap>
#include <QtDebug>
#include <QDateTime>
#include <QThreadPool>

#include "FitnessRunnable.h"


Planner::Planner()
{
    qsrand(QDateTime::currentDateTime().toTime_t());
}

Individual Planner::plan(int generations)
{
    QList<Individual> genePool;
    QMap<qreal, int> fitnesses;

    int mutantCount = 5;

    //Must be at least mutantCount
    int generationSize = 50;

    //Generate an initial set
    for (int i = 0; i < generationSize; i++)
        genePool.append(Individual());

    for (int g = 0; g < generations; g++)
    {
        if (g % 100 == 0)
            qDebug() << "Generation" << g;
        fitnesses.clear();

        //Rank current generation
/*
        for (int i = 0; i < genePool.size(); i++)
        {
            const Individual& ind = genePool[i];
            fitnesses.insert(this->fitness(ind),
                           i);
        }
*/


        QList<FitnessRunnable *> runnables;
        QThreadPool * pool = QThreadPool::globalInstance();
        for (int i = 0; i < genePool.size(); i++)
        {
            FitnessRunnable * runnable = new FitnessRunnable(this,&genePool[i]);
            pool->start(runnable);
            runnables.append(runnable);
        }
        pool->waitForDone();
        for (int i = 0; i < runnables.size(); i++)
        {
            fitnesses.insert(runnables[i]->result(),i);
            delete runnables[i];
        }

        QList<int> eliminationIndices = fitnesses.values();
        //Eliminate the bottom half of the population
        for (int i = 0; i < fitnesses.size()/2; i++)
        {
            int individualIndex = eliminationIndices[i];
            genePool.removeAt(individualIndex);
        }

        //Breed a few of the survivors
        int survivorMaxSize = genePool.size();
        while (genePool.size() < generationSize - mutantCount)
        {
            int parentAIndex = qrand() % survivorMaxSize;
            int parentBIndex = qrand() % survivorMaxSize;
            if (parentAIndex == parentBIndex)
                continue;
            genePool.append(Individual(genePool[parentAIndex],
                                       genePool[parentBIndex]));
        }

        //Create some random "mutants." Hooray for genetic diversity.
        for (int i = 0; i < mutantCount; i++)
            genePool.append(Individual());
    }
    return genePool[fitnesses.values().last()];
}

qreal Planner::fitness(const Individual &individual) const
{
    qreal toRet = 500.0;

    QList<Individual::YawAction> actions = individual.yawActions();

    //penalize each unit of path length
    //toRet -= (actions.size()*10.0);

    //Penalize insufficient path length
    int deficit = qMax<int>(0,80 - actions.size());
    toRet -= (deficit*50.0);

    //Penalize actions that don't have a left turn of radius 50
    foreach(Individual::YawAction action, actions)
    {
        /*
        if (!action.isStraight)
            toRet -= 30;
        if (action.isStraight)
            toRet -= 30;
        else
        {
            int deviation = qAbs(action.radius + 50);
            toRet -= deviation * 10.0;
        }
        */

        toRet -= qAbs(100 - action.distance)*0.09;
    }

    QList<QPointF> geoPositions = individual.generateGeoPoints(QPointF(-111.649253,40.249707));

    //Reward distance between the start and the end position
    toRet -= (geoPositions.first() - geoPositions.last()).manhattanLength()*1000;

    return toRet;
}
