#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include <QList>
#include <QPointF>

/*!
 \brief Represents an individual in the genetic planner. An individual is basically a series of genes.

*/
class Individual
{
public:
    struct YawAction
    {
        bool isStraight;
        qint32 distance;
        qreal turningRateRadsPerSecond;
    };
public:
    /*!
     \brief Creates a random individual with a random number of actions/genes

    */
    Individual();

    /*!
     \brief Creates an Individual that is a "child" of Individuals A and B

     \param A
     \param B
*/
    Individual(const Individual& A, const Individual& B);

    /*!
     \brief Creates an individual with random actions. Takes the number of actions desired

     \param numberOfActions
    */
    Individual(int numberOfActions);

    ~Individual();

    QList<YawAction> yawActions() const;

    void appendYawAction(const YawAction&);

    QList<QPointF> generateGeoPoints(const QPointF& startingPos) const;

private:
    QList<YawAction> _yawActions;

    static YawAction randomYawAction();

};

#endif // INDIVIDUAL_H
