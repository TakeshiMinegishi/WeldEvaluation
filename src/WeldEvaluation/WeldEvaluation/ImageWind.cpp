// ImageWind.cpp : 実装ファイル
//

#include "stdafx.h"
#include "WeldEvaluation.h"
#include "ImageWind.h"
#include "afxdialogex.h"
#include "message.h"
#include <vector>


// CImageWind ダイアログ

IMPLEMENT_DYNAMIC(CImageWind, CDialog)

#define BG_INIT_COLOR		RGB(0, 0, 0)
#define BG_COLOR			RGB(0, 0, 0)

/// <summary>
/// コンストラクタ
/// </summary>
CImageWind::CImageWind(CWnd* pParent /*=NULL*/)
	: CDialog(CImageWind::IDD, pParent)
{
	m_bActive		= false;
	m_Type			= eResinSurface;
	m_mode			= 0;
	m_bButtonDown	= false;
	m_bAriaSelect	= false;
	m_pImg			= nullptr;
	m_zoomRetio		= 1.0;
}

/// <summary>
/// デストラクタ
/// </summary>
CImageWind::~CImageWind()
{
	if (m_pImg) {
		delete m_pImg;
		m_pImg = nullptr;
	}
}

/// <summary>
/// データエクスチェンジ
/// </summary>
/// <param name="pDX"CDataExchangeオブジェクトへのポインター</param>
void CImageWind::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CImageWind, CDialog)
	ON_WM_PAINT()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEACTIVATE()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEWHEEL()
	ON_WM_NCDESTROY()
END_MESSAGE_MAP()


// CImageWind メッセージ ハンドラー

/// <summary>
/// 初期化処理
/// </summary>
BOOL CImageWind::OnInitDialog()
{
	m_bActive	= false;
	m_pImg		= new CImage();
	return TRUE;
}

/// <summary>
/// ウインド破棄前処理
/// </summary>
void CImageWind::OnNcDestroy()
{
	CDialog::OnNcDestroy();

	if (m_pImg) {
		m_pImg->Detach();
		m_pImg->Destroy();
		delete m_pImg;
		m_pImg = nullptr;
	}
}

/// <summary>
/// CImageオブジェクトの取得
/// </summary>
/// <param name="bDetach">Detachの実行有無</param>
/// <returns>CImageオブジェクトへのポインタを返す</returns>
CImage *CImageWind::GetImage(bool bDetach/* = true*/)
{
	if (m_pImg && bDetach) {
		if (!m_pImg->IsNull()) {
			m_pImg->Detach();
		}
	}
	return m_pImg;
}

/// <summary>
/// 描画処理
/// </summary>
void CImageWind::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rect;
	GetClientRect(&rect);
	CreateBackground(rect);

	if (m_bActive) {
		if (m_pImg == nullptr) {
			return;
		}

		CBitmap *bmp;
		CDC		*pDC = GetDC();
		CDC		cDC;				// ビットマップ表示用DC
		BITMAP	bm;					// ビットマップオブジェクト
		cDC.CreateCompatibleDC(pDC);
		bmp = CBitmap::FromHandle(*m_pImg);
		if (bmp) {
			CBitmap *oldBMP = cDC.SelectObject(bmp);
			bmp->GetBitmap(&bm);
			pDC->SetStretchBltMode(HALFTONE);
			pDC->SetStretchBltMode(COLORONCOLOR);
			pDC->StretchBlt(m_imageX, m_imageY, m_imageWidth, m_imageHeight, &cDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

			cDC.SelectObject(oldBMP);
		}
	}
}

/// <summary>
/// 背景描画
/// </summary>
/// <param name="bgArea">背景領域</param>
void CImageWind::CreateBackground(CRect &bgArea)
{
	CDC *dc = GetDC();
	if (dc == NULL)
	{
		return;
	}

	COLORREF BGCol;
	if (m_bActive) {
		BGCol = BG_COLOR;
	} else {
		BGCol = BG_INIT_COLOR;
	}
	CBrush blackBrush(BGCol);
	CBrush *orgBrush;
	orgBrush = dc->SelectObject(&blackBrush);
	dc->FillRect(&bgArea, &blackBrush);
	dc->SelectObject(orgBrush);
	ReleaseDC(dc);		
}

