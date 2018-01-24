#include "solverFactory.h"
#include "cmdline.h"

// -i F:/products/emt/new_version/example1/main.inp -j F:/products/emt/new_version/example1/input.json -o d:\a
int main(int argc, char *argv[])
{
	

	cmdline::parser a;//����һ�������н�����

					  // ���ָ�����͵��������
					  // ��һ��������������
					  // �ڶ��������������ƣ�'\0'��ʾû�ж����ƣ�
					  // ��������������������
					  // ���ĸ�������boolֵ����ʾ�ò����Ƿ������ڣ���ѡ��Ĭ��ֵ��false��
					  // �����������������Ĭ��ֵ����ѡ�������ĸ�����Ϊfalseʱ�ò�����Ч��

	a.add<string>("json", 'j', "Json file path and name", true, "");
	a.add<string>("inputfile", 'i', "Mesh file (format:inp) path and name", true, "");
	a.add<string>("outputpath", 'o', "Solver input file (format:inp) path", false, "");
	a.add("help",0,"Need two parameters: json and inp file. Json file: -j \"path + name\". Inp file: -i \"path + name\"");
	a.parse_check(argc, argv);

	string jsonFile = a.get<string>("json");
	string inpFile = a.get<string>("inputfile");
	string outputPath = a.get<string>("outputpath");
	string outputFile;

	solverGen * solver = NULL;
	solverFactory solverF;
	string solverType;
	
	solverType = solverF.getTypeFromJsonFile(jsonFile);
	if (solverType == "")
	{
		return -1;
	}
	else
	{
		solver = solverF.createSolver(solverType);
	}

	outputFile = "solverInput" + solverF.getFileExt();

	if (outputPath != "")
	{
		outputFile = outputPath + "/" + outputFile;
	}
	cout << "Progress: 0 % " << endl;

	if (solver && solver->setJsonFile(jsonFile) == 0 && solver->setInpFile(inpFile) == 0 && solver->inputGen(outputFile) == 0)
	{

     cout << "Progress: 100 % " << endl;
		return 0;
	}
	else
	{
		return -1;
	}
	
	// test code:
	//abaqusInput.aVKIReader.SetFileName("F:\\products\\emt\\new_version\\main.inp");
	//abaqusInput.aVKIReader.ReadFile();
	//abaqusInput.aVKIReader.writeConnect("E:\\a.inp");
	//	json j;
	//	j = abaqusInput.j;

	//	cout << j["Materials"] << endl;

	return 0;
 }