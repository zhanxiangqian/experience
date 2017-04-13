

#include "filematerialmodel.h"


FileMaterialModel::FileMaterialModel(QString dirPath, QObject *parent) : QFileSystemModel(parent)
{

	rootPath = dirPath;
	this->setRootPath(rootPath);

}

FileMaterialModel::FileMaterialModel(QObject *parent) : QFileSystemModel(parent)
{

}

FileMaterialModel::~FileMaterialModel()
{

}

QVariant FileMaterialModel::data(const QModelIndex &index, int role )const
{
	QVariant res;
	if (role == Qt::DisplayRole)
	{
		res = QFileSystemModel::data(index, Qt::DisplayRole);
		if (res.toString().contains(".xml"))
		{
			res = res.toString().remove(".xml");
		}
	}
	else
	{
		res = QFileSystemModel::data(index, Qt::DisplayRole);
	}
	return QFileSystemModel::data(index, role);
 }


int FileMaterialModel::columnCount(const QModelIndex &parent) const
{
	return 1;
}

