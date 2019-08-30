#include <QPushButton>
#include <QHBoxLayout>
#include <QTreeView>
#include <QStringList>
#include <QFileDialog>
#include <QGroupBox>  
#include <QLineSeries> 
#include <QAction>
#include <QMenu>
#include <QLabel>
#include <QHeaderView>
#include <QTableView>
#include <QComboBox>
#include <QSpacerItem>
#include <QMessageBox>
#include <QSplitter>
#include "materialconfigparser.h"
#include "materialviewer.h"
#include "imaterialdimensionconvert.h"
#include "materiallineeditdelegate.h"
#include "materialcomboboxdelegate.h"
#include "materialchartview.h" 
#include "materialmenubar.h"
#include "materialtoolbar.h"
#include "materialtypeparser.h"
#include "styleSheet.h"
#include "materialsavedialog.h"

MaterialViewer::MaterialViewer(IMaterialDimensionConvert* convert)
:QMainWindow(NULL),
	dimensionConvert(convert),
	matData(nullptr),
	propertyTableModel(new MaterialTableModel),
	propertyData(nullptr),
	materialOpState(MATERIAL_UNKNOW_STATE)
{
	menuBar = new MaterialMenuBar(this);
// 	this->setMenuBar(menuBar);
	toolBar = new MaterialToolBar(this);
// 	toolBar->setMovable(false);
// 	this->addToolBar(toolBar);
	//this->setMinimumSize(QSize(1024, 600));
	
	memset(&controlMask, 0, sizeof(controlMask)); 
	this->setCentralWidget(new QWidget);
	QHBoxLayout *hlayout = new QHBoxLayout();

	delete this->centralWidget()->layout();
	this->centralWidget()->setLayout(hlayout);

	QSplitter* hSplitter = new QSplitter; 

	hSplitter->setOrientation(Qt::Horizontal);
	hlayout->addWidget(hSplitter);
	 
	QWidget *leftWidget = new QWidget(this);
	QVBoxLayout *vleftLayout = new QVBoxLayout();
	leftWidget->setLayout(vleftLayout);
	leftWidget->setMinimumWidth(200);
	 
	matDatabaseDirPath =MaterialConfigParser::configs[MaterialConfigParser::originalDir]; 
	

	QHBoxLayout *leftTopHLayout = new QHBoxLayout(); 
	filterComboBox = new QComboBox;
//   	filterComboBox->setFixedWidth(80);
	filterComboBox->setMinimumWidth(50);
	filterComboBox->setFixedHeight(25);
	filterComboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	filterComboBox->addItem("");
	matTypeConfParser = new MaterialTypeParser;
	matTypeConfParser->read(MaterialConfigParser::configs[MaterialConfigParser::typeconfPath]);
	const std::map<QString, QStringList>& matTypeMap = matTypeConfParser->getTypeMap();
	std::map<QString, QStringList>::const_iterator iter = matTypeMap.begin();
	while (iter!=matTypeMap.end() )
	{
		filterComboBox->addItem(iter->first);
		iter++;
	}

	QLabel *filterLabel = new QLabel("Filter");
	leftTopHLayout->addWidget(filterLabel);
	leftTopHLayout->addWidget(filterComboBox);
// 	QSpacerItem* spacerItem = new QSpacerItem(6, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);
// 	leftTopHLayout->addSpacerItem(spacerItem);

	searchLineEdit = new QLineEdit;
	searchLineEdit->setFixedHeight(25);
	searchLineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
// 	searchLineEdit->setMinimumWidth(80);

	QLabel *searchLabel = new QLabel("Search");
	leftTopHLayout->addWidget(searchLabel);
	leftTopHLayout->addWidget(searchLineEdit); 
// 	spacerItem = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
// 	leftTopHLayout->addItem(spacerItem);

	vleftLayout->addLayout(leftTopHLayout);

	QSplitter *leftSplitter = new QSplitter;
	leftSplitter->setOrientation(Qt::Vertical);

	vleftLayout->addWidget(leftSplitter);

	databaseMatTree = new MaterialTreeView();
	databaseMatTree->setSelectionMode(QAbstractItemView::ContiguousSelection);
	leftSplitter->addWidget(databaseMatTree);

	currentMatTree = new MaterialTreeView();
	currentMatTree->setSelectionMode(QAbstractItemView::ContiguousSelection);
	MaterialLineEditDelegate* lineEditDelegate = new MaterialLineEditDelegate;
	currentMatTree->setItemDelegate(lineEditDelegate);
	leftSplitter->addWidget(currentMatTree);

	leftSplitter->setStretchFactor(0, 1);
	leftSplitter->setStretchFactor(1, 1);
	leftSplitter->setCollapsible(0, false);
	leftSplitter->setCollapsible(1, false);

	QWidget * rightWidget = new QWidget;
	QVBoxLayout *vrightLayout = new QVBoxLayout();
	rightWidget->setLayout(vrightLayout);

	QSplitter *vrightSplitter = new QSplitter;
	vrightSplitter->setOrientation(Qt::Vertical);
	vrightLayout->addWidget(vrightSplitter);

	propertyTreeView = new MaterialTreeView;
	propertyTreeView->header()->setVisible(true);
	lineEditDelegate = new MaterialLineEditDelegate;
	propertyTreeView->setItemDelegateForColumn(1, lineEditDelegate);
	propertyTreeModel = new MaterialPropertyTreeModel(dimensionConvert, propertyTreeView);
	MaterialComboBoxDelegate *comboboxDelegate = new MaterialComboBoxDelegate(dimensionConvert);
	propertyTreeView->setItemDelegateForColumn(2, comboboxDelegate);
	propertyTreeViewGroupBox = new QGroupBox;
	QVBoxLayout *propertyTreeViewLayout = new QVBoxLayout;
	propertyTreeViewGroupBox->setLayout(propertyTreeViewLayout);
	propertyTreeViewLayout->addWidget(propertyTreeView);

	/*QHBoxLayout *rightTopRightLayout = new QHBoxLayout();
	rightTopRightLayout->addWidget(propertyTreeViewGroupBox, 1);*/

	propertyTableView = new MaterialTableView;
	lineEditDelegate = new MaterialLineEditDelegate;
	propertyTableView->setItemDelegate(lineEditDelegate);
	propertyTableViewGroupBox = new QGroupBox;
	QVBoxLayout *propertyTableViewLayout = new QVBoxLayout;
	propertyTableViewGroupBox->setLayout(propertyTableViewLayout);
	propertyTableViewLayout->addWidget(propertyTableView);

	chartView = new MaterialChartView(this);
	chartViewGroupBox = new QGroupBox;
	QVBoxLayout *chartViewLayout = new QVBoxLayout;
	chartViewGroupBox->setLayout(chartViewLayout);
	chartViewLayout->addWidget(chartView);
	chartViewGroupBox->setTitle("Object Display");
	chartView->hide();
	chartViewGroupBox->hide();
	
	QHBoxLayout* rightBottomLayout = new QHBoxLayout;
	QSplitter *rightBottomSplitter = new QSplitter;
	rightBottomSplitter->setOrientation(Qt::Horizontal);
	rightBottomLayout->addWidget(rightBottomSplitter);

	QWidget *rightBottomWidget = new QWidget;
	rightBottomWidget->setLayout(rightBottomLayout);

	rightBottomSplitter->addWidget(propertyTableViewGroupBox);
	rightBottomSplitter->addWidget(chartViewGroupBox);
	rightBottomSplitter->setStretchFactor(0, 3);
	rightBottomSplitter->setStretchFactor(1, 5);
	rightBottomSplitter->setCollapsible(0, false);
	rightBottomSplitter->setCollapsible(1, false);

	vrightSplitter->addWidget(propertyTreeViewGroupBox);
	vrightSplitter->addWidget(rightBottomWidget);
	vrightSplitter->setCollapsible(0, false);
	vrightSplitter->setCollapsible(1, false);
	
	hSplitter->addWidget(leftWidget );
	hSplitter->addWidget(rightWidget);
	hSplitter->setStretchFactor(0, 1);
	hSplitter->setStretchFactor(1, 3);
	hSplitter->setCollapsible(0, false);
	hSplitter->setCollapsible(1, false);
	

	materialFileModel = new MaterialFileModel(this);
	materialFileModel->setFilter(QDir::Files | QDir::AllDirs | QDir::NoDotAndDotDot);
	QStringList filters;
	filters << "*.xml";
	materialFileModel->setNameFilters(filters);
	materialFileModel->setNameFilterDisables(false);

	dataBaseMatSelectToAct = new QAction(QIcon(":/resources/material/label_copy.png"), "Add to current", this);

	//createMatAct = new QAction(QIcon(":/resources/material/file_new.png"), "Create Material", this);
	editMatAct = new QAction(QIcon(":/resources/material/file_edit.png"), "Edit Material", this);
	currentMatAddToAct = new QAction(QIcon(":/resources/material/label_add.png"), "Add to library", this);
	deleteAct = new QAction(QIcon(":/resources/material/label_remove.png"), "Delete", this);

	propertyTableAddRowBeforeAct =new QAction("Insert above", this );
	propertyTableAddRowAfterAct =new QAction("Insert below", this );
	propertyTableAddColumnBeforeAct =new QAction("Insert before", this );
	propertyTableAddColumnAfterAct =new QAction("Insert after", this );
	propertyTableDeleteRowAct = new QAction("Delete row", this);
	propertyTableDeleteColumnAct = new QAction("Delete column", this);
	propertyTableCopyDataAct = new QAction("Copy", this);
	propertyTablePasteDataAct = new QAction("Paste", this);
	propertyTableImportDataAct = new QAction("Import", this);
	 
	initCurrentMat();

	connect(databaseMatTree, SIGNAL(rightButtonClicked(QMouseEvent *)), this, SLOT(sl_databaseMatTreeRightClicked(QMouseEvent *)));
	connect(currentMatTree, SIGNAL(rightButtonClicked(QMouseEvent *)), this, SLOT(sl_currentMatTreeRightClicked(QMouseEvent *)));
	connect(databaseMatTree, SIGNAL(leftButtonClicked(QMouseEvent *)), this, SLOT(sl_databaseMatTreeLeftClicked(QMouseEvent *)));
	connect(currentMatTree, SIGNAL(leftButtonClicked(QMouseEvent *)), this, SLOT(sl_currentMatTreeLeftClicked(QMouseEvent *))); 
	connect(currentMatTree, SIGNAL(leftButtonReleased()), this, SLOT(sl_currentMatTreeLeftButReleased() ));
	
	connect(dataBaseMatSelectToAct, SIGNAL(triggered(bool)), this, SLOT(sl_databaseMatSelectToCurrent()));
	//connect(createMatAct, SIGNAL(triggered(bool)), this, SLOT(sl_createMaterial()));
	connect(editMatAct, SIGNAL(triggered(bool)), this, SLOT(sl_editMaterial()));
	connect(currentMatAddToAct, SIGNAL(triggered(bool)), this, SLOT(sl_addCurrentMatAddDataBase()));
	connect(deleteAct, SIGNAL(triggered(bool)), this, SLOT(sl_deleteMat()));
	connect(propertyTreeView, SIGNAL(leftButtonDoubleClicked(QMouseEvent *)), this, SLOT(sl_propertyTreeViewItemDoubleClicked(QMouseEvent *)));

	connect(propertyTreeModel, SIGNAL(treeDataChanged() ), this, SLOT(sl_currentMatDataChanged() ) );
	connect(propertyTableModel, SIGNAL(tableDataChanged() ), this, SLOT(sl_currentMatDataChanged()));

	connect(propertyTableView, SIGNAL(rightButtonClicked(QMouseEvent *)), this, SLOT(sl_propertyTableRightClicked(QMouseEvent*)));
	connect(propertyTableAddRowBeforeAct, SIGNAL(triggered(bool)), this, SLOT(sl_addRowBefore()));
	connect(propertyTableAddRowAfterAct, SIGNAL(triggered(bool)), this, SLOT(sl_addRowAfter()));
	connect(propertyTableAddColumnBeforeAct, SIGNAL(triggered(bool)), this, SLOT(sl_addColumnBefore()));
	connect(propertyTableAddColumnAfterAct, SIGNAL(triggered(bool)), this, SLOT(sl_addColumnAfter()));
	connect(propertyTableDeleteRowAct, SIGNAL(triggered(bool)), this, SLOT(sl_deleteRow()));
	connect(propertyTableDeleteColumnAct, SIGNAL(triggered(bool)), this, SLOT(sl_deleteColumn()));
	connect(propertyTableCopyDataAct, SIGNAL(triggered(bool)), this, SLOT(sl_copyData()));
	connect(propertyTablePasteDataAct, SIGNAL(triggered(bool)), this, SLOT(sl_pasteData()));
	connect(propertyTableImportDataAct, SIGNAL(triggered(bool)), this, SLOT(sl_importData()));

	connect(searchLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(sl_searchLineEditDataChanged(const QString&) ));
	connect(filterComboBox, SIGNAL(currentTextChanged(const QString &)), this, SLOT(sl_filterComboboxDataChanged(const QString&))); 

	reloadMatDatabase();
	this->resize(1280, 720);
	QWidget::setStyleSheet(QString(stylesheet)); 
}

