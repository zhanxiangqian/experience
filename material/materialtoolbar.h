#pragma once
#include <QToolBar>
class MaterialViewer;
class MaterialToolBar :public QToolBar
{
	Q_OBJECT
public:
	MaterialToolBar(MaterialViewer *,QWidget* parent = nullptr);
	~MaterialToolBar();
public:
	void setLibraryMateriasSelected(const bool&);
	void setCurrentMatSelected(const bool&);
	void setUserDefineMatSelected(const bool&);
	void setCompareControlActCheckState(const bool &);
	void setEditMaterialActCheckState(const bool &);
	void setCreateMaterial();
	void setSaveMaterial();
private:
	void init();
	private slots:
	void sl_OpenActTriggered();
	void sl_ExportActTriggered();
	void sl_AddToCurrentActTriggered();
	void sl_CreateMaterialActTriggered();
	void sl_EditMaterialActTriggered();
	void sl_SaveMaterialActTriggered();
	void sl_AddToLibraryActTriggered(); 
	void sl_deleteFromCurrentActTriggered();
	void sl_lineCompareControlActTriggered(const bool);
private:
	MaterialViewer * matViewer;
	QAction* openAct;
	QAction* exportAct;
	QAction* addToCurrentAct;
	QAction * createMaterialAct;
	QAction *editMaterialAct;
	QAction * saveMaterialAct;
	QAction *addToLibraryAct; 
	QAction *deleteAct;
	QAction *lineCompareControlAct;
};

