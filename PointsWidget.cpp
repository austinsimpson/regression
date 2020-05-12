#include "PointsWidget.h"

#include <QBitmap>
#include <QPainter>
#include <QMouseEvent>

#include <cmath>

#include <QDebug>

constexpr int kGridIncrement = 1;
constexpr auto kE = 2.71828182845904523536;
constexpr auto kPi = 3.1415;
constexpr auto kAxisTextMargins = 5.;

constexpr auto kGridUpdateThreshold = 100.;

const QMap<int, int> kMajorIndexForIncrementBase = {
    {1, 5},
    {2, 4},
    {5, 5}
};

PointsWidget::PointsWidget
(
    QWidget* parent
):
    QWidget(parent),
    _pointRadius(3),
    _mouseLocation(-1, -1),
    _zoomLevel(0),
    _translationInPixelSpace(0.0,0.0),
    _zoomCenter(0, 0),
    _currentIncrementBase(2),
    _currentIncrementExponent(0)
{
    setMouseTracking(true);
    ///setCursor(Qt::BlankCursor);

    computeTransform();

    _majorGridlinePen.setColor(QColor(25, 25, 25));
    _majorGridlinePen.setWidth(1);

    _minorGridlinePen.setColor(Qt::lightGray);
    _minorGridlinePen.setWidth(1);
}


void PointsWidget::paintEvent
(
    QPaintEvent* paintEvent
)
{
    Q_UNUSED(paintEvent);

    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);
    
	painter.setBrush(Qt::lightGray);
	painter.fillRect(0, 0, width(), height(), Qt::white);

    drawGrid(painter);

    painter.setPen(QColor(0, 0, 0, 126));
    painter.setBrush(QColor(0, 0, 0, 126));

    for (const auto& point : _points)
    {
        if (_logicalBounds.contains(point))
        {
            painter.drawEllipse(_viewportTransform.map(point), _pointRadius, _pointRadius);
        }
    }

    painter.end();
}

void PointsWidget::drawGrid(QPainter& painter)
{
    if (!painter.isActive())
    {
        painter.begin(this);
    }
    const auto logicalLineMajorIncrement = _currentIncrementBase * pow(10, _currentIncrementExponent);
    const double logicalLineMinorIncrement = static_cast<double>(logicalLineMajorIncrement) / kMajorIndexForIncrementBase[_currentIncrementBase];
    
    const auto firstVerticalLineCoord = _logicalBounds.left() - fmod(_logicalBounds.left(), logicalLineMinorIncrement);
    const auto numberOfVerticalLines = static_cast<int>(_logicalBounds.width() / logicalLineMinorIncrement);

    for (auto i = 0; i < numberOfVerticalLines; ++i)
    {
        const auto logicalLinePosition = firstVerticalLineCoord + (i * logicalLineMinorIncrement);
        if (qFuzzyCompare(fmod(logicalLinePosition, logicalLineMajorIncrement), 0.0))
        {
            painter.setPen(_majorGridlinePen);
            drawMajorGridline(painter, logicalLinePosition, Qt::Vertical);
        }
        else
        {
            painter.setPen(_minorGridlinePen);
            drawMinorGridline(painter, logicalLinePosition, Qt::Vertical);
        }
    }
    
    const auto firstHorizontalLineCoord = _logicalBounds.top() - fmod(_logicalBounds.top(), logicalLineMinorIncrement);
    const auto numberOfHorizontalLines = static_cast<int>(_logicalBounds.height() / logicalLineMinorIncrement);
    for (auto i = 0; i < numberOfHorizontalLines; ++i)
    {
        const auto logicalLinePosition = firstHorizontalLineCoord + (i * logicalLineMinorIncrement);
        if (qFuzzyCompare(fmod(logicalLinePosition, logicalLineMajorIncrement), 0.0))
        {
            painter.setPen(_majorGridlinePen);
            drawMajorGridline(painter, logicalLinePosition, Qt::Horizontal);
        }
        else
        {
            painter.setPen(_minorGridlinePen);
            drawMinorGridline(painter, logicalLinePosition, Qt::Horizontal);
        }
    }

    auto pen = painter.pen();
    pen.setWidth(2);
    pen.setColor(Qt::black);
    painter.setPen(pen);

    painter.drawLine(_viewportTransform.map(QPointF(0, _logicalBounds.bottom())), _viewportTransform.map(QPointF(0, _logicalBounds.top())));
    painter.drawLine(_viewportTransform.map(QPointF(_logicalBounds.left(), 0)), _viewportTransform.map(QPointF(_logicalBounds.right(), 0)));
}

