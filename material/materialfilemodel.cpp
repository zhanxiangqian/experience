#include "materialfilemodel.h"
#include <QFont>
ListModel::ListModel(QObject *parent)
	:QAbstractListModel(parent),
	modelData(nullptr )
{
	
}
QVariant ListModel::data(const QModelIndex &index, int role ) const
{
	QVariant res;

	if (role == Qt::DisplayRole)
	{
		if (modelData!=nullptr )
		{
			res = modelData->at(index.row());
		}
	}

	return res;
}

bool ListModel::setData(const QModelIndex &index, const QVariant &value, int role )
{
	if (!index.isValid())
	{
		return false;
	}

	if (Qt::EditRole==role )
	{
		if (modelData!=nullptr&& !value.toString().isEmpty() )
		{
			if ((*modelData)[index.row()] != value.toString())
			{
				(*modelData)[index.row()] = value.toString();
				emit listDataChanged(index);
			}
		}
	} 
	return true;
}

int	ListModel::rowCount(const QModelIndex &parent ) const
{
	int rowCount = 0;
	if (modelData!=nullptr )
	{
		rowCount = modelData->length();
	}
	return rowCount;
}

Qt::ItemFlags ListModel::flags(const QModelIndex &index) const
{
	if (!index.isValid()) return 0;
	return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QVariant ListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	QVariant ret;
	if (role == Qt::DisplayRole)
	{
		ret = "Current Material";
	}else if (role == Qt::TextAlignmentRole)
	{
		return Qt::AlignLeft;
	}/*else if (role == Qt::FontRole)
	{
	QFont ft;
	ft.setWeight(99);
	}*/
	return ret;
}

MaterialFileModel::MaterialFileModel(QString dirPath, QObject *parent)
	:MaterialModel(parent)
{ 
	rootPath = dirPath;
}

MaterialFileModel::MaterialFileModel(QObject *parent)
	:MaterialModel(parent)
{
}

MaterialFileModel::~MaterialFileModel()
{

}

QVariant MaterialFileModel::data(const QModelIndex &index, int role )const
{
	QVariant res;
	res = QFileSystemModel::data(index, role);
	if (role == Qt::DisplayRole)
	{ 
		if (res.toString().contains(".xml"))
		{
			res = res.toString().remove(".xml"); 
		}
	}
	if (role == Qt::DecorationRole)
	{
		return QVariant();
	}
	return res;
 }


int MaterialFileModel::columnCount(const QModelIndex &parent) const
{
	return 1;
}

QVariant MaterialFileModel::headerData(int section, Qt::Orientation orientation, int role ) const
{
	QFileSystemModel::headerData(section, orientation, role);
	QVariant ret;
	if (role == Qt::DisplayRole)
	{
		ret = "Material Database";
	}
	/*else if (role == Qt::FontRole)
	{
		QFont ft;
		ft.setWeight(99);
	}*/
	return ret;
}

