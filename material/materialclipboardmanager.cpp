#include "materialclipboardmanager.h"
#include <QtWidgets/QApplication>
#include <QClipboard>
#include <QString>
#include <QString>
MaterialClipboardManager::MaterialClipboardManager(QClipboard* clipBoard)
{
	this->clipBoard = clipBoard;
}

MaterialClipboardManager::~MaterialClipboardManager()
{

}

void MaterialClipboardManager::getTable(std::vector<std::vector<double>>& oTable)
{
	parseTable();
	oTable = table;
}

void MaterialClipboardManager::setTable(const std::vector<std::vector<double>>& iTable)
{
	table = iTable;
	saveTable();
}
 
void MaterialClipboardManager::parseTable()
{
	if (clipBoard != nullptr)
	{
		table.clear();
		bool isDouble = true;
		QString text = clipBoard->text(QClipboard::Clipboard);
		text.remove(text.length()-1, 1);
		QStringList rows = text.split("\n");
		for (int i = 0; i < rows.length(); i++)
		{
			QStringList row = rows[i].split("\t");
			std::vector<double> dRow;
			for (int j = 0; j < row.length(); j++)
			{
				QString value = row[j];
				double dValue = value.toDouble(&isDouble);
				if (!isDouble)
				{
					table.clear();
					return;
				}
				dRow.push_back(dValue);
			}
			table.push_back(dRow);
		}

		if (!table.empty())
		{
			size_t columnLen = table[0].size();
			for (int i = 0; i < table.size(); i++)
			{
				if (columnLen != table[i].size())
				{
					table.clear();
					return;
				}
			}
		}

	}
}

void MaterialClipboardManager::saveTable()
{
	QString valueStr;
	for (int i = 0; i < table.size(); i++)
	{
		for (int j = 0; j < table.at(i).size(); j++)
		{
			valueStr.append(QString("%1").arg(table[i][j]));
			valueStr.append("\t");
		}
		valueStr.remove(valueStr.length() - 1, 1);
		valueStr.append("\n");
	}
	clipBoard->setText(valueStr);
}

