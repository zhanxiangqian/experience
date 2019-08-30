#include "materialconfigparser.h"
#include <QFileInfo>
#include <QDomDocument>
#include <QDomElement>
#include <QTextStream>

const QString MaterialConfigParser::originalDir = "originaldir";
const QString MaterialConfigParser::selectDir = "selectdir";
const QString MaterialConfigParser::dimensionPath = "dimensionpath";
const QString MaterialConfigParser::typeconfPath = "typeconfpath";

QString MaterialConfigParser::configPath = "";
std::map<QString, QString> MaterialConfigParser::configs = std::map<QString, QString>();
std::map<QString, bool> MaterialConfigParser::usedWorkMap = std::map<QString, bool>();
MaterialConfigParser::MaterialConfigParser() 
{

}


MaterialConfigParser::~MaterialConfigParser()
{
}

void MaterialConfigParser::read()
{
	QFileInfo fi(configPath+"/material-config/conifg.xml");
	if (fi.isFile())
	{
		QFile file(configPath + "/material-config/conifg.xml");
		QDomDocument doc("xmlDoc");
		if (!file.open(QIODevice::ReadOnly))
			return ;// open file failed
		if (!doc.setContent(&file)) {
			file.close();
			return ; //QDomDocument set file failed
		}
		file.close();
		QDomElement& xml = doc.documentElement();
		
		
		QDomElement element =xml.firstChildElement();
		while (!element.isNull() )
		{
			bool useWorkdir = true;
			if (element.hasAttribute("useworkdir"))
			{
				QString used = element.attribute("useworkdir");
				if (used == "true")
				{
					useWorkdir = true;
				}
				else
				{
					useWorkdir = false;
				}
			}

			if (element.hasAttribute("name"))
			{
				QString name =element.attribute("name");
				QString path;
				if (name==originalDir)
				{
					path = element.text();
					QFileInfo info(path);
					if (!(info.isDir() && info.exists()))
					{
						path = configPath + "/material-data/MaterialLibrary"; 
					}
				}
				else
				{ 
					if (useWorkdir)
					{
						path = configPath + element.text();
					}
					else
					{
						path = element.text();
					}
				} 
				usedWorkMap.insert(std::make_pair(name, useWorkdir) );
				configs.insert(std::make_pair(name,path ));
			}
			element = element.nextSiblingElement();
		}
	} 
}

void MaterialConfigParser::save()
{
	QDomDocument doc("xmlDoc");
	QDomProcessingInstruction instruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF - 8\"");
	doc.appendChild(instruction);
	QDomElement  rootElement = doc.createElement("config");
	 
	rootElement.setAttribute("name", "default"); 
	
	std::map<QString, QString>::iterator iter =configs.begin();
	while (iter!=configs.end() )
	{
		QDomElement propertyEleemnt = doc.createElement("property");
		propertyEleemnt.setAttribute("name", iter->first);
		std::map<QString, bool>::iterator usedIter =usedWorkMap.find(iter->first);
		bool useWorkdir = false;
		if (usedIter!=usedWorkMap.end() )
		{
			useWorkdir = usedIter->second;
		}

		if (useWorkdir)
		{
			propertyEleemnt.setAttribute("useworkdir", "true");
		}
		else
		{
			propertyEleemnt.setAttribute("useworkdir", "false");
		}

		QString path = iter->second;
		if (iter->first!=originalDir )
		{
			path.remove(configPath);
		}
		QDomText txtNode= doc.createTextNode(path);
		propertyEleemnt.appendChild(txtNode);
		rootElement.appendChild(propertyEleemnt);
		iter++;
	} 
	doc.appendChild(rootElement); 
	QFile fileWrite(configPath+"/material-config/conifg.xml");
	if (!fileWrite.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)){
		return;
	}
	QTextStream out(&fileWrite);
	out.setCodec("UTF-8");
	doc.save(out, 4, QDomNode::EncodingFromTextStream);
	fileWrite.close();
}
