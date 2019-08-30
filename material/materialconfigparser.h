#pragma once
#include <QString>
#include <map>
class QDomDocument;
class MaterialConfigParser
{
public:
	MaterialConfigParser();
	~MaterialConfigParser();
public:
	static void read();
	static void save();
public:
	const static QString originalDir; 
	const static QString selectDir;
	const static QString dimensionPath;
	const static QString typeconfPath;
	static QString configPath;
	static std::map<QString, bool> usedWorkMap;
public:
	static std::map<QString, QString> configs;
}; 

