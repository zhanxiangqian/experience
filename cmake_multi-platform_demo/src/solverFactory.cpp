#include "solverFactory.h"
#include <fstream>
using namespace std;

solverFactory::solverFactory()
{
	fileExt = "";
}
solverFactory::~solverFactory()
{
}
string solverFactory::getTypeFromJsonFile(string fileName)
{
	ifstream inputFile;
	//inputFile.open("F:\\products\\emt\\new_version\\input.json");
	json j;
	if (fileName.find(".json") != string::npos)
	{
		inputFile.open(fileName);
		if (!inputFile)
		{
			cout << "Open file error!" << endl;
			return "";
		}
		else
		{
			inputFile >> j;
			inputFile.close();
			return j["Analysis"]["SOLVERNAME"];
		}
	}
	else
	{
		cout << "Error json file name!" << endl;
		return "";
	}
}
solverGen * solverFactory::createSolver(string & type)
{
	if (type == "ABAQUS")
	{
		fileExt = ".inp";
		return new abaqusInputGen();
	}
	else if (type == "CALCULIX")
	{
		fileExt = ".inp";
		return new abaqusInputGen();
	}
	else
	{
		return NULL;
	}
}
string solverFactory::getFileExt()
{
	return fileExt;
}