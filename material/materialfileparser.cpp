#include "materialfileparser.h"
#include <QFileInfo>
#include <QDomDocument>
#include <QTextStream>

 
MaterialFileParser::MaterialFileParser(const QString & path )
:filePath(path )
{

}

MaterialFileParser::~MaterialFileParser()
{

}

void MaterialFileParser::setPath(const QString & path, bool bTemplate)
{
	this->bTemplate = bTemplate;
	filePath = path;
}

unsigned char MaterialFileParser::readFromFile(Node* root)
{ 
	QFileInfo fi(filePath);
	if (fi.isDir())
	{
		return 1; //file is dir
	}
	else if (fi.isFile())
	{
		QDomDocument doc("xmlDoc");
		QFile file(filePath);
		if (!file.open(QIODevice::ReadOnly))
			return 2;// open file failed
		if (!doc.setContent(&file)) {
			file.close();
			return 3; //QDomDocument set file failed
		}
		file.close();
		QDomElement& xml = doc.documentElement();
		/*if (xml.hasAttribute(ATT_NAME_STR ) )
		{
			root->name = xml.attribute(ATT_NAME_STR);

		}*/
		if (xml.hasAttribute(ATT_TID_STR ) )
		{
			root->templateId = xml.attribute(ATT_TID_STR);
		}
		readGroups(xml, root);
	}
	return 0;
}

unsigned char MaterialFileParser::readGroups(QDomElement& xml, Node* root)
{
	//group
	QDomNodeList items = xml.elementsByTagName("group");
	int cnt = items.count();
	for (int i = 0; i < cnt; i++)
	{
		QDomNode n = items.at(i);
		if (n.isElement())
		{
			QDomElement e = n.toElement();
			GroupNode* group =new GroupNode;
			group->name = e.attribute("name"); 
			QString typeStr = e.attribute("type");
			if (typeStr==VALUE_MULTI_STR)
			{
				group->groupType = MULTI;
			}
			else if (typeStr == VALUE_SINGLE_STR)
			{
				group->groupType = SINGLE;
			} 
			readTypes(e, group );
			group->parent = root;
			root->addChild(group);
		}
	}
	return 0;
}

unsigned char MaterialFileParser::readTypes(QDomElement& group, Node* groupNode )
{
	if (group.isElement())
	{
		QDomNode typeNode = group.firstChild();//type item

		while (!typeNode.isNull())
		{
			if (typeNode.isElement())
			{
				TypeNode* tp =new TypeNode;
				QDomElement typeElement = typeNode.toElement();
				tp->name = typeElement.attribute("name");
				QString typeStr = typeElement.attribute("type"); 
				if (typeStr == VALUE_ELEMENT_STR)
				{
					tp->itemType = ELEMENT;
				}
				else if (typeStr == VALUE_LIST_STR)
				{
					tp->itemType = LIST;
				}
				else if (typeStr==VALUE_COMPLEX_STR)
				{
					tp->itemType = COMPLEX;
				}
				else if (typeStr == VALUE_MATRIX_STR)
				{
					tp->itemType = MATRIX;
				}

				if (typeElement.hasAttribute("chart"))
				{
					QString chartStr = typeElement.attribute("chart");
					if (chartStr==VALUE_SNCHART_STR )
					{
						tp->itemChart = SNCHART;
					}
					else if (chartStr==VALUE_LISTCHART_STR )
					{
						tp->itemChart = LISTCHART;
					}
				}

				if (typeElement.hasAttribute(ATT_XNAME_STR))
				{
					tp->xName = typeElement.attribute(ATT_XNAME_STR); 
				}

				if (typeElement.hasAttribute(ATT_YNAME_STR))
				{
					tp->yName = typeElement.attribute(ATT_YNAME_STR);
				}
				
				readItems(typeElement, tp );
				tp->parent = groupNode;
				if (bTemplate)
				{
					tp->used = false;
				}
				groupNode->addChild(tp); 
				typeNode = typeNode.nextSibling();
			}
		}
	}
	return 0;
}

