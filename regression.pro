QT += core gui widgets

CONFIG += c++17

SOURCES += \
    FunctionModel.cpp \
    PointSetModel.cpp \
    RegressionWindow.cpp \
    GraphWidget.cpp \
    main.cpp

HEADERS += \
    FunctionModel.h \
    IPointSetModel.h \
    PointSetModel.h \
    Range.h \
    Regression.h \
    RegressionWindow.h \
    GraphWidget.h \
    VectorND.h

INCLUDEPATH += C:\boost

FORMS += \
    RegressionWindow.ui