MaterialViewer::~MaterialViewer()
{
	
}

void MaterialViewer::setCurrentMatDirPath(const QString & path )
{
	if(path.at(path.length()-1)=="/" || path.at(path.length() - 1)== "\\")
	{
		matCurrentDirPath =path.left(path.length()-1);
	}
	else
	{
		matCurrentDirPath = path;
	}

	MaterialConfigParser::configs[MaterialConfigParser::selectDir] = matCurrentDirPath;
	MaterialConfigParser::usedWorkMap[MaterialConfigParser::selectDir] = false;
	MaterialConfigParser::save();
	updateCurrentMat();
}

QString MaterialViewer::getCurrentMatDirPath()
{
	return matCurrentDirPath;
}

QStringList MaterialViewer::getCurrentMats()
{
	return currentMatNameList;
}
 
void MaterialViewer::reloadMatDatabase()
{
	MaterialConfigParser::configs[MaterialConfigParser::originalDir] = matDatabaseDirPath;
	MaterialConfigParser::save();
	materialFileModel->setRootPath(matDatabaseDirPath);
	databaseMatTree->setModel(materialFileModel);
	databaseMatTree->setRootIndex(materialFileModel->index(matDatabaseDirPath)); 
	
	matTemplatePath = matDatabaseDirPath + "/materialType.tp";
	if (!QFile::exists(matTemplatePath))
	{
		matTemplatePath = "";
	}
}

