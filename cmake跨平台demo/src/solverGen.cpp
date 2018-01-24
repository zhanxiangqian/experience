#include "solverGen.h"
#include <fstream>
#include <string>
using namespace std;

int solverGen::inputGen(string fileName)
{
	ofstream inputFile;
	
	//inputFile.open("E:\\out.inp");
	if (fileName.empty())
	{
		cout << "Solver input file name is not set!" << endl;
		return -1;
	}
	else
	{
		inputFile.open(fileName);

		inputFile << nodesGen();
		inputFile << elementsGen();
		inputFile << setsGen();
		inputFile << materialsGen();
		inputFile << contactsGen();
		inputFile << constraintsGen();
		inputFile << loadsGen();

		inputFile.close();
		return 0;
	}
}
string solverGen::headGen()
{
	return "";
}
string solverGen::nodesGen()
{
	return "";
}
string solverGen::elementsGen()
{
	return "";
}
string solverGen::materialsGen()
{
	return "";
}
string solverGen::constraintsGen()
{
	return "";
}
string solverGen::loadsGen()
{
	return "";
}
string solverGen::contactsGen()
{
	return "";
}
string solverGen::setGen(settype setType, string setName, vector<string> setIds)
{
	return "";
}
string solverGen::setsGen()
{
	return "";
}
int solverGen::setJsonFile(string fileName)
{
	ifstream inputFile;
	//inputFile.open("F:\\products\\emt\\new_version\\input.json");

	if (fileName.find(".json") != string::npos)
	{
		inputFile.open(fileName);
		if (!inputFile)
		{
			cout << "Open file error!" << endl;
			return -1;
		}
		else
		{
			inputFile >> j;
			inputFile.close();
			return 0;
		}
	}
	else
	{
		cout << "Error json file name!" << endl;
		return -1;
	}
}
string solverGen::transSetName(string setName)
{
	return setName.replace(setName.find("@"), 1, ".");
}
int solverGen::setInpFile(string fileName)
{
	//aVKIReader.SetFileName("F:\\products\\emt\\new_version\\main.inp");
	if (fileName.find(".inp") != string::npos)
	{
		aVKIReader.SetFileName(fileName.c_str());
		aVKIReader.ReadFile();
		if (aVKIReader.isOpened)
		{
			return 0;
		}
		else
		{
			cout << "Open inp file failed!" << endl;
			return -2;
		}
	}
	else
	{
		cout << "Error inp file name!" << endl;
		return -1;
	}
}
solverGen::solverGen()
{

}
solverGen::~solverGen()
{
}