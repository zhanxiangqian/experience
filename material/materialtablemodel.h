#pragma once
#include <QAbstractTableModel>
#include <QStringList>
#include <vector>
#include "materialuihandledef.h"

class MaterialTableModel:public QAbstractTableModel
{
	Q_OBJECT
public:
	MaterialTableModel();
	~MaterialTableModel();
	friend class MaterialTableView;
public:
	void setModelData(std::vector<std::vector<double>>* modelData, MaterialOperationState opState =MATERIAL_BROWSE_STATE )
	{
		this->materialOpState = opState;
		this->modelData = modelData;
		if (modelData!=nullptr )
		{
			rowTotalNum = modelData->size();
			if (modelData->size()>0 )
			{
				columnTotalNum = modelData->at(0).size();
			}
			else
			{
				columnTotalNum = 0;
			}
		}
	}

	inline  std::vector<std::vector<double>> * getModelData()
	{
		return modelData;
	}

	void setHorHearData(const QStringList& data)
	{
		horHeaderData = data;
	}

	void setVerHearData(const QStringList& data)
	{
		verHeaderData = data;
	}

public:
	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
	virtual Qt::ItemFlags flags(const QModelIndex &index) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation,
		int role = Qt::DisplayRole) const;
signals:
	void tableDataChanged();
private:
	std::vector<std::vector<double>> *modelData;
	size_t rowTotalNum;
	size_t columnTotalNum;
	MaterialOperationState materialOpState;
	QStringList horHeaderData;
	QStringList verHeaderData;
};

