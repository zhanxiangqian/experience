#include <QtWidgets/QApplication>  
#include "materialviewer.h"
#include "materialconfigparser.h"
#include "materialdimensionconvert.h"
#include "materialtypeparser.h" 
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QString appDirPath =a.applicationDirPath(); 

	MaterialConfigParser::configPath = appDirPath;
	MaterialConfigParser::read();
	

	IMaterialDimensionConvert * dimensionConvert = new MaterialDimensionConvert;
	dimensionConvert->setDimensionDocPath(MaterialConfigParser::configs[MaterialConfigParser::dimensionPath]);
	dimensionConvert->readDimension();
	MaterialViewer w(dimensionConvert); 
	w.show();
	return a.exec();
}