/// <summary>
/// 表示ウインドタイプの設定
/// </summary>
/// <param name="type">表示ウインドタイプ</param>
void CImageWind::setType(int type)
{
	m_Type = type;
}

/// <summary>
/// モードの設定
/// </summary>
/// <param name="type">表示ウインドタイプ</param>
/// @remark 0:ノーマルモード、1:区間スペクトルモード
void CImageWind::setMode(int mode)
{
	m_mode = mode;
}

/// <summary>
/// 画像の削除
/// </summary>
void CImageWind::Erase()
{
	m_bActive = false;
	OnPaint();
}

void CImageWind::Draw()
{
	CBitmap *virtualBmp;
	CDC *pDC = GetDC();
	CDC virtualDC;			// ビットマップ表示用DC
	BITMAP bm;				// ビットマップオブジェクト

	CRect sizeRect;
	GetClientRect(sizeRect);
	virtualBmp = CBitmap::FromHandle(*m_pImg);
	virtualDC.CreateCompatibleDC(pDC);
	CBitmap *oldBMP = virtualDC.SelectObject(virtualBmp);

	virtualBmp->GetBitmap(&bm);

	//--座標演算
	double flameAspect = (double)sizeRect.Height() / (double)sizeRect.Width();
	double imageAspect = (double)bm.bmHeight / (double)bm.bmWidth;
	m_orgImageWidth		= 0;
	m_orgImageHeight	= 0;
	m_orgImageX			= 0;
	m_orgImageY			= 0;

	if (flameAspect > imageAspect) {//--フレームアスペクト比＞画像アスペクト比
		// 横基準
		m_orgImageWidth		= sizeRect.Width();
		m_orgImageHeight	= (int)((double)bm.bmHeight*((double)sizeRect.Width() / (double)bm.bmWidth));
		m_orgImageX			= 0;
		m_orgImageY			= ((sizeRect.Height() - m_orgImageHeight) / 2);
	}
	else {
		// 縦基準
		m_orgImageWidth		= (int)((double)bm.bmWidth*((double)sizeRect.Height() / (double)bm.bmHeight));
		m_orgImageHeight	= sizeRect.Height();
		m_orgImageX			= ((sizeRect.Width() - m_orgImageWidth) / 2);
		m_orgImageY			= 0;
	}

	m_imageWidth  = (int)(m_orgImageWidth * m_zoomRetio);
	m_imageHeight = (int)(m_orgImageHeight * m_zoomRetio);
	m_imageX = m_orgImageX;
	m_imageY = m_orgImageY;

	pDC->SetStretchBltMode(HALFTONE); 
	pDC->SetStretchBltMode(COLORONCOLOR);
	pDC->StretchBlt(m_imageX, m_imageY, m_imageWidth, m_imageHeight, &virtualDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

	virtualDC.SelectObject(oldBMP);
	m_bActive = true;
}


void CImageWind::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	m_bButtonDown = false;
	m_bAriaSelect = false;

	CDialog::OnLButtonDblClk(nFlags, point);
}


void CImageWind::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_bButtonDown && m_bActive) {
		m_bButtonDown = false;
		double dist = sqrt((point.x - m_pLButtonDownosPos.x)*(point.x - m_pLButtonDownosPos.x) + (point.y - m_pLButtonDownosPos.y)*(point.y - m_pLButtonDownosPos.y));
		if (dist < 2.0) {
			CRect rect;
			GetWindowRect(rect);

			double wRetio = (double)m_imageWidth / (double)rect.Width();
			double hRetio = (double)m_imageHeight / (double)rect.Height();

			CPoint pos;
			pos.x = (int)((double)point.x * wRetio);
			pos.y = (int)((double)point.y * hRetio);
		}
		else {
			if (m_mode == 1) {
				m_bAriaSelect = false;
			}

			if (nFlags == MK_CONTROL) {
				TRACE(_T("Zoom (%d,%d)-(%d,%d)\n"), m_pLButtonDownosPos.x, m_pLButtonDownosPos.y, point.x, point.y);
			} else {
				AfxGetMainWnd()->GetActiveWindow()->SendMessage(WM_AREA_SPECTRUM_GRAPH_REQUEST, (WPARAM)&m_pLButtonDownosPos, (LPARAM)&point);
				TRACE(_T("Line (%d,%d)-(%d,%d)\n"), m_pLButtonDownosPos.x, m_pLButtonDownosPos.y, point.x, point.y);
			}
		}
	}
	TRACE(_T("LButton UP\n"));
	CDialog::OnLButtonUp(nFlags, point);
}

