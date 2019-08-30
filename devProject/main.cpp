#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include<iostream>
#include <windows.h>>
using namespace std;
int main(void)
{
	HANDLE f = ::CreateFile("a.txt", GENERIC_READ|GENERIC_WRITE, FILE_SHARE_WRITE, NULL,OPEN_ALWAYS, NULL, NULL);
	::SetStdHandle(STD_OUTPUT_HANDLE, f);
	if(f == ::GetStdHandle(STD_OUTPUT_HANDLE))
	{
		int a= 0;
	}
	cout << "aaa" << endl; //我指望这个输出到文件中.
	::CloseHandle(f);
	return 0;
}
