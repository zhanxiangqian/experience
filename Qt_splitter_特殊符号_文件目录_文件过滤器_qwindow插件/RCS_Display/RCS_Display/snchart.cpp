
#include <QLineSeries>
#include <QScatterSeries>
#include <QLogValueAxis>
#include <QValueAxis>

#include "snchart.h"
#include <QDebug>

SNChart::SNChart(QWidget *parent)
	:QGraphicsView(new QGraphicsScene, parent), m_chart(0)
{
	m_chart = new QChart();
	m_chart->setMinimumSize(400, 200);

	m_chart->legend()->hide();
	m_chart->setTitle("SN Curve");
	
	QLineSeries *s1 = new QLineSeries();
// 	s1->append(1000., 102.);
// 	s1->append(1008., 106.);

	s1->append(102., 1); 
	s1->append(106.,1008. );
	m_chart->addSeries(s1);

	QValueAxis *axisX = new QValueAxis;
	axisX->setLabelFormat("%f");
	axisX->setTitleText("Fatigue life,N[cycles]");

	QLogValueAxis *axisY = new QLogValueAxis;
	axisY->setLabelFormat("%f");
	axisY->setTitleText("Stress range");
	axisY->setBase(10);

	m_chart->addAxis(axisX, Qt::AlignBottom);//, Qt::AlignBottom
	m_chart->addAxis(axisY, Qt::AlignLeft);//, Qt::AlignLeft

	s1->attachAxis(axisX);
	s1->attachAxis(axisY);

// 	QLineSeries *series = new QLineSeries();
// 	*series << QPointF(1, 1) << QPointF(2, 73) << QPointF(3, 268) << QPointF(4, 17) << QPointF(5, 4325) << QPointF(6, 723);
// 	m_chart->addSeries(series);
// 	QValueAxis *axisX = new QValueAxis;
// 	axisX->setTitleText("Data point");
// 	axisX->setTickCount(6);
// 	axisX->setLabelFormat("%i");
// 	m_chart->addAxis(axisX, Qt::AlignBottom);
// 	series->attachAxis(axisX);
// 
// 	QLogValueAxis *axisY = new QLogValueAxis;
// 	axisY->setLabelFormat("%g");
// 	axisY->setTitleText("Values");
// 	axisY->setBase(10);
// 	m_chart->addAxis(axisY, Qt::AlignLeft);
// 	series->attachAxis(axisY);


	updateChart();
	setRenderHint(QPainter::Antialiasing);
	scene()->addItem(m_chart);
}

SNChart::~SNChart()
{

}

void SNChart::resizeEvent(QResizeEvent *event)
{
		m_chart->resize(this->rect().width() - 10, this->rect().height() - 10);

 }

void SNChart::updateChart()
{

// 
// 	QLineSeries *s2 = new QLineSeries();
// 	s2->append(10000000, 58.48);
// 	s2->append(10, 12);

// 	QScatterSeries *s3 = new QScatterSeries();
// 	s3->append(100, 58.48);
 

	//m_chart->addSeries(s1);
	//m_chart->addSeries(s3);







// 	QGraphicsSimpleTextItem *text = new QGraphicsSimpleTextItem();
// 	text->setText("m=3.0");
// 
// 	scene()->addItem(text);

}