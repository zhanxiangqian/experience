#include "rcs_display.h"
#include "materialviewer.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF8"));
	QApplication a(argc, argv);
	MaterialViewer w;
	w.show();
	return a.exec();
}
