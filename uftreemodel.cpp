#include "UfTreeModel.h"

UfTreeModel::UfTreeModel(UfTreeObject* root, QObject *parent) : QAbstractItemModel(parent)
{
	setRoot(root);
}

UfTreeModel::~UfTreeModel()
{

}

int UfTreeModel::columnCount(const QModelIndex &parent/* = QModelIndex()*/) const
{
	return 2;
}

int UfTreeModel::rowCount(const QModelIndex &parent/* = QModelIndex()*/) const
{
	if (parent.column() > 0)
		return 0;

	return getTreeObject(parent)->childCount();
}

QVariant UfTreeModel::data(const QModelIndex &index, int role/* = Qt::DisplayRole*/) const
{
	if (!index.isValid())
		return QVariant();

	if (role != Qt::DisplayRole)
		return QVariant();

	UfTreeObject *item = static_cast<UfTreeObject*>(index.internalPointer());

	return item->text(index.column());
/*
	if (!index.isValid())
	{
		return QVariant();
	}
	QVariant res;
	int column = index.column();
	UfTreeObject* obj = getTreeObject(index);
	switch (role)
	{
	case Qt::DisplayRole:
	case Qt::EditRole:
	{
		res = obj->text(column);
		break;
	}
	case Qt::ToolTipRole:
	{
		res = obj->toolTip(column);
		break;
	}
	case Qt::StatusTipRole:
	{
							  res = obj->statusTip(column);
							  break;
	}
	case Qt::DecorationRole:
	{
							   res = obj->icon(column);
							   break;
	}
	default:
		break;
	}
	return res;
*/
}

QModelIndex UfTreeModel::index(int row, int column, const QModelIndex &parent/* = QModelIndex()*/) const
{
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	UfTreeObject* parentObj = getTreeObject(parent);
	if (parentObj)
	{
		UfTreeObject* childObj = parentObj->childObject(row);
		if (childObj)
		{
			return createIndex(row, column, childObj);
		}
		else
		{
			return QModelIndex();
		}
	}
	else
	{
		return QModelIndex();
	}
}

QVariant UfTreeModel::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole) 
	{
		if (section == 0) 
		{
			return tr("Name");
		}
		else if (section == 1) 
		{
			return tr("Value");
		}
	}
	return QVariant();
}

QModelIndex UfTreeModel::parent(const QModelIndex &index) const
{
	if (!index.isValid())
	{
		return QModelIndex();
	}
	UfTreeObject* childObj = getTreeObject(index);
	UfTreeObject* parentObj = childObj->parent();
	if (parentObj && parentObj != rootObject)
	{
		return createIndex(parentObj->position(), 0, parentObj);
	}
	return QModelIndex();
}

void UfTreeModel::setRoot(UfTreeObject* root)
{
	rootObject = root;
}

UfTreeObject* UfTreeModel::getTreeObject(QModelIndex index) const
{
	return index.isValid() ? static_cast<UfTreeObject*>(index.internalPointer()) : rootObject;
}
