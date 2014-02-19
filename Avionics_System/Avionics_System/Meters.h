#pragma once


typedef struct _Reading
{
	long lAirSpeed;
	long lAltitude;
	long lQNH;
}READING, *PREADING;

class IPointer
{
public:
	virtual void Initialize(const Gdiplus::Point pt[], int Count, const Gdiplus::Point& ptRotatePoint) = 0;
	virtual void DrawPointer(const PREADING pRading, Gdiplus::Graphics *pg) = 0;
};

class IPointer2
{
public:
	virtual BOOL Initialize(LPCWSTR lpFileName, const Gdiplus::Point& ptRotatePoint) = 0;
	virtual void DrawPointer(const PREADING pRading, Gdiplus::Graphics *pg) = 0;
};

class ILabel
{
public:
	virtual BOOL Initialize(const Gdiplus::Rect& rcLabel) = 0;
	virtual void DrawLabel(const PREADING pRading, Gdiplus::Graphics *pg) = 0;
};

class IMeter
{
public:
	virtual BOOL Initialize(LPCWSTR lpFileName, const Gdiplus::Point& ptLeftTop, const Gdiplus::Point& ptRotatePoint) = 0;
	virtual void SetReadingAndOutput(const PREADING pRading, Gdiplus::Graphics *pg) = 0;
	virtual void GetMeterSize(Gdiplus::Size *pSize) = 0;
};

class IWarningLight
{
	
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Barometric_Altimeter;

extern __declspec(selectany)
const Gdiplus::Point pt_AS_Pointer[5] = {Gdiplus::Point(166, 165),	Gdiplus::Point(172, 169),
																		Gdiplus::Point(220, 87),		Gdiplus::Point(224, 71),
																		Gdiplus::Point(213, 83)};

extern __declspec(selectany)
const Gdiplus::Point pt_ALT_Pointer[5] = {Gdiplus::Point(467, 122),	Gdiplus::Point(475, 122),
																			Gdiplus::Point(475, 66),		Gdiplus::Point(471, 57),
																			Gdiplus::Point(467, 66)};

extern __declspec(selectany)
const Gdiplus::Point pt_ALT_Label[12] = {	Gdiplus::Point(379, 141),	Gdiplus::Point(425, 141),
																			Gdiplus::Point(425, 127/*6*/),	Gdiplus::Point(448, 127/*6*/),
																			Gdiplus::Point(448, 141),	Gdiplus::Point(494, 141),
																			Gdiplus::Point(494, 176),	Gdiplus::Point(448, 176),
																			Gdiplus::Point(448, 190/*1*/),	Gdiplus::Point(425, 190/*1*/),
																			Gdiplus::Point(425, 176),	Gdiplus::Point(379, 176)};


class AirSpeed_Indicator : public IMeter
{
	friend class Barometric_Altimeter;
	class PT : public IPointer
	{
	public:
		virtual void Initialize(const Gdiplus::Point pt[], int Count, const Gdiplus::Point& ptRotatePoint);
		virtual void DrawPointer(const PREADING pRading, Gdiplus::Graphics *pg);
	protected:
		virtual float GetAngle(const PREADING pRading);
	private:
		Gdiplus::GraphicsPath m_PT;
		Gdiplus::Point m_ptRotatePoint;
	};
public:
	AirSpeed_Indicator();
	~AirSpeed_Indicator();

	virtual BOOL Initialize(LPCWSTR lpFileName, const Gdiplus::Point& ptLeftTop, const Gdiplus::Point& ptRotatePoint);
	virtual void SetReadingAndOutput(const PREADING pRading, Gdiplus::Graphics *pg);
	virtual void GetMeterSize(Gdiplus::Size *pSize);

private:
	Gdiplus::Point m_ptLeftTop;
	Gdiplus::Image *m_pBkgund;
	Gdiplus::Size m_Size;
	Gdiplus::Point m_ptRotatePoint;
	PT m_Pointer;
};

class Barometric_Altimeter : public IMeter
{
	class ALT_Label : public ILabel
	{
	public:
		ALT_Label();
		//~ALT_Label();
		virtual BOOL Initialize(const Gdiplus::Rect& rcLabel);
		virtual void DrawLabel(const PREADING pRading, Gdiplus::Graphics *pg);
	protected:
		//Gdiplus::GraphicsPath *m_pClipBound;
		Gdiplus::Font m_Font;
		Gdiplus::SolidBrush m_Brush;
		Gdiplus::RectF m_rcLabel;
	};

