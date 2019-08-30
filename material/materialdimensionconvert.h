#pragma once 
 
#include "imaterialdimensionconvert.h"


class QDomElement; 
class MaterialDimensionConvert :public IMaterialDimensionConvert
{
public:
	MaterialDimensionConvert();
	~MaterialDimensionConvert();

public:
	virtual double calculate(const QString& name, const QString& des, const QString& src);
	virtual void setDimensionDocPath(const QString& path); 
	virtual void readDimension();
	virtual QStringList getUnitNameList(const QString & name );

private:
	void readUnit(QDomElement&, UnitMap&);
	void readProperty(QDomElement&, PropertyVector& );
	void sortByRank(std::vector<Property>&);

private:
	QString dimensionDocPath;
	UnitMap unitMap;
};

