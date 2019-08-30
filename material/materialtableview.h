#pragma once
#include <QTableView>
#include <vector>
class MaterialTableModel;
class MaterialClipboardManager;

typedef struct  SelectInfo
{
	SelectInfo()
	{
		min = 0;
		max = 0;
	}
	int min;
	int max;
	QList<int> selects;
}SelectInfo;
class MaterialTableView:public QTableView
{
	Q_OBJECT
public:
	MaterialTableView(QWidget *parent = 0);
public:
	virtual void mousePressEvent(QMouseEvent *event); 
	virtual void mouseDoubleClickEvent(QMouseEvent *event);

signals:
	void rightButtonClicked(QMouseEvent *event);
	void leftButtonClicked(QMouseEvent *event);
	void leftButtonDoubleClicked(QMouseEvent *event);

public:
	void setModel(MaterialTableModel *model); 

public:
	void addRowBefore();
	void addRowAfter();
	void addColumnBefore();
	void addColumnAfter();
	void deleteRow();
	void deleteColumn();
	void copyData();
	void pasteData();
	void importData();

	SelectInfo currentSelectRow();
	SelectInfo currentSelectColumn();
private:
	void insertRow(int, bool);
	void insertColumn(int, bool);
	void deleteRow(int);
	void deleteColumn(int);
	void copyDataFromTable();
	void pasteDataToTable(int);
	void importData(const QString& path);

private:
	static MaterialClipboardManager* clipboardMgr;
	std::vector<std::vector<double>> *modelData;
};

