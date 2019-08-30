#pragma once
#include <QAbstractItemModel> 
#include "materialdatadef.h"
#include "materialtreeviewcallback.h"
#include "materialuihandledef.h"

class IMaterialDimensionConvert;
class MaterialPropertyTreeModel :public QAbstractItemModel
{
	Q_OBJECT
public:
	MaterialPropertyTreeModel(IMaterialDimensionConvert* convert, TreeViewCallBack* callBack);
	~MaterialPropertyTreeModel();

public:
	void setModelData(Node*data, MaterialOperationState opState =MATERIAL_BROWSE_STATE )
	{
		modelData = data;
		materialOpState = opState;
	}

public:
	virtual QModelIndex index(int row, int column,
		const QModelIndex &parent = QModelIndex()) const;
	virtual QModelIndex parent(const QModelIndex &child) const;

	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

	virtual QVariant headerData(int section, Qt::Orientation orientation,
		int role = Qt::DisplayRole) const;

	virtual Qt::ItemFlags flags(const QModelIndex &index) const;

signals:
	void treeDataChanged();
private:
	Node* modelData; 
	TreeViewCallBack * callBack;
	MaterialOperationState materialOpState;
	IMaterialDimensionConvert *dimensionConvert;
};

