#include "materialcomboboxdelegate.h"
#include "materialdatadef.h"
#include <QComboBox>
#include "imaterialdimensionconvert.h"
MaterialComboBoxDelegate::MaterialComboBoxDelegate(IMaterialDimensionConvert* convert, QObject *parent)
: dimensionConvert(convert),
QStyledItemDelegate(parent)
{
} 

QWidget *MaterialComboBoxDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem &/* option */,
    const QModelIndex & index ) const
{
	Node * curNode = static_cast<Node*>(index.internalPointer());
	if (curNode!=nullptr)
	if (curNode->nodeType==ITEMTYPE )
	{
		ItemNode* itemNode = (ItemNode*)curNode;
		if (itemNode->unitName.isEmpty() )
		{
			return nullptr;
		}
		else
		{
			if (dimensionConvert!=nullptr )
			{
				QStringList nameList = dimensionConvert->getUnitNameList(itemNode->unitName);
				QComboBox *editor = new QComboBox(parent);
				editor->addItems(nameList);
				editor->setFrame(false);
				return editor;
			}
			else
			{
				return nullptr;
			}
			
		}
	}
	return nullptr;
} 

void MaterialComboBoxDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    QString value = index.model()->data(index, Qt::DisplayRole).toString();

    QComboBox *comboBox = static_cast<QComboBox*>(editor);
	comboBox->setCurrentText(value);
}

void MaterialComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
	QComboBox *comboBox = static_cast<QComboBox*>(editor);
	QString value = comboBox->currentText(); 
    model->setData(index, value, Qt::EditRole);
} 

void MaterialComboBoxDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
} 