void MaterialViewer::addMatFromDatabaseToCurrent()
{
	QItemSelectionModel* selectionModel = databaseMatTree->selectionModel();
	QString curName;
	if (selectionModel)
	{
		QModelIndexList indexList = selectionModel->selectedIndexes();
		for (int i = 0; i < indexList.length(); i++)
		{
			QModelIndex &index = indexList[i];
			if (index.isValid())
			{

				QString fromFilePath =materialFileModel->filePath(index); 
				curName = index.model()->data(index, Qt::DisplayRole).toString();

				if (!curName.isEmpty())
				{
					curName += ".xml";
					QString toFilePath(matCurrentDirPath + "/" + curName);
					QFile toFile(toFilePath);
					if (toFile.exists())
					{
						toFile.remove();
					}
					QFile fromFile(fromFilePath);
					if (fromFile.exists( ))
					{
						fromFile.copy(toFilePath);
					} 
				}
			}
		}
		updateCurrentMat();
	}
}

void MaterialViewer::deleteMatFromUserDefine()
{
	menuBar->setCurrentMatSelected(false);
	toolBar->setCurrentMatSelected(false);
	QDir dir(matDatabaseDirPath+"/UserDefine");
	QItemSelectionModel* selectionModel = databaseMatTree->selectionModel();
	
	if (selectionModel)
	{
		QString curName;
		QModelIndexList indexList = selectionModel->selectedIndexes();
		for (int i = 0; i<indexList.length(); i++ )
		{
			QModelIndex index = indexList[i];
			if (index.isValid())
			{
				curName = index.model()->data(index, Qt::DisplayRole).toString();
				if (!curName.isEmpty())
				{
					dir.remove(curName + ".xml");
					if (curDatabaseTreeItemName == curName)
					{
						if (matData != nullptr)
						{
							resetView();
							delete matData;
							matData = nullptr;
						}
					}
				}
			}
		} 
	}
}

