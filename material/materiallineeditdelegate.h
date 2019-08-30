#pragma once
#include <QStyledItemDelegate>
#include <QLineEdit>
class MaterialLineEditDelegate :public QStyledItemDelegate
{  
public:
	MaterialLineEditDelegate();
	~MaterialLineEditDelegate();
public:
	QWidget *createEditor(QWidget *parent,
		const QStyleOptionViewItem &option,
		const QModelIndex &index) const;
 
	void destroyEditor(QWidget *, const QModelIndex &) const;
	 
	void setEditorData(QWidget *editor, const QModelIndex &index)const;

	void setModelData(QWidget *editor, QAbstractItemModel *model,
		const QModelIndex &index) const;

	void updateEditorGeometry(QWidget *editor,
		const QStyleOptionViewItem &option, const QModelIndex &index) const;
 
private:
	mutable QLineEdit *editor;
};

