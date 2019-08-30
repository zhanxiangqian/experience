#include "qtDock.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qtDock w;
    w.show();
    return a.exec();
}