void MaterialViewer::initCurrentMat()
{ 
	matCurrentDirPath = MaterialConfigParser::configs[MaterialConfigParser::selectDir];
	QDir dir(matCurrentDirPath);
	if (!dir.exists())
	{
		dir.mkdir(matCurrentDirPath);
	}
	listModel = new ListModel(this);
	connect(listModel, SIGNAL(listDataChanged(const QModelIndex &)),
		this, SLOT(sl_currentMatListModelDataChanged(const QModelIndex &)));
	updateCurrentMat();
}

void MaterialViewer::updateCurrentMat()
{
	QDir dir(matCurrentDirPath);
	QStringList filters;
	filters.append("*.xml");
	QStringList tempMatNameList = dir.entryList(filters, QDir::Files);
	for (int i = 0; i < tempMatNameList.length(); i++ )
	{
		QString & curName = tempMatNameList[i];
		curName.remove(curName.length() - 4, 4);
	}

	bool flag = true;
	int len = tempMatNameList.length();
	if (len==currentMatNameList.length() )
	{ 
		flag = false;
		for (int i=0; i<len; i++ )
		{
			if (currentMatNameList.at(i)!=tempMatNameList.at(i) )
			{
				flag = true;
				break;
			}
		}
	}

	currentMatNameList = tempMatNameList;
	listModel->setModelData(&currentMatNameList);
	currentMatTree->setModel(nullptr);
	currentMatTree->setModel(listModel);
	currentMatTree->update();

	if (flag)
	{
		sendCurrentMatChanged(currentMatNameList);
	}
}

void MaterialViewer::sendCurrentMatChanged(const QStringList & strList )
{
	emit sigCurrentMaterialChanged(strList);
}

void MaterialViewer::addCurrentMatToDatabase()
{
	QString desDirPath = matDatabaseDirPath + "/UserDefine";
	QDir dir(desDirPath);
	if (!dir.exists() )
	{
		dir.mkdir(desDirPath);
	}
	QItemSelectionModel* selectionModel = currentMatTree->selectionModel();
	QString curName;
	if (selectionModel)
	{
		QModelIndexList indexList = selectionModel->selectedIndexes();
		for (int i = 0; i < indexList.length(); i++)
		{
			QModelIndex &index = indexList[i];
			if (index.isValid())
			{
				curName = index.model()->data(index, Qt::DisplayRole).toString();
				if (!curName.isEmpty())
				{
					curName += ".xml";
					QFile file(matCurrentDirPath + "/" + curName);
					QFile desFile(desDirPath + "/" + curName);
					if (desFile.exists())
					{
						desFile.remove();
					}
					file.copy(desDirPath + "/" + curName);
				}
			}
		}
		reloadMatDatabase();
	} 
}

void MaterialViewer::deleteMatFromCurrent()
{
	menuBar->setCurrentMatSelected(false);
	toolBar->setCurrentMatSelected(false);
	QDir dir(matCurrentDirPath);
	QItemSelectionModel* selectionModel = currentMatTree->selectionModel();

	if (selectionModel )
	{
		QString curName;
		QModelIndexList indexList = selectionModel->selectedIndexes();
		for (int i = 0; i<indexList.length(); i++ )
		{
			QModelIndex index = indexList[i];
			if (index.isValid())
			{
				curName = index.model()->data(index, Qt::DisplayRole).toString();
				if (!curName.isEmpty())
				{
					dir.remove(curName + ".xml");
					if (curSelectName == curName)
					{
						if (matData != nullptr)
						{
							resetView();
							delete matData;
							matData = nullptr;
						}
					}
				}
			}
		} 
	} 
	updateCurrentMat();
}

