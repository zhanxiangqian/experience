
#ifndef COMBOBOXDELEGATE_H
#define COMBOBOXDELEGATE_H

#include <QStyledItemDelegate>

class IMaterialDimensionConvert;
class MaterialComboBoxDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
	MaterialComboBoxDelegate(IMaterialDimensionConvert* convert, QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const override;
private:
	IMaterialDimensionConvert *dimensionConvert;
};  
#endif//COMBOBOXDELEGATE_H
