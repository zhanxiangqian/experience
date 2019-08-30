#include <QIcon>

#include "materialpropertytreemodel.h"
#include "imaterialdimensionconvert.h"

MaterialPropertyTreeModel::MaterialPropertyTreeModel(IMaterialDimensionConvert* convert, TreeViewCallBack* callBack) :
materialOpState(MATERIAL_UNKNOW_STATE),
dimensionConvert(convert)
{
	this->callBack = callBack;
}


MaterialPropertyTreeModel::~MaterialPropertyTreeModel()
{

}

QModelIndex MaterialPropertyTreeModel::index(int row, int column,
	const QModelIndex &parent) const
{
	if (!hasIndex(row, column, parent)) return QModelIndex();

	Node* pNode = nullptr;
	if (!parent.isValid())
	{
		pNode = modelData;
	}
	else
	{
		pNode = static_cast<Node*>(parent.internalPointer() ); 
	}

	if (pNode->nodeType==TYPETYPE)
	{
		TypeNode* typeNode = static_cast<TypeNode*>(pNode);
		if (typeNode->itemType == LIST)
		{
			return QModelIndex();
		}
	}

	Node* pChildNode = pNode->childAt(row);
	if (pChildNode != nullptr)
	{
		return createIndex(row, column, pChildNode);
	}
	else{

		return QModelIndex();
	}
}

QModelIndex MaterialPropertyTreeModel::parent(const QModelIndex &child) const
{
	if (!child.isValid())
		return QModelIndex();

	Node *pChildNode= static_cast<Node*>(child.internalPointer());
	Node *pParentNode = pChildNode->parent;
	 
	if (pParentNode == modelData)
		return QModelIndex();
	 
	return createIndex(pParentNode->row(), 0, pParentNode);
}

int MaterialPropertyTreeModel::rowCount(const QModelIndex &parent) const
{
	Node* pParentNode;

	if (!parent.isValid())
		pParentNode = modelData;
	else
		pParentNode = static_cast<Node*>(parent.internalPointer());

	int totalRow =static_cast<int>(pParentNode->childCount());
	if (pParentNode->nodeType==TYPETYPE)
	{
		TypeNode *typeNode = static_cast<TypeNode*>(pParentNode);
		if (typeNode->itemType==LIST)
		{
			totalRow = 0;
		}
	}
	return totalRow;
}

int MaterialPropertyTreeModel::columnCount(const QModelIndex &parent) const
{
// 	return 3;
	return 2;//暂时去掉unit这一列 modified by zxq at 2018/10/16 11:17
}

QVariant MaterialPropertyTreeModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
	{
		return QVariant();
	}

	if (role == Qt::CheckStateRole&&(materialOpState&MATERIAL_CREATE_STATE))
	{
		Node* curNode = static_cast<Node*>(index.internalPointer());

		if (curNode->nodeType==TYPETYPE&&index.column()==0)
		if (curNode != nullptr)
		{
			if (curNode->used==true )
			{
				return Qt::Checked;
			}
			else
			{
				return Qt::Unchecked;
			}
		}
	}

	if (role== Qt::DisplayRole)
	{
		Node* curNode = static_cast<Node*>(index.internalPointer());

		if (curNode != nullptr)
		{
			if (curNode->nodeType!=ITEMTYPE )
			{
				if (index.column()==0)
				{
					return curNode->name;
				} 
			}
			else
			{
				ItemNode * itemNode = (ItemNode*)curNode;
				if (index.column()==0)
				{
					return itemNode->desc;
				}
				else if (index.column()==1)
				{
					if (itemNode->itemType==ELEMENT)
					{
						if (itemNode->value.size()==1)
						{
							if (itemNode->value[0].size()==1 )
							{
								return itemNode->value[0][0];
							}
							else
							{
								return 0;
							}
						}
					}
				}
				else if (index.column()==2)
				{
					return itemNode->unit;
				} 
				
			}
			
		} 
	}
	else if (role == Qt::DecorationRole&&index.column() == 1)
	{
		Node* curNode = static_cast<Node*>(index.internalPointer());

		if (curNode != nullptr)
		{
			if (curNode->nodeType == TYPETYPE)
			{
				TypeNode* typeNode = static_cast<TypeNode*>(curNode);
				if (typeNode->itemType == LIST)
				{
					return QIcon(":/resources/material/zoom.png");
				} 
			}
			else if(curNode->nodeType == ITEMTYPE)
			{
				ItemNode * itemNode = (ItemNode*)curNode;
				if (itemNode->itemType == COMPLEX)
				{
					return QIcon(":/resources/material/zoom.png");
				}
				else if (itemNode->itemType == MATRIX)
				{
					return QIcon(":/resources/material/zoom.png");
				}
			}
		} 
	}
	return QVariant();
}

