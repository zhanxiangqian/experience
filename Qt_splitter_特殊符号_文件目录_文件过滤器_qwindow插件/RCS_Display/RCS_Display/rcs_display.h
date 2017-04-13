#ifndef RCS_DISPLAY_H
#define RCS_DISPLAY_H

#include <QtWidgets/QMainWindow>
#include "ui_rcs_display.h"

class RCS_Display : public QMainWindow
{
	Q_OBJECT

public:
	RCS_Display(QWidget *parent = 0);
	~RCS_Display();

private:
	Ui::RCS_DisplayClass ui;
};

#endif // RCS_DISPLAY_H
