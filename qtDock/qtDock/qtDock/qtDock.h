#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_qtDock.h"
#include <QDockWidget>
class qtDock : public QMainWindow
{
    Q_OBJECT

public:
    qtDock(QWidget *parent = Q_NULLPTR);

private:
    Ui::qtDockClass ui;
	QDockWidget* dock1;
	QDockWidget* dock2;
	QDockWidget* dock3;
	QDockWidget* dock4;
};