bool MaterialPropertyTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	Node* curNode = static_cast<Node*>(index.internalPointer()); 
	if (curNode != nullptr) 
	if (role == Qt::CheckStateRole)
	{
		if (value.toInt() == Qt::Checked)
		{
			//setCurNodeUsed(curNode,true );
			if (curNode->nodeType == TYPETYPE)
			{
				curNode->used = true;
				if (((GroupNode*)(curNode->parent))->groupType == SINGLE)
				{
					for (int i = 0; i < curNode->parent->childCount(); i++)
					{
						Node * tempNode = curNode->parent->childAt(i);
						if (curNode != tempNode)
						{
							tempNode->used = false;
						}
					}
				}
			}
		}
		else if (value.toInt() == Qt::Unchecked)
		{
			curNode->used = false;
		}
		emit treeDataChanged();
	}
	else if (role==Qt::EditRole )
	{ 
		if (curNode->nodeType == ITEMTYPE)
		{
			ItemNode *itemNode = (ItemNode*)curNode;
			if (index.column() == 1)
			{ 
				if (itemNode->itemType == ELEMENT)
				{
					bool isNum = true;
					double dVal = value.toDouble(&isNum);
					if (isNum )
					{
						if (itemNode->value.size() == 1)
						{
							if (itemNode->value[0].size() == 1)
							{
								if (itemNode->value[0][0] != dVal)
								{
									itemNode->value[0][0] = dVal;
									emit treeDataChanged();
								}

							}
							else
							{
								itemNode->value[0].clear();
								itemNode->value[0].push_back(dVal);
								emit treeDataChanged();
							}
						}
						else
						{
							itemNode->value.clear();
							std::vector<double> curVal;
							curVal.push_back(dVal);
							itemNode->value.push_back(curVal);
							emit treeDataChanged();
						}
					} 
				}
			}
			else if (index.column() == 2)
			{  
				if (dimensionConvert!=nullptr )
				{
					QString desName = value.toString();
					double ratio = dimensionConvert->calculate(itemNode->unitName, desName, itemNode->unit);
					for (int i = 0; i < itemNode->value.size(); i++)
					{
						for (int j = 0; j < itemNode->value[i].size(); j++)
						{
							itemNode->value[i][j] *= ratio;
						}
					}
					itemNode->unit = desName;
					emit treeDataChanged();
				}
				
			} 
		}
		
	}
	callBack->needUpdate();
	return true;
}

QVariant MaterialPropertyTreeModel::headerData(int section, Qt::Orientation orientation,
	int role) const
{
	if (orientation == Qt::Horizontal&&role==Qt::DisplayRole )
	{
		if (section ==0)
		{
			return "Property";
		}
		else if (section==1)
		{
			return "Value";
		}
		else if (section==2)
		{
			return"Unit";
		}
	}
	return QVariant();
}

Qt::ItemFlags MaterialPropertyTreeModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return 0;
	Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;
	Node* curNode = static_cast<Node*>(index.internalPointer());
	if ((index.column() == 1 || index.column() == 2) && curNode->nodeType == ITEMTYPE && ((materialOpState&MATERIAL_EDIT_STATE) | (materialOpState&MATERIAL_CREATE_STATE)))
	{
		ItemNode * itemNode = (ItemNode*)curNode;
		if (itemNode->itemType ==ELEMENT)
		{
			flags |= Qt::ItemIsEditable;
		} 
	}
	return flags;
}
