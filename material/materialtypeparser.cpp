#include <QDomDocument>
#include <QFile>
#include <QFileInfo>
#include <QDomElement>
#include "materialtypeParser.h"


MaterialTypeParser::MaterialTypeParser()
{
}


MaterialTypeParser::~MaterialTypeParser()
{
}

void MaterialTypeParser::read(const QString& path)
{
	QFileInfo fi(path);
	if (fi.isFile() )
	{
		QFile file(path);
		QDomDocument doc("xmlDoc");
		if (!file.open(QIODevice::ReadOnly))
			return;// open file failed
		if (!doc.setContent(&file)) {
			file.close();
			return; //QDomDocument set file failed
		}

		QDomElement rootElement = doc.firstChildElement();
		QDomElement typeElement = rootElement.firstChildElement();
		while (!typeElement.isNull() )
		{
			QString typeName = typeElement.attribute("name","");
			if (!typeName.isEmpty())
			{
				QStringList matList;
				QDomElement entityElement = typeElement.firstChildElement();
				while (!entityElement.isNull() )
				{

					QString matName = entityElement.attribute("name", "");
					if (!matName.isEmpty())
					{
						matList.append(matName);
					} 
					entityElement = entityElement.nextSiblingElement();
				}
				matTypeMap.insert(std::make_pair(typeName, matList));
			} 
			typeElement = typeElement.nextSiblingElement();
		}
	}
}

const std::map<QString, QStringList> & MaterialTypeParser::getTypeMap()
{
	return matTypeMap;
}