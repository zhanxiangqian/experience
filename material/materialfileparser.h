#ifndef MATERIALFILEPARSER_H_
#define MATERIALFILEPARSER_H_ 

#include "MaterialDataDef.h"

class QDomDocument;
class QDomElement;
class  MaterialFileParser
{ 
public:
	MaterialFileParser(const QString & path ="" );
	~MaterialFileParser(); 
public:
	void setPath(const QString & path, bool bTemplate = false);

	unsigned char readFromFile(Node* );
	unsigned char  saveToFile(const Node* );

private:
	unsigned char readGroups(QDomElement& , Node* );
	unsigned char readTypes(QDomElement& , Node* );
	unsigned char readItems(QDomElement& , Node* );

private:
	unsigned char saveGroups(QDomDocument&, QDomElement&, const Node*);
	unsigned char saveTypes(QDomDocument&, QDomElement&, const Node*);
	unsigned char saveItems(QDomDocument&, QDomElement&, const Node*);
private:
	QString filePath;
	bool bTemplate;
	const QString VALUE_MULTI_STR = "multi";
	const QString VALUE_SINGLE_STR = "single";
	const QString VALUE_ELEMENT_STR = "element";
	const QString VALUE_COMPLEX_STR = "complex";
	const QString VALUE_LIST_STR = "list";
	const QString VALUE_MATRIX_STR = "matrix";
	const QString VALUE_SNCHART_STR = "snchart";
	const QString VALUE_LISTCHART_STR = "list";
	const QString VALUE_XVALUE_STR = "x";
	const QString VALUE_YVALUE_STR = "y";
	const QString VALUE_YMIN_STR = "ymin";
	const QString VALUE_YMAX_STR = "ymax";
	const QString VALUE_XFIRST_STR = "xfirst";
	const QString VALUE_XSECOND_STR = "xsecond";
	const QString VALUE_XMID_STR = "xmid";
	const QString VALUE_YMID_STR = "ymid";

	const QString ATT_NAME_STR = "name";
	const QString ATT_UNITNAME_STR = "unitName";
	const QString ATT_TYPE_STR = "type";
	const QString ATT_CHART_STR = "chart";
	const QString ATT_TID_STR = "tid";
	const QString ATT_VALUEAXIS_STR = "valueaxis";
	const QString ATT_XNAME_STR = "xname";
	const QString ATT_YNAME_STR = "yname";

	const QString TAG_MAT_STR = "mat";
	const QString TAG_GROUP_STR = "group";
	const QString TAG_TYPE_STR = "type";
	const QString TAG_ITEM_STR = "item";
	const QString TAG_DESC_STR = "desc";
	const QString TAG_UNIT_STR = "unit";
	const QString TAG_VALUE_STR = "value";
	
};

#endif//MATERIALFILEPARSER_H_