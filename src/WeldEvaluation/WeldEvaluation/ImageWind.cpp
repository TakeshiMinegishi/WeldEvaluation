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

#define BG_INIT_COLOR		RGB(0, 0, 0)		///< 初期カラー値
#define BG_COLOR			RGB(0, 0, 0)		///< バックグラウンドカラー

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
			TRACE(_T("OnPaint(%d) X=%d Y=%d, W=%d H=%d\n"), m_Type, m_imageX, m_imageY, m_imageWidth, m_imageHeight);

			cDC.SelectObject(oldBMP);
		}
	}
}


/// <summary>
/// マウス左ボタンダブルクリック時処理
/// </summary>
/// <param name="nFlags">仮想キーフラグ</param>
/// <param name="point">カーソル座標</param>
/// <returns>成功の場合はTRUE、失敗の場合はFALSEを返す</returns>
/// @remark 仮想キーフラグnFlagsはMK_CONTROL CTRL キー、MK_LBUTTON マウスの左ボタン、MK_MBUTTON マウスの中央ボタン、MK_RBUTTON マウスの右ボタン、MK_SHIFT SHIFT キー
void CImageWind::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	m_bButtonDown = false;
	m_bAriaSelect = false;

	CDialog::OnLButtonDblClk(nFlags, point);
}


/// <summary>
/// マウス左ボタン押上げ時処理
/// </summary>
/// <param name="nFlags">仮想キーフラグ</param>
/// <param name="point">カーソル座標</param>
/// <returns>成功の場合はTRUE、失敗の場合はFALSEを返す</returns>
/// @remark 仮想キーフラグnFlagsはMK_CONTROL CTRL キー、MK_LBUTTON マウスの左ボタン、MK_MBUTTON マウスの中央ボタン、MK_RBUTTON マウスの右ボタン、MK_SHIFT SHIFT キー
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
//				TRACE(_T("Zoom (%d,%d)-(%d,%d)\n"), m_pLButtonDownosPos.x, m_pLButtonDownosPos.y, point.x, point.y);
			} else {
				AfxGetMainWnd()->GetActiveWindow()->SendMessage(WM_AREA_SPECTRUM_GRAPH_REQUEST, (WPARAM)&m_pLButtonDownosPos, (LPARAM)&point);
//				TRACE(_T("Line (%d,%d)-(%d,%d)\n"), m_pLButtonDownosPos.x, m_pLButtonDownosPos.y, point.x, point.y);
			}
		}
	}
//	TRACE(_T("LButton UP\n"));
	CDialog::OnLButtonUp(nFlags, point);
}

/// <summary>
/// マウス左ボタン押下時処理
/// </summary>
/// <param name="nFlags">仮想キーフラグ</param>
/// <param name="point">カーソル座標</param>
/// <returns>成功の場合はTRUE、失敗の場合はFALSEを返す</returns>
/// @remark 仮想キーフラグnFlagsはMK_CONTROL CTRL キー、MK_LBUTTON マウスの左ボタン、MK_MBUTTON マウスの中央ボタン、MK_RBUTTON マウスの右ボタン、MK_SHIFT SHIFT キー
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

/// <summary>
/// マウス移動時処理
/// </summary>
/// <param name="nFlags">仮想キーフラグ</param>
/// <param name="point">カーソル座標</param>
/// <returns>成功の場合はTRUE、失敗の場合はFALSEを返す</returns>
/// @remark 仮想キーフラグnFlagsはMK_CONTROL CTRL キー、MK_LBUTTON マウスの左ボタン、MK_MBUTTON マウスの中央ボタン、MK_RBUTTON マウスの右ボタン、MK_SHIFT SHIFT キー
void CImageWind::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bActive) {
		if (m_bButtonDown) {
			CPoint delta;
			delta.x = m_deltaPos.x - point.x;
			delta.y = m_deltaPos.y - point.y;

			int x = m_imageX;
			int y = m_imageY;
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
//				TRACE(_T("imageY:%d Height:%d-%d\n"), m_imageY, m_imageHeight, rect.Height());
			}
			if ((m_deltaPos.x != point.x) || (m_deltaPos.y != point.y)) {
				m_deltaPos = point;
				if ((x != m_imageX) || (y != m_imageY)) {
					OnPaint();
					CRect ImageRect(m_imageX, m_imageY, m_imageX + m_imageWidth, m_imageY + m_imageHeight);
					GetParent()->SendMessage(WM_IMAGE_MOVEING, (WPARAM)m_Type, (LPARAM)&ImageRect);
				}
			}
		}
		else {
			CPoint pos = point;
			if (clientToScan(pos)) {
//				TRACE(_T("Pos(%d,%d)->(%d,%d)\n"), point.x, point.y, pos.x, pos.y);
				GetParent()->SendMessage(WM_SPECTRUM_GRAPH_REQUEST, (WPARAM)m_Type, (LPARAM)&pos);
			}
		}
	}

