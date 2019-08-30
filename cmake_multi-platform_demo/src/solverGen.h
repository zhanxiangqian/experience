#include "VKIReader.h"
#include <string>
#include "json.hpp"
using namespace std;
using json = nlohmann::json;

enum settype { NODESET = 1, ELEMENTSET, SURFACESET };

class solverGen
{
public:
	solverGen();
	~solverGen();
	int inputGen(string fileName);
	virtual string headGen();
	virtual string nodesGen();
	virtual string elementsGen();
	virtual string materialsGen();
	virtual string contactsGen();
	virtual string constraintsGen();
	virtual string loadsGen();
	virtual string setsGen();
	virtual string setGen(settype setType, string setName, vector<string> setIds);
	int setJsonFile(string fileName);
	string transSetName(string setName);
	int setInpFile(string fileName);

	VKIReader aVKIReader;
	json j;
private:
	string inputName;
};
