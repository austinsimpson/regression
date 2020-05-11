#ifndef REGRESSIONTRAINER_H
#define REGRESSIONTRAINER_H

#include "VectorND.h"

#include <algorithm>

constexpr auto kLearningRate = 0.01;


//Represents a regression model with n many independent variables.
template <int n>
class Regression
{
public:
    typedef QPair<VectorND<n>, qreal> TrainingPoint;
    typedef QVector<TrainingPoint> TrainingPoints; //Represents a contiguous array in memory whose elements are the training point x and its real observed value y.

    Regression(){}
    Regression(const TrainingPoints& trainingPoints) : _trainingPoints(trainingPoints) {}



    void setTrainingPoints(const TrainingPoints& trainingPoints)
    {
        _trainingPoints = trainingPoints;
    }

    void train(int numberOfBatches)
    {
        if (_trainingPoints.size() == 0)
            return;

        for (auto unused : range(0, numberOfBatches))
        {
            _weights -= kLearningRate * costDerivative();
        }
    }

    qreal evaluate(const VectorND<n>& input) const
    {
        return _weights * input;
    }

    qreal cost() const
    {
        qreal result = std::accumulate(_trainingPoints.begin(), _trainingPoints.end(), 0.0, [this](qreal runningSum, const TrainingPoint& trainingPoint)
        {
            qreal approximationDifference = _weights * trainingPoint.first.asHomogenous() - trainingPoint.second;
            return runningSum + ((approximationDifference) * (approximationDifference));
        });

        return result / (2 * _trainingPoints.size());
    }

    VectorND<n + 1> costDerivative() const
    {
        qreal result = std::accumulate(_trainingPoints.begin(), _trainingPoints.end(), 0.0, [this](TrainingPoint runningSum, const TrainingPoint& trainingPoint)
        {
            return runningSum + trainingPoint.first * (_weights * trainingPoint.first.asHomogenous() - trainingPoint.second);
        });
        return result / _trainingPoints.size();
    }

private:
    VectorND<n + 1> _weights; //We add an additional dimension to allow for biasing, IE y-intercept.
    TrainingPoints _trainingPoints;
};

#endif // REGRESSIONTRAINER_H
