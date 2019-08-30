#pragma once
#include <QStringList>
#include <map>
class MaterialTypeParser
{
public:
	MaterialTypeParser();
	~MaterialTypeParser();
	void read(const QString& path);
	const std::map<QString, QStringList> & getTypeMap();
private:
	std::map<QString, QStringList> matTypeMap;
};

