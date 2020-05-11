#include "RegressionWindow.h"

RegressionWindow::RegressionWindow
(
    QWidget* parent
):
    QMainWindow(parent),
    Ui::RegressionWindow()
{
    setupUi(this);
}

RegressionWindow::~RegressionWindow()
{
}

void RegressionWindow::showEvent
(
    QShowEvent* showEvent
)
{
    _pointsWidgets->centerOnLogicalCoordinate(QPointF(0.0, 0.0));
}

