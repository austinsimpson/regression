QT += core gui widgets

CONFIG += c++17

SOURCES += \
    RegressionWindow.cpp \
    GraphWidget.cpp \
    main.cpp

HEADERS += \
    Regression.h \
    RegressionWindow.h \
    GraphWidget.h \
    VectorND.h

INCLUDEPATH += C:\boost

FORMS += \
    RegressionWindow.ui
