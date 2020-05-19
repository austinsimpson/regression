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

#ifndef GRAPHLAYER_H
#define GRAPHLAYER_H

#include <QPen>
#include <QPainter>

#include "FunctionModel.h"

class GraphLayer
{
public:
	enum LayerType
	{
		Points,
		Function
	};

	GraphLayer(const FunctionModel& functionModel);
	GraphLayer(std::unique_ptr<IPointSetModel> pointModel);
	GraphLayer(GraphLayer&& other);
	~GraphLayer() = default;

	void setPen(const QPen& pen);
	const QPen& pen() const;
	QPen& pen();

	void setColor(const QColor& color);
	QColor color() const;

	bool isVisible() const;
	void setVisible(bool isVisible);

	void draw(QPainter& painter, const QRectF& logicalBounds = QRectF(0, 0, 0, 0), const QTransform& viewportTransform = QTransform()) const;

private:
	void drawFunction(QPainter& painter, qreal xLeft, qreal xRight, const QTransform& viewportTransform) const;

	LayerType _layerType;
	QPen _pen;

	bool _isVisible;

	FunctionModel _functionModel;
	std::unique_ptr<IPointSetModel> _pointsModel;

};

#endif // GRAPHLAYER_H
