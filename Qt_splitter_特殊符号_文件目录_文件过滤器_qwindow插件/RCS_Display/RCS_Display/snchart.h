#ifndef SNCHART_H_
#define SNCHART_H_

#include <QGraphicsView>
#include <QtCharts>
using namespace QtCharts;

class  SNChart : public QGraphicsView
{

public:
	SNChart(QWidget *parent = 0);
	~SNChart();

	void resizeEvent(QResizeEvent *event);


	void updateChart();
private:
	QChart *m_chart;

};

#endif