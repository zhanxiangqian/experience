#include <QKeySequence>
#include "materialtoolbar.h"
#include "materialviewer.h"

MaterialToolBar::MaterialToolBar(MaterialViewer *viewer, QWidget* parent)
:QToolBar(parent ),
matViewer(viewer)
{
	init();
}


MaterialToolBar::~MaterialToolBar()
{
}

void MaterialToolBar::setLibraryMateriasSelected(const bool& flag)
{
	exportAct->setEnabled(flag);
	addToCurrentAct->setEnabled(flag);
}

void MaterialToolBar::setCurrentMatSelected(const bool& flag)
{
	exportAct->setEnabled(flag);
	addToLibraryAct->setEnabled(flag);
	editMaterialAct->setEnabled(flag);
	deleteAct->setEnabled(flag);
}

void MaterialToolBar::setUserDefineMatSelected(const bool& flag)
{
	deleteAct->setEnabled(flag);
}

void MaterialToolBar::setCompareControlActCheckState(const bool & flag)
{
	//lineCompareControlAct->setChecked(flag);
}

void MaterialToolBar::setEditMaterialActCheckState(const bool & flag )
{
	editMaterialAct->setEnabled(flag);
}

void MaterialToolBar::setCreateMaterial()
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

void MaterialToolBar::setSaveMaterial()
{
	createMaterialAct->setEnabled(true);
	saveMaterialAct->setEnabled(false);
	openAct->setEnabled(true); 
} 

void MaterialToolBar::init()
{
	
	openAct = this->addAction(QIcon(":/resources/material/file_open.png"), "Open (Ctrl+O)");
	openAct->setShortcut(tr("Ctrl+O"));
	connect(openAct, SIGNAL(triggered()), this, SLOT(sl_OpenActTriggered()));

	exportAct = this->addAction(QIcon(":/resources/material/file_export_stl.png"), "Export");
	connect(exportAct, SIGNAL(triggered()), this, SLOT(sl_ExportActTriggered()));

	createMaterialAct = this->addAction(QIcon(":/resources/material/file_new.png"), "Create Material (Ctrl+A)"); 
	createMaterialAct->setShortcut(tr("Ctrl+A"));
	connect(createMaterialAct, SIGNAL(triggered()), this, SLOT(sl_CreateMaterialActTriggered()));

	editMaterialAct = this->addAction(QIcon(":/resources/material/file_edit.png"), "Edit Material");
	connect(editMaterialAct, SIGNAL(triggered()), this, SLOT(sl_EditMaterialActTriggered()));

	saveMaterialAct = this->addAction(QIcon(":/resources/material/label_save.png"), "Save Material (Ctrl+S)");
	QKeySequence ksSave(QKeySequence::Save);
	saveMaterialAct->setShortcut(ksSave);
	connect(saveMaterialAct, SIGNAL(triggered()), this, SLOT(sl_SaveMaterialActTriggered()));

	addToCurrentAct = this->addAction(QIcon(":/resources/material/label_copy.png"),"Add to current");
	connect(addToCurrentAct, SIGNAL(triggered()), this, SLOT(sl_AddToCurrentActTriggered()));
	 
	addToLibraryAct = this->addAction(QIcon(":/resources/material/label_add.png"),"Add to library");
	connect(addToLibraryAct, SIGNAL(triggered()), this, SLOT(sl_AddToLibraryActTriggered()));
	 
	deleteAct = this->addAction(QIcon(":/resources/material/label_remove.png"),"Delete");
	connect(deleteAct, SIGNAL(triggered()), this, SLOT(sl_deleteFromCurrentActTriggered()));

	/*lineCompareControlAct = this->addAction(QIcon(":/resources/material/label_compare.png"), "Compare");
	connect(lineCompareControlAct, SIGNAL(triggered(const bool)), this, SLOT(sl_lineCompareControlActTriggered(const bool)));*/

	//lineCompareControlAct->setCheckable(true);
	setLibraryMateriasSelected(false);
	setCurrentMatSelected(false);
	saveMaterialAct->setEnabled(false);
}
 
void MaterialToolBar::sl_OpenActTriggered()
{
	matViewer->selectMatDatabase();
}

void MaterialToolBar::sl_ExportActTriggered()
{
	matViewer->exportMaterial();
}

void MaterialToolBar::sl_AddToCurrentActTriggered()
{
	matViewer->addMatFromDatabaseToCurrent();
}

void MaterialToolBar::sl_CreateMaterialActTriggered()
{
	matViewer->createMat();
}

void MaterialToolBar::sl_EditMaterialActTriggered()
{ 
	matViewer->editMat();
}

void MaterialToolBar::sl_SaveMaterialActTriggered()
{
	matViewer->saveMat();
}

void MaterialToolBar::sl_AddToLibraryActTriggered()
{
	matViewer->addCurrentMatToDatabase();
}

void MaterialToolBar::sl_deleteFromCurrentActTriggered()
{
	matViewer->deleteMatFromCurrent();
	matViewer->deleteMatFromUserDefine();
}

void MaterialToolBar::sl_lineCompareControlActTriggered(const bool checked )
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