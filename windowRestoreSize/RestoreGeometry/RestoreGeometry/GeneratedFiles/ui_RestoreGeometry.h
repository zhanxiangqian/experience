/********************************************************************************
** Form generated from reading UI file 'RestoreGeometry.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RESTOREGEOMETRY_H
#define UI_RESTOREGEOMETRY_H

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

class Ui_RestoreGeometryClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *RestoreGeometryClass)
    {
        if (RestoreGeometryClass->objectName().isEmpty())
            RestoreGeometryClass->setObjectName(QStringLiteral("RestoreGeometryClass"));
        RestoreGeometryClass->resize(600, 400);
        menuBar = new QMenuBar(RestoreGeometryClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        RestoreGeometryClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(RestoreGeometryClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        RestoreGeometryClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(RestoreGeometryClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        RestoreGeometryClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(RestoreGeometryClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        RestoreGeometryClass->setStatusBar(statusBar);

        retranslateUi(RestoreGeometryClass);

        QMetaObject::connectSlotsByName(RestoreGeometryClass);
    } // setupUi

    void retranslateUi(QMainWindow *RestoreGeometryClass)
    {
        RestoreGeometryClass->setWindowTitle(QApplication::translate("RestoreGeometryClass", "RestoreGeometry", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class RestoreGeometryClass: public Ui_RestoreGeometryClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RESTOREGEOMETRY_H
