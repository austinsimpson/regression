QT += core gui widgets

CONFIG += c++17

SOURCES += \
    RegressionWindow.cpp \
    PointsWidget.cpp \
    main.cpp

HEADERS += \
    Regression.h \
    RegressionWindow.h \
    PointsWidget.h \
    VectorND.h

INCLUDEPATH += C:\boost

FORMS += \
    RegressionWindow.ui
