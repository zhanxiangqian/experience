#ifndef MATERIAL_SNCHART_H_
#define MATERIAL_SNCHART_H_

#include <QChartView>
#include <QChart>
#include <QPointF> 
#include <map>
#include "materiallineseries.h"
 
class MaterialChart;
class MaterialLineSeries;
namespace QtCharts
{
	class QValueAxis;
} 
class MaterialChartView :public QChartView
{
public:
	MaterialChartView(QWidget *parent =nullptr );
	~MaterialChartView(); 
public:
	void addSeries(MaterialLineSeries *series);
	void removeAllSeries();

	QList<QAbstractSeries *> series() const;
	void removeSeries(QAbstractSeries *series); 

	void setAxisXFlag(const bool on = false);
	void setAxisYFlag(const bool on = false);

	inline void setXLabel(const QString& label)
	{
		xLabel = label;
	}

	inline void setYLabel(const QString& label)
	{
		yLabel = label;
	}

protected:
	void resizeEvent(QResizeEvent *event);
	 
private:
	void updateText();
	void createAxis(); 

private:
	MaterialChart *chart;
	std::map<QAbstractSeries*, std::vector<LineMarkerInfo> > seriesMap;
	std::vector<QGraphicsItem *> lineMarkerItems;
	double axisXmin;
	double axisXmax;
	double axisYmin;
	double axisYmax; 
	unsigned char paintAxisMask;// first bit is axisX, second bit is axisY
	MaterialLineSeries *curSeries;

	QValueAxis *axisX;
	QValueAxis *axisY;

	QString xLabel;
	QString yLabel;
};
#endif