void PointsWidget::drawMinorGridline
(
    QPainter& painter, 
    qreal logicalPosition, 
    Qt::Orientation orientation
)
{
    switch (orientation)
    {
        case Qt::Horizontal:
        {
            QPointF left(_logicalBounds.left(), logicalPosition);
            QPointF right(_logicalBounds.right(), logicalPosition);
            painter.drawLine(_viewportTransform.map(left), _viewportTransform.map(right));
            break;
        }
        case Qt::Vertical:
        {
            QPointF top(logicalPosition, _logicalBounds.top());
            QPointF bottom(QPointF(logicalPosition, _logicalBounds.bottom()));
            painter.drawLine(_viewportTransform.map(top), _viewportTransform.map(bottom));
            break;
        }
    }
}

void PointsWidget::drawMajorGridline
(
    QPainter& painter, 
    qreal logicalPosition, 
    Qt::Orientation orientation
)
{
    //First we get some information about the text and what it looks like in the pixel space.
    const auto numberAsString = QString::number(logicalPosition);
    const auto textWidthInPixelSpace = QFontMetricsF(painter.font()).width(numberAsString);
    const auto textHeightInPixelSpace = orientation == Qt::Horizontal ? QFontMetricsF(painter.font()).capHeight() : QFontMetricsF(painter.font()).height();

    QPointF textCoords;
    switch (orientation)
    {
        case Qt::Orientation::Horizontal:
        {
            textCoords = _viewportTransform.map(QPointF(0.0, logicalPosition));
            textCoords.rx() -= textWidthInPixelSpace + kAxisTextMargins;
            textCoords.ry() += textHeightInPixelSpace / 2;

            const QPointF left(_logicalBounds.left(), logicalPosition);
            QPointF textLeft = textCoords;
            textLeft.rx() -= kAxisTextMargins;
            textLeft.ry() -= textHeightInPixelSpace / 2;
            painter.drawLine(_viewportTransform.map(left), textLeft);

            QPointF onAxis(0.0, logicalPosition);
            QPointF right(_logicalBounds.right(), logicalPosition);
            painter.drawLine(_viewportTransform.map(onAxis), _viewportTransform.map(right));
            break;
        }
      
        case Qt::Orientation::Vertical:
        {
            textCoords = _viewportTransform.map(QPointF(logicalPosition, 0.0));
            textCoords.rx() -= textWidthInPixelSpace / 2;
            textCoords.ry() += textHeightInPixelSpace + kAxisTextMargins;

            //I know this reads really weird, but Qt's Rectangle class's bottom is larger than the top. So the bottom is the effective top. Fun.

            const QPointF top(logicalPosition, _logicalBounds.bottom());
            const QPointF onAxis(logicalPosition, 0.0);

            QPointF textBottom = textCoords;
            textBottom.ry() += textHeightInPixelSpace;
            textBottom.rx() += textWidthInPixelSpace / 2;
            const QPointF bottom(QPointF(logicalPosition, _logicalBounds.top()));
            
            painter.drawLine(_viewportTransform.map(top), _viewportTransform.map(onAxis));
            painter.drawLine(textBottom, _viewportTransform.map(bottom));

            
            break;
        }
    }
    if (abs(logicalPosition) > 0.000000001)
        painter.drawText(textCoords, numberAsString);
}

