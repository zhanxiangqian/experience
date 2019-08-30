#include "materiallineeditdelegate.h"
#include "materialdatadef.h"
MaterialLineEditDelegate::MaterialLineEditDelegate() : QStyledItemDelegate()
{
	
}

MaterialLineEditDelegate::~MaterialLineEditDelegate()
{
	delete editor;
}

QWidget *MaterialLineEditDelegate::createEditor(QWidget *parent,
	const QStyleOptionViewItem &option,
	const QModelIndex &index) const
{
	editor = new QLineEdit(parent);
	editor->setGeometry(option.rect); 
	Node* pNode = static_cast<Node*>(index.internalPointer());
	//if (pNode != nullptr) editor->setText(pNode->name);
	return editor;
}

void MaterialLineEditDelegate::destroyEditor(QWidget *editor, const QModelIndex &) const
{
	editor->setParent(0); 
}

void MaterialLineEditDelegate::setEditorData(QWidget *editor, const QModelIndex &index)const
{
	if (index.isValid())
	{
		QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
		lineEdit->setText(index.model()->data(index).toString());
	}
}

void MaterialLineEditDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
	const QModelIndex &index) const
{
	if (index.isValid() )
	{
		QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
		model->setData(index, lineEdit->text(), Qt::EditRole);
	}
}

void MaterialLineEditDelegate::updateEditorGeometry(QWidget *editor,
	const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
	editor->setGeometry(option.rect);
}