unsigned char MaterialFileParser::readItems(QDomElement& type, Node* typeNode )
{
	if (type.isElement())
	{
		QDomNode itemNode = type.firstChild();//type item

		while (!itemNode.isNull())
		{
			if (itemNode.isElement())
			{ 
				QDomNode itemText =itemNode.firstChild();
				ItemNode * pItem = new ItemNode;
				QDomElement itemElement = itemNode.toElement();
				if (itemElement.hasAttribute(ATT_NAME_STR))
				{
					pItem->name = itemElement.attribute(ATT_NAME_STR);
				}

				if (itemElement.hasAttribute(ATT_UNITNAME_STR))
				{
					pItem->unitName = itemElement.attribute(ATT_UNITNAME_STR);
				}

				if (itemElement.hasAttribute(ATT_VALUEAXIS_STR))
				{
					QString valueAxisStr = itemElement.attribute(ATT_VALUEAXIS_STR);
					if (valueAxisStr==VALUE_XVALUE_STR)
					{
						pItem->vAxis = XVALUE;
					}
					else if (valueAxisStr == VALUE_YVALUE_STR)
					{
						pItem->vAxis = YVALUE;
					}
					else if (valueAxisStr == VALUE_YMIN_STR)
					{
						pItem->vAxis = YMIN;
					}
					else if (valueAxisStr == VALUE_YMAX_STR)
					{
						pItem->vAxis = YMAX;
					}
					else if (valueAxisStr == VALUE_XFIRST_STR)
					{
						pItem->vAxis = XFIRST;
					}
					else if (valueAxisStr == VALUE_XSECOND_STR)
					{
						pItem->vAxis = XSECOND;
					}
					else if (valueAxisStr == VALUE_XMID_STR)
					{
						pItem->vAxis = XMID;
					}
					else if (valueAxisStr == VALUE_YMID_STR)
					{
						pItem->vAxis = YMID;
					}
				}

				pItem->itemType = ((TypeNode*)typeNode)->itemType;
				pItem->itemChart = ((TypeNode*)typeNode)->itemChart;
				while (!itemText.isNull() )
				{
					QDomElement itemTextElement = itemText.toElement();
					if (itemTextElement.tagName() == "desc" )
					{
						pItem->desc = itemTextElement.text();
					}
					else if (itemTextElement.tagName() == "unit" )
					{
						pItem->unit = itemTextElement.text();
					}
					else if (itemTextElement.tagName() == "value")
					{
						QString value = itemTextElement.text(); 
						QStringList rowList = value.split(";");
						int rowLen = rowList.length();
						for (int i = 0; i < rowLen; i++ )
						{
							QString columnStr = rowList[i];
							QStringList columnList = columnStr.split(",");
							int columnLen = columnList.length();
							std::vector<double> row;
							for (int j = 0; j < columnLen; j++ )
							{
								row.push_back(columnList[j].toDouble());
							}
							pItem->value.push_back(row);
						}   
					}
					itemText = itemText.nextSibling();
				}
				if (pItem->value.empty() )
				{
					std::vector<double> defautVal;
					if (pItem->itemType == ELEMENT)
					{
						defautVal.assign(1, 1.0f);
					}
					else
					{
						defautVal.assign(2, 1.0f);
					} 
					pItem->value.push_back(defautVal);
				} 
				pItem->parent = typeNode;
				typeNode->addChild(pItem);
				itemNode = itemNode.nextSibling();
			}
		}
	} 
	return 0;
}

unsigned char  MaterialFileParser::saveToFile(const Node*root)
{
	QFileInfo fi(filePath);
	if (fi.isDir())
	{
		return 1; //file is dir
	}
	else
	{
		QDomDocument doc("xmlDoc");
		QDomProcessingInstruction instruction =doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF - 8\"");
		doc.appendChild(instruction);
		QDomElement  rootElement =doc.createElement(TAG_MAT_STR);
		//if (!root->name.isEmpty() )
		//{
		rootElement.setAttribute(ATT_NAME_STR, "default" );
		//}

		if (!root->templateId.isEmpty() )
		{
			rootElement.setAttribute(ATT_TID_STR, root->templateId);
		}

		saveGroups(doc, rootElement, root );
		doc.appendChild(rootElement);

		QFile fileWrite(filePath);
		if (!fileWrite.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)){
			return -1;
		} 
		QTextStream out(&fileWrite); 
		out.setCodec("UTF-8"); 
		doc.save(out, 4, QDomNode::EncodingFromTextStream); 
		fileWrite.close();
	}
	return 0;
}

unsigned char MaterialFileParser::saveGroups(QDomDocument& doc, QDomElement& rootElement, const Node* groups)
{
	for (int i = 0; i < groups->childCount(); i++ )
	{
		GroupNode *groupNode = static_cast<GroupNode*>(groups->childAt(i));
		if (groupNode->childUsedCount() == 0)
		{
			continue;
		}

		QDomElement groupElement = doc.createElement(TAG_GROUP_STR);

		if (!groupNode->name.isEmpty())
		{
			groupElement.setAttribute(ATT_NAME_STR, groupNode->name);
		} 
		if (groupNode->groupType != UNKNOWGROUPTYPE)
		{
			QString typeStr = "";
			if (groupNode->groupType == MULTI)
			{
				typeStr = VALUE_MULTI_STR;
			}
			else if (groupNode->groupType == SINGLE)
			{
				typeStr = VALUE_SINGLE_STR;
			}
			groupElement.setAttribute(ATT_TYPE_STR, typeStr);
		}

		saveTypes(doc, groupElement, groupNode );
		rootElement.appendChild(groupElement);
	}
	return 0;
}

