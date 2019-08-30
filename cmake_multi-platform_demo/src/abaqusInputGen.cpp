#include "abaqusInputGen.h"
#include <sstream>
#include <cmath>
using namespace std;

string abaqusInputGen::headGen()
{
	stringstream ss;

	ss << "*HEADING" << endl;
	return ss.str();
}

string abaqusInputGen::nodesGen()
{
	stringstream ss;

	ss << "*NODE" << ",NSET=ALL" << endl;
	for (int i = 1; i <= aVKIReader.n_Nodes; i++)
	{
		double coords[3];
		aVKIReader.getCoord(i, coords);
		ss << i << "," << coords[0] << "," << coords[1] << "," << coords[2] << endl;
	}

	return ss.str();
}
string abaqusInputGen::elementsGen()
{
	int type = 0;
	int curType = 0;
	stringstream ss;

	for (int i = 1; i <= aVKIReader.n_Elements; i++)
	{
		string name;
		vector<int> elemNode;
		char nameC[9] = { 0 };
		aVKIReader.getElementExtname(i,nameC);
		name = nameC;
		
		if (name.find("C3D4") != string::npos)
		{
			type = 1;
			if (curType != type)
			{
				ss << "*ELEMENT, TYPE=C3D4, ELSET=ALL" << endl;
				curType = type;
			}
		aVKIReader.getElementNode(i, elemNode);
		ss << i << "," << elemNode[0] << "," << elemNode[1] << "," << elemNode[2] << "," << elemNode[3] << endl;
		}
		else if (name.find("C3D8") != string::npos)
		{
			type = 2;
			if (curType != type)
			{
				ss << "*ELEMENT, TYPE=C3D8, ELSET=ALL" << endl;
				curType = type;
			}
			aVKIReader.getElementNode(i, elemNode);
			ss << i << "," << elemNode[0] << "," << elemNode[1] << "," << elemNode[2] << "," << elemNode[3] << "," << elemNode[4] << "," << elemNode[5] << "," << elemNode[6] << "," << elemNode[7] << endl;
		}
	}

	return ss.str();
}
string abaqusInputGen::setsGen()
{
	stringstream ss;
	for (json::iterator it = j.begin(); it != j.end(); ++it)
	{
		cout << it.key() << endl;
		if (it.key() == "Materials")
		{
			for (json::iterator idIt = it.value().begin(); idIt != it.value().end(); ++idIt)
			{
				string setname = (*idIt)["SETNAME"];
				ss << setGen(ELEMENTSET, setname, (*idIt)["SETID"].get<std::vector<string>>());
			}
		}
		else if (it.key() == "Constraints")
		{
			for (json::iterator idIt = it.value().begin(); idIt != it.value().end(); ++idIt)
			{
				string setname = (*idIt)["SETNAME"];
				ss << setGen(NODESET, setname, (*idIt)["SETID"].get<std::vector<string>>());
			}
		}
		else if (it.key() == "Contacts")
		{
			for (json::iterator idIt = it.value().begin(); idIt != it.value().end(); ++idIt)
			{
				string setname = (*idIt)["MASTERSETNAME"];
				ss << setGen(SURFACESET, setname, (*idIt)["MASTERSETID"].get<std::vector<string>>());
				setname = (*idIt)["SLAVESETNAME"];
				ss << setGen(SURFACESET, setname, (*idIt)["SLAVESETID"].get<std::vector<string>>());
			}
		}
		else if (it.key() == "Loads")
		{
			for (json::iterator idIt = it.value().begin(); idIt != it.value().end(); ++idIt)
			{
				string setname;
				if (idIt->find("SETNAME") != idIt->end())
				{
					setname = (*idIt)["SETNAME"];
				}
				else
				{
					continue;
				}
				string type = (*idIt)["TYPE"];
				if (type == "Pressure")
				{
					ss << setGen(SURFACESET, setname, (*idIt)["SETID"].get<std::vector<string>>());
				}
				else if (type == "Gravity")
				{
					if(it->find("SETID") != it->end())
						ss << setGen(ELEMENTSET, setname, (*idIt)["SETID"].get<std::vector<string>>());
				}
				else if (type == "Force")
				{
					ss << setGen(NODESET, setname, (*idIt)["SETID"].get<std::vector<string>>());
				}

			}
		}
	}
	//(*it)["SETID"].get<std::vector<string>>();
	//ss << setGen(ELEMENTSET, (*idIt));
	//for (json::iterator idIt = (*it)["SETID"].begin(); idIt != (*it)["SETID"].end(); ++idIt)
	//{
	//	ss << setGen(SURFACESET, (*idIt));
	//	setlist = setlist + (*idIt).get<std::string>() + ",";
	//}

	return ss.str();
}