void MaterialViewer::onDatabaseMatTreeItemClicked(const QModelIndex &index)
{
	MaterialFileModel * fmmodel = (MaterialFileModel *)databaseMatTree->model();
	QVariant val = fmmodel->data(index, Qt::DisplayRole); 
	QFileInfo fi(fmmodel->filePath(index));
	if (fi.isFile())
	{
		fmmodel->setFile(fi.absoluteFilePath()); 
		prevMaterialOpState = materialOpState;
		materialOpState = MATERIAL_BROWSE_STATE;
		curDatabaseTreeItemName = val.toString();
		curDatabaseTreeItemPath = fi.absoluteFilePath();
		resetView();
		curSelectName = "";
		updatePropertyTreeViewByData(fi.absoluteFilePath() ); 
		controlMask |= 0x04;
	}
}

void MaterialViewer::onCurrentMatTreeItemClicked(const QModelIndex &index)
{
	if (index.isValid() )
	{
		prevMaterialOpState = materialOpState;
		materialOpState = MATERIAL_EDIT_STATE;
		if (curSelectName != currentMatNameList[index.row()])
		{
			curSelectName = currentMatNameList[index.row()];
		}
		else{
			return;
		}
		
		QString curSelectFilePath = matCurrentDirPath + "/" + curSelectName + ".xml";
		resetView();
		updatePropertyTreeViewByData(curSelectFilePath);
	}
}

void MaterialViewer::renameCurrentMat(int pos)
{
	QStringList filters;
	filters << "*.xml";
	QStringList existNameList = findAllFiles(matDatabaseDirPath, filters);
	for (int i = 0; i < existNameList.length(); i++)
	{
		existNameList[i].remove(".xml");
	}
	existNameList.append(currentMatNameList);
	int flag = 0;
	for (int i = 0; i < existNameList.length(); i++ )
	{
		if (currentMatNameList[pos]==existNameList[i])
		{
			flag++;
		} 
		if (flag==2)
		{
			currentMatNameList[pos] = curSelectName;
			return;
		}
	} 

	QFile file(matCurrentDirPath + "/" + curSelectName+".xml");
	file.rename(matCurrentDirPath + "/" + currentMatNameList[pos] + ".xml");
	curSelectName = currentMatNameList[pos];
	savePath = matCurrentDirPath + "/" + curSelectName + ".xml";
	matData->name =curSelectName+"(CUR)";
	sendCurrentMatChanged(currentMatNameList);
}

void MaterialViewer::saveDataToFile(const QString & path)
{
	 MaterialFileParser parser;
	 parser.setPath(path);
	 parser.saveToFile(matData ); 
}

void MaterialViewer::createMat()
{
	if (matTemplatePath.isEmpty())
	{
		QMessageBox::information(nullptr, "Create material", "Material template is not exist!");
		return;
	}
	prevMaterialOpState = materialOpState;
	materialOpState = MATERIAL_CREATE_STATE;
	
	resetView();
	curSelectName = "new";
	updatePropertyTreeViewByData(matTemplatePath);
	if (matData->childCount() == 0)
	{
		QMessageBox::information(nullptr, "Create material", "Material template format is not right!");
		return;
	}
	menuBar->setCreateMaterial();
	toolBar->setCreateMaterial();
	controlMask |= 0x02;
}

void MaterialViewer::editMat()
{
	if (matTemplatePath.isEmpty())
	{
		QMessageBox::information(nullptr, "Edit material", "Material template is not exist!");
		return;
	}

	QItemSelectionModel* selectModel = currentMatTree->selectionModel();
	QString curName;
	if (selectModel)
	{
		QModelIndex index = selectModel->currentIndex();
		if (index.isValid())
		{
			curName = index.model()->data(index, Qt::DisplayRole).toString();
		}
	}

	if (!curName.isEmpty())
	{
		resetView();
		MaterialFileParser parser;
		parser.setPath(matTemplatePath, true);
		if (matData != nullptr)
		{
			delete matData;
		}
		matData = new Node(curName + "(CUR)");
		parser.readFromFile(matData); 
		if (matData->childCount()==0)
		{
			QMessageBox::information(nullptr, "Edit material", "Material template format is not right!");
			return;
		}
		Node* curData = new Node(curName + "(CUR)");
		curSelectName = curName;
		QString curPath = matCurrentDirPath + "/" + curName + ".xml";
		parser.setPath(curPath);
		parser.readFromFile(curData);
		if (matData->templateId==curData->templateId)
		{
			combineNode(matData, curData);
			delete curData;
			savePath = curPath;
			prevMaterialOpState = materialOpState;
			materialOpState = MATERIAL_CREATE_STATE;
			updatePropertyTreeView(); 
		}
		else
		{
			delete matData;
			matData = curData;
			updatePropertyTreeView();
			QMessageBox::information(nullptr, "Edit material", "The material and template is not matched");  
		}
	}   
}

