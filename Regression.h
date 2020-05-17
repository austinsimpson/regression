#ifndef REGRESSIONTRAINER_H
#define REGRESSIONTRAINER_H

#include "VectorND.h"
#include "Range.h"

#include <algorithm>

//Copyright(c) 2020 Austin Simpson
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files(the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions :
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.

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

		for (auto _ : range(0, numberOfBatches))
        {
			_weights -= costDerivative() * kLearningRate;
        }
		int i = 0;
		i++;
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
		auto result = std::accumulate(_trainingPoints.begin(), _trainingPoints.end(), VectorND<n+1>(), [this](VectorND<n + 1> runningSum, const TrainingPoint& trainingPoint)
        {
			return runningSum + trainingPoint.first.asHomogenous() * (_weights * trainingPoint.first.asHomogenous() - trainingPoint.second);
        });
        return result / _trainingPoints.size();
    }

private:
    VectorND<n + 1> _weights; //We add an additional dimension to allow for biasing, IE y-intercept.
    TrainingPoints _trainingPoints;
};

#endif // REGRESSIONTRAINER_H