#if 0
	if (m_bButtonDown && m_bActive) {
		if (nFlags == MK_CONTROL) {
//			TRACE(_T("OnMouseMove(%d,%d)\n"), point.x, point.y);
		}
	}
#endif
	CDialog::OnMouseMove(nFlags, point);
}

/// <summary>
/// マウスホイールを回転時処理
/// </summary>
/// <param name="nFlags">仮想キーフラグ</param>
/// <param name="zDelta">回転距離</param>
/// <param name="pt">カーソル座標</param>
/// <returns>成功の場合はTRUE、失敗の場合はFALSEを返す</returns>
/// @remark 仮想キーフラグnFlagsはMK_CONTROL CTRL キー、MK_LBUTTON マウスの左ボタン、MK_MBUTTON マウスの中央ボタン、MK_RBUTTON マウスの右ボタン、MK_SHIFT SHIFT キー
/// @remark 回転距離zDelta 値は120の倍数
BOOL CImageWind::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
#if 0
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

//		TRACE(_T("pos(%d,%d) Width= %d height = %d (%lf)\n"), pos.x, pos.y, m_imageWidth, m_imageHeight, m_zoomRetio);
		OnPaint();
	}
#else
	if (m_bActive) {
		CPoint pos = pt;
		ScreenToClient(&pos);

		CRect rect;
		GetClientRect(&rect);
		CPoint delta;
		delta.x = pos.x - rect.Width() / 2;
		delta.y = pos.y - rect.Height() / 2;
		pos.x = m_imageX - delta.x;
		pos.y = m_imageY - delta.y;
		double scalingRetio = m_zoomRetio + ((double)zDelta / 120.0) * 0.1;

		Scaling(scalingRetio, pos);
		CRect ImageRect(m_imageX, m_imageY, m_imageX + m_imageWidth, m_imageY + m_imageHeight);
		GetParent()->SendMessage(WM_IMAGE_SCALING, (WPARAM)m_Type, (LPARAM)&ImageRect);
	}
#endif
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

/// <summary>
/// マウスポインタがクライアント領域から出た通知
/// </summary>
void CImageWind::OnMouseLeave()
{
	if (m_bButtonDown) {
		m_bAriaSelect = false;
		m_bButtonDown = false;
	}
	CDialog::OnMouseLeave();
}


////////////////////////////////////////////////////////////////////////////////////////
//
//

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
	}
	else {
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
/// <param name="mode">表示ウインドタイプ</param>
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
	m_zoomRetio = 1.0;
	m_imageY = (int)0.0;
	m_imageX = (int)0.0;
	OnPaint();
}

/// <summary>
/// 画像の描画
/// </summary>
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
	m_orgImageWidth = 0;
	m_orgImageHeight = 0;
	m_orgImageX = 0;
	m_orgImageY = 0;

	if (flameAspect > imageAspect) {//--フレームアスペクト比＞画像アスペクト比
		// 横基準
		m_orgImageWidth = sizeRect.Width();
		m_orgImageHeight = (int)((double)bm.bmHeight*((double)sizeRect.Width() / (double)bm.bmWidth));
		m_orgImageX = 0;
		m_orgImageY = ((sizeRect.Height() - m_orgImageHeight) / 2);
	}
	else {
		// 縦基準
		m_orgImageWidth = (int)((double)bm.bmWidth*((double)sizeRect.Height() / (double)bm.bmHeight));
		m_orgImageHeight = sizeRect.Height();
		m_orgImageX = ((sizeRect.Width() - m_orgImageWidth) / 2);
		m_orgImageY = 0;
	}

	m_imageWidth = (int)(m_orgImageWidth * m_zoomRetio);
	m_imageHeight = (int)(m_orgImageHeight * m_zoomRetio);
	m_imageX = m_orgImageX;
	m_imageY = m_orgImageY;

	pDC->SetStretchBltMode(HALFTONE);
	pDC->SetStretchBltMode(COLORONCOLOR);
	pDC->StretchBlt(m_imageX, m_imageY, m_imageWidth, m_imageHeight, &virtualDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

	virtualDC.SelectObject(oldBMP);
	m_bActive = true;
}

/// <summary>
/// 画像の再配置
/// </summary>
/// <param name="x">左X点</param>
/// <param name="y">上Y点</param>
/// <param name="width">幅</param>
/// <param name="height">高さ</param>
/// <param name="scaingRetio">スケール比</param>
bool CImageWind::MoveImage(int x, int y, int width, int height, double scaingRetio)
{
	m_imageX		= x;
	m_imageY		= y;
	m_imageWidth	= width;
	m_imageHeight	= height;
	m_zoomRetio		= scaingRetio;

	TRACE(_T("Type:%d pos(%d,%d) IW= %d IH = %d (%lf)\n"),m_Type, x, y, m_imageWidth, m_imageHeight, m_zoomRetio);
	OnPaint();
	return true;
}