void PointsWidget::adjustGridlineScale(bool didZoomOut)
{
    const QPointF origin(0.0, 0.0);
    const QPointF firstGridline(_currentIncrementBase  * pow(10, _currentIncrementExponent), 0.0);

    const auto mappedOrigin = _viewportTransform.map(origin);
    const auto mappedFirstGrid = _viewportTransform.map(firstGridline);

    const auto horizontalDistance = mappedFirstGrid.x() - mappedOrigin.x();
    if (didZoomOut) {
        if (horizontalDistance < kGridUpdateThreshold)
        {
            switch (_currentIncrementBase)
            {
            case 1:
                _currentIncrementBase = 2;
                break;
            case 2:
                _currentIncrementBase = 5;
                break;
            case 5:
                _currentIncrementBase = 1;
                _currentIncrementExponent++;
                break;
            default:
                throw;
            }
        }
    }
    else if (horizontalDistance > 200.0) 
    {
        switch (_currentIncrementBase)
        {
        case 1:
            _currentIncrementBase = 5;
            _currentIncrementExponent--;
            break;
        case 2:
            _currentIncrementBase = 1;
            break;
        case 5:
            _currentIncrementBase = 2;
            break;
        default:
            throw;
        }
    }
}

void PointsWidget::mousePressEvent
(
    QMouseEvent* mouseEvent
)
{
    _mouseLocation = _inverseViewportTransform.map(mouseEvent->localPos());

    update();
}

void PointsWidget::mouseMoveEvent
(
    QMouseEvent* mouseEvent
)
{
    const auto newMouseLocation = mouseEvent->localPos();
    //newMouseLocation.ry();
    if (mouseEvent->buttons().testFlag(Qt::LeftButton))
    {
        const auto mousePosTemp = _mouseLocation;
        auto difference = newMouseLocation - _viewportTransform.map(mousePosTemp);
        //if (QPointF::dotProduct(difference, difference) < 100)
        {
            _translationInPixelSpace += difference;
            computeTransform();
        }
    }

    _mouseLocation = _inverseViewportTransform.map(newMouseLocation);
    update();
}

void PointsWidget::mouseReleaseEvent
(
    QMouseEvent* mouseEvent
)
{
    _mouseLocation = _inverseViewportTransform.map(mouseEvent->localPos());
    update();
}

void PointsWidget::wheelEvent
(
    QWheelEvent* wheelEvent
)
{
    const auto oldFactor = exp(_zoomLevel);
    _zoomLevel += (qreal)wheelEvent->delta() / 4000;

    _zoomCenter = _inverseViewportTransform.map(wheelEvent->posF());
    computeTransform();


    const auto newZoomLocation = _viewportTransform.map(_zoomCenter);
    _translationInPixelSpace += wheelEvent->posF() - newZoomLocation;
    computeTransform();

    _mouseLocation = _inverseViewportTransform.map(wheelEvent->position());

    adjustGridlineScale(wheelEvent->delta() < 0);
    update();
}

void PointsWidget::resizeEvent
(
    QResizeEvent* resizeEvent
)
{
    resizeEvent->accept();
    computeTransform();
    update();
}

void PointsWidget::computeTransform()
{
    QTransform centerOriginTransform;
    centerOriginTransform.scale(1.0, -1.0);

    auto zoomScale = exp(_zoomLevel);

    QTransform logicalToViewport;
    logicalToViewport.scale(50.0, 50.0);

    QTransform zoomScaleTransform;
    zoomScaleTransform.scale(zoomScale, zoomScale);

    QTransform zoomCenterTransform;
    zoomCenterTransform.translate(_zoomCenter.x(), _zoomCenter.y());
    auto zoomInPixelSpace = _viewportTransform.map(_zoomCenter);
    
    QTransform translation;
    translation.translate(_translationInPixelSpace.x(), _translationInPixelSpace.y());

    _viewportTransform = ((zoomCenterTransform.inverted() * zoomScaleTransform * zoomCenterTransform.scale(zoomScale, zoomScale) * centerOriginTransform) * logicalToViewport) * translation;
    _inverseViewportTransform = _viewportTransform.inverted();

    _logicalBounds = visibleBounds();
}

void PointsWidget::reset()
{
    _points.clear();
}


void PointsWidget::centerOnLogicalCoordinate
(
    const QPointF& logicalCoordinate
)
{
    _zoomLevel = 0;
    _translationInPixelSpace += QPointF(width() / 2, height() / 2) - _viewportTransform.map(logicalCoordinate);
    computeTransform();
}

QRectF PointsWidget::visibleBounds() const
{
    return _inverseViewportTransform.mapRect(QRectF(0.0, 0.0, width(), height())).normalized();
}