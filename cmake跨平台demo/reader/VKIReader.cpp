#include "VKIReader.h"

int VKIReader::ReadFile()
{
	if (NULL == FileName)
	{
		return -1;
	}
	datafun = vdm_DataFunBegin();

	// determine file type from file extension
	if (strstr(this->FileName, ".bdf") != NULL ||
		strstr(this->FileName, ".dat") != NULL ||
		strstr(this->FileName, ".nas") != NULL)
	{
		filetype = VDM_NASTRAN_BULKDATA;
		nasfil = vdm_NASFilBegin();
		vdm_NASFilDataFun(nasfil, datafun);
	}
	else if (strstr(this->FileName, ".op2") != NULL)
	{
		filetype = VDM_NASTRAN_OUTPUT2;
		naslib = vdm_NASLibBegin();
		vdm_NASLibDataFun(naslib, datafun);
	}
	else if (strstr(this->FileName, ".inp") != NULL)  /*  ||strstr(this->FileName, ".meh") != NULL)*/
	{
		filetype = VDM_ABAQUS_INPUT;
		abafil = vdm_ABAFilBegin();
		vdm_ABAFilDataFun(abafil, datafun);
	}
	else if (strstr(this->FileName, ".fil") != NULL)
	{
		filetype = VDM_ABAQUS_FIL;
		abalib = vdm_ABALibBegin();
		vdm_ABALibDataFun(abalib, datafun);
	}
	else if (strstr(this->FileName, ".odb") != NULL)
	{
		filetype = VDM_ABAQUS_ODB;
		abalib = vdm_ABALibBegin();
		vdm_ABALibDataFun(abalib, datafun);
	}
	else if (strstr(this->FileName, ".cdb") != NULL)
	{
		filetype = VDM_ANSYS_INPUT;
		ansfil = vdm_ANSFilBegin();
		vdm_ANSFilDataFun(ansfil, datafun);
	}
	else if (strstr(this->FileName, ".rst") != NULL || strstr(this->FileName, ".rth") != NULL)
	{
		filetype = VDM_ANSYS_RESULT;
		anslib = vdm_ANSLibBegin();
		vdm_ANSLibDataFun(anslib, datafun);
	}
	else if (strstr(this->FileName, ".neu") != NULL)
	{
		filetype = VDM_MECHANICA_STUDY;
		raslib = vdm_RASLibBegin();
		vdm_RASLibDataFun(raslib, datafun);
	}
	else if (strstr(this->FileName, ".out") != NULL)
	{
		filetype = VDM_PATRAN_NEUTRAL;
		patlib = vdm_PatLibBegin();
		vdm_PatLibDataFun(patlib, datafun);
	}
	else if (strstr(this->FileName, ".cgns") != NULL)
	{
		filetype = VDM_CGNS;
		cgnsvlib = vdm_CGNSVLibBegin();
		vdm_CGNSVLibDataFun(cgnsvlib, datafun);
	}
	else if (strstr(this->FileName, ".unv") != NULL ||
		strstr(this->FileName, ".bun") != NULL ||
		strstr(this->FileName, ".bud") != NULL)
	{
		filetype = VDM_SDRC_UNIVERSAL;
		sdrclib = vdm_SDRCLibBegin();
		vdm_SDRCLibDataFun(sdrclib, datafun);
	}
	else if (strstr(this->FileName, ".cas") != NULL ||
		strstr(this->FileName, ".msh") != NULL)
	{
		filetype = VDM_FLUENT_MESH;
		fluentlib = vdm_FLUENTLibBegin();
		vdm_FLUENTLibDataFun(fluentlib, datafun);
	}
	else if (strstr(this->FileName, ".k") != NULL ||
		strstr(this->FileName, ".key") != NULL ||
		strstr(this->FileName, ".dyn") != NULL ||
		strstr(this->FileName, ".DYN") != NULL)
	{
		filetype = VDM_LSTC_INPUT;
		d3dfil = vdm_D3DFilBegin();
		vdm_D3DFilDataFun(d3dfil, datafun);
	}
	else if (strstr(this->FileName, "d3plot") != NULL)
	{
		filetype = VDM_LSTC_STATE;
		d3dlib = vdm_D3DLibBegin();
		vdm_D3DLibDataFun(d3dlib, datafun);
	}
	else if (strstr(this->FileName, ".stl") != NULL)
	{
		filetype = VDM_STL;
		stlfil = vdm_STLFilBegin();
		vdm_STLFilDataFun(stlfil, datafun);
	}
	else if (strstr(this->FileName, ".STL") != NULL)
	{
		filetype = VDM_STLBIN;
		stlfil = vdm_STLFilBegin();
		vdm_STLFilDataFun(stlfil, datafun);
	}
	else if (strstr(this->FileName, ".dsy") != NULL ||
		strstr(this->FileName, ".DSY") != NULL)
	{
		filetype = VDM_PAM_DAISY;
		pamlib = vdm_PAMLibBegin();
		vdm_PAMLibDataFun(pamlib, datafun);
	}
	else if (strstr(this->FileName, ".x") != NULL)
	{
		filetype = VDM_PLOT3D_GRID;
		plot3dlib = vdm_PLOT3DLibBegin();
		vdm_PLOT3DLibSetGridType(plot3dlib, PLOT3DLIB_MULTIPLE, 3, SYS_OFF);
		vdm_PLOT3DLibDataFun(plot3dlib, datafun);
	}
	else if (strstr(this->FileName, ".encas") != NULL)
	{
		filetype = VDM_ENSIGHT;
		ensightlib = vdm_EnSightLibBegin();
		vdm_EnSightLibDataFun(ensightlib, datafun);
	}
	else if (strstr(this->FileName, ".ccm") != NULL)
	{
		filetype = VDM_STARCCM;
		starccmlib = vdm_STARCCMLibBegin();
		vdm_STARCCMLibDataFun(starccmlib, datafun);
	}
	else if (strstr(this->FileName, ".plt") != NULL)
	{
		filetype = VDM_TECPLOT;
		tecplotlib = vdm_TecplotLibBegin();
		vdm_TecplotLibDataFun(tecplotlib, datafun);
	}
	else if (strstr(this->FileName, ".res") != NULL)
	{
		filetype = VDM_CFX_RESULT;
		cfxLib = vdm_CFXLibBegin();
		vdm_CFXLibDataFun(cfxLib, datafun);
		int errNo = vdm_CFXLibError(cfxLib);

	}
	else if (strstr(this->FileName, "controlDict") != NULL)
	{
		filetype = VDM_OPENFOAM;
		openFOAMLib = vdm_OpenFOAMLibBegin();
		vdm_OpenFOAMLibDataFun(openFOAMLib, datafun);
		int errNo = vdm_OpenFOAMLibError(openFOAMLib);
	}
	else if (strstr(this->FileName, "txt") != NULL)
	{
		filetype = VDM_COMSOL_SECTION;
		comsolLib = vdm_COMSOLLibBegin();
		vdm_COMSOLLibDataFun(comsolLib, datafun);
		int error = vdm_COMSOLLibError(comsolLib);
	}
	else
	{
		return -2;
	}

	// open library device
	vdm_DataFunOpen(datafun, 0, this->FileName, filetype);


	// instance model object for finite element model
	model = vis_ModelBegin();

	// use Library Manager object to load model
	lman = vdm_LManBegin();
	vdm_LManSetObject(lman, VDM_DATAFUN, datafun);
	vdm_LManLoadModel(lman, model);
	if (vdm_LManError(lman))
	{
		return -3;
	}

	// Connect model to vis connect
	vis_ModelGetObject(model, VIS_CONNECT, (Vobject**)&connect);
	if (connect == NULL)
	{
		return -4;
	}

	// get library object 
	vdm_DataFunGetLibrary(datafun, &library);

	// try to append other result files
	//this->try_append_result_files();
	vdm_LibraryTOC(library, "*", SYS_ON);
	// create a grid function object
	gridfun = vis_GridFunBegin();
	vis_ConnectGridFun(connect, gridfun);
	vis_ConnectNumber(connect, SYS_NODE, &n_Nodes);
	vis_ConnectNumber(connect, SYS_ELEM, &n_Elements);
	isOpened = true;
	return 0;
}


