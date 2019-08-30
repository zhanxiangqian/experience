#pragma once

#include <QtWidgets/QWidget>
#include "ui_QtGuiApplication1.h"

class QtGuiApplication1 : public QWidget
{
    Q_OBJECT

public:
    QtGuiApplication1(QWidget *parent = Q_NULLPTR);

private:
    Ui::QtGuiApplication1Class ui;
};
