#include "qtDock.h"
#include <QPushButton>
#include <QLabel>
#include <QTableWidget>
qtDock::qtDock(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	dock1 = new QDockWidget;
	dock1->setWidget(new QTableWidget(2,2));
	dock2 = new QDockWidget;
	dock2->setWidget(new QTableWidget(2, 2));
	dock3 = new QDockWidget;
	dock3->setWidget(new QTableWidget(2, 2));
	dock4 = new QDockWidget;
	dock4->setWidget(new QTableWidget(2, 2));
	addDockWidget(Qt::TopDockWidgetArea, dock1);
	addDockWidget(Qt::BottomDockWidgetArea, dock2);
	addDockWidget(Qt::LeftDockWidgetArea,dock3);
	addDockWidget(Qt::RightDockWidgetArea, dock4);

	setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
	setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);


}
