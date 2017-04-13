#ifndef MATERIALVIEWER_H_
#define MATERIALVIEWER_H_

#include <QDialog>
#include <QFileSystemModel>
#include <QVBoxLayout>

#include <QFileSystemModel>
#include <QDir>
#include "filematerialmodel.h"
#include "snchart.h"

class  MaterialViewer : public QDialog
{
	Q_OBJECT
public:
	MaterialViewer(QWidget *parent = 0);
	~MaterialViewer();

public slots:
	void onMatTreeItemClicked(const QModelIndex &index);


private:
	QTreeView *matTree;
	QVBoxLayout *vlayoutMaterial;

	QLineEdit *pline_edit;
	QLabel *plabel_isar;
	QLabel *plabel_rcs;
	QWidget *lisence;
	FileMaterialModel *fileMaterialModel;
};

#endif