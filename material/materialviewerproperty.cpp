#include <QGroupBox>
#include <QMessageBox>
#include <QMenu>
#include <cmath>
#include "materialviewer.h"
#include "materialchartview.h"
#include "materialtreeview.h"
#include "materialmenubar.h"
#include "materialtoolbar.h"
void MaterialViewer::updatePropertyTreeViewByData(const QString& path)
{
	propertyTreeView->setModel(nullptr);
	if (matData != nullptr)
	{ 
		delete matData;
	}
	if (path.contains(matCurrentDirPath))
	{
		matData = new Node(curSelectName+"(CUR)");
	}
	else if (curSelectName == "new")
	{
		matData = new Node(curSelectName);
	}
	else  
	{
		matData = new Node(curDatabaseTreeItemName);
	}
	
	MaterialFileParser parser;
	if (materialOpState&MATERIAL_CREATE_STATE)
	{
		parser.setPath(path, true);
	}
	else
	{
		parser.setPath(path, false);
	}

	parser.readFromFile(matData);

	propertyTreeModel->setModelData(matData, materialOpState);
	propertyTreeViewGroupBox->setTitle(matData->name);
	propertyTreeView->setModel(propertyTreeModel);
	int propertyTreeWidth = propertyTreeView->width();

	propertyTreeView->setColumnWidth(0, propertyTreeWidth*0.4);
	propertyTreeView->setColumnWidth(1, propertyTreeWidth*0.3);
	propertyTreeView->setColumnWidth(2, propertyTreeWidth*0.3);
	propertyTreeView->expandToDepth(0);
}

void MaterialViewer::updatePropertyTreeView()
{ 
	if (matData != nullptr)
	{
		propertyTreeViewGroupBox->setTitle(matData->name);
		propertyTreeView->setModel(nullptr);
		propertyTreeModel->setModelData(matData, materialOpState);
		propertyTreeView->setModel(propertyTreeModel);
		propertyTreeView->update();
		int propertyTreeWidth = propertyTreeView->width();
		propertyTreeView->setColumnWidth(0, propertyTreeWidth*0.4);
		propertyTreeView->setColumnWidth(1, propertyTreeWidth*0.3);
		propertyTreeView->setColumnWidth(2, propertyTreeWidth*0.3);
		propertyTreeView->expandToDepth(0);
	}  
}

void MaterialViewer::updateTableView()
{
	if (propertyData != nullptr)
	{
		propertyTableView->reset();
		propertyTableView->setModel(nullptr);
		
		QStringList horHeaderList;
		if (propertyData->nodeType==ITEMTYPE )
		{
			ItemNode * itemNode = static_cast<ItemNode*>(propertyData);
			propertyTableModel->setModelData(&(itemNode->value), materialOpState);
			propertyTableViewGroupBox->setTitle(itemNode->desc);
			if (itemNode->itemType == ELEMENT)
			{
				horHeaderList << "Value";
				propertyTableModel->setHorHearData(horHeaderList);
			}
			else if (itemNode->itemType == COMPLEX)
			{
				horHeaderList << "Real" << "Inscriber";
				propertyTableModel->setHorHearData(horHeaderList);
			}
			else if (itemNode->itemType==MATRIX)
			{
				if (itemNode->value.size() > 0)
				for (int i = 0; i < itemNode->value[0].size(); i++)
				{
					horHeaderList.append(QString("Value%1").arg(i + 1));
				}
				propertyTableModel->setHorHearData(horHeaderList);
			} 
		}
		else if (propertyData->nodeType==TYPETYPE)
		{
			listItemData.clear();
			TypeNode* typeNode = static_cast<TypeNode*>(propertyData);
			if (typeNode->itemType==LIST )
			{
				for (int i = 0; i < typeNode->childCount(); i++)
				{
					ItemNode * listItemNode = (ItemNode*)typeNode->childAt(i);
					if (listItemNode->vAxis==XVALUE)
					{
						horHeaderList << listItemNode->desc;
						for (int j = 0; j < listItemNode->value[0].size(); j++)
						{
							std::vector<double> pt;
							pt.push_back(listItemNode->value[0][j]);
							listItemData.push_back(pt); 
						} 
						break;
					} 
				}

				for (int i = 0; i < typeNode->childCount(); i++)
				{
					ItemNode * listItemNode = (ItemNode*)typeNode->childAt(i);
					if (listItemNode->vAxis == YVALUE)
					{
						horHeaderList << listItemNode->desc;
						for (int j = 0; j < listItemData.size(); j++ )
						{
							listItemData[j].push_back(listItemNode->value[0][j]);
						} 
						break;
					}
				}
			}
			propertyTableModel->setModelData(&listItemData, materialOpState);
			propertyTableModel->setHorHearData(horHeaderList);
			propertyTableViewGroupBox->setTitle(propertyData->name);
		} 
		propertyTableView->setModel(propertyTableModel);
		propertyTableView->update();
	}
}

