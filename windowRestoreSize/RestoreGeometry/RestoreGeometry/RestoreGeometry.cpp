#include "RestoreGeometry.h"
#include <QSettings>
#include <QTextBrowser>
RestoreGeometry::RestoreGeometry(QWidget *parent)
	: QMainWindow(parent)
{
	setObjectName("RestoreGeometryWidget");
	ui.setupUi(this);
	m_dock[0] = new QDockWidget();
 	m_dock[0]->setObjectName("Dock1");//注:没有名字时状态无法恢复
	m_dock[1] = new QDockWidget();
 	m_dock[1]->setObjectName("Dock2");
 	setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
 	setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);

	m_dock[0]->setWidget(new QTextBrowser());
	m_dock[1]->setWidget(new QTextBrowser());
	addDockWidget(Qt::LeftDockWidgetArea, m_dock[0]);
	addDockWidget(Qt::BottomDockWidgetArea, m_dock[1]);
}

void RestoreGeometry::closeEvent(QCloseEvent *event)
{
	QSettings settings;
	settings.setValue("RestoreGeometryWidget/geometry", saveGeometry());
	settings.setValue("RestoreGeometryWidget/windowState", saveState());
	QWidget::closeEvent(event);
}

