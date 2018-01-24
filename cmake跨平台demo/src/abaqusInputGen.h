#include "solverGen.h"
#include <vector>
#include <map>


class abaqusInputGen: public solverGen
{
public:
	abaqusInputGen();
	~abaqusInputGen();
	string headGen();
	string nodesGen();
	string elementsGen();
	string materialsGen();
	string contactsGen();
	string constraintsGen();
	string loadsGen();
	string setsGen();
	string setGen(settype setType, string setName, vector<string> setIds);
	string setIdGen(set<int> setObj);
	string setIdGen(set<pair<int,int>> setObj);
	string stepGen();
	string outputGen();
	int setInpFile(string fileName);

private:
		map<string, vector<int>> VKI2ABA = { { "TET4",{0,1,2,4,3 }},{"WED6",{0,1,2,3,5,4}},{ "HEX8",{0,1,2,3,5,6,4} } };

};