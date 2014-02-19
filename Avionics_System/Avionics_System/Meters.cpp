#include "stdafx.h"
#include "Meters.h"
#include "MainWnd.h"

AirSpeed_Indicator::AirSpeed_Indicator() : m_Size(248, 247)
{
	
}

AirSpeed_Indicator::~AirSpeed_Indicator()
{
	if (m_pBkgund != NULL)
		delete m_pBkgund;
}

BOOL AirSpeed_Indicator::Initialize(LPCWSTR lpFileName, const Gdiplus::Point& ptLeftTop, const Gdiplus::Point& ptRotatePoint)
{
	m_pBkgund = Gdiplus::Image::FromFile(lpFileName, FALSE);
	if (m_pBkgund == NULL)
		return FALSE;

	m_ptLeftTop = ptLeftTop;
	m_ptRotatePoint = ptRotatePoint;

	m_Pointer.Initialize(pt_AS_Pointer, 5, m_ptRotatePoint);
	//m_ALT_Label.Initialize();
	return TRUE;
}

void AirSpeed_Indicator::SetReadingAndOutput(const PREADING pRading, Gdiplus::Graphics *pg)
{
	pg->DrawImage(m_pBkgund, m_ptLeftTop);
	m_Pointer.DrawPointer(pRading, pg);
}

void AirSpeed_Indicator::GetMeterSize(Gdiplus::Size *pSize)
{
	if (pSize == NULL)
		return;
	*pSize = m_Size;
}

void AirSpeed_Indicator::PT::DrawPointer(const PREADING pRading, Gdiplus::Graphics *pg)
{
	Gdiplus::GraphicsPath path;
	path.AddPath(&m_PT, FALSE);
	Gdiplus::Matrix m;
	float a = GetAngle(pRading);
	m.RotateAt(a, Gdiplus::PointF(m_ptRotatePoint.X, m_ptRotatePoint.Y));
	path.Transform(&m);
	Gdiplus::SolidBrush sb(Gdiplus::Color::White);
	pg->FillPath(&sb, &path);
}

void AirSpeed_Indicator::PT::Initialize(const Gdiplus::Point pt[], int Count, const Gdiplus::Point& ptRotatePoint)
{
	m_ptRotatePoint = ptRotatePoint;
	m_PT.AddLines(pt, 5);
}

float AirSpeed_Indicator::PT::GetAngle(const PREADING pRading)
{
	if (pRading == NULL)
		return 0;
	if (pRading->lAirSpeed <= 5)
		return 4.0F * 0.25F;
	if (pRading->lAirSpeed <= 40)
		return pRading->lAirSpeed * 0.25F;
	else
		return ((pRading->lAirSpeed - 40) * 2.0F)+10.0F;
}

Barometric_Altimeter::Barometric_Altimeter() : m_rcALTLabel(377, 138, 121, 35),
																						m_rcQNHLabel(501, 195, 55, 20)
{
	
}

Barometric_Altimeter::~Barometric_Altimeter()
{
	if (m_pBkgund != NULL)
		delete m_pBkgund;
}

BOOL Barometric_Altimeter::Initialize(LPCWSTR lpFileName, const Gdiplus::Point& ptLeftTop, const Gdiplus::Point& ptRotatePoint)
{
	m_pBkgund = Gdiplus::Image::FromFile(lpFileName, FALSE);
	if (m_pBkgund == NULL)
		return FALSE;

	m_ptLeftTop = ptLeftTop;
	m_RotatePoint = ptRotatePoint;

	m_Pointer.Initialize(pt_ALT_Pointer, 5, m_RotatePoint);
	if (!m_ALT_Label.Initialize(m_rcALTLabel))
		return FALSE;
	return m_QNH_Label.Initialize(m_rcQNHLabel);
}

void Barometric_Altimeter::SetReadingAndOutput(const PREADING pRading, Gdiplus::Graphics *pg)
{
	pg->DrawImage(m_pBkgund, m_ptLeftTop);
	m_Pointer.DrawPointer(pRading, pg);
	m_ALT_Label.DrawLabel(pRading, pg);
	m_QNH_Label.DrawLabel(pRading, pg);
}

void Barometric_Altimeter::GetMeterSize(Gdiplus::Size *pSize)
{
	if (pSize == NULL)
		return;

	pSize->Height = 247;
	pSize->Width = 248;
}

long Barometric_Altimeter::FormatReading(IN const PREADING pReading,
													OUT LPWSTR lpALTReading/*[6]*/, OUT LPWSTR lpQNHReading/*[5]*/)
{
	//pReading->lAltitude = 35454;
	if (pReading->lAltitude > 99999)
	{
		if (lpALTReading != NULL)
			ZeroMemory(lpALTReading, sizeof(WCHAR) * 6);
		if (lpQNHReading != NULL)
			ZeroMemory(lpQNHReading, sizeof(WCHAR) * 5);
		return 0;
	}
	
	long lAltitude = pReading->lAltitude;
	long lQNH = pReading->lQNH;
	BYTE w = 0, q = 0, b = 0, s = 0, g = 0;
	USHORT retVal = 0;
	w = lAltitude / 10000;//取万位
	q=(lAltitude-w*10000)/1000;//取千位
	b=(lAltitude-w*10000-q*1000)/100;//取百位
	s=(lAltitude-w*10000-q*1000-b*100)/10;//取十位
	g=lAltitude-w*10000-q*1000-b*100-s*10;
	retVal = b*10 + s + (g >= 5 ? 1 : 0);//气压高度表指针显示0-1000Ft(四舍五入个位).高度在1000Ft以下时高度标签显示精确高度

	if (lpALTReading != NULL)
	{
		if (FAILED(::StringCchPrintfW(lpALTReading, 6, L"%05d", lAltitude)))
			ZeroMemory(lpALTReading, sizeof(WCHAR) * 6);
	}
	if (lpQNHReading != NULL)
	{
		if (FAILED(::StringCchPrintfW(lpQNHReading, 5, L"%04d", lQNH)))
			ZeroMemory(lpQNHReading, sizeof(WCHAR) * 5);
	}

	return retVal;
}

