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