void MaterialViewer::saveMat()
{
	if (matData!=nullptr)
	{
		MaterialSaveDialog dialog;
		QStringList pathList;
		pathList << "Current Material" << "Material Database";
		dialog.setComboboxList(pathList);

		QStringList filters;
		filters << "*.xml"; 
		QStringList existNameList = findAllFiles(matDatabaseDirPath, filters);
		for (int i = 0; i < existNameList.length(); i++ )
		{
			existNameList[i].remove(".xml");
		}
		existNameList.append(currentMatNameList);

		dialog.setExistNames(existNameList);
		dialog.initUI();
		dialog.setFixedSize(180, 100);
		dialog.setWindowTitle("Save Material");
		if (dialog.exec() == QDialog::Accepted)
		{
			if ("Current Material" == dialog.getPath())
			{
				matData->name = dialog.getName() + "(CUR)";
				savePath = matCurrentDirPath;
				materialOpState = MATERIAL_EDIT_STATE;
			}
			else if ("Material Database" == dialog.getPath())
			{
				matData->name = dialog.getName();
				savePath = matDatabaseDirPath + "/UserDefine";
				materialOpState = MATERIAL_BROWSE_STATE;
			}
			savePath = savePath + "/" + dialog.getName() + ".xml";
			if ((materialOpState | prevMaterialOpState)&(MATERIAL_CREATE_STATE | MATERIAL_EDIT_STATE))
			{
				saveDataToFile(savePath);
				if ("Current Material" == dialog.getPath())
				{
					updateCurrentMat();
				}
			}
		}
		else
		{
			delete matData;
			matData = nullptr;
			propertyData = nullptr;
			snCharNode = nullptr;
			resetView();
		}
	}
	
	controlMask &= 0xfd;
	updatePropertyTreeView();
	menuBar->setSaveMaterial();
	toolBar->setSaveMaterial();
}

bool MaterialViewer::checkSaveState()
{
	bool flag = true; 
	QMessageBox msgBox;
	msgBox.setText("Save the material");
	msgBox.setInformativeText("Do you want to save your changes?");
	msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel | QMessageBox::Discard);
	msgBox.setDefaultButton(QMessageBox::Save);
	int ret = msgBox.exec();
	if (ret == QMessageBox::Save)
	{
		saveMat();
		flag = true;
	}
	else if (ret == QMessageBox::Discard)
	{
		delete matData;
		matData = nullptr;
		propertyData = nullptr;
		snCharNode = nullptr;
		resetView();
		controlMask &= 0xfd;
		updatePropertyTreeView();
		menuBar->setSaveMaterial();
		toolBar->setSaveMaterial();
		flag = true;
	}
	else
	{
		flag = false;
	}

	return flag;
}

void MaterialViewer::setCompareState(const unsigned char mask)
{
	if (mask & 0x01)
	{
		controlMask |= 0x1;
		menuBar->setCompareControlActCheckState(true);
		toolBar->setCompareControlActCheckState(true);
	}
	else
	{
		menuBar->setCompareControlActCheckState(false);
		toolBar->setCompareControlActCheckState(false);
		controlMask &= 0xfe;
	}
}

void MaterialViewer::filter()
{
	QStringList filters = filterStrList;
	if (filters.size() > 0)
	{
		for (int i = filters.size() - 1; i >= 0; i--)
		{
			if (!filters[i].contains(filterStr))
			{
				filters.removeAt(i);
			}
		}
		if (filters.size()==0 )
		{
			filters.append("#$~^&12!");
		}
	}
	else
	{
		filters.append( filterStr + "*.xml");
	}

	if (filters.size()==0)
	{
		filters.append("*.xml");
	}
	materialFileModel->setFilter(materialFileModel->filter()|QDir::Dirs | QDir::Files);
	materialFileModel->setNameFilters(filters);
	reloadMatDatabase();
	databaseMatTree->expandAll();
}

void MaterialViewer::exportMaterial()
{  
	QString savePath =QFileDialog::getSaveFileName(nullptr, tr("Export"), QString(), tr("*.xml") );
	if (!savePath.isEmpty())
	{
		saveDataToFile(savePath);
	}
}
 
void MaterialViewer::selectMatDatabase()
{
	QFileDialog fileDialog;

	QString fileName = fileDialog.getExistingDirectory(NULL, "select material library", matDatabaseDirPath );

	if (!fileName.isEmpty())
	{
		matDatabaseDirPath =fileName;
	}

	reloadMatDatabase();
}

