#ifndef _MATERIALDATADEF_H
#define _MATERIALDATADEF_H

#include <QString>
#include <vector>
#include <memory>
#include <QHash>
#include <cstdio>

enum ItemType
{
	UNKNOWITEMTYPE =0,
	ELEMENT,
	COMPLEX,
	LIST,
	MATRIX
};

enum GroupType
{
	UNKNOWGROUPTYPE = 0,
	MULTI,
	SINGLE
};

enum NodeType
{
	GROUPTYPE,
	TYPETYPE,
	ITEMTYPE
};

enum ItemChart
{
	UNKNOWITEMCHART =0,
	SNCHART,
	LISTCHART
};

enum ValueAxis
{
	UNKNOWVALUE = 0x00,
	XVALUE,
	YVALUE,
	YMIN,
	YMAX,
	XFIRST,
	XSECOND,
	XMID,
	YMID
};

struct Node
{
	Node() :parent(nullptr)
	{
		used = true;
	}

	Node(const QString& name ) :parent(nullptr)
	{
		this->name = name;
		used = true;
	}

	virtual ~Node()
	{
		std::vector<Node*>::iterator iter = children.begin();
		while (iter != children.end())
		{
			if (*iter != nullptr)
			{
				delete *iter;
			}
			iter++;
		}
	}

	inline void addChild(Node* child)
	{
		children.push_back(child);
	}

	inline void removeChild(Node *child)
	{
		std::vector<Node*>::iterator iter = children.begin();
		while (iter!=children.end() )
		{
			if (*iter == child)
			{
				children.erase(iter);
				delete child;
			}
			iter++;
		}
	}

	inline void removeChildren()
	{
		std::vector<Node*>::iterator iter = children.begin();
		while (iter != children.end())
		{
			if (*iter != nullptr)
			{
				delete *iter;
			}
			iter++;
		}
		children.clear();
	}

	inline int row()
	{
		if (parent!=nullptr )
		{
			for (int i = 0; i < parent->children.size(); i++ )
			{
				if (parent->children[i]==this )
				{
					return i;
				}
			}
		}
		return 0;
	}

	inline Node* childAt(int index) const
	{
		if (index<childCount() )
		{
			return children[index];
		}
		return nullptr;
	}

	inline size_t childCount() const
	{
		return children.size();
	}

	inline int childUsedCount()
	{
		int usedCount = 0;
		for (int i = 0; i < children.size(); i++ )
		{
			if (children[i]->used)
			{
				usedCount++;
			}
		}
		return usedCount;
	}

	NodeType nodeType;
	QString name;
	QString templateId;
	Node *parent;
	std::vector<Node*> children;
	bool used;
	bool modify;
};

struct GroupNode :public Node
{ 
	GroupNode()
	{
		nodeType = GROUPTYPE;
		groupType = UNKNOWGROUPTYPE;
	}

	GroupType groupType; 
};

struct TypeNode :public Node
{
	TypeNode()
	{
		nodeType = TYPETYPE;
		itemType = UNKNOWITEMTYPE;
		itemChart = UNKNOWITEMCHART;
	}
	ItemType itemType;
	ItemChart itemChart;
	QString xName;
	QString yName;
};

struct ItemNode :public Node
{ 
	ItemNode()
	{
		nodeType = ITEMTYPE;
		itemType = UNKNOWITEMTYPE;
		itemChart = UNKNOWITEMCHART;
		vAxis = UNKNOWVALUE;
	}

	inline QString toStrValue()
	{
		QString valueStr;
		char buffer[255];
		memset(buffer, 0, 255);
		for (int i = 0; i < value.size(); i++)
		{
			for (int j = 0; j < value.at(i).size(); j++)
			{
				sprintf_s(buffer, "%.15g", value[i][j]);
				valueStr.append( QString(buffer));
				valueStr.append( ",");
			}
			valueStr.remove(valueStr.length() - 1, 1);
			valueStr.append(";");
		}
		valueStr.remove(valueStr.length() - 1, 1);
		return valueStr;
	}
	ItemType itemType;
	ItemChart itemChart;
	ValueAxis vAxis;
	QString desc; //ÃèÊö
	QString unit; //Á¿¸Ù
	QString unitName;
	std::vector<std::vector<double> > value;
};
 
inline void setCurNodeUsed(Node* curNode, bool used)
{
	curNode->used = used;
	for (int i = 0; i < curNode->childCount(); i++)
	{
		setCurNodeUsed(curNode->childAt(i), used);
	}
}

inline void combineNode(Node* des, Node* src)
{
	if (des==nullptr||src==nullptr)
	{
		return;
	} 

	for (int i = 0; i < des->childCount(); i++)
	{
		Node * desGroupNode = des->childAt(i);
		for (int j = 0; j < desGroupNode->childCount(); j++)
		{
			Node * desTypeNode = desGroupNode->childAt(j);
			desTypeNode->used = false;
		}
	} 

	for (int i = 0; i <src->childCount(); i++)
	{
		for (int ii = 0; ii < des->childCount(); ii++)
		{
			if (des->childAt(ii)->name == src->childAt(i)->name)
			{
				Node * desGroupNode = des->childAt(ii);
				Node * srcGroupNode = src->childAt(i); 
				for (int j = 0; j <srcGroupNode->childCount(); j++)
				{
					for (int jj = 0; jj<desGroupNode->childCount();jj++)
					{
						if (desGroupNode->childAt(jj)->name == srcGroupNode->childAt(j)->name)
						{
							Node * desTypeNode = desGroupNode->childAt(jj);
							Node * srcTypeNode = srcGroupNode->childAt(j);
							size_t len = desTypeNode->childCount();
							if (len != srcTypeNode->childCount())
							{
								return;
							}
							desTypeNode->used = srcTypeNode->used;
							for (int k = 0; k < len; k++)
							{
								desTypeNode->childAt(k)->parent = srcTypeNode;
								srcTypeNode->childAt(k)->parent = desTypeNode;
								Node* tempNode = desTypeNode->childAt(k);
								desTypeNode->children[k] = srcTypeNode->children[k];
								srcTypeNode->children[k] = tempNode; 
							} 
						}
					} 
				}
			} 
		} 
	}
}
#endif //_MATERIALDATADEF_H