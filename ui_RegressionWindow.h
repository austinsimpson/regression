/********************************************************************************
** Form generated from reading UI file 'RegressionWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGRESSIONWINDOW_H
#define UI_REGRESSIONWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "PointsWidget.h"

QT_BEGIN_NAMESPACE

class Ui_RegressionWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QWidget *_regressionWidget;
    QVBoxLayout *verticalLayout_2;
    PointsWidget *_pointsWidgets;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *RegressionWindow)
    {
        if (RegressionWindow->objectName().isEmpty())
            RegressionWindow->setObjectName(QString::fromUtf8("RegressionWindow"));
        RegressionWindow->resize(800, 600);
        centralwidget = new QWidget(RegressionWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        _regressionWidget = new QWidget(centralwidget);
        _regressionWidget->setObjectName(QString::fromUtf8("_regressionWidget"));
        verticalLayout_2 = new QVBoxLayout(_regressionWidget);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        _pointsWidgets = new PointsWidget(_regressionWidget);
        _pointsWidgets->setObjectName(QString::fromUtf8("_pointsWidgets"));

        verticalLayout_2->addWidget(_pointsWidgets);


        verticalLayout->addWidget(_regressionWidget);

        RegressionWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(RegressionWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 21));
        RegressionWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(RegressionWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        RegressionWindow->setStatusBar(statusbar);

        retranslateUi(RegressionWindow);

        QMetaObject::connectSlotsByName(RegressionWindow);
    } // setupUi

    void retranslateUi(QMainWindow *RegressionWindow)
    {
        RegressionWindow->setWindowTitle(QCoreApplication::translate("RegressionWindow", "MainWindow", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RegressionWindow: public Ui_RegressionWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGRESSIONWINDOW_H
