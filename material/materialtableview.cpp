#include "materialtableview.h"
#include "materialclipboardmanager.h"
#include "materialtablemodel.h"
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QtWidgets/QApplication> 
#include <QMouseEvent>
#include <QFileDialog>


MaterialClipboardManager* MaterialTableView::clipboardMgr = nullptr; 
MaterialTableView::MaterialTableView(QWidget *parent)
:QTableView(parent)
{ 
	if (clipboardMgr == nullptr)
	{
		clipboardMgr = new MaterialClipboardManager(QApplication::clipboard());
	}
}

void MaterialTableView::mousePressEvent(QMouseEvent *event)
{ 
	QTableView::mousePressEvent(event);
	if (event->button() == Qt::LeftButton)
	{
		emit leftButtonClicked(event);
	}
	else if (event->button() == Qt::RightButton)
	{
		emit rightButtonClicked(event);
	}
}

void MaterialTableView::mouseDoubleClickEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		emit leftButtonDoubleClicked(event);
	}
	QTableView::mouseDoubleClickEvent(event);
}

void MaterialTableView::setModel(MaterialTableModel *model)
{
	QTableView::setModel(model );
	if (model==nullptr )
	{
		modelData = nullptr;
	}
	else
	{
		modelData = model->modelData;
	} 
}

void MaterialTableView::addRowBefore()
{
	SelectInfo info = currentSelectRow();
	if (info.selects.length()==1)
	{
		insertRow(info.selects[0], true);
	} 
}

void MaterialTableView::addRowAfter()
{
	SelectInfo info = currentSelectRow();
	if (info.selects.length() == 1)
	{
		insertRow(info.selects[0], false);
	} 
}

void MaterialTableView::addColumnBefore()
{
	SelectInfo info = currentSelectColumn();
	if (info.selects.length() == 1)
	{
		insertColumn(info.selects[0], true);
	} 
}

void MaterialTableView::addColumnAfter()
{
	SelectInfo info = currentSelectColumn();
	if (info.selects.length() == 1)
	{
		insertColumn(info.selects[0], false);
	}
}

void MaterialTableView::deleteRow()
{
	SelectInfo info = currentSelectRow();
	if (info.selects.length() == (info.max - info.min + 1))
	{
		for (int i = info.max; i >= info.min; i--)
		{
			deleteRow(i);
		} 
	}
}

void MaterialTableView::deleteColumn()
{
	SelectInfo info = currentSelectColumn();
	if (info.selects.length() == (info.max - info.min + 1))
	{
		for (int i = info.max; i >= info.min; i--)
		{
			deleteColumn(i);
		} 
	}
}

void MaterialTableView::copyData()
{
	copyDataFromTable();
}

 

void MaterialTableView::pasteData()
{
	SelectInfo info = currentSelectRow();
	if (info.selects.length()==1)
	{
		pasteDataToTable(info.selects[0]);
	} 
}

void MaterialTableView::importData()
{
	QString openPath = QFileDialog::getOpenFileName(nullptr, tr("Export"), QString(), tr("*"));
	if (!openPath.isEmpty())
	{
		importData(openPath);
	} 
}

void MaterialTableView::insertRow(int row, bool flag)
{
	if (flag == false)
	{
		row++;
	}
	if (modelData != nullptr)
	{
		size_t rowSize = modelData->size();
		if (rowSize == 0)
		{
			return;
		}
		size_t colSize = (*modelData)[0].size();
		if (colSize == 0)
		{
			return;
		}
		std::vector<double> insertRowVec;
		insertRowVec.assign(colSize, 0.0f);
		if (row > rowSize)
		{
			modelData->push_back(insertRowVec);
		}
		else
		{
			std::vector<std::vector<double>>::iterator iter = modelData->begin();
			iter = iter + row;
			modelData->insert(iter, insertRowVec);
		}
	}
}

void MaterialTableView::insertColumn(int column, bool flag)
{
	if (flag == false)
	{
		column++;
	}

	if (modelData != nullptr)
	{
		size_t rowSize = modelData->size();
		if (rowSize == 0)
		{
			return;
		}
		size_t colSize = (*modelData)[0].size();
		if (colSize == 0)
		{
			return;
		}

		if (column > colSize)
		{
			for (int i = 0; i < rowSize; i++)
			{
				(*modelData)[i].push_back(0);
			}
		}
		else
		{
			for (int i = 0; i < rowSize; i++)
			{
				std::vector<double>::iterator iter = (*modelData)[i].begin();
				iter = iter + column;
				(*modelData)[i].insert(iter, 0.0f);
			}
		}
	}
}

void MaterialTableView::deleteRow(int row)
{
	if (modelData != nullptr)
	{
		size_t rowSize = modelData->size();
		if (row<0 || row>rowSize || rowSize <= 1)
		{
			return;
		}
		std::vector<std::vector<double>>::iterator iter = modelData->begin();
		iter = iter + row;
		modelData->erase(iter);
	}
}

void MaterialTableView::deleteColumn(int column)
{
	if (modelData != nullptr)
	{
		size_t rowSize = modelData->size();
		if (rowSize == 0)
		{
			return;
		}
		size_t colSize = (*modelData)[0].size();
		if (column<0 || column>colSize || colSize <= 2)
		{
			return;
		}
		for (int i = 0; i < rowSize; i++)
		{
			std::vector<double>::iterator iter = (*modelData)[i].begin();
			iter = iter + column;
			(*modelData)[i].erase(iter);
		}
	}
}

