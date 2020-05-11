#include <QApplication>

#include "RegressionWindow.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    RegressionWindow w;
    w.show();
    return app.exec();
}
