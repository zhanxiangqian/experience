#include "RestoreGeometry.h"
#include <QtWidgets/QApplication>
#include <QSettings>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	//QSettings »áÐ´Èë×¢²á±í
	RestoreGeometry w;
	QCoreApplication::setOrganizationName("Zhan");
	QCoreApplication::setApplicationName("RestoreGeometry");

	QSettings settings;
	w.restoreGeometry(settings.value("RestoreGeometryWidget/geometry").toByteArray());
 	w.restoreState(settings.value("RestoreGeometryWidget/windowState").toByteArray());
	w.show();
    return a.exec();
}
