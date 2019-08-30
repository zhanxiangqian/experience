#include "materialdimensionconvert.h"
#include <QFileInfo>
#include <QDomDocument>
#include <QFile>

MaterialDimensionConvert::MaterialDimensionConvert()
{
}


MaterialDimensionConvert::~MaterialDimensionConvert()
{
}

double MaterialDimensionConvert::calculate(const QString& name, const QString& des, const QString& src)
{
	UnitMap::iterator iter = unitMap.find(name);
	double value = 0;
	if (iter!=unitMap.end())
	{
		const std::vector<Property>& propVec = iter->second;
		int desPos =0, srcPos =0;
		for (int i = 0; i<propVec.size(); i++ )
		{
			if (des==propVec[i].name)
			{
				desPos = i;
			}
			else if (src == propVec[i].name )
			{
				srcPos = i;
			}
		}
		value = 1.0f;
		if (desPos>srcPos)
		{
			for (int i = srcPos; i < desPos; i++)
			{
				value *= propVec[i].convert;
			}
		}
		else
		{
			for (int i = desPos; i <srcPos; i++)
			{
				value /= propVec[i].convert;
			}
		}
	}
	return value;
}

void MaterialDimensionConvert::setDimensionDocPath(const QString& path)
{
	dimensionDocPath = path;
}

void MaterialDimensionConvert::readDimension()
{
	QFileInfo fi(dimensionDocPath );
	if (fi.isFile())
	{
		QDomDocument doc("xmlDoc");
		QFile file(dimensionDocPath );
		if (!file.open(QIODevice::ReadOnly))
			return ;// open file failed
		if (!doc.setContent(&file)) {
			file.close();
			return ; //QDomDocument set file failed
		}
		file.close();
		QDomElement& xml = doc.documentElement(); 
		readUnit(xml, unitMap);
	}
}

QStringList MaterialDimensionConvert::getUnitNameList(const QString & name )
{
	QStringList nameList;
	UnitMap::iterator iter = unitMap.find(name);
	if (iter!=unitMap.end() )
	{
		const std::vector<Property>& propVec = iter->second;
		for (int i = 0; i < propVec.size(); i++ )
		{
			nameList.append(propVec[i].name);
		}
	}
	return nameList;
}

void MaterialDimensionConvert::readUnit(QDomElement& element , UnitMap& uMap)
{
	QDomElement uElement = element.firstChildElement();
	while (!uElement.isNull())
	{ 
		if (uElement.hasAttribute("name") )
		{
			PropertyVector vectorNode;
			readProperty(uElement, vectorNode);
			sortByRank(vectorNode);
			uMap.insert(std::make_pair(uElement.attribute("name"),vectorNode));
		} 
		uElement = uElement.nextSiblingElement();
	}
}

void MaterialDimensionConvert::readProperty(QDomElement& element, PropertyVector& vec)
{
	QDomElement propElement = element.firstChildElement();
	while (!propElement.isNull())
	{
		Property prop;
		if (propElement.hasAttribute("name"))
		{
			prop.name = propElement.attribute("name");
		}
		
		if (propElement.hasAttribute("rank"))
		{
			prop.rank = propElement.attribute("rank").toUInt();
		}
		
		if (propElement.hasAttribute("convert"))
		{
			prop.convert = propElement.attribute("convert").toDouble();
		}
		vec.push_back(prop);
		propElement = propElement.nextSiblingElement();
	}
}

void MaterialDimensionConvert::sortByRank(std::vector<Property>& vec )
{
	for (int i = 1; i < vec.size(); i++)
	{ 
		Property prop = vec[i];
		int j = i; 
		for (; j>0 && (prop.rank < vec[j - 1].rank); j--)
		{
			vec[j] = vec[j - 1];
		}
		vec[j] = prop;
	}
}
