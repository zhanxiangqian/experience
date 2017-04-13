#include <windows.h>  
#include <stdio.h>  
#include <tchar.h>  
#include <atlstr.h>  


LRESULT CALLBACK WinSunProc(  
	HWND hwnd,      // handle to window           ���ھ��  
	UINT uMsg,      // message identifier         ��Ϣ����  
	WPARAM wParam,  // first message parameter    ��Ϣ���븽�Ӳ���1  
	LPARAM lParam   // second message parameter   ��Ϣ���븽�Ӳ���2  
	);  

int WINAPI WinMain(  
	HINSTANCE hInstance,      // handle to current instance  �ó���ǰ���е�ʵ���ľ��  
	HINSTANCE hPrevInstance,  // handle to previous instance ��ǰʵ����ǰһ��ʵ���ľ��  
	LPSTR lpCmdLine,          // command line                һ���Կ���ֹ���ַ���  
	int nCmdShow              // show state                  ָ������Ĵ���Ӧ�������ʾ  
	)  
{  
	WNDCLASS wndcls;                //���һ��������  
	wndcls.cbClsExtra=0;  
	wndcls.cbWndExtra=0;  
	wndcls.hbrBackground=(HBRUSH)GetStockObject(BLACK_BRUSH);      //������ˢ���  
	wndcls.hCursor=LoadCursor(NULL,IDC_CROSS);     //�����  
	wndcls.hIcon=LoadIcon(NULL,IDI_ERROR);         //ͼ����  
	wndcls.hInstance=hInstance;                    //�������ڹ��̵ĳ����ʵ�����  
	wndcls.lpfnWndProc=WinSunProc;                 //����ָ�룬ָ�򴰿ڹ��̺���  
	wndcls.lpszClassName=_T("sunxin2006");             //ָ������������  
	wndcls.lpszMenuName=NULL;                      //ָ���˵���Դ����  
	wndcls.style=CS_HREDRAW | CS_VREDRAW;          //������ʽ  

	RegisterClass(&wndcls);                        //ע�ᴰ����  

	HWND hwnd;  

	//��������  
	hwnd=CreateWindow(_T("sunxin2006"),_T("http://www.sunxin.org"),WS_OVERLAPPEDWINDOW,  
		0,0,600,400,NULL,NULL,hInstance,NULL);  

	//��ʾ����  
	ShowWindow(hwnd,SW_SHOWNORMAL);  

	//���´���  
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
	ATL::CString data=_T("����Ա֮��");//ԭ����������strlen������Unicode���̵�����  
	//��һ�ַ�����ʹ����atl��CString��Ҳ����ʹ��MFC�е�CString  

	switch(uMsg)  
	{  
	case WM_CHAR:  
		TCHAR szChar[20];  
		//������ΪUnicode����  
		wsprintf(szChar,_T("char code is %d"),wParam);  
		MessageBox(hwnd,szChar,_T("char"),0);  
		break;  
	case WM_LBUTTONDOWN:  
		MessageBox(hwnd,_T("mouse clicked"),_T("message"),0);  
		HDC hdc;  
		hdc=GetDC(hwnd);  
		TextOut(hdc,0,150,data,data.GetLength());//��һ�ַ���  
		TextOutA(hdc,0,50,"����Ա֮��",strlen("����Ա֮��"));//�ڶ��ַ���ʹ��TextOutA,���unicode����  
		//ReleaseDC(hwnd,hdc);  
		break;  
	case WM_PAINT:  
		HDC hDC;  
		PAINTSTRUCT ps;  
		hDC=BeginPaint(hwnd,&ps);      //�õ�DC  
		TextOut(hDC,0,0,_T("http://www.sunxin.org"),strlen("http://www.sunxin.org"));  
		EndPaint(hwnd,&ps);            //�ͷ�  
		break;  
	case WM_CLOSE:  
		if(IDYES==MessageBox(hwnd,_T("�Ƿ���Ľ�����"),_T("message"),MB_YESNO))  
		{  
			DestroyWindow(hwnd);      //���ٴ���  
		}  
		break;  
	case WM_DESTROY:  
		PostQuitMessage(0);           //��Ӧ�ó������Ϣ����Ͷ��һ��WM_QUIT��Ϣ������  
		break;  
	default:  
		return DefWindowProc(hwnd,uMsg,wParam,lParam);  
	}  
	return 0;  
}  