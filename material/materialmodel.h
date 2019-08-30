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
	 //����һ�����Ͽ⣬����һ���ļ��У���ȡ����Ĳ�������
		//�����һ�����ݿ⣬Ҳ��Ҫ��ȡ��������
		
	 
	virtual QStringList getParamTypes() = 0;
	 //���ݲ�������,��ȡ���ϵĲ�������
 

	virtual QDomElement getItemsXml(QString paramType) = 0;
	 //���ݲ�������,��ȡ���ϲ�����,����ֵ��

};

#endif