VKIReader::VKIReader()
{
	fileType = 0;
	abafil = NULL;
	abalib = NULL;
	ansfil = NULL;
	anslib = NULL;
	nasfil = NULL;
	naslib = NULL;
	raslib = NULL;
	sdrclib = NULL;
	patlib = NULL;
	cgnsvlib = NULL;
	fluentlib = NULL;
	d3dfil = NULL;
	d3dlib = NULL;
	stlfil = NULL;
	pamlib = NULL;
	plot3dlib = NULL;
	ensightlib = NULL;
	starccmlib = NULL;
	tecplotlib = NULL;
	cfxLib = NULL;
	openFOAMLib = NULL;

	datafun = NULL;
	lman = NULL;
	model = NULL;
	gridfun = NULL;
	library = NULL;
	connect = NULL;
	FileName = NULL;
	isOpened = false;

}

VKIReader::VKIReader(const char* filename)
{

	fileType = 0;
	abafil = NULL;
	abalib = NULL;
	ansfil = NULL;
	anslib = NULL;
	nasfil = NULL;
	naslib = NULL;
	raslib = NULL;
	sdrclib = NULL;
	patlib = NULL;
	cgnsvlib = NULL;
	fluentlib = NULL;
	d3dfil = NULL;
	d3dlib = NULL;
	stlfil = NULL;
	pamlib = NULL;
	plot3dlib = NULL;
	ensightlib = NULL;
	starccmlib = NULL;
	tecplotlib = NULL;
	cfxLib = NULL;
	openFOAMLib = NULL;
	datafun = NULL;
	lman = NULL;
	model = NULL;
	gridfun = NULL;
	library = NULL;
	connect = NULL;
	FileName = NULL;
	isOpened = false;
	SetFileName(filename);
}

