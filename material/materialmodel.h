#ifndef MATERIALMODEL_H_
#define MATERIALMODEL_H_

#include <QObject>
#include <QDomDocument>

class  MaterialModel : public QObject
{
	Q_OBJECT
public:
	MaterialModel(QObject *parent = 0);
	virtual ~MaterialModel(); 

public:
	virtual void getLibTree(QDomDocument &domDoc) = 0;
	 //给定一个材料库，比如一个文件夹，获取里面的材料名字
		//如果是一个数据库，也是要获取材料名字
		
	 
	virtual QStringList getParamTypes() = 0;
	 //根据材料名字,获取材料的参数类型
 

	virtual QDomElement getItemsXml(QString paramType) = 0;
	 //根据参数类型,获取材料参数名,参数值等

};

#endif