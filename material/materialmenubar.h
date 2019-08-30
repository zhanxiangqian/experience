#pragma once
#include <QMenuBar>

class MaterialViewer;
class MaterialMenuBar:public QMenuBar
{
	Q_OBJECT
public:
	MaterialMenuBar(MaterialViewer * );
	~MaterialMenuBar();
public:
	void setLibraryMateriasSelected(const bool&);
	void setCurrentMatSelected(const bool&);
	void setUserDefineMatSelected(const bool&);
	void setCompareControlActCheckState(const bool&);
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
	void sl_AddToLibraryActTriggered();
	void sl_EditMaterialActTriggered();
	void sl_SaveMaterialActTriggered();
	void sl_deleteFromCurrentActTriggered();
	void sl_lineCompareControlActTriggered(const bool);

	void onHelp();
	void onAbout();
private:
	MaterialViewer * matViewer;

	QAction* openAct;
	QAction* exportAct;

	QAction* addToCurrentAct;
	QAction * createMaterialAct;
	QAction *addToLibraryAct;
	QAction *editMaterialAct;
	QAction * saveMaterialAct;
	QAction *deleteAct;
	QAction *lineCompareControlAct;

	QAction* helpAct;
	QAction* aboutAct;
};