void MaterialViewer::resetView()
{
	resetPropertyTreeView();
	resetTableView();
	resetChartView();
}

void MaterialViewer::resetPropertyTreeView()
{
	propertyTreeView->setModel(nullptr);
	propertyTreeViewGroupBox->setTitle("");
}

void MaterialViewer::resetTableView()
{
	propertyTableViewGroupBox->setTitle("");
	propertyTableView->reset();
	propertyTableView->setModel(nullptr); 
}

void MaterialViewer::resetChartView()
{
	chartView->hide();
	chartViewGroupBox->hide();
}

void MaterialViewer::addLineSeriesForList()
{
	if (propertyData != nullptr)
	{ 
		QString seriesName;
		if (matData->name.isEmpty())
		{
			seriesName = propertyData->name;
		}
		else
		{
			seriesName = propertyData->name + "(" + matData->name + ")";
		}
		if ((~controlMask) & 0x01)
		{
			chartView->removeAllSeries();
			MaterialLineSeries *lineSeries = new MaterialLineSeries(seriesName);
			lineSeries->append(listItemData);

			TypeNode* typeNode = static_cast<TypeNode*>(propertyData);
			if (typeNode->itemType == LIST)
			{
				for (int i = 0; i < typeNode->childCount(); i++)
				{
					ItemNode * listItemNode = (ItemNode*)typeNode->childAt(i);
					if (listItemNode->vAxis == XVALUE)
					{ 
						chartView->setXLabel(listItemNode->desc); 
					}
					else if (listItemNode->vAxis == YVALUE)
					{ 
						chartView->setYLabel(listItemNode->desc);
					}
				}
			}

			
			chartView->addSeries(lineSeries); 
		}
		else
		{
			QList<QAbstractSeries *> serieses = chartView->series();
			for (int i = 0; i < serieses.length(); i++)
			{
				if (serieses[i]->name() == seriesName)
				{
					chartView->removeSeries(serieses[i]);
					break;
				}
			}
			MaterialLineSeries *lineSeries = new MaterialLineSeries(seriesName);
			lineSeries->append(listItemData);
			chartView->addSeries(lineSeries);
		}
	}
}

void MaterialViewer::addLineSeriesForSnChart()
{
	if (snCharNode!=nullptr )
	{
		double dsMax = 0.0f;
		double dsMin = 0.0f;
		double m1 = 0.0f;
		double m2 = 0.0f;
		double kps = 0.0f;
		double kpn = 0.0f;
		for (int i = 0; i < snCharNode->childCount();i++)
		{
			ItemNode *itemNode = static_cast<ItemNode*>(snCharNode->childAt(i));
			if (itemNode!=nullptr)
			{
				if (itemNode->vAxis==YMAX)
				{
					dsMax = itemNode->value[0][0];
				}
				else if (itemNode->vAxis == YMIN)
				{
					dsMin = itemNode->value[0][0];
				}
				else if (itemNode->vAxis == XFIRST)
				{
					m1 = itemNode->value[0][0];
				}
				else if (itemNode->vAxis == XSECOND)
				{
					m2 = itemNode->value[0][0];
				}
				else if (itemNode->vAxis == YMID)
				{
					kps = itemNode->value[0][0];
				}
				else if (itemNode->vAxis == XMID)
				{
					kpn = itemNode->value[0][0];
				}
			}
		}

		double dn1 = pow(kps, m1)*kpn / (pow(dsMax, m1));
		double dn2 = pow(kps, m2)*kpn / (pow(dsMin, m2));

		/*double dn1 = kpn / pow(10, (dsMax - kps) / m1);
		double dn2 = kpn / pow(10, (dsMin - kps) / m2);*/

		QPointF pt1(log10(dn1), log10(dsMax));
		QPointF pt2(log10(kpn), log10(kps));
		QPointF pt3(log10(dn2), log10(dsMin));

		QString seriesName;
		if (matData->name.isEmpty())
		{
			seriesName = snCharNode->name;
		}
		else
		{
			seriesName = snCharNode->name + "(" + matData->name + ")";
		}

		chartView->removeAllSeries();
		MaterialLineSeries *lineSeries = new MaterialLineSeries(seriesName);
		lineSeries->append(pt1);
		lineSeries->append(pt2);
		lineSeries->append(pt3);

		double xSpace =pt2.x()-pt1.x();
		double ySpace =pt2.y()-pt1.y();
		LineMarkerInfo m1Info(QPointF(pt1.x() + xSpace*0.3, pt1.y() + ySpace*0.3 ),
			QPointF(pt1.x() + xSpace*0.7, pt1.y() + ySpace*0.7),
			QString("m1=%1").arg(m1)); 

		xSpace = pt3.x() - pt2.x();
		ySpace = pt3.y() - pt2.y();
		LineMarkerInfo m2Info(QPointF(pt2.x() + xSpace*0.3, pt2.y() + ySpace*0.3),
			QPointF(pt2.x() + xSpace*0.7, pt2.y() + ySpace*0.7),
			QString("m2=%1").arg(m2));
		lineSeries->appendMarkerInfo(m1Info);
		lineSeries->appendMarkerInfo(m2Info);
		chartView->setXLabel(snCharNode->xName);
		chartView->setYLabel(snCharNode->yName);
		chartView->addSeries(lineSeries); 
	}
}

