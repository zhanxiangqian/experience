/********************************************************************************
** Form generated from reading UI file 'qtDock.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTDOCK_H
#define UI_QTDOCK_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_qtDockClass
{
public:
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *qtDockClass)
    {
        if (qtDockClass->objectName().isEmpty())
            qtDockClass->setObjectName(QStringLiteral("qtDockClass"));
        qtDockClass->resize(600, 400);
        qtDockClass->setDockOptions(QMainWindow::AnimatedDocks);
        centralWidget = new QWidget(qtDockClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        qtDockClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(qtDockClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 23));
        qtDockClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(qtDockClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        qtDockClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(qtDockClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        qtDockClass->setStatusBar(statusBar);

        retranslateUi(qtDockClass);

        QMetaObject::connectSlotsByName(qtDockClass);
    } // setupUi

    void retranslateUi(QMainWindow *qtDockClass)
    {
        qtDockClass->setWindowTitle(QApplication::translate("qtDockClass", "qtDock", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class qtDockClass: public Ui_qtDockClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTDOCK_H
