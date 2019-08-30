#include <cmath>
#include "materiallineseries.h"
#include <QDebug>


MaterialLineSeries::MaterialLineSeries(const QString& name, QObject *parent)
:QLineSeries(parent)
{
	this->setName(name);
	setPointsVisible(true);
	setPointLabelsClipping(true);
	setPointLabelsColor(Qt::black);
}


MaterialLineSeries::~MaterialLineSeries()
{ 
}

void MaterialLineSeries::append(const QPointF &pt)
{ 
	QLineSeries::append(pt);
}

void MaterialLineSeries::append(const std::vector<QPointF> &points)
{
	QList<QPointF> ptList;
	for (int i = 0; i < points.size(); i++ )
	{
		ptList.append(points[i]);
	}
	QLineSeries::append(ptList);
}

void MaterialLineSeries::append(const std::vector<double> &values)
{
	QList<QPointF> ptList;
	for (int i = 0; i < values.size(); i++)
	{
		QPointF pt(1.0f*i, values[i]);
		ptList.append(pt );
	}
	QLineSeries::append(ptList);
}

void MaterialLineSeries::append(const std::vector<std::vector<double>> &values)
{
	QList<QPointF> ptList;
	for (int i = 0; i < values.size(); i++)
	{
		QPointF pt(log10(values[i][0]), log10(values[i][1]) );
		ptList.append(pt);
	}

	for (int i = 0; i < ptList.length(); i++)
	{
		int j = i;
		QPointF tempPt = ptList[i];
		for (; j>0; j--)
		{
			if (ptList[j - 1].rx() > tempPt.rx())
			{
				ptList[j] = ptList[j - 1];
			}
			else
			{
				break;
			}
		}
		ptList[j] = tempPt;
	}
	for (int i = 0; i < ptList.length(); i++ )
	{
		qDebug() << ptList[i].x();
	}
	
	QLineSeries::append(ptList);
}

void MaterialLineSeries::appendMarkerInfo(const LineMarkerInfo& info )
{
	lineMarkerInfoVec.push_back(info);
}

void MaterialLineSeries::setMarkerInfo(const std::vector<LineMarkerInfo>& infos )
{
	lineMarkerInfoVec = infos;
}

void MaterialLineSeries::replace(const std::vector<double> &values)
{
	QList<QPointF> ptList;
	for (int i = 0; i < values.size(); i++)
	{
		QPointF pt(1.0f*i, values[i]);
		ptList.append(pt);
	}
	QLineSeries::replace(ptList);
} 