/// <summary>
/// 画像のスケーリング
/// </summary>
/// <param name="ScalingRetio">スケール比</param>
/// <param name="pt">画像位置</param>
void CImageWind::Scaling(double ScalingRetio, CPoint pt)
{
	if (!m_bActive) {
		return;
	}

	CRect rect;
	GetClientRect(&rect);
	int imageX = pt.x;
	int imageY = pt.y;
	double zoomRetio = ScalingRetio;

	int imageWidth = (int)(m_orgImageWidth * zoomRetio);
	int imageHeight = (int)(m_orgImageHeight * zoomRetio);

	if (imageWidth > imageHeight) {
		if (imageHeight < rect.Height()) {
			zoomRetio = 1.0;
			imageWidth = m_orgImageWidth;
			imageHeight = m_orgImageHeight;
			imageY = (int)((rect.Height() - imageHeight) / 2.0);
		}
		if (imageWidth < rect.Width()) {
			imageX = (int)((rect.Width() - imageWidth) / 2.0);
		}
		else {

		}
	}
	else {
		if (imageHeight < rect.Height()) {
			imageY = (int)((rect.Height() - imageHeight) / 2.0);
		}
		if (imageWidth < rect.Width()) {
			zoomRetio = 1.0;
			imageWidth = m_orgImageWidth;
			imageHeight = m_orgImageHeight;
			imageX = (int)((rect.Width() - imageWidth) / 2.0);
		}
	}

	if (imageWidth > rect.Width()) {
		if (imageX > 0) {
			imageX = 0;
		}
		else if ((rect.Width() - imageWidth) > m_imageX) {
			imageX = (rect.Width() - imageWidth);
		}
	}

	if (m_imageHeight > 0) {
		if (imageY > 0) {
			imageY = 0;
		}
		else if ((rect.Height() - imageHeight) > imageY) {
			imageY = (rect.Height() - imageHeight);
		}
	}
	MoveImage(imageX, imageY, imageWidth, imageHeight, zoomRetio);
}

/// <summary>
/// スケーリング情報の取得
/// </summary>
/// <param name="ScalingRetio">スケール比</param>
/// <param name="pt">画像位置</param>
/// <returns>成功の場合はtrue、失敗の場合はfalseを返す</returns>
bool CImageWind::GetScalingInfo(double &ScalingRetio, CPoint &pt)
{
	if (!m_bActive) {
		return false;
	}
	pt.x = m_imageX;
	pt.y = m_imageY;
	ScalingRetio = m_zoomRetio;

	return true;
}

/// <summary>
/// クライアント座標からスキャン画像座標への変換
/// </summary>
/// <param name="pos">(IN)クライアント座標/(OUT)スキャン画像座標</param>
/// <returns>成功の場合はtrue、失敗の場合はfalseを返す</returns>
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

/// <summary>
/// スキャン画像座標からクライアント座標への変換
/// </summary>
/// <param name="pos">(IN)スキャン画像座標/(OUT)クライアント座標</param>
/// <returns>成功の場合はtrue、失敗の場合はfalseを返す</returns>
bool CImageWind::ScanToclient(CPoint &pos)
{
	CPoint point = pos;
	pos = point;
	return true;
}

/// <summary>
/// 画像サイズの設定
/// </summary>
/// <param name="rect">画像サイズ</param>
void CImageWind::SetImagePos(CRect &rect)
{
	m_imageX		= rect.left;		///< 表示画像X位置
	m_imageY		= rect.top;			///< 表示画像Y位置
	m_imageWidth	= rect.Width();		///< 表示画像幅
	m_imageHeight	= rect.Height();	///< 表示画像高さ
}

/// <summary>
/// 点の位置を上下反転させる
/// </summary>
/// <param name="rect">点の位置</param>
/// <returns>反転した座標点を返す</returns>
CPoint CImageWind::ConvertImagePos(CPoint pos)
{
	CRect rect;
	CPoint rpos;
	GetWindowRect(rect);

	rpos.x = pos.x;
//	rpos.y = rect.Height() - (m_imageHeight + pos.y);
//	rpos.y = m_imageHeight - rect.Height() - pos.y;
	rpos.y = rect.Height() + pos.y - m_imageHeight - m_imageY;
	TRACE(_T("Type:%d Y:%d->%d VH=%d IH=%d IY=%d\n"),m_Type, pos.y,rpos.y, rect.Height(), m_imageHeight, m_imageY);
	return rpos;
}