VKIReader::~VKIReader()
{

}

void VKIReader::getCoord(int index, double coord[3])
{

	vis_ConnectCoordsdv(connect, 1, &index, (double(*)[3])coord);
	//return coord;
}

int VKIReader::getUserID(int index)
{
	int uID;
	vis_ConnectNodeAssoc(connect, VIS_USERID, 1, &index, &uID);
	return uID;
}

void VKIReader::getElementExtname(int index,char * name)
{
	vis_ConnectElemAssoc(connect, VIS_EXTNAMEA, 1, &index, (Vint*)name);
	vis_ConnectElemAssoc(connect, VIS_EXTNAMEB, 1, &index, (Vint*)&name[4]);

	for (int nameI = 0; nameI < 8; nameI++)
	{
		if (name[nameI] == '\0')
		{
			name[nameI] = ' ';
		}
		else
		{
			break;
		}
	}
	name[8] = '\0';
}

int VKIReader::getElementNode(int index, vector<int> &topo)
{
	int max_ele_node;
	vis_ConnectMaxElemNode(connect, &max_ele_node);
	int nix;
	int *ix = new int[max_ele_node];
	vis_ConnectElemNode(connect, index, &nix, ix);
	for (int i = 0; i < nix; i++)
	{
		topo.push_back(ix[i]);
	}
	delete[] ix;
	return nix;
}

int VKIReader::getElementEntNode(int index, int no, int type, vector<int> &topo)
{
	int max_ele_node;
	vis_ConnectMaxElemNode(connect, &max_ele_node);
	int nix;
	int *ix = new int[max_ele_node];
	int entType;
	if (type == 0)
	{
		entType = SYS_ELEM;
	}
	else if (type == 1)
	{
		entType = SYS_FACE;
	}
	else if (type == 2)
	{
		entType = SYS_EDGE;
	}
	else if (type == 3)
	{
		nix = 1;
		topo.push_back(index - 1);
		return nix;
	}
	vis_ConnectElemCon(connect, entType, index, no, &nix, ix);
	for (int i = 0; i < nix; i++)
	{
		topo.push_back(ix[i] - 1);
	}
	delete[] ix;
	return nix;
}

