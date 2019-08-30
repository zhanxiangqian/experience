#pragma once
#include <QLineSeries>
#include <QString>
#include <QPointF>
using namespace QtCharts;
struct LineMarkerInfo
{
	LineMarkerInfo(QPointF start, QPointF end, QString text )
	{
		pts.push_back(start);
		if (start.y()>end.y() )
		{
			pts.push_back(QPointF(start.x(),end.y()));
		}
		else
		{
			pts.push_back(QPointF(end.x(), start.y()));
		}
		pts.push_back(end); 
		this->text = text;
	}
	QString text;
	std::vector<QPointF> pts;
};
class MaterialLineSeries:public QLineSeries
{
public:
	MaterialLineSeries(const QString& name, QObject *parent = nullptr);
	~MaterialLineSeries();
public:
	void append(const QPointF&);
	void append(const std::vector<QPointF> &points);
	void append(const std::vector<double> &values);
	void append(const std::vector<std::vector<double>> &values);
	void appendMarkerInfo(const LineMarkerInfo&);
	void setMarkerInfo(const std::vector<LineMarkerInfo>&);
	void replace(const std::vector<double> &values);


	inline std::vector<LineMarkerInfo>& getLineMarkerInfos()
	{
		return lineMarkerInfoVec;
	}
private:
	std::vector<LineMarkerInfo> lineMarkerInfoVec; 
};

