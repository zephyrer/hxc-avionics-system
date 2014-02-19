
// Avionics_System.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "MainApp.h"
#include "MainWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainApp

BEGIN_MESSAGE_MAP(CMainApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CMainApp 构造

CMainApp::CMainApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CMainApp 对象

CMainApp theApp;


// CMainApp 初始化

BOOL CMainApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}
	
	AfxEnableControlContainer();

	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
	
	CMainWnd *pMainWnd = new CMainWnd();
	pMainWnd->Create(NULL, _T("Punica_Avionics System"));
	m_pMainWnd = pMainWnd;
	pMainWnd->ShowWindow(m_nCmdShow);
	pMainWnd->UpdateWindow();
	
	return TRUE;
}