unsigned char MaterialFileParser::saveTypes(QDomDocument& doc, QDomElement& groupElement, const Node* types)
{
	for (int i = 0; i < types->childCount(); i++ )
	{
		TypeNode *typeNode = static_cast<TypeNode*>(types->childAt(i) );
		if (typeNode->used == false || typeNode->childUsedCount() == 0)
		{
			continue;
		}
		QDomElement typeElement = doc.createElement(TAG_TYPE_STR );
		
		if (!typeNode->name.isEmpty())
		{
			typeElement.setAttribute(ATT_NAME_STR, typeNode->name);
		}

		if (!typeNode->xName.isEmpty())
		{
			typeElement.setAttribute(ATT_XNAME_STR, typeNode->xName);
		}

		if (!typeNode->yName.isEmpty())
		{
			typeElement.setAttribute(ATT_YNAME_STR, typeNode->yName);
		}

		if (typeNode->itemType!=UNKNOWITEMTYPE )
		{
			QString typeStr = "";
			if (typeNode->itemType==ELEMENT )
			{
				typeStr = VALUE_ELEMENT_STR;
			}
			else if (typeNode->itemType==COMPLEX)
			{
				typeStr = VALUE_COMPLEX_STR;
			}
			else if (typeNode->itemType==LIST)
			{
				typeStr = VALUE_LIST_STR;
			}
			else if (typeNode->itemType==MATRIX)
			{
				typeStr = VALUE_MATRIX_STR;
			}
			typeElement.setAttribute(ATT_TYPE_STR, typeStr ); 
		}
		if (typeNode->itemChart != UNKNOWITEMCHART)
		{
			QString chartStr = "";
			if (typeNode->itemChart == SNCHART)
			{
				chartStr = VALUE_SNCHART_STR;
			}
			else if (typeNode->itemChart == LISTCHART)
			{
				chartStr = VALUE_LISTCHART_STR;
			}
			typeElement.setAttribute(ATT_CHART_STR, chartStr);
		}
		saveItems(doc, typeElement, typeNode);
		groupElement.appendChild(typeElement);
	}
	return 0;
}

unsigned char MaterialFileParser::saveItems(QDomDocument& doc, QDomElement& typeElement, const Node* typeNode)
{
	for (int i = 0; i < typeNode->childCount(); i++)
	{
		ItemNode *itemNode = static_cast<ItemNode*>(typeNode->childAt(i) );
		QDomElement itemElement = doc.createElement(TAG_ITEM_STR);

		if (!itemNode->name.isEmpty())
		{
			itemElement.setAttribute(ATT_NAME_STR, itemNode->name);
		}

		if (!itemNode->unitName.isEmpty())
		{
			itemElement.setAttribute(ATT_UNITNAME_STR, itemNode->unitName);
		}

		if (itemNode->vAxis!=UNKNOWVALUE)
		{
			if (itemNode->vAxis==XVALUE)
			{
				itemElement.setAttribute(ATT_VALUEAXIS_STR, VALUE_XVALUE_STR);
			}
			else if(itemNode->vAxis == YVALUE)
			{
				itemElement.setAttribute(ATT_VALUEAXIS_STR, VALUE_YVALUE_STR);
			}
			else if (itemNode->vAxis == YMIN)
			{
				itemElement.setAttribute(ATT_VALUEAXIS_STR, VALUE_YMIN_STR);
			}
			else if (itemNode->vAxis == YMAX)
			{
				itemElement.setAttribute(ATT_VALUEAXIS_STR, VALUE_YMAX_STR);
			}
			else if (itemNode->vAxis == XFIRST)
			{
				itemElement.setAttribute(ATT_VALUEAXIS_STR, VALUE_XFIRST_STR);
			}
			else if (itemNode->vAxis == XSECOND)
			{
				itemElement.setAttribute(ATT_VALUEAXIS_STR, VALUE_XSECOND_STR);
			}
			else if (itemNode->vAxis == XMID)
			{
				itemElement.setAttribute(ATT_VALUEAXIS_STR, VALUE_XMID_STR);
			}
			else if (itemNode->vAxis == YMID)
			{
				itemElement.setAttribute(ATT_VALUEAXIS_STR, VALUE_YMID_STR);
			}
		}

		if (!itemNode->desc.isEmpty())
		{
			QDomElement descElement = doc.createElement(TAG_DESC_STR);
			QDomText descText = doc.createTextNode(itemNode->desc);
			descElement.appendChild(descText);
			itemElement.appendChild(descElement);
		} 
		if (!itemNode->unit.isEmpty())
		{
			QDomElement unitElement = doc.createElement(TAG_UNIT_STR);
			QDomText unitText = doc.createTextNode(itemNode->unit);
			unitElement.appendChild(unitText);
			itemElement.appendChild(unitElement);
		} 
		QString valueStr = itemNode->toStrValue();
		if (!valueStr.isEmpty())
		{
			QDomElement valueElement = doc.createElement(TAG_VALUE_STR);
			QDomText valueText = doc.createTextNode(valueStr);
			valueElement.appendChild(valueText);
			itemElement.appendChild(valueElement);
		}

		typeElement.appendChild(itemElement);
	}
	return 0;
}



