
// Avionics_System.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMainApp:
// �йش����ʵ�֣������ Avionics_System.cpp
//

class CMainApp : public CWinAppEx
{
public:
	CMainApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��
	DECLARE_MESSAGE_MAP()
private:
	ULONG_PTR m_gdiplusToken;
};

extern CMainApp theApp;