void MaterialViewer::resizeEvent(QResizeEvent *event)
{
	QMainWindow::resizeEvent(event);
	int propertyTreeWidth = propertyTreeView->width(); 
	propertyTreeView->setColumnWidth(0, propertyTreeWidth*0.4);
	propertyTreeView->setColumnWidth(1, propertyTreeWidth*0.3);
	propertyTreeView->setColumnWidth(2, propertyTreeWidth*0.3);
}

QStringList MaterialViewer::findAllFiles(const QString& dirPath, const QStringList& filters)
{
	QStringList fileList;
	QDir dir(dirPath);
	fileList.append(dir.entryList(filters, QDir::Files));
	QStringList dirList = dir.entryList(QDir::AllDirs | QDir::NoDotDot | QDir::NoDot);
	for (int i = 0; i < dirList.length(); i++ )
	{ 
		fileList.append(
			findAllFiles(dirPath+"/"+dirList.at(i), filters)
			);
	}
	return fileList;
}

void MaterialViewer::sl_databaseMatTreeLeftClicked(QMouseEvent *event)
{
	currentMatTree->clearSelection();
	menuBar->setCurrentMatSelected(false);
	toolBar->setCurrentMatSelected(false);
	QModelIndex index = databaseMatTree->indexAt(event->pos());
	MaterialFileModel * fmmodel = (MaterialFileModel *)databaseMatTree->model();   
	QFileInfo fi(fmmodel->filePath(index));
	if (fi.isFile())
	{ 
		if (controlMask & 0x02)
		{
			if (!checkSaveState())
			{
				return;
			} 
		}
		menuBar->setLibraryMateriasSelected(true);
		toolBar->setLibraryMateriasSelected(true);
	}
	else
	{
		menuBar->setLibraryMateriasSelected(false);
		toolBar->setLibraryMateriasSelected(false);
	}

	if (index.isValid() )
	{
		QModelIndex parentIndex = index.parent();
		if (parentIndex.isValid())
		{
			if (fmmodel->data(parentIndex, Qt::DisplayRole).toString() == "UserDefine")
			{
				menuBar->setUserDefineMatSelected(true);
				toolBar->setUserDefineMatSelected(true);
			}
		}
	}  
	onDatabaseMatTreeItemClicked(databaseMatTree->indexAt(event->pos()));
}

void MaterialViewer::sl_databaseMatTreeRightClicked(QMouseEvent *event)
{
	currentMatTree->clearSelection();
	menuBar->setCurrentMatSelected(false);
	toolBar->setCurrentMatSelected(false);
	QModelIndex index =databaseMatTree->indexAt(event->pos());
	MaterialFileModel * fmmodel = (MaterialFileModel *)databaseMatTree->model();

	QVariant var = fmmodel->data(index, Qt::DisplayRole);//!!!

	QFileInfo fi(fmmodel->filePath(index));
	if (fi.isDir())
	{
		menuBar->setLibraryMateriasSelected(false);
		toolBar->setLibraryMateriasSelected(false);
	}
	else if (fi.isFile())
	{
		if (controlMask & 0x02)
		{
			checkSaveState();
			return;
		}

		curDatabaseTreeItemName = var.toString();
		curDatabaseTreeItemPath = fi.absoluteFilePath();
		controlMask |= 0x04;
		menuBar->setLibraryMateriasSelected(true);
		toolBar->setLibraryMateriasSelected(true);

		QPoint p = mapToParent(event->pos());
		QMenu *menu = new QMenu(this);
		menu->addAction(dataBaseMatSelectToAct);
		QModelIndex parentIndex = index.parent();
		if (parentIndex.isValid() )
		{
			if (fmmodel->data(parentIndex, Qt::DisplayRole).toString() == "UserDefine")
			{
				menu->addAction(deleteAct);
				menuBar->setUserDefineMatSelected(true);
				toolBar->setUserDefineMatSelected(true);
			}
		}
		p.setY(p.y() + 40 + menuBar->height() + toolBar->height());
		menu->exec(event->globalPos());
	}
	else
	{
		menuBar->setLibraryMateriasSelected(false);
		toolBar->setLibraryMateriasSelected(false);
	}
}


void MaterialViewer::sl_currentMatTreeLeftClicked(QMouseEvent *event)
{
	databaseMatTree->clearSelection();
	menuBar->setLibraryMateriasSelected(false);
	toolBar->setLibraryMateriasSelected(false);
	QItemSelectionModel* selectModel = currentMatTree->selectionModel();
	if (selectModel)
	{
		QModelIndex index = selectModel->currentIndex();
		if (index.isValid())
		{
			if (controlMask & 0x02)
			{
				if (!checkSaveState())
				{
					return;
				} 
			} 
			menuBar->setCurrentMatSelected(true);
			toolBar->setCurrentMatSelected(true); 
		}
		else
		{
			menuBar->setCurrentMatSelected(false);
			toolBar->setCurrentMatSelected(false);
		}
	}
	onCurrentMatTreeItemClicked(currentMatTree->indexAt(event->pos()));
}

