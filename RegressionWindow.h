#ifndef REGRESSIONWINDOW_H
#define REGRESSIONWINDOW_H

#include <QMainWindow>
#include "ui_RegressionWindow.h"

class RegressionWindow : public QMainWindow, Ui::RegressionWindow
{
    Q_OBJECT

public:
    explicit RegressionWindow(QWidget* parent = nullptr);
    ~RegressionWindow();


    void showEvent(QShowEvent* showEvent) override;
private:

};

#endif // REGRESSIONWINDOW_H
