// MainView.cpp : 实现文件
//

#include "stdafx.h"
#include "MainWnd.h"
#include "MainApp.h"
#include "Meters.h"

// CMainView

IMPLEMENT_DYNAMIC(CMainWnd, CFrameWnd)

CMainWnd::CMainWnd()
{
	m_pMemBMP = NULL;
}

CMainWnd::~CMainWnd()
{
	if (m_pMemG != NULL)
		delete m_pMemG;
	if (m_pMemBMP != NULL)
		delete m_pMemBMP;
	if (m_pAS_ALT_HSI)
		delete m_pAS_ALT_HSI;
}


BEGIN_MESSAGE_MAP(CMainWnd, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_GETMINMAXINFO()
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CMainView 消息处理程序


int CMainWnd::OnCreate(LPCREATESTRUCT lpcs)
{
	if (CWnd::OnCreate(lpcs) == -1)
		return -1;
	
	//CWnd::SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	WINDOWPLACEMENT wp = {sizeof(WINDOWPLACEMENT)};
	::GetWindowRect(::GetDesktopWindow(), &wp.rcNormalPosition);
	::AdjustWindowRectEx(&wp.rcNormalPosition, CWnd::GetStyle(), FALSE, CWnd::GetExStyle());
	wp.showCmd = SW_SHOWNORMAL;
	CWnd::SetWindowPlacement(&wp);

	CWnd::SetTimer(TIMER_ID_REFRESH, 20, NULL);
	int cxScreen = ::GetSystemMetrics(SM_CXSCREEN);
	int cyScreen = ::GetSystemMetrics(SM_CYSCREEN);
	m_pMemBMP = new Gdiplus::Bitmap(cxScreen, cyScreen);
	if (m_pMemBMP == NULL)
		return -1;
	m_pMemG = Gdiplus::Graphics::FromImage(m_pMemBMP);
	if (m_pMemG == NULL)
		return -1;
	m_pMemG->SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
	Gdiplus::Point pt(424, 0);
	m_pAS_ALT_HSI = new Panel_AS_ALT_HSI();
	if (m_pAS_ALT_HSI == NULL)
		return -1;
	if (!m_pAS_ALT_HSI->Create(pt))
		return -1;
	return 0;
}

void CMainWnd::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	CRect rcDesktop;
	::GetWindowRect(::GetDesktopWindow(), &rcDesktop);
	::AdjustWindowRectEx(&rcDesktop, CWnd::GetStyle(), FALSE, CWnd::GetExStyle());
	lpMMI->ptMaxSize.x = rcDesktop.Width();
	lpMMI->ptMaxTrackSize.x = rcDesktop.Width();
	lpMMI->ptMaxSize.y = rcDesktop.Height();
	lpMMI->ptMaxTrackSize.y = rcDesktop.Height();
}

BOOL CMainWnd::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CMainWnd::OnPaint()
{
	using namespace Gdiplus;
	CPaintDC dc(this);
	Graphics g(dc.GetSafeHdc());
	g.DrawImage(m_pMemBMP, Rect(0, 0, m_pMemBMP->GetWidth(), m_pMemBMP->GetHeight()));
	Graphics *pg = Graphics::FromImage(m_pMemBMP);
	CRect rcClient;
	CWnd::GetClientRect(&rcClient);
	SolidBrush sb(Color::Black);
	pg->FillRectangle(&sb, rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height());
	delete pg;
}

void CMainWnd::OnTimer(UINT_PTR nIDEvent)
{
	using namespace std;
	if (nIDEvent == TIMER_ID_REFRESH)
	{
		using namespace Gdiplus;
		CRect rcClient;
		CWnd::GetClientRect(&rcClient);
		SolidBrush sb(Color::Black);
		m_pMemG->FillRectangle(&sb, rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height());
		static READING rd;
		rd.lAirSpeed = 98;
		rd.lAltitude += 1;
		if (rd.lAirSpeed > 180)
			rd.lAirSpeed = 0;
		if (rd.lAltitude > 99999)
			rd.lAltitude = 0;
		READING rd1 = {5, 0};
		rd.lQNH = 3061;
		m_pAS_ALT_HSI->SetReadingAndOutput(&rd, m_pMemG);
		//CString str;
		//str.Format(L"%d", rd.lAltitude);
		//m_pMemG->DrawString((LPCWSTR)str, -1, &Gdiplus::Font(L"宋体", 35, Gdiplus::FontStyleRegular), Gdiplus::PointF(0, 0), &Gdiplus::SolidBrush(Gdiplus::Color::White));
		CWnd::RedrawWindow();
	}
}