	class QNH_Label : public ILabel
	{
	public:
		QNH_Label();
		virtual BOOL Initialize(const Gdiplus::Rect& rcLabel);
		virtual void DrawLabel(const PREADING pRading, Gdiplus::Graphics *pg);
	protected:
		Gdiplus::Font m_Font;
		Gdiplus::SolidBrush m_Brush;
		Gdiplus::RectF m_rcLabel;
	};

	class PT : public AirSpeed_Indicator::PT
	{
	protected:
		virtual float GetAngle(const PREADING pRading);
	};

public:
	Barometric_Altimeter();
	~Barometric_Altimeter();

	virtual BOOL Initialize(LPCWSTR lpFileName, const Gdiplus::Point& ptLeftTop, const Gdiplus::Point& ptRotatePoint);
	virtual void SetReadingAndOutput(const PREADING pRading, Gdiplus::Graphics *pg);
	virtual void GetMeterSize(Gdiplus::Size *pSize);
protected:
	static long FormatReading(IN const PREADING pReading, OUT LPWSTR lpALTReading/*[6]*/, OUT LPWSTR lpQNHReading/*[5]*/);
private:
	Gdiplus::Point m_ptLeftTop;
	Gdiplus::Image *m_pBkgund;
	Gdiplus::Size m_Size;
	Gdiplus::Point m_RotatePoint;
	PT m_Pointer;
	Gdiplus::Rect m_rcALTLabel;
	ALT_Label m_ALT_Label;
	Gdiplus::Rect m_rcQNHLabel;
	QNH_Label m_QNH_Label;
};

class HSI : public IMeter
{
	class Heading_Dial : public IPointer2
	{
		
	};

	class CRS_Pointer 
	{
		
	};
};

class Panel_AS_ALT_HSI
{
private:
	enum{AS_POINTER_COUNT = 5, ALT_POINTER_COUNT = 5,
				HSI_CRS_POINTER_COUNT = 18, HSI_HDG_POINTER_COUNT = 5,
				HSI_CDI_POINTER_COUNT = 4, HSI_FLAG_POINTER_COUNT = 6};
	LPCWSTR m_pBMP_AS_Path;//空速表表盘位图路径
	LPCWSTR m_pBMP_ALT_Path;//气压高度表表盘位图路径
	LPCWSTR m_pBMP_HSI_Bkg_Path;//HSI背景位图路径
	LPCWSTR m_pBMP_HSI_HDG_Path;//HSI航向表盘位图路径

	/*const*/ Gdiplus::Point m_pt_LeftTop_AS;//空速表表盘贴图位置
	/*const*/ Gdiplus::Point m_pt_LeftTop_ALT;//气压高度表表盘贴图位置
	/*const*/ Gdiplus::Point m_pt_LeftTop_HSI_Bkg;//HSI背景贴图位置
	/*const*/ Gdiplus::Point m_pt_LeftTop_HSI_HDG;//HSI航向表盘贴图位置

	/*const*/ Gdiplus::Point m_pt_AS_RA;//空速表指针旋转中心
	/*const*/ Gdiplus::Point m_pt_ALT_RA;//气压高度表指针旋转中心
	/*const*/ Gdiplus::Point m_pt_HSI_RA;//HSI指针旋转中心

	/*const*/ Gdiplus::Point *m_pt_AS_Pointer;//空速表指针5个点的位置
	/*const*/ Gdiplus::Point *m_pt_ALT_Pointer;//气压高度表指针5个点的位置
	/*const*/ Gdiplus::Point *m_pt_HSI_CRS_Pointer;//HSI  CRS指针18个点的位置
	/*const*/ Gdiplus::Point *m_pt_HSI_HDG_Pointer;//HSI  CRS指针5个点的位置
	/*const*/ Gdiplus::Point *m_pt_HSI_CDI_Pointer;//HSI  CDI(航道偏离指示器)指针4个点的位置
	/*const*/ Gdiplus::Point *m_pt_HSI_Flag_Pointer;//HSI  CDI(航道偏离指示器)指针6个点的位置

public:
	Panel_AS_ALT_HSI();
	~Panel_AS_ALT_HSI();

	BOOL Create(const Gdiplus::Point &pt);
	BOOL IsUpdated();
	void SetReadingAndOutput(const PREADING pRading, Gdiplus::Graphics *pg);
	void GetPanelRect(Gdiplus::Rect *prc);

private:
	Gdiplus::Point m_pt_LeftTop_Panel;
	static Gdiplus::Size sm_sizePanel;

private:
	AirSpeed_Indicator m_AS;
	Barometric_Altimeter m_ALT;
};