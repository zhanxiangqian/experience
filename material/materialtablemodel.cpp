#include "materialtablemodel.h"

MaterialTableModel::MaterialTableModel() : 
materialOpState(MATERIAL_UNKNOW_STATE)
{
}


MaterialTableModel::~MaterialTableModel()
{
}

int MaterialTableModel::rowCount(const QModelIndex &parent) const
{
	return static_cast<int>(rowTotalNum);
}

int MaterialTableModel::columnCount(const QModelIndex &parent) const
{
	return static_cast<int>(columnTotalNum);
}

QVariant MaterialTableModel::data(const QModelIndex &index, int role ) const
{
	if (Qt::DisplayRole==role&&modelData!=nullptr )
	{ 
		return modelData->at(index.row()).at(index.column()); 
	}
	return QVariant();
}

bool MaterialTableModel::setData(const QModelIndex &index, const QVariant &value, int role )
{
	if (index.isValid()&&Qt::EditRole==role &&modelData!=nullptr )
	{

		if (!value.toString().isEmpty() )
		{
			bool isNum = true;
			double dVal = value.toDouble(&isNum);
			if (isNum==true)
			{
				if ((*modelData)[index.row()][index.column()] != dVal )
				{
					(*modelData)[index.row()][index.column()] = dVal;
					emit tableDataChanged();
				}
			} 
		} 
	}
	return true;
}

Qt::ItemFlags MaterialTableModel::flags(const QModelIndex &index) const
{
	if (!index.isValid()) return 0; 
	Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
	if ((materialOpState&MATERIAL_EDIT_STATE) |(materialOpState&MATERIAL_CREATE_STATE) )
	{
		flags |= Qt::ItemIsEditable;
	} 
	return flags;
}

QVariant MaterialTableModel::headerData(int section, Qt::Orientation orientation,
	int role ) const
{
	if (role==Qt::DisplayRole)
	{
		if (orientation == Qt::Horizontal)
		{
			if (section<horHeaderData.length())
			{
				return horHeaderData[section];
			}
		}

		if (orientation==Qt::Vertical)
		{
			return section + 1;
		}
	}

	if (role == Qt::TextAlignmentRole)
	{
		return Qt::AlignLeft;
	}
	return QVariant();
}