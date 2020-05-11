#ifndef POINTSWIDGET_H
#define POINTSWIDGET_H

#include <QWidget>

#include <QVector>
#include <QPoint>



class PointsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PointsWidget(QWidget* parent = nullptr);

    void paintEvent(QPaintEvent* paintEvent);

    void mousePressEvent(QMouseEvent* mouseEvent);
    void mouseMoveEvent(QMouseEvent* mouseEvent);
    void mouseReleaseEvent(QMouseEvent* mouseEvent);

    void wheelEvent(QWheelEvent* wheelEvent);
    void resizeEvent(QResizeEvent* resizeEvent);

    void centerOnLogicalCoordinate(const QPointF& logicalCoord);

public slots:
    void reset();
signals:


private:
    QRectF visibleBounds() const;

    void drawGrid(QPainter& painter);
    void drawMinorGridline(QPainter& painter, int index, Qt::Orientation orientation);
    void drawMajorGridline(QPainter& painter, int index, Qt::Orientation orientation);
    void adjustGridlineScale();

    void computeTransform();

    QPointF _zoomCenterInPixelSpace;


    QPen penForGridlineIndex(int index);

    uint8_t _currentIncrementBase;
    int32_t _currentIncrementExponent;

    int _pointRadius;
    QPointF _mouseLocation;
    QPointF* _hoveredPoint;
    QVector<QPointF> _points;

    QTransform _viewportTransform;
    QTransform _inverseViewportTransform;

    qreal _zoomLevel;
    QPointF _translationInPixelSpace;
    QPointF _zoomCenter;

    QRectF _logicalBounds;
};

#endif // POINTSWIDGET_H
