#pragma once
#include <QDialog>
#include <QStringList>

class QLineEdit;
class QComboBox;
class QPushButton;
class MaterialSaveDialog:public QDialog
{
	Q_OBJECT
public:
	MaterialSaveDialog(QWidget* parent =nullptr);
	~MaterialSaveDialog();

public:
	inline void setComboboxList(const QStringList& strList)
	{
		comboboxList = strList;
	}

	inline void setExistNames(const QStringList& names)
	{
		existNames = names;
	}

	void initUI();

	inline QString getName()
	{
		return name;
	}

	inline QString getPath()
	{
		return path;
	} 

	private slots:

	void sl_OkButClicked();
	void sl_CancelButClicked();
	void sl_LineEidtFinished();


private:
	QString name;
	QString path;
	QStringList existNames;
	QStringList comboboxList;

	QComboBox* combobox;
	QLineEdit* lineEdit;
	QPushButton* okButton;
	QPushButton* cancelButton;
};

