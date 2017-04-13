#pragma once

#include "C:\Qt\Qt5.8.0\5.8\msvc2013_64\include\QtWidgets\qwidget.h"
#include "materialviewer.h"
#include "snchart.h"
#include <QTableView>
class mainwidget :
	public QWidget
{
public:
	mainwidget();
	~mainwidget();

private:
	void init();
	QTableView*     m_ptreeview;
	MaterialViewer* m_pdisplay;
	SNChart *       m_pchart;
};

