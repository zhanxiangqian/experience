#pragma once
#include "abaqusInputGen.h"

class solverFactory
{
public:
	solverFactory();
	~solverFactory();
	solverGen * createSolver(string & type);
	string getTypeFromJsonFile(string fileName);
	string getFileExt();

private:
	string fileExt;
};