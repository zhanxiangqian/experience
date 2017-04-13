#include <windows.h>  
#include <stdio.h>  
#include <tchar.h>  
#include <atlstr.h>  


LRESULT CALLBACK WinSunProc(  
	HWND hwnd,      // handle to window           窗口句柄  
	UINT uMsg,      // message identifier         消息代码  
	WPARAM wParam,  // first message parameter    消息代码附加参数1  
	LPARAM lParam   // second message parameter   消息代码附加参数2  
	);  

int WINAPI WinMain(  
	HINSTANCE hInstance,      // handle to current instance  该程序当前运行的实例的句柄  
	HINSTANCE hPrevInstance,  // handle to previous instance 当前实例的前一个实例的句柄  
	LPSTR lpCmdLine,          // command line                一个以空终止的字符串  
	int nCmdShow              // show state                  指定程序的窗口应该如何显示  
	)  
{  
	WNDCLASS wndcls;                //设计一个窗口类  
	wndcls.cbClsExtra=0;  
	wndcls.cbWndExtra=0;  
	wndcls.hbrBackground=(HBRUSH)GetStockObject(BLACK_BRUSH);      //背景画刷句柄  
	wndcls.hCursor=LoadCursor(NULL,IDC_CROSS);     //光标句柄  
	wndcls.hIcon=LoadIcon(NULL,IDI_ERROR);         //图标句柄  
	wndcls.hInstance=hInstance;                    //包含窗口过程的程序的实例句柄  
	wndcls.lpfnWndProc=WinSunProc;                 //函数指针，指向窗口过程函数  
	wndcls.lpszClassName=_T("sunxin2006");             //指定窗口类名字  
	wndcls.lpszMenuName=NULL;                      //指定菜单资源名字  
	wndcls.style=CS_HREDRAW | CS_VREDRAW;          //窗口样式  

	RegisterClass(&wndcls);                        //注册窗口类  

	HWND hwnd;  

	//创建窗口  
	hwnd=CreateWindow(_T("sunxin2006"),_T("http://www.sunxin.org"),WS_OVERLAPPEDWINDOW,  
		0,0,600,400,NULL,NULL,hInstance,NULL);  

	//显示窗口  
	ShowWindow(hwnd,SW_SHOWNORMAL);  

	//更新窗口  
	UpdateWindow(hwnd);  

	MSG msg;  
	while(GetMessage(&msg,NULL,0,0))  
	{  
		TranslateMessage(&msg);  
		DispatchMessage(&msg);  
	}  
	return msg.wParam;  
}  

LRESULT CALLBACK WinSunProc(  
	HWND hwnd,      // handle to window  
	UINT uMsg,      // message identifier  
	WPARAM wParam,  // first message parameter  
	LPARAM lParam   // second message parameter  
	)  
{  
	ATL::CString data=_T("程序员之家");//原来的乱码是strlen函数在Unicode工程的问题  
	//第一种方法，使用了atl的CString，也可以使用MFC中的CString  

	switch(uMsg)  
	{  
	case WM_CHAR:  
		TCHAR szChar[20];  
		//本工程为Unicode工程  
		wsprintf(szChar,_T("char code is %d"),wParam);  
		MessageBox(hwnd,szChar,_T("char"),0);  
		break;  
	case WM_LBUTTONDOWN:  
		MessageBox(hwnd,_T("mouse clicked"),_T("message"),0);  
		HDC hdc;  
		hdc=GetDC(hwnd);  
		TextOut(hdc,0,150,data,data.GetLength());//第一种方法  
		TextOutA(hdc,0,50,"程序员之家",strlen("程序员之家"));//第二种方法使用TextOutA,针对unicode工程  
		//ReleaseDC(hwnd,hdc);  
		break;  
	case WM_PAINT:  
		HDC hDC;  
		PAINTSTRUCT ps;  
		hDC=BeginPaint(hwnd,&ps);      //得到DC  
		TextOut(hDC,0,0,_T("http://www.sunxin.org"),strlen("http://www.sunxin.org"));  
		EndPaint(hwnd,&ps);            //释放  
		break;  
	case WM_CLOSE:  
		if(IDYES==MessageBox(hwnd,_T("是否真的结束？"),_T("message"),MB_YESNO))  
		{  
			DestroyWindow(hwnd);      //销毁窗口  
		}  
		break;  
	case WM_DESTROY:  
		PostQuitMessage(0);           //向应用程序的消息队列投递一条WM_QUIT消息并返回  
		break;  
	default:  
		return DefWindowProc(hwnd,uMsg,wParam,lParam);  
	}  
	return 0;  
}  