void MaterialTableView::copyDataFromTable()
{
	QItemSelectionModel* selectionModel = this->selectionModel();
	if (selectionModel != nullptr)
	{
		if (selectionModel->hasSelection())
		{
			QModelIndexList indexList = selectionModel->selectedIndexes();
			int minRow = 0;
			int maxRow = 0;
			int minCol = 0;
			int maxCol = 0;
			int len = indexList.length();
			if (len > 0)
			{
				QModelIndex index = indexList[0];
				minRow = index.row();
				maxRow = minRow;
				minCol = index.column();
				maxCol = minCol;
			}
			for (int i = 0; i < indexList.length(); i++)
			{
				int row = indexList[i].row();
				int col = indexList[i].column();
				minRow = minRow>row ? row : minRow;
				maxRow = maxRow<row ? row : maxRow;
				minCol = minCol>col ? col : minCol;
				maxCol = maxCol < col ? col : maxCol;
			}
			std::vector<std::vector<double>> table;
			if (maxRow < modelData->size())
			{
				if ((!modelData->empty()) && (maxCol < modelData->at(0).size()))
				{
					for (int i = minRow; i <= maxRow; i++)
					{
						std::vector<double> row;
						for (int j = minCol; j <= maxCol; j++)
						{
							row.push_back((*modelData)[i][j]);
						}
						table.push_back(row);
					}
					clipboardMgr->setTable(table);
				}
			}

		}
	}
}

void MaterialTableView::pasteDataToTable(int row )
{
	if (clipboardMgr != nullptr && modelData != nullptr)
	{
		std::vector<std::vector<double>> table;
		clipboardMgr->getTable(table);
		if (modelData != nullptr)
		{
			if (table.size() > 0 && modelData->size() > 0)
			{
				if (table[0].size() == modelData->at(0).size())
				{
					size_t tableSize = table.size();
					size_t dataSize = modelData->size();
					modelData->resize(tableSize + dataSize);
					for (int i = 0; i<dataSize-row; i++)
					{
						(*modelData)[tableSize+dataSize-1-i] = (*modelData)[dataSize-1-i];
					} 
					for (int i = 0; i < tableSize; i++ )
					{
						(*modelData)[row+1+i] = table[i];
					}
				}
				else
				{
					QMessageBox::warning(nullptr, "Import data", "The column of property table is not equal the original data");
				}
			}
		}
	} 
}

void  MaterialTableView::importData(const QString& path)
{
	std::vector<std::vector<double>> table;
	QIODevice* ioDev = new QFile(path);
	if (ioDev->open(QIODevice::ReadOnly))
	{
		QTextStream textStream(ioDev);
		QString text = textStream.readAll();
		if (text.length() >= 1000)
		{
			return;
		}
		QStringList rowStrList = text.split("\n");
		for (int i = 0; i < rowStrList.length(); i++)
		{
			QString rowStr = rowStrList[i];
			std::vector<double> rows;
			QStringList colmnStrList = rowStr.split(",");
			for (int j = 0; j < colmnStrList.length(); j++)
			{
				double val = colmnStrList.at(j).toDouble();
				rows.push_back(val);
			}
			table.push_back(rows);
		}
	}

	if (modelData != nullptr)
	{
		if (table.size()>0 && modelData->size() > 0)
		{
			if (table[0].size() == modelData->at(0).size())
			{
				for (int i = 0; i < table.size(); i++)
				{
					modelData->push_back(table[i]);
				}
			}
			else
			{
				QMessageBox::warning(nullptr, "Import data", "The column of property table is not equal the original data");
			}
		}
	}
}

SelectInfo MaterialTableView::currentSelectRow()
{
	SelectInfo selectInfo;
	QItemSelectionModel *selectModel = this->selectionModel();
	if (selectModel != nullptr)
	{
		QModelIndexList selectIndexList = selectModel->selectedIndexes();
		if (selectIndexList.length() != 0)
		{
			selectInfo.min = selectInfo.max = selectIndexList[0].row();
			for (int i = 0; i < selectIndexList.length(); i++)
			{
				int curRow = selectIndexList[i].row();
				if (selectInfo.min>curRow)
				{
					selectInfo.min = curRow;
				}
				if (selectInfo.max < curRow)
				{
					selectInfo.max = curRow;
				}
				if (-1 == selectInfo.selects.indexOf(curRow))
				{
					selectInfo.selects.append(curRow);
				}
			}
		}
	}
	return selectInfo;
}

SelectInfo MaterialTableView::currentSelectColumn()
{
	SelectInfo selectInfo;
	QItemSelectionModel *selectModel = this->selectionModel();
	if (selectModel != nullptr)
	{
		QModelIndexList selectIndexList = selectModel->selectedIndexes();
		if (selectIndexList.length() != 0)
		{
			selectInfo.min = selectInfo.max = selectIndexList[0].column();
			for (int i = 0; i < selectIndexList.length(); i++)
			{
				int curColumn = selectIndexList[i].column();
				if (selectInfo.min>curColumn)
				{
					selectInfo.min = curColumn;
				}
				if (selectInfo.max < curColumn)
				{
					selectInfo.max = curColumn;
				}
				if (-1 == selectInfo.selects.indexOf(curColumn))
				{
					selectInfo.selects.append(curColumn);
				}
			} 
		}
	}
	return selectInfo;
}