void CImageWind::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_bActive) {
		if (m_mode == 1) {
			if (m_bAriaSelect) {
				m_bAriaSelect = false;
				std::vector<CPoint> pos;
				CPoint cpt = m_pLButtonDownosPos;
				clientToScan(cpt);
				pos.push_back(cpt);
				cpt = point;
				clientToScan(cpt);
				pos.push_back(cpt);
				GetParent()->SendMessage(WM_AREA_SPECTRUM_GRAPH_SET, (WPARAM)m_Type, (LPARAM)&pos);
				pos.clear();
			}
			else {
				m_bButtonDown = true;
				m_bAriaSelect = true;
				m_pLButtonDownosPos = point;
				m_deltaPos = point;
			}
		}
		else {
			m_bButtonDown = true;
			m_pLButtonDownosPos = point;
			m_deltaPos = point;
		}
	}
	CDialog::OnLButtonDown(nFlags, point);
}


int CImageWind::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	TRACE(_T("OnMouseActivate\n"));
	return CDialog::OnMouseActivate(pDesktopWnd, nHitTest, message);
}


void CImageWind::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bActive) {
		if (m_bButtonDown) {
			CPoint delta;
			delta.x = m_deltaPos.x - point.x;
			delta.y = m_deltaPos.y - point.y;

			CRect rect;
			GetClientRect(rect);
			if (m_imageWidth > rect.Width()) {
				m_imageX -= delta.x;
				if (m_imageX > 0) {
					m_imageX = 0;
				}
				else if ((rect.Width() - m_imageWidth) > m_imageX) {
					m_imageX = (rect.Width() - m_imageWidth);
				}
			}

			if (m_imageHeight > 0) {
				m_imageY -= delta.y;
				if (m_imageY > 0) {
					m_imageY = 0;
				}
				else if ((rect.Height() - m_imageHeight)  > m_imageY) {
					m_imageY = (rect.Height() - m_imageHeight);
				}
				TRACE(_T("imageY:%d Height:%d-%d\n"), m_imageY, m_imageHeight, rect.Height());
			}
			if ((m_deltaPos.x != point.x) || (m_deltaPos.y != point.y)) {
				m_deltaPos = point;
				OnPaint();
			}
		}
		else {
			CPoint pos = point;
			if (clientToScan(pos)) {
				TRACE(_T("Pos(%d,%d)->(%d,%d)\n"), point.x, point.y, pos.x, pos.y);
				GetParent()->SendMessage(WM_SPECTRUM_GRAPH_REQUEST, (WPARAM)m_Type, (LPARAM)&pos);
			}
		}
	}

#if 0
	if (m_bButtonDown && m_bActive) {
		if (nFlags == MK_CONTROL) {
			TRACE(_T("OnMouseMove(%d,%d)\n"), point.x, point.y);
		}
	}
#endif
	CDialog::OnMouseMove(nFlags, point);
}


void CImageWind::OnMouseLeave()
{
	if (m_bButtonDown) {
		m_bAriaSelect = false;
		m_bButtonDown = false;
	}
	CDialog::OnMouseLeave();
}