void MaterialViewer::sl_propertyTreeViewItemDoubleClicked(QMouseEvent *event)
{
	QModelIndex index = propertyTreeView->indexAt(event->pos()); 
	Node* curNode = static_cast<Node*>(index.internalPointer());
	if (curNode != nullptr)
	{
		if (ITEMTYPE == curNode->nodeType)
		{
			ItemNode* itemNode = (ItemNode*)curNode;
			propertyData = itemNode;
			updateTableView(); 
			if (((TypeNode*)(itemNode->parent))->itemChart != SNCHART)
			{
				chartView->hide();
				chartViewGroupBox->hide();
			}
		}
		else if (TYPETYPE == curNode->nodeType)
		{
			TypeNode* typeNode = (TypeNode*)curNode;
			if (typeNode->itemType==LIST)
			{
				propertyData = typeNode;
				updateTableView();
				chartView->show();
				chartViewGroupBox->show();
				addLineSeriesForList();
			}
			else if (typeNode->itemChart==SNCHART )
			{
				snCharNode = typeNode;
				addLineSeriesForSnChart();
				chartViewGroupBox->show();
				chartView->show();
			}
		}
	}
	
}

void MaterialViewer::sl_propertyTableRightClicked(QMouseEvent *event)
{
	if (propertyData != nullptr)
	{
		ItemType itemType =UNKNOWITEMTYPE;
		if (propertyData->nodeType==TYPETYPE)
		{
			TypeNode* typeNode = static_cast<TypeNode*>(propertyData);
			if (typeNode->itemType==LIST )
			{
				itemType = LIST;
			}
		}
		else if (propertyData->nodeType == ITEMTYPE)
		{
			ItemNode *itemNode = static_cast<ItemNode*>(propertyData);
			itemType = itemNode->itemType;
		}

		if (itemType == COMPLEX || itemType == ELEMENT)
		{
			return;
		}

		if (materialOpState&(MATERIAL_CREATE_STATE | MATERIAL_EDIT_STATE))
		{
			QModelIndex index = propertyTableView->indexAt(event->pos());
			QPoint p = mapToParent(event->pos());
			p.setX(p.x() + currentMatTree->width());
			p.setY(p.y() + propertyTreeView->height() + menuBar->height() + toolBar->height());
			QMenu *menu = new QMenu(this);
			if (index.isValid())
			{
				 menu->addAction(propertyTableAddRowBeforeAct);
				 menu->addAction(propertyTableAddRowAfterAct);  

				if (itemType == MATRIX)
				{
					menu->addAction(propertyTableAddColumnBeforeAct);
					menu->addAction(propertyTableAddColumnAfterAct);
				}

				menu->addAction(propertyTableDeleteRowAct);
				if (itemType == MATRIX)
				{
					menu->addAction(propertyTableDeleteColumnAct);
					
				}
				
				menu->addAction(propertyTableCopyDataAct);
				menu->addAction(propertyTablePasteDataAct);
				if (itemType == MATRIX)
				{
					menu->addAction(propertyTableImportDataAct);
				} 
			}
			else
			{
				if (itemType == MATRIX)
				{
					menu->addAction(propertyTableImportDataAct);
				}
				p.setY(p.y() + 40);
			}
			menu->exec(p);
		}
	}
}

void MaterialViewer::sl_addRowBefore()
{
	propertyTableView->addRowBefore();
	sl_currentMatDataChanged();
	updateTableView();
	
}

void MaterialViewer::sl_addRowAfter()
{
	propertyTableView->addRowAfter();
	sl_currentMatDataChanged();
	updateTableView(); 
}

void MaterialViewer::sl_addColumnBefore()
{
	propertyTableView->addColumnBefore();
	sl_currentMatDataChanged();
	updateTableView();
 }

void MaterialViewer::sl_addColumnAfter()
{
	propertyTableView->addColumnAfter();
	sl_currentMatDataChanged();
	updateTableView();
}

void MaterialViewer::sl_deleteRow()
{
	propertyTableView->deleteRow();
	sl_currentMatDataChanged();
	updateTableView(); 
}

void MaterialViewer::sl_copyData()
{
	propertyTableView->copyData();
}

void MaterialViewer::sl_deleteColumn()
{
	propertyTableView->deleteColumn();
	sl_currentMatDataChanged();
	updateTableView(); 
}


void MaterialViewer::sl_pasteData()
{
	propertyTableView->pasteData();
	sl_currentMatDataChanged();
	updateTableView();
}

void MaterialViewer::sl_importData()
{
	propertyTableView->importData();
	sl_currentMatDataChanged();
	updateTableView();
}