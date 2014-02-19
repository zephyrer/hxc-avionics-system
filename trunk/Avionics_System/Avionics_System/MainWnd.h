#pragma once

//class Panel
//{
//public:
//	Panel();
//	~Panel();
//	BOOL Create(LPCTSTR pFileName, POINT pt, Gdiplus::Graphics *pg);
//	BOOL IsUpdated();
//	virtual BOOL SetReading(PREADING pRading);
//protected:
//	Gdiplus::Graphics *m_pMemG;
//	//Gdiplus::Bitmap *m_pMenBmp;
//	Gdiplus::Bitmap *m_pBmpBkgund;
//	Gdiplus::Point m_ptPanel;
//	//std::vector<IPointer*> m_Pointers;
//};

class Panel_AS_ALT_HSI;

// CMainView


class CMainWnd : public CFrameWnd
{
	DECLARE_DYNAMIC(CMainWnd)
	enum {TIMER_ID_REFRESH = 1,};
public:
	CMainWnd();
	virtual ~CMainWnd();
	Gdiplus::Bitmap *m_pMemBMP;
	Gdiplus::Graphics *m_pMemG;
	Panel_AS_ALT_HSI *m_pAS_ALT_HSI;
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpcs);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
