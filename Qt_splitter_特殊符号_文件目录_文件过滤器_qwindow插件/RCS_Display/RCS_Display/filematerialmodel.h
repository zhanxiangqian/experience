#ifndef FILEMATERIALMODEL_H_
#define FILEMATERIALMODEL_H_

#include <QDir>
#include <QDomDocument>
#include <QFileInfoList>
#include <QDebug>
#include <QFile>
#include <QFileSystemModel>
#include "materialmodel.h"

class  FileMaterialModel : public QFileSystemModel
{
	Q_OBJECT
public:
	FileMaterialModel(QObject *parent = 0);
	FileMaterialModel(QString dirPath, QObject *parent = 0);
	virtual ~FileMaterialModel();


public:
	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	virtual int	columnCount(const QModelIndex &parent = QModelIndex()) const;

private:
	QString rootPath;//材料库根目录
	QString domFile;//材料xmldom

};

#endif