string abaqusInputGen::materialsGen()
{
	stringstream ss;
	json m = j["Materials"];
	if (m.is_null())
	{
		cout << "No material" << endl;
	}
	else
	{
		for (json::iterator it = m.begin(); it != m.end(); ++it) 
		{
			string matname = (*it)["NAME"];
			string setname = (*it)["SETNAME"];

			ss << "*MATERIAL,NAME=" << matname << endl;
			if ((*it)["TYPE"] == "IsoElas")
			{
				ss << "*DENSITY" << endl;
				ss << (*it)["RHO"] << endl;
				ss << "*ELASTIC, TYPE=ISO" << endl;
				ss << (*it)["E"] << "," << (*it)["NU"] << endl;
			}
			
			ss << "*SOLID SECTION, ELSET=" << setname << ", MATERIAL=" << matname << endl;
		}
	}

	return ss.str();
}
string abaqusInputGen::setGen(settype setType, string setName, vector<string> setIds)
{
	stringstream ss;
	if (setType == NODESET)
	{
		ss << "*NSET, NSET=" << setName << endl;
		for (vector<string>::iterator it = setIds.begin(); it != setIds.end(); ++it)
		{
			set<int> nset;			
			aVKIReader.getNodesetFromName(nset, transSetName(*it).c_str());
			ss << setIdGen(nset);
		}
	}
	else if (setType == ELEMENTSET)
	{
		ss << "*ELSET, ELSET=" << setName << endl;
		for (vector<string>::iterator it = setIds.begin(); it != setIds.end(); ++it)
		{
			set<int> eset;
			aVKIReader.getElemsetFromName(eset, it->c_str());
			ss << setIdGen(eset);
		}
	}
	else if (setType == SURFACESET)
	{
		ss << "*SURFACE, NAME=" << setName << ", TYPE=ELEMENT" << endl;
		for (vector<string>::iterator it = setIds.begin(); it != setIds.end(); ++it)
		{
			set<pair<int, int>> sset;
			aVKIReader.getSurfacesetFromName(sset, transSetName(*it).c_str());
			ss << setIdGen(sset);
		}
	}

	return ss.str();
}
string abaqusInputGen::setIdGen(set<int> setObj)
{
	stringstream ss;

	int tempIndex = 0;

	for (set<int>::iterator it = setObj.begin(); it != setObj.end(); ++it)
	{
		ss << *it << ",";
		tempIndex++;
		if (tempIndex % 10 == 0)
		{
			ss << endl;
		}
	}
	if (tempIndex % 10 != 0)
	{
		ss << endl;
	}
	return ss.str();
}
string abaqusInputGen::setIdGen(set<pair<int, int>> setObj)
{
	stringstream ss;

	for (set<pair<int, int>>::iterator it = setObj.begin(); it != setObj.end(); ++it)
	{
		string name;
		char nameC[9] = { 0 };
		aVKIReader.getElementExtname(it->first,nameC);
		name = nameC;
		if (name.find("C3D4") != string::npos)
		{
			ss << it->first << ", S" << VKI2ABA["TET4"][it->second] << endl;
		}
		if (name.find("C3D8") != string::npos)
		{
			ss << it->first << ", S" << VKI2ABA["HEX8"][it->second] << endl;
		}
		else
		{ 
			ss << it->first << ", S" << it->second << endl;
		}
	}

	return ss.str();
}
string abaqusInputGen::constraintsGen()
{
	stringstream ss;
	json m = j["Constraints"];

	ss << stepGen();

	if (m.is_null())
	{
		cout << "No constraint" << endl;
	}
	else
	{
		for (json::iterator it = m.begin(); it != m.end(); ++it)
		{
			string setname = (*it)["SETNAME"];
			ss << "*BOUNDARY" << endl;
			if ((*it)["TYPE"] == "Fix")
			{
				ss << setname << ", 1, 3," << " 0.0" << endl;
			}
		}
	}

	return ss.str();
}
string abaqusInputGen::loadsGen()
{
	stringstream ss;
	json m = j["Loads"];
	if (m.is_null())
	{
		cout << "No load" << endl;
	}
	else
	{
		for (json::iterator it = m.begin(); it != m.end(); ++it)
		{
			string setname;
			if (it->find("SETNAME") != it->end())
			{
				setname = (*it)["SETNAME"];
			}

			if ((*it)["TYPE"] == "Pressure")
			{
				ss << "*DSLOAD" << endl;
				ss << setname << ", P, " << (*it)["VALUE"] << endl;
			}
			else if ((*it)["TYPE"] == "Gravity")
			{
				double x, y, z;
				double mag;

				x = (*it)["X"];
				y = (*it)["Y"];
				z = (*it)["Z"];
				mag = sqrt((x*x + y*y + z*z));

				ss << "*DLOAD" << endl;
				if (it->find("SETNAME") == it->end() && it->find("SETID") == it->end())
				{
					ss << "ALL, GRAV, "<< mag << ", " << x/mag <<", "<< y/mag << ", " << z/mag << endl;
				}
				else
				{
					ss << setname << ", GRAV, "<< mag << ", " << x/mag << ", " << y/mag << ", " << z/mag << endl;
				}
			}
			else if ((*it)["TYPE"] == "Force")
			{
				ss << "*CLOAD" << endl;
				ss << setname << ", 1, " << (*it)["X"] << endl;
				ss << setname << ", 2, " << (*it)["Y"] << endl;
				ss << setname << ", 3, " << (*it)["Z"] << endl;
			}
		}
	}

	ss << outputGen();

	return ss.str();
}
string abaqusInputGen::contactsGen()
{
	stringstream ss;
	json m = j["Contacts"];
	if (m.is_null())
	{
		cout << "No contact" << endl;
	}
	else
	{
		for (json::iterator it = m.begin(); it != m.end(); ++it)
		{
			string mastername = (*it)["MASTERSETNAME"];
			string slavename = (*it)["SLAVESETNAME"];
			string name = (*it)["NAME"];
			if ((*it)["TYPE"] == "Bond")
			{
				ss << "*TIE, NAME=" << name << endl;
				ss << mastername << "," << slavename << endl;
			}
		}
	}
	return ss.str();
}
string abaqusInputGen::stepGen()
{
	stringstream ss;
	ss << "*STEP" << endl;
	json m = j["Analysis"];
	if (m.is_null())
	{
		cout << "No analysis" << endl;
	}
	else
	{
		if (m["TYPE"] == "Struc_static")
		{
			ss << "*STATIC" << endl;
		}
	}
	return ss.str();
}
string abaqusInputGen::outputGen()
{
	stringstream ss;
	ss << "*OUTPUT, FIELD" << endl;
	ss << "*NODE OUTPUT" << endl;
	ss << "U" << endl;
	ss << "*ELEMENT OUTPUT" << endl;
	ss << "S" << endl;
	ss << "*NODE FILE" << endl;
	ss << "U" << endl;
	ss << "*EL FILE" << endl;
	ss << "S" << endl;
	ss << "*END STEP" << endl;
	return ss.str();
}
abaqusInputGen::abaqusInputGen()
{
}
abaqusInputGen::~abaqusInputGen()
{
}