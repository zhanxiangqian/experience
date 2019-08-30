#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_RestoreGeometry.h"
#include <QDockWidget>
class RestoreGeometry : public QMainWindow
{
    Q_OBJECT

public:
    RestoreGeometry(QWidget *parent = Q_NULLPTR);
protected:
	void closeEvent(QCloseEvent *event);
private:
    Ui::RestoreGeometryClass ui;
	QDockWidget* m_dock[2];
};