int VKIReader::getElementShape(int index)
{
	int shape, maxi, maxj, maxk;
	vis_ConnectTopology(connect, index, &shape, &maxi, &maxj, &maxk);
	return shape;
}

int VKIReader::getElementUserID(int index)
{
	int eID;
	vis_ConnectElemAssoc(connect, VIS_USERID, 1, &index, &eID);
	return eID;
}

void VKIReader::SetFileName(const char* filename)
{
	if (NULL == filename)
	{
		return;
	}
	if (FileName)
	{
		delete FileName;
		FileName = NULL;
	}
	size_t len = strlen(filename);
	FileName = new char[len + 1];
	strncpy(FileName, filename, len + 1);
}
void VKIReader::writeConnect(const char* filename)
{
	vis_ConnectWrite(connect, SYS_ABAQUS_INPUT, filename);
}
void VKIReader::getNodesetFromName(set<Vint> &nodeids, const char *labelName)
{
	vis_IdTran *idtran;
	Vint iset;
	Vint i = 0, lrec;
	Vint id;
	Vchar name[256];
	vsy_HashTable *nsethash;
	vis_ModelGetHashTable(model, VIS_IDTRAN_NODE, &nsethash);
	if (nsethash != NULL)
	{
		vsy_HashTableInitIter(nsethash);
		while (vsy_HashTableNextIter(nsethash, &iset, (Vobject**)&idtran), idtran)
		{
			vis_IdTranGetName(idtran, name);
			string base = name;
			size_t pos = base.find_first_of(".");
			if (pos != string::npos && base.substr(pos + 1) == labelName)
			{
				vis_IdTranCount(idtran, IDTRAN_MAXINDEX, &lrec);
				for (i = 1; i <= lrec; i++)
				{
					vis_IdTranGetId(idtran, i, &id);
					nodeids.insert(id);
				}
			}
		}
	}
}
void VKIReader::getElemsetFromName(set<Vint> &elemids, const char *labelName)
{
	vis_IdTran *idtran;
	Vint iset;
	Vint i = 0, lrec;
	Vint id;
	Vchar name[256];
	vsy_HashTable *nsethash;
	vis_ModelGetHashTable(model, VIS_IDTRAN_ELEM, &nsethash);
	if (nsethash != NULL)
	{
		vsy_HashTableInitIter(nsethash);
		while (vsy_HashTableNextIter(nsethash, &iset, (Vobject**)&idtran), idtran)
		{
			vis_IdTranGetName(idtran, name);
			string base = name;
			size_t pos = base.find_last_of(".");
			if (pos != string::npos && base.substr(pos + 1) == labelName)
			{
				vis_IdTranCount(idtran, IDTRAN_MAXINDEX, &lrec);
				for (i = 1; i <= lrec; i++)
				{
					vis_IdTranGetId(idtran, i, &id);
					elemids.insert(id);
				}
			}
		}
	}
}
void VKIReader::getSurfacesetFromName(set<std::pair<int, int> > &sufaceids, const char *labelName)
{
	vis_IdTran *idtran;
	Vint iset;
	Vint i = 0, lrec;
	Vint id, entid;
	Vchar name[256];
	vsy_HashTable *sethash;
	vis_ModelGetHashTable(model, VIS_IDTRAN_ELEMENT, &sethash);
	if (sethash != NULL)
	{
		vsy_HashTableInitIter(sethash);
		while (vsy_HashTableNextIter(sethash, &iset, (Vobject**)&idtran), idtran)
		{
			vis_IdTranGetName(idtran, name);
			string base = name;
			size_t pos = base.find_first_of(".");
			if (pos != string::npos && base.substr(pos + 1) == labelName)
			{
				vis_IdTranCount(idtran, IDTRAN_MAXINDEX, &lrec);
				for (i = 1; i <= lrec; i++)
				{
					vis_IdTranGetId(idtran, i, &id);
					vis_IdTranGetEnt(idtran, i, &entid);
					sufaceids.insert(std::make_pair(id, entid));
				}
			}
		}
	}
}
