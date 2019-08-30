#ifndef MATERIALVIEWER_H_
#define MATERIALVIEWER_H_

#include <QMainWindow>
#include <QFileSystemModel>
#include <QVBoxLayout>
#include <QList>
#include "materialfilemodel.h" 
#include "materialtreeview.h"
#include "materialfileparser.h"
#include "materialpropertytreemodel.h" 
#include "materialtablemodel.h" 
#include "materialuihandleDef.h"
#include "materialtableview.h"
#include "materialclipboardmanager.h"
#include "materialglobal.h"

class QGroupBox;
class QComboBox;
class QLineEdit;
class QDir;
class IMaterialDimensionConvert;
class MaterialTypeParser;
class MaterialChartView;
class MaterialMenuBar;
class MaterialToolBar;
class MaterialViewerHandle;
class  MaterialLib_EXPORT  MaterialViewer : public QMainWindow
{
	friend MaterialMenuBar;
	friend MaterialToolBar;
	friend MaterialViewerHandle;
	Q_OBJECT
public:
	MaterialViewer(IMaterialDimensionConvert*);
	~MaterialViewer();

public:
	void		setCurrentMatDirPath(const QString &);
	QString		getCurrentMatDirPath();
	QStringList getCurrentMats();
signals:
	void sigCurrentMaterialChanged(const QStringList &);

private:    
	void reloadMatDatabase();
	void addMatFromDatabaseToCurrent();
	void deleteMatFromUserDefine();

	void initCurrentMat();
	void updateCurrentMat();
	void sendCurrentMatChanged(const QStringList & );
	void addCurrentMatToDatabase();
	void deleteMatFromCurrent();

	void onDatabaseMatTreeItemClicked(const QModelIndex &index);
	void onCurrentMatTreeItemClicked(const QModelIndex &index);

	void updatePropertyTreeViewByData(const QString& path);
	void updatePropertyTreeView(); 
	void updateTableView(); 

	void resetView();
	void resetPropertyTreeView();
	void resetTableView();
	void resetChartView();

	void renameCurrentMat(int );
	void saveDataToFile(const QString & path);

	void createMat();
	void editMat();
	void saveMat();
	bool checkSaveState();

	void insertRow(int ,ItemNode*, bool );
	void insertColumn(int, ItemNode*, bool );
	void deleteRow(int, ItemNode*);
	void deleteColumn(int,ItemNode*);
	void copyDataFromTable(ItemNode*);
	void pasteDataToTable(int, int, ItemNode*); 

	void addLineSeriesForList(); 
	void addLineSeriesForSnChart();

	void setCompareState(const unsigned char ); 

	void filter(); 
	void exportMaterial();
	void selectMatDatabase();
	 
protected:
	void resizeEvent(QResizeEvent *event);

private:
	QStringList findAllFiles(const QString&, const QStringList&);

public slots:
	void sl_databaseMatTreeLeftClicked(QMouseEvent *event);
	void sl_databaseMatTreeRightClicked(QMouseEvent *event);

	void sl_currentMatTreeLeftClicked(QMouseEvent *event); 
	void sl_currentMatTreeRightClicked(QMouseEvent *event);
	void sl_currentMatTreeLeftButReleased();
	void sl_currentMatListModelDataChanged(const QModelIndex& );
	void sl_currentMatDataChanged();

	void sl_databaseMatSelectToCurrent();

	//void sl_createMaterial();
	void sl_editMaterial();
	void sl_addCurrentMatAddDataBase();
	void sl_deleteMat(); 

	void sl_propertyTreeViewItemDoubleClicked(QMouseEvent *event ); 

	void sl_propertyTableRightClicked(QMouseEvent *event);

	void sl_addRowBefore();
	void sl_addRowAfter();
	void sl_addColumnBefore();
	void sl_addColumnAfter();
	void sl_deleteRow();
	void sl_deleteColumn();
	void sl_copyData();
	void sl_pasteData();
	void sl_importData();

	void sl_searchLineEditDataChanged(const QString&);
	void sl_filterComboboxDataChanged(const QString&);

private:
	MaterialTreeView *databaseMatTree;
	MaterialFileModel *materialFileModel;
	QString curDatabaseTreeItemPath;
	QString curDatabaseTreeItemName;
	QString matDatabaseDirPath;

	MaterialTreeView *currentMatTree;
	ListModel *listModel;
	QStringList currentMatNameList;
	QString matCurrentDirPath;
	QString curSelectName; 
	QString savePath;
	QString matTemplatePath;
	unsigned char controlMask;  //The first bit is CompareSate
								//the second bit is CreateMatState
							   //the third bit is all Tree Item selected 

	//QAction *createMatAct;//新建材料
	QAction *editMatAct;//编辑材料
	QAction *dataBaseMatSelectToAct;//加入当前工程 
	QAction *currentMatAddToAct;//加入到材料库
	QAction *deleteAct;//从当前材料库删除 
	QAction *propertyTableAddRowBeforeAct;
	QAction *propertyTableAddRowAfterAct;
	QAction *propertyTableAddColumnBeforeAct;
	QAction *propertyTableAddColumnAfterAct;
	QAction *propertyTableDeleteRowAct;
	QAction *propertyTableDeleteColumnAct;
	QAction *propertyTableCopyDataAct;
	QAction *propertyTablePasteDataAct;
	QAction *propertyTableImportDataAct;

	QGroupBox *chartViewGroupBox;
	MaterialChartView *chartView;

	
	QComboBox *filterComboBox;
	QLineEdit *searchLineEdit;
	MaterialMenuBar *menuBar;
	MaterialToolBar *toolBar;

	QGroupBox *propertyTreeViewGroupBox;
	MaterialTreeView * propertyTreeView;
	MaterialPropertyTreeModel * propertyTreeModel; 
	
	QGroupBox *propertyTableViewGroupBox;
	MaterialTableView *propertyTableView;
	MaterialTableModel * propertyTableModel; 
	Node* propertyData;
	std::vector<std::vector<double>> listItemData;
	TypeNode* snCharNode;

	Node* matData;
	QDomDocument templateDoc;
	MaterialOperationState materialOpState;
	MaterialOperationState prevMaterialOpState;
	IMaterialDimensionConvert* dimensionConvert;
	MaterialTypeParser* matTypeConfParser;
	QStringList filterStrList;
	QString filterStr;
};

#endif