/********************************************************************************
** Form generated from reading UI file 'rcs_display.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RCS_DISPLAY_H
#define UI_RCS_DISPLAY_H

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

class Ui_RCS_DisplayClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *RCS_DisplayClass)
    {
        if (RCS_DisplayClass->objectName().isEmpty())
            RCS_DisplayClass->setObjectName(QStringLiteral("RCS_DisplayClass"));
        RCS_DisplayClass->resize(600, 400);
        menuBar = new QMenuBar(RCS_DisplayClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        RCS_DisplayClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(RCS_DisplayClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        RCS_DisplayClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(RCS_DisplayClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        RCS_DisplayClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(RCS_DisplayClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        RCS_DisplayClass->setStatusBar(statusBar);

        retranslateUi(RCS_DisplayClass);

        QMetaObject::connectSlotsByName(RCS_DisplayClass);
    } // setupUi

    void retranslateUi(QMainWindow *RCS_DisplayClass)
    {
        RCS_DisplayClass->setWindowTitle(QApplication::translate("RCS_DisplayClass", "RCS_Display", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class RCS_DisplayClass: public Ui_RCS_DisplayClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RCS_DISPLAY_H
