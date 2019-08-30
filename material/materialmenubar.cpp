
#include <QMenu>
#include <QDesktopServices>
#include <QUrl>

#include "materialmenubar.h"
#include "materialviewer.h"


MaterialMenuBar::MaterialMenuBar(MaterialViewer * viewer )
:matViewer(viewer)
{
	init();
}

MaterialMenuBar::~MaterialMenuBar()
{
}

void MaterialMenuBar::setLibraryMateriasSelected(const bool& flag)
{
	exportAct->setEnabled(flag);
	addToCurrentAct->setEnabled(flag);
}

void MaterialMenuBar::setCurrentMatSelected(const bool& flag)
{  
	exportAct->setEnabled(flag);
	addToLibraryAct->setEnabled(flag);
	editMaterialAct->setEnabled(flag);
	deleteAct->setEnabled(flag);
}

void MaterialMenuBar::setUserDefineMatSelected(const bool& flag)
{
	deleteAct->setEnabled(flag);
}

void MaterialMenuBar::setCompareControlActCheckState(const bool & flag)
{
	//lineCompareControlAct->setChecked(flag);
}

void MaterialMenuBar::setEditMaterialActCheckState(const bool & flag)
{
	editMaterialAct->setEnabled(flag);
}

void MaterialMenuBar::setCreateMaterial()
{
	openAct->setEnabled(false);
	exportAct->setEnabled(false);
	addToCurrentAct->setEnabled(false);
	createMaterialAct->setEnabled(false);
	editMaterialAct->setEnabled(false);
	saveMaterialAct->setEnabled(true);
	addToLibraryAct->setEnabled(false);
	deleteAct->setEnabled(false);
}

void MaterialMenuBar::setSaveMaterial()
{ 
	createMaterialAct->setEnabled(true);
	saveMaterialAct->setEnabled(false);
	openAct->setEnabled(true);
}

void MaterialMenuBar::init()
{
	QMenu* fileMenu =this->addMenu("File");
	
	openAct = fileMenu->addAction(QIcon(":/resources/material/file_open.png"), "Open (Ctrl+O)");
	//openAct->setShortcut(tr("Ctrl+O"));
	connect(openAct, SIGNAL(triggered()), this, SLOT(sl_OpenActTriggered()));
	
	exportAct = fileMenu->addAction(QIcon(":/resources/material/file_export_stl.png"), "Export");
	connect(exportAct, SIGNAL(triggered()), this, SLOT(sl_ExportActTriggered()));

	QMenu* operationMenu =this->addMenu("Operation");

	createMaterialAct = operationMenu->addAction(QIcon(":/resources/material/file_new.png"), "Create Material (Ctrl+A)");
	//createMaterialAct->setShortcut(tr("Ctrl+A"));
	connect(createMaterialAct, SIGNAL(triggered()), this, SLOT(sl_CreateMaterialActTriggered()));

	editMaterialAct = operationMenu->addAction(QIcon(":/resources/material/file_edit.png"), "Edit Material");
	connect(editMaterialAct, SIGNAL(triggered()), this, SLOT(sl_EditMaterialActTriggered()));

	saveMaterialAct = operationMenu->addAction(QIcon(":/resources/material/label_save.png"), "Save Material (Ctrl+S)");
	//saveMaterialAct->setShortcut(tr("Ctrl+S"));
	connect(saveMaterialAct, SIGNAL(triggered()), this, SLOT(sl_SaveMaterialActTriggered()));

	
	addToCurrentAct = operationMenu->addAction(QIcon(":/resources/material/label_copy.png"), "Add to current");
	connect(addToCurrentAct, SIGNAL(triggered()), this, SLOT(sl_AddToCurrentActTriggered()));

	addToLibraryAct = operationMenu->addAction(QIcon(":/resources/material/label_add.png"), "Add to library");
	connect(addToLibraryAct, SIGNAL(triggered()), this, SLOT(sl_AddToLibraryActTriggered() ));

	deleteAct = operationMenu->addAction(QIcon(":/resources/material/label_remove.png"), "Delete");
	connect(deleteAct, SIGNAL(triggered()), this, SLOT(sl_deleteFromCurrentActTriggered()));

	/*lineCompareControlAct = operationMenu->addAction(QIcon(":/resources/material/label_compare.png"), "Compare");
	connect(lineCompareControlAct, SIGNAL(triggered(const bool)), this, SLOT(sl_lineCompareControlActTriggered(const bool)));*/

	//lineCompareControlAct->setCheckable(true);

	QMenu* helpMenu = this->addMenu("Help");
	helpAct = helpMenu->addAction(QIcon(":/resources/material/file_new.png"), "Help");
	connect(helpAct, SIGNAL(triggered()), this, SLOT(onHelp()));

	aboutAct = helpMenu->addAction(QIcon(":/resources/material/file_new.png"), "About");
	connect(aboutAct, SIGNAL(triggered()), this, SLOT(onAbout()));

	setLibraryMateriasSelected(false);
	setCurrentMatSelected(false);
	saveMaterialAct->setEnabled(false);
}

void MaterialMenuBar::sl_OpenActTriggered()
{
	matViewer->selectMatDatabase();
}

void MaterialMenuBar::sl_ExportActTriggered()
{
	matViewer->exportMaterial();
}

void MaterialMenuBar::sl_AddToCurrentActTriggered()
{
	matViewer->addMatFromDatabaseToCurrent();
}

void MaterialMenuBar::sl_CreateMaterialActTriggered()
{
	matViewer->createMat();
}

void MaterialMenuBar::sl_AddToLibraryActTriggered()
{
	matViewer->addCurrentMatToDatabase();
}

void MaterialMenuBar::sl_EditMaterialActTriggered()
{
	matViewer->editMat();
}

void MaterialMenuBar::sl_SaveMaterialActTriggered()
{
	createMaterialAct->setEnabled(true);
	editMaterialAct->setEnabled(true);
	saveMaterialAct->setEnabled(false);
	matViewer->saveMat();
}

void MaterialMenuBar::sl_deleteFromCurrentActTriggered()
{
	matViewer->deleteMatFromCurrent();
	matViewer->deleteMatFromUserDefine();
}

void MaterialMenuBar::sl_lineCompareControlActTriggered(const bool checked)
{
	if (checked)
	{
		matViewer->setCompareState(0x01);
	}
	else
	{
		matViewer->setCompareState(0x00);
	}
}


void MaterialMenuBar::onHelp()
{
	QDesktopServices::openUrl(QUrl("http://www.demxs.com"));
}

void MaterialMenuBar::onAbout()
{
}