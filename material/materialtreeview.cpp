
#include <QMouseEvent>
#include "materialtreeview.h"


MaterialTreeView::MaterialTreeView(QWidget *parent)
:QTreeView(parent)
{


}

void MaterialTreeView::mousePressEvent(QMouseEvent *event)
{
	QTreeView::mousePressEvent(event);
	if (event->button() == Qt::LeftButton)
	{
		emit leftButtonClicked(event);
	}
	else if (event->button() == Qt::RightButton)
	{
		emit rightButtonClicked(event);
	} 
}

void MaterialTreeView::mouseDoubleClickEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		emit leftButtonDoubleClicked(event);
	}   
	QTreeView::mouseDoubleClickEvent(event);
}

void MaterialTreeView::mouseReleaseEvent(QMouseEvent *event)
{
	QTreeView::mouseReleaseEvent(event);
	if (event->button() == Qt::LeftButton)
	{
		emit leftButtonReleased();
	} 
}

void MaterialTreeView::needUpdate()
{
	this->updateGeometries();
}