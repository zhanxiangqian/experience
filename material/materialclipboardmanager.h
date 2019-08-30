#pragma once
#include <QObject>
#include <vector>
class QClipboard;
class MaterialClipboardManager : public QObject
{
	Q_OBJECT
public:
	MaterialClipboardManager(QClipboard* clipBoard );
	~MaterialClipboardManager();

public:
	void getTable(std::vector<std::vector<double>>& oTable);
	void setTable(const std::vector<std::vector<double>>& iTable );
private:
	void parseTable();
	void saveTable();
private: 
	QClipboard * clipBoard;
	std::vector<std::vector<double>> table;
};

