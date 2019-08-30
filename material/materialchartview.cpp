#include <QLegend>
#include <QLegendMarker>
#include <QGraphicsItem>
#include <QValueAxis>
#include <QGraphicsLineItem>
#include "materialchartview.h"
#include "materialchart.h"
MaterialChartView::MaterialChartView(QWidget *parent)
:QChartView(parent),
axisX(nullptr),
axisY(nullptr)
{
	chart = new MaterialChart;
	this->setChart(chart); 
	this->setRenderHint(QPainter::Antialiasing);
	chart->legend()->setAlignment(Qt::AlignTop);
	paintAxisMask = 0;
}

MaterialChartView::~MaterialChartView()
{

}

void MaterialChartView::addSeries(MaterialLineSeries *series)
{
	if (series!=nullptr )
	{ 
		std::vector<LineMarkerInfo> infos = series->getLineMarkerInfos();
		seriesMap.insert(std::make_pair(series, infos)); 
		chart->addSeries(series); 
		this->curSeries = series;
		createAxis( );
		updateText();
	} 
}

void MaterialChartView::removeAllSeries()
{
	chart->removeAllSeries();
	seriesMap.clear();
	updateText();
}

QList<QAbstractSeries *> MaterialChartView::series() const
{
	return chart->series();
}


void MaterialChartView::removeSeries(QAbstractSeries *series)
{ 
	std::map<QAbstractSeries*, std::vector<LineMarkerInfo>>::iterator iter =seriesMap.find(series);
	if (iter!=seriesMap.end() )
	{ 
		seriesMap.erase(iter);
	} 
	chart->removeSeries(series);
} 

void MaterialChartView::setAxisXFlag(const bool on )
{
	if (on)
	{
		paintAxisMask |= 0x01;
	}
	else
	{
		paintAxisMask &= 0xfe;
	}
}

void MaterialChartView::setAxisYFlag(const bool on)
{
	if (on)
	{
		paintAxisMask |= 0x02;
	}
	else
	{
		paintAxisMask &= 0xfd;
	}
	
}

void MaterialChartView::resizeEvent(QResizeEvent *event)
{
	QChartView::resizeEvent(event);
	updateText(); 
}

void MaterialChartView::updateText()
{
	for (int i = 0; i < lineMarkerItems.size(); i++)
	{
		this->scene()->removeItem(lineMarkerItems[i]);
		delete lineMarkerItems[i];
	}
	lineMarkerItems.clear();

	std::map<QAbstractSeries*, std::vector<LineMarkerInfo>>::iterator iter = seriesMap.begin();
	while (iter != seriesMap.end())
	{
		std::vector<LineMarkerInfo>::iterator lineMarkerInfoIter = iter->second.begin();
		while (lineMarkerInfoIter != iter->second.end())
		{ 
			QPointF pt1 = chart->mapToPosition(lineMarkerInfoIter->pts.at(0));
			QPointF pt2 = chart->mapToPosition(lineMarkerInfoIter->pts.at(1));
			QPointF pt3 = chart->mapToPosition(lineMarkerInfoIter->pts.at(2));
			QPointF pt = QPointF((pt1.x() + pt3.x()) / 2, pt2.y());

			QGraphicsLineItem * line = new QGraphicsLineItem(chart);
			QPen pen = line->pen();
			pen.setColor(Qt::black);
			line->setPen(pen); 
			line->setLine(pt1.x(),pt1.y(),pt2.x(),pt2.y());
			line->setZValue(20);
			lineMarkerItems.push_back(line);

			line = new QGraphicsLineItem(chart);
			pen = line->pen();
			pen.setColor(Qt::black);
			line->setPen(pen);
			line->setLine(pt2.x(), pt2.y(), pt3.x(), pt3.y());
			line->setZValue(20);
			lineMarkerItems.push_back(line);
			 
			QGraphicsSimpleTextItem *text = new QGraphicsSimpleTextItem(chart);
			pen =text->pen();
			pen.setColor(Qt::black);
			text->setPen(pen);
			text->setText(lineMarkerInfoIter->text); 
			text->setPos(QPointF(pt.x()-10, pt.y()));
			text->setZValue(20); 
			lineMarkerItems.push_back(text);
			lineMarkerInfoIter++;
		}
		iter++;
	}
}

void MaterialChartView::createAxis( )
{
	QList<QPointF> pts = curSeries->points();
	if (pts.length() > 0)
	{
		axisXmin = floor(pts.at(0).x());
		axisXmax = ceil(pts.at(pts.length() - 1).x());

		axisYmax = axisYmin = pts.at(0).y();
		for (int i = 0; i < pts.length(); i++)
		{
			if (axisYmin>pts.at(i).y())
			{
				axisYmin = pts.at(i).y();
			}

			if (axisYmax < pts.at(i).y())
			{
				axisYmax = pts.at(i).y();
			}
		}
		axisYmin = floor(axisYmin);
		axisYmax = ceil(axisYmax);

		if (axisX)
		{
			chart->removeAxis(axisX);
		} 
		axisX = new QValueAxis;
		axisX->setRange(axisXmin, axisXmax);
		axisX->setTitleText(xLabel);
		axisX->setTickCount(axisXmax-axisXmin+1);
		axisX->setLabelFormat("1e+%i");
		axisX->setMinorTickCount(10);
		chart->addAxis(axisX, Qt::AlignBottom); 
		curSeries->attachAxis(axisX);


		if (axisY)
		{
			chart->removeAxis(axisY);
		}
		axisY = new QValueAxis;
		axisY->setRange(axisYmin, axisYmax);
		axisY->setTitleText(yLabel);
		axisY->setTickCount(axisYmax- axisYmin+1);
		axisY->setLabelFormat("1e+%i");
		chart->addAxis(axisY, Qt::AlignLeft);
		curSeries->attachAxis(axisY);
	}
}
