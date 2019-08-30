#pragma once

#include <vdm.h>
#include <vector>
#include <map>
#include <set>
#include <string>
using namespace std;

class VKIReader
{
public:
	VKIReader(const char* filename);
	VKIReader();
	~VKIReader();
	void SetFileName(const char* filename);
	int ReadFile();
	void getCoord(int index, double coord[3]);
	int getUserID(int index);
	int getElementUserID(int index);
	int getElementShape(int index);
	int getElementNode(int index, vector<int> &topo);
	void getElementExtname(int index, char * name);// [9]

	int getElementEntNode(int index, int no, int type, vector<int> &topo);

	void getNodesetFromName(set<int> &nodeids, const char *labelName);
	void getElemsetFromName(set<int> &nodeids, const char *labelName);
	void getSurfacesetFromName(set<std::pair<int, int>> &nodeids, const char *labelName);

	void writeConnect(const char* filename);

	char *FileName;

	int fileType;
	vdm_ABAFil  *abafil;
	vdm_ABALib  *abalib;
	vdm_ANSFil  *ansfil;
	vdm_ANSLib  *anslib;
	vdm_NASFil  *nasfil;
	vdm_NASLib  *naslib;
	vdm_RASLib  *raslib;
	vdm_SDRCLib *sdrclib;
	vdm_PatLib  *patlib;
	vdm_CGNSVLib *cgnsvlib;
	vdm_FLUENTLib *fluentlib;
	vdm_D3DFil  *d3dfil;
	vdm_D3DLib  *d3dlib;
	vdm_STLFil  *stlfil;
	vdm_PAMLib  *pamlib;
	vdm_PLOT3DLib *plot3dlib;
	vdm_EnSightLib *ensightlib;
	vdm_STARCCMLib *starccmlib;
	vdm_TecplotLib *tecplotlib;
	vdm_CFXLib *cfxLib;
	vdm_OpenFOAMLib* openFOAMLib;
	vdm_COMSOLLib *comsolLib;


	vdm_DataFun *datafun;
	vdm_LMan    *lman;
	Vint filetype;
	vis_Model *model;
	vis_Connect *connect;
	vdm_Library   *library;
	vis_GridFun *gridfun;
	int n_Nodes;
	int n_Elements;

	bool isOpened;
};
