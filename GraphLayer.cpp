#include "GraphLayer.h"

GraphLayer::GraphLayer
(
	const FunctionModel& functionModel
):
	_isVisible(true),
	_pen(QColor(0, 0, 0), 2.),
	_layerType(LayerType::Function),
	_functionModel(functionModel)
{

}

GraphLayer::GraphLayer
(
	std::unique_ptr<IPointSetModel> pointSetModel
):
	_isVisible(true),
	_layerType(LayerType::Points),
	_pointsModel(std::move(pointSetModel)),
	_pen(QColor(0, 0, 0), 3)
{

}

GraphLayer::GraphLayer
(
	GraphLayer&& other
):
	_isVisible(true),
	_pointsModel(std::move(other._pointsModel)),
	_functionModel(other._functionModel),
	_layerType(other._layerType),
	_pen(QColor(0, 0, 0), 3)
{

}

void GraphLayer::setPen
(
	const QPen& pen
)
{
	_pen = pen;
}

const QPen& GraphLayer::pen() const
{
	return _pen;
}

QPen& GraphLayer::pen()
{
	return _pen;
}

void GraphLayer::setColor
(
	const QColor& color
)
{
	_pen.setColor(color);
}

QColor GraphLayer::color() const
{
	return _pen.color();
}

bool GraphLayer::isVisible() const
{
	return _isVisible;
}
void GraphLayer::setVisible
(
	bool isVisible
)
{
	_isVisible = isVisible;
}

void GraphLayer::draw
(
	QPainter& painter,
	const QRectF& logicalBounds,
	const QTransform& viewportTransform
)	const
{
	if (_isVisible)
	{
		painter.setPen(_pen);
		switch (_layerType)
		{
		case Points:
			for (const auto& point: *_pointsModel)
			{
				if (logicalBounds.contains(point))
				{
					painter.drawPoint(viewportTransform.map(point));
				}
			}
			break;
		case Function:
			drawFunction(painter, logicalBounds.left(), logicalBounds.right(), viewportTransform);
			break;
		}
	}
}

void GraphLayer::drawFunction
(
	QPainter& painter,
	qreal xLeft,
	qreal xRight,
	const QTransform& viewportTransform
)	const
{
	const auto distanceThreshold = pow(painter.pen().width(), 2);

	const auto leftPoint = viewportTransform.map(QPointF{ xLeft, _functionModel(xLeft) });
	const auto rightPoint = viewportTransform.map(QPointF{ xRight, _functionModel(xRight)});

	painter.drawPoint(leftPoint);
	painter.drawPoint(rightPoint);
	const auto difference = leftPoint - rightPoint;
	if (QPointF::dotProduct(difference, difference) > distanceThreshold)
	{
		const auto xMid = (xLeft + xRight) / 2;
		drawFunction(painter, xLeft, xMid, viewportTransform);
		drawFunction(painter, xMid, xRight, viewportTransform);
	}
}
