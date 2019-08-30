#ifndef MATERIALFILEMODEL_H_
#define MATERIALFILEMODEL_H_

#include <QDir>
#include <QDomDocument>
#include <QFileInfoList>
#include <QDebug>
#include <QFile>
#include <QFileSystemModel>
#include "materialmodel.h"


class ListModel :public QAbstractListModel
{
	Q_OBJECT
public:
	ListModel(QObject *parent = 0);
public:
	void setModelData(QStringList* data)
	{
		modelData = data;
	}
public:
	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
	virtual int	rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual Qt::ItemFlags flags(const QModelIndex &index) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

signals:
	void listDataChanged(const QModelIndex & index );
private:
	QStringList * modelData;
};

class  MaterialFileModel : public QFileSystemModel,public MaterialModel
{
	Q_OBJECT
public:
	MaterialFileModel(QObject *parent = 0);
	MaterialFileModel(QString dirPath, QObject *parent = 0);
	virtual ~MaterialFileModel();

public:
	void travelPath(QDomDocument &domDoc, QDomElement &domElement, QDir dir,QString sub)
	{

		QFileInfo fi(dir.absolutePath(), sub);
	
		if (fi.isFile())
		{//file
			QDomElement elem = domDoc.createElement(fi.baseName());
			domElement.appendChild(elem);

			qDebug() << "   "<<fi.absoluteFilePath();
		}
		else if (fi.isDir())
		{//dir
			QDomElement elem = domDoc.createElement(fi.baseName());
			domElement.appendChild(elem);

			QDir subDir(fi.absoluteFilePath());
			qDebug()<<subDir.absolutePath();

			QStringList filters;
			filters << "*.xml";
			QStringList entryList = subDir.entryList(filters, QDir::Files | QDir::AllDirs | QDir::NoDotAndDotDot);

			foreach(QString entry, entryList)
			{
				travelPath(domDoc, elem, fi.absoluteFilePath(), entry);
			}
		}
	}

public:
	virtual void getLibTree(QDomDocument &domDoc)
	{
		if (rootPath == "")
		{
			return;
		}

		QDir dir(rootPath);
		QDomElement root = domDoc.createElement(dir.dirName());
		domDoc.appendChild(root);

		travelPath(domDoc, root, dir.absolutePath(), "");

		QString str = domDoc.toString();
	
	 
	}

	virtual QDomElement getRootElement()
	{
		return domDoc.documentElement();
	}

	virtual QStringList getParamTypes()
	{
		QStringList res;
		res << "All Types";

		QDomElement  root = domDoc.documentElement();
		QDomNodeList children = root.childNodes();

		int cnt = children.length();
		for (int i = 0; i < cnt; i++)
		{
			QDomNode node = children.at(i);
			res << node.toElement().attribute("name");
		}
		return res;
	}

	virtual QDomElement getItemsXml(QString paramType = "")
	{
		QDomElement res;
		QDomElement  root = domDoc.documentElement();
		QDomNodeList children = root.childNodes();

		int cnt = children.length();
		for (int i = 0; i < cnt; i++)
		{
			QDomNode node = children.at(i);
			if (paramType == node.toElement().attribute("name"))
			{
				res = node.toElement();
				return res;
			}
		}

		if (res.tagName().isEmpty())
		{
			res = root;
		}
		return res;

	}

	void setFile(QString filePath)
	{
		domFile = filePath;

		QFileInfo fi(domFile);
		if (fi.exists())
		{
			domDoc.clear();

			QFile file(fi.absoluteFilePath());
			if (!file.open(QIODevice::ReadOnly))
				return;

			if (!domDoc.setContent(&file))
			{
				qDebug() << "not valid material file!";
				file.close();
				return;
			}
			file.close();
		}
		else
		{
			qDebug() << "not valid file!";
		}
	}

public:
	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	virtual int	columnCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

private:
	QString rootPath;//材料库根目录
	QString domFile;//材料xmldom
	QDomDocument domDoc;

};

#endif