float Barometric_Altimeter::PT::GetAngle(const PREADING pRading)
{
	return 3.6F * Barometric_Altimeter::FormatReading(pRading, NULL, NULL);
}

Barometric_Altimeter::ALT_Label::ALT_Label() : m_Font(L"宋体", 31),
																					m_Brush(Gdiplus::Color::White)
{
	
}

BOOL Barometric_Altimeter::ALT_Label::Initialize(const Gdiplus::Rect& rcLabel)
{
	if ( (m_Font.GetLastStatus() != Gdiplus::Ok) || (m_Font.GetLastStatus() != Gdiplus::Ok) )
		return FALSE;
	m_rcLabel.X = rcLabel.X;
	m_rcLabel.Y = rcLabel.Y;
	m_rcLabel.Width = rcLabel.Width;
	m_rcLabel.Height = rcLabel.Height;
	return TRUE;
}

void Barometric_Altimeter::ALT_Label::DrawLabel(const PREADING pRading, Gdiplus::Graphics *pg)
{
	//pg->SetClip(m_pClipBound, Gdiplus::CombineModeIntersect);
	WCHAR AltReading[10] = {};
	Barometric_Altimeter::FormatReading(pRading, AltReading, NULL);
	pg->DrawString(AltReading,  -1, &m_Font, m_rcLabel, NULL, &m_Brush);
	//pg->ResetClip();
}

Barometric_Altimeter::QNH_Label::QNH_Label() : m_Font(L"宋体", 16.625),
																							m_Brush(Gdiplus::Color::White)
{

}

BOOL Barometric_Altimeter::QNH_Label::Initialize(const Gdiplus::Rect& rcLabel)
{
	if ( (m_Font.GetLastStatus() != Gdiplus::Ok) || (m_Font.GetLastStatus() != Gdiplus::Ok) )
		return FALSE;
	m_rcLabel.X = rcLabel.X;
	m_rcLabel.Y = rcLabel.Y;
	m_rcLabel.Width = rcLabel.Width;
	m_rcLabel.Height = rcLabel.Height;
	return TRUE;
}

void Barometric_Altimeter::QNH_Label::DrawLabel(const PREADING pRading, Gdiplus::Graphics *pg)
{
	WCHAR QNHReading[10] = {};
	Barometric_Altimeter::FormatReading(pRading, NULL, QNHReading);
	pg->DrawString(QNHReading,  -1, &m_Font, m_rcLabel, NULL, &m_Brush);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Gdiplus::Size Panel_AS_ALT_HSI::sm_sizePanel(941, 317);//面板尺寸

Panel_AS_ALT_HSI::Panel_AS_ALT_HSI() :	m_pt_LeftTop_AS(52, 36),//51,36
																			m_pt_LeftTop_ALT(347, 35),
																			m_pt_LeftTop_HSI_Bkg(636, 9),
																			m_pt_LeftTop_HSI_HDG(645, 35),

																			m_pt_AS_RA(174, 159),
																			m_pt_ALT_RA(471, 159),
																			m_pt_HSI_RA(768, 159)
{
	using namespace Gdiplus;
	m_pBMP_AS_Path = L"Meters\\AS.png";
	m_pBMP_ALT_Path = L"Meters\\ALT.png";
	m_pBMP_HSI_Bkg_Path = L"Meters\\HSI.BKG.png";
	m_pBMP_HSI_HDG_Path = L"Meters\\HSI.HDG.png";

	//m_pt_AS_Pointer = pt_AS_Pointer;
	//m_pt_ALT_Pointer = pt_ALT_Pointer;
}

Panel_AS_ALT_HSI::~Panel_AS_ALT_HSI()
{
	
}

BOOL Panel_AS_ALT_HSI::Create(const Gdiplus::Point& pt)
{
	m_pt_LeftTop_Panel = pt;
	BOOL retVal = m_AS.Initialize(m_pBMP_AS_Path, m_pt_LeftTop_AS, m_pt_AS_RA);
	if (!retVal)
		return FALSE;
	retVal = m_ALT.Initialize(m_pBMP_ALT_Path, m_pt_LeftTop_ALT, m_pt_ALT_RA);
	if (!retVal)
		return FALSE;
	return retVal;
}

void Panel_AS_ALT_HSI::SetReadingAndOutput(const PREADING pRading, Gdiplus::Graphics *pg)
{
	Gdiplus::Rect rcPanel;
	Gdiplus::Pen pen(Gdiplus::Color::White);
	Panel_AS_ALT_HSI::GetPanelRect(&rcPanel);
	pg->DrawRectangle(&pen, rcPanel);
	pg->TranslateTransform(m_pt_LeftTop_Panel.X, m_pt_LeftTop_Panel.Y);
	m_AS.SetReadingAndOutput(pRading, pg);
	m_ALT.SetReadingAndOutput(pRading, pg);
	pg->ResetTransform();
}

void Panel_AS_ALT_HSI::GetPanelRect(Gdiplus::Rect *prc)
{
	if (prc == NULL)
		return;
	prc->X = m_pt_LeftTop_Panel.X;
	prc->Y = m_pt_LeftTop_Panel.Y;
	prc->Height = sm_sizePanel.Height;
	prc->Width = sm_sizePanel.Width;
}

