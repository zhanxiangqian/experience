#ifndef _UFTREEMODULE_H_
#define _UFTREEMODULE_H_

#include <QAbstractItemModel>
#include "uftreeobject.h"
#include <QVariant>

class UfTreeModel : public QAbstractItemModel
{
public:
	UfTreeModel(UfTreeObject* root, QObject *parent = NULL);
	~UfTreeModel();


	virtual int          columnCount(const QModelIndex &parent = QModelIndex()) const;
	virtual int          rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual QVariant     data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	virtual QModelIndex  index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
	virtual QVariant     headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	virtual QModelIndex  parent(const QModelIndex &index) const;
	void                 setRoot(UfTreeObject* root);

private:
	UfTreeObject*        getTreeObject(QModelIndex index) const;
	UfTreeObject* rootObject;
};


#endif // _UFTREEMODULE_H_
