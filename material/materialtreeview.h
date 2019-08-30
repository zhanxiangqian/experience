#ifndef MATERIALTREEVIEW_H_
#define MATERIALTREEVIEW_H_
#include <QTreeView>
#include "materialtreeviewcallback.h"
class  MaterialTreeView : public QTreeView,public TreeViewCallBack
{
	Q_OBJECT
public:
	MaterialTreeView(QWidget *parent = 0);

	virtual void mousePressEvent(QMouseEvent *event);

	virtual void mouseDoubleClickEvent(QMouseEvent *event);

	virtual void mouseReleaseEvent(QMouseEvent *event);

	virtual void needUpdate();

signals:
	void rightButtonClicked(QMouseEvent *event);
	void leftButtonClicked(QMouseEvent *event);

	void leftButtonDoubleClicked(QMouseEvent *event); 
	void leftButtonReleased();
};

#endif