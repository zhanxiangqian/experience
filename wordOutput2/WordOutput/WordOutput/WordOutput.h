
// WordOutput.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CWordOutputApp: 
// �йش����ʵ�֣������ WordOutput.cpp
//

class CWordOutputApp : public CWinApp
{
public:
	CWordOutputApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CWordOutputApp theApp;