void MaterialViewer::sl_currentMatTreeRightClicked(QMouseEvent *event)
{
	databaseMatTree->clearSelection();
	menuBar->setLibraryMateriasSelected(false);
	toolBar->setLibraryMateriasSelected(false);
	QItemSelectionModel* selectModel = currentMatTree->selectionModel(); 
	if (selectModel)
	{
		QModelIndex index = selectModel->currentIndex();
		if (index.isValid())
		{ 
			if (controlMask & 0x02)
			{
				checkSaveState();
				return;
			}
			
			menuBar->setCurrentMatSelected(true);
			toolBar->setCurrentMatSelected(true);
			if (selectModel->selectedIndexes().length()>1)
			{
				menuBar->setEditMaterialActCheckState(false);
				toolBar->setEditMaterialActCheckState(false);
			}
		}
		else
		{
			menuBar->setCurrentMatSelected(false);
			toolBar->setCurrentMatSelected(false);
		}
	}

	QModelIndex index = currentMatTree->indexAt(event->pos());
	QPoint p = mapToParent(event->pos());
	p.setY(p.y() + databaseMatTree->height()+menuBar->height()+toolBar->height());
	QMenu *menu = new QMenu(this); 
	//menu->addAction(createMatAct);
	if (index.isValid())
	{ 
		menu->addAction(editMatAct );
		menu->addAction(currentMatAddToAct);
		menu->addAction(deleteAct);
	}
	else
	{
		p.setY(p.y() + 40);
	} 
	
	menu->exec(event->globalPos());
}

void MaterialViewer::sl_currentMatTreeLeftButReleased()
{
	QItemSelectionModel* selectModel = currentMatTree->selectionModel();
	if (selectModel)
	{
		QModelIndexList indexes = selectModel->selectedIndexes();
		if (indexes.length()>1)
		{
			menuBar->setEditMaterialActCheckState(false);
			toolBar->setEditMaterialActCheckState(false);
		}
		else if (indexes.length()==1)
		{
			menuBar->setEditMaterialActCheckState(true);
			toolBar->setEditMaterialActCheckState(true);
		}
	}
}

void MaterialViewer::sl_currentMatListModelDataChanged(const QModelIndex & index )
{
	renameCurrentMat(index.row());
}

void MaterialViewer::sl_currentMatDataChanged()
{
	if (propertyData!=nullptr)
	{
		if (propertyData->nodeType==TYPETYPE)
		{
			TypeNode* typeNode = static_cast<TypeNode*>(propertyData);
			if (typeNode->itemType == LIST)
			{
				ItemNode* dsNode = nullptr;
				ItemNode* dnNode = nullptr;
				for (int i = 0; i < typeNode->childCount(); i++)
				{
					ItemNode * listItemNode = (ItemNode*)typeNode->childAt(i);
					if (listItemNode->vAxis == YVALUE)
					{
						dsNode = listItemNode;
					}
					else if (listItemNode->vAxis == XVALUE)
					{
						dnNode = listItemNode;
					}
				}
				if (dsNode != nullptr && dnNode != nullptr)
				{
					dsNode->value[0].clear();
					dnNode->value[0].clear();
					for (int i = 0; i < listItemData.size(); i++)
					{
						dnNode->value[0].push_back(listItemData[i][0]);
						dsNode->value[0].push_back(listItemData[i][1]);
					}
				}
				addLineSeriesForList();
			}
		}
		else if (propertyData->nodeType == ITEMTYPE)
		{
			ItemNode * itemNode = (ItemNode*)propertyData;
			if (((TypeNode*)(itemNode->parent) )->itemChart==SNCHART)
			{
				snCharNode = static_cast<TypeNode*>(itemNode->parent);
				addLineSeriesForSnChart();
			}
		}
		
	} 
	propertyTableView->update();

	savePath = matCurrentDirPath + "/" + curSelectName + ".xml";
	if ((materialOpState | prevMaterialOpState)&(MATERIAL_CREATE_STATE | MATERIAL_EDIT_STATE) && !(controlMask&0x2))
	{
		saveDataToFile(savePath);
	}
}

void MaterialViewer::sl_databaseMatSelectToCurrent()
{
	addMatFromDatabaseToCurrent();
}

//void MaterialViewer::sl_createMaterial()
//{
//	createMat();
//}

void MaterialViewer::sl_editMaterial()
{
	editMat();
}

void MaterialViewer::sl_addCurrentMatAddDataBase()
{
	addCurrentMatToDatabase();
}

void MaterialViewer::sl_deleteMat()
{
	deleteMatFromCurrent();
	deleteMatFromUserDefine();
}
 
void MaterialViewer::sl_searchLineEditDataChanged(const QString& text )
{
	filterStr = text;
	filter();
}

void MaterialViewer::sl_filterComboboxDataChanged(const QString& text )
{
	filterStrList.clear();
	const std::map<QString, QStringList>& matTypeMap = matTypeConfParser->getTypeMap();
	std::map<QString, QStringList>::const_iterator iter = matTypeMap.find(text);
	if (iter != matTypeMap.end())
	{
		filterStrList = iter->second;
	} 
	filter();
}