BOOL CImageWind::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
#if 0
	if (m_bActive) {
		TRACE(_T("OnSetCursor()\n"));
		MOUSEMOVEPOINT pos;
		ZeroMemory(&pos, sizeof(pos));
		int nBufPoints = 10;
		MOUSEMOVEPOINT poss[10];

		int result = GetMouseMovePointsEx(sizeof(MOUSEMOVEPOINT), &pos, poss, nBufPoints, GMMP_USE_DISPLAY_POINTS);
		if (result >= 0) {
			if ((pos.x >= m_imageX) && (pos.x <= (m_imageX + m_imageWidth))) {
				if ((pos.y >= m_imageY) && (pos.y <= (m_imageY + m_imageHeight))) {
					SetCursor(AfxGetApp()->LoadCursor(IDC_CROSS));
				}
			}
		}
		else {
			DWORD err = GetLastError();
			LPVOID lpMsgBuf;
			FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_FROM_SYSTEM |
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				err,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPTSTR)&lpMsgBuf,
				0, NULL);
			TRACE(CString((LPTSTR)lpMsgBuf));
		}
	}
	else {
		SetCursor(m_hCursor);
	}
#endif
	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}


BOOL CImageWind::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if (m_bActive) {
		CPoint pos = pt;
		ScreenToClient(&pos);

		CRect rect;
		GetClientRect(&rect);
		CPoint delta;
		delta.x = pos.x - rect.Width() / 2;
		delta.y = pos.y - rect.Height() / 2;
		m_imageX -= delta.x;
		m_imageY -= delta.y;

		m_zoomRetio += ((double)zDelta / 120.0) * 0.1;
		m_imageWidth = (int)(m_orgImageWidth * m_zoomRetio);
		m_imageHeight = (int)(m_orgImageHeight * m_zoomRetio);;

		if (m_imageWidth > m_imageHeight) {
			if (m_imageHeight < rect.Height()) {
				m_zoomRetio = 1.0;
				m_imageWidth = m_orgImageWidth;
				m_imageHeight = m_orgImageHeight;
				m_imageY = (int)((rect.Height() - m_imageHeight) / 2.0);
			}
			if (m_imageWidth < rect.Width()) {
				m_imageX = (int)((rect.Width() - m_imageWidth) / 2.0);
			}
			else {

			}
		}
		else {
			if (m_imageHeight < rect.Height()) {
				m_imageY = (int)((rect.Height() - m_imageHeight) / 2.0);
			}
			if (m_imageWidth < rect.Width()) {
				m_zoomRetio = 1.0;
				m_imageWidth = m_orgImageWidth;
				m_imageHeight = m_orgImageHeight;
				m_imageX = (int)((rect.Width() - m_imageWidth) / 2.0);
			}
		}

		if (m_imageWidth > rect.Width()) {
			if (m_imageX > 0) {
				m_imageX = 0;
			}
			else if ((rect.Width() - m_imageWidth) > m_imageX) {
				m_imageX = (rect.Width() - m_imageWidth);
			}
		}

		if (m_imageHeight > 0) {
			if (m_imageY > 0) {
				m_imageY = 0;
			}
			else if ((rect.Height() - m_imageHeight) > m_imageY) {
				m_imageY = (rect.Height() - m_imageHeight);
			}
		}

		TRACE(_T("pos(%d,%d) Width= %d height = %d (%lf)\n"), pos.x, pos.y, m_imageWidth, m_imageHeight, m_zoomRetio);
		OnPaint();
	}
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

bool CImageWind::clientToScan(CPoint &pos)
{
	CPoint point = pos;
	if (((point.x < m_imageX) || (point.x >= (m_imageX + m_imageWidth))) ||
		((point.y < m_imageY) || (point.y >= (m_imageY + m_imageHeight)))) {
		return false;
	}
	point.x = (point.x - m_imageX) * m_pImg->GetHeight() / m_imageHeight;
	point.y = (point.y - m_imageY) * m_pImg->GetWidth() / m_imageWidth;
	pos = point;
	return true;
}

bool CImageWind::ScanToclient(CPoint &pos)
{
	CPoint point = pos;
	pos = point;
	return true;
}

void CImageWind::SetImagePos(CRect &rect)
{
	m_imageX		= rect.left;		///< 表示画像X位置
	m_imageY		= rect.top;			///< 表示画像Y位置
	m_imageWidth	= rect.Width();		///< 表示画像幅
	m_imageHeight	= rect.Height();	///< 表示画像高さ

	double		m_zoomRetio;					///< ズーム比

}
