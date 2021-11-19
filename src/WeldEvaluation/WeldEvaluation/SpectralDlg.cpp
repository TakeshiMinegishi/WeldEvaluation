// SpectralDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "WeldEvaluation.h"
#include "SpectralDlg.h"
#include "afxdialogex.h"
#include "message.h"


// CSpectralDlg ダイアログ

IMPLEMENT_DYNAMIC(CSpectralDlg, CDialog)


/// <summary>
/// コンストラクタ
/// </summary>
CSpectralDlg::CSpectralDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_SPECTRAL_DLG, pParent)
{
	m_pParemt	= pParent;
	m_pGraphWnd = nullptr;
}

/// <summary>
/// デストラクタ
/// </summary>
CSpectralDlg::~CSpectralDlg()
{
	if (m_pGraphWnd) {
		delete m_pGraphWnd;
		m_pGraphWnd = nullptr;
	}
}

/// <summary>
/// データエクスチェンジ処理
/// </summary>
/// <param name="pDX">CDataExchangeオブジェクト</param>
void CSpectralDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSpectralDlg, CDialog)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_SIZING()
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CSpectralDlg メッセージ ハンドラー
/// <summary>
/// 初期化処理
/// </summary>
BOOL CSpectralDlg::OnInitDialog()
{
	// グラフウインド構築
	if (m_pGraphWnd == nullptr) {
		try {
			m_pGraphWnd = new CGraphWind;
			if (m_pGraphWnd->Create(IDD_GRAPH_WIND, this) == 0) {
				return FALSE;
			}
		}
		catch (CException *e) {
			e->Delete();
			return FALSE;
		}
	}

	CWnd *pWnd = GetDlgItem(IDC_STC_GRAPH);
	if (pWnd) {
		CRect rcClientMain;
		GetClientRect(&rcClientMain);
		m_orgClientSize = rcClientMain;
		m_bfClientSize  = rcClientMain;

		CRect rcClient;
		pWnd->GetWindowRect(rcClient);
		ScreenToClient(rcClient);

		m_pGraphWnd->MoveWindow(rcClient);
		pWnd->ShowWindow(SW_HIDE);
		m_pGraphWnd->ShowWindow(SW_SHOW);

	}
	return TRUE;
}

/// <summary>
/// ウインドクローズ処理
/// </summary>
void CSpectralDlg::OnClose()
{
	if (m_pParemt) {
		m_pParemt->PostMessageW(WM_SPECTRUME_CLOSE_REQUEST, NULL, NULL);
	}
	else {
		CDialog::OnClose();
	}
}

/// <summary>
/// ウインド破棄時処理
/// </summary>
void CSpectralDlg::OnDestroy()
{
	CDialog::OnDestroy();

	if (m_pGraphWnd) {
		delete m_pGraphWnd;
		m_pGraphWnd = nullptr;
	}

	// 全Viewを再描画
	m_pParemt->Invalidate(FALSE);
	m_pParemt->UpdateWindow();
}

/// <summary>
/// ウインドサイズ変更中の処理
/// </summary>
/// <param name="fwSide">移動するウィンドウの端</param>
/// <param name="pRect">変更後のサイズ</param>
void CSpectralDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
	CRect	rect;
	GetWindowRect(&rect);
	if (((CRect*)pRect)->Width() < m_orgClientSize.Width()) {
		pRect->left = rect.left;
		pRect->right = rect.right;
	}
	if (((CRect*)pRect)->Height() < m_orgClientSize.Height()) {
		pRect->top = rect.top;
		pRect->bottom = rect.bottom;
	}

	int deltaWidth = ((CRect*)pRect)->Width() - rect.Width();
	int deltaHeight = ((CRect*)pRect)->Height() - rect.Height();

	CWnd *pWnd = GetDlgItem(IDC_STC_GRAPH);
	if (pWnd) {
		pWnd->GetWindowRect(rect);
		TRACE(_T(" static rect  (%d, %d) W:%d, H:%d\n"), rect.left, rect.top, rect.Width(), rect.Height());

		int sx = rect.Width() + deltaWidth;
		int sy = rect.Height() + deltaHeight;
		::SetWindowPos(pWnd->m_hWnd, NULL, 0, 0, sx, sy, (SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOMOVE));
		pWnd->GetWindowRect(rect);
		TRACE(_T(" ->     rect  (%d, %d) W:%d, H:%d\n"), rect.left, rect.top, rect.Width(), rect.Height());

		CRect rcClient;
		pWnd->GetWindowRect(rcClient);
		ScreenToClient(rcClient);
		m_pGraphWnd->MoveWindow(rcClient);
//		pWnd->ShowWindow(SW_HIDE);
		m_pGraphWnd->Invalidate();
//		m_pGraphWnd->ShowWindow(SW_SHOW);
		m_bfClientSize = CRect(0, 0, 0, 0);

		m_pGraphWnd->GetWindowRect(rcClient);
		TRACE(_T(" graph rect      (%d, %d) W:%d, H:%d\n"), rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height());
	}
	CDialog::OnSizing(fwSide, pRect);
}

/// <summary>
/// ウインドの作成
/// </summary>
/// <param name="lpCreateStruct">CREATESTRUCTオブジェクトへのポインタ</param>
/// <returns>成功した場合は0、失敗した場合は0以外を返す</returns>
int CSpectralDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

/// <summary>
/// ウインドサイズの変更
/// </summary>
/// <param name="nType">ウインドタイプ</param>
/// <param name="cx">変更後のウインド幅</param>
/// <param name="cy">変更後のウインド高さ</param>
void CSpectralDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if ((m_bfClientSize.Width() == 0) || (m_bfClientSize.Height() == 0)) {
		return;
	}
	TRACE(_T("OnSize %d, %d\n"), cx, cy);
	TRACE(_T("       %d, %d\n"), m_bfClientSize.Width(), m_bfClientSize.Height());


	CWnd *pWnd = GetDlgItem(IDC_STC_GRAPH);
	if (pWnd) {
		int deltaWidth = cx - m_bfClientSize.Width();
		int deltaHeight = cy - m_bfClientSize.Height();
		TRACE(_T("       delta %d, %d\n"), deltaWidth, deltaHeight);

		CRect	rect;
		pWnd->GetWindowRect(rect);
		int sx = rect.Width() + deltaWidth;
		int sy = rect.Height() + deltaHeight;
		::SetWindowPos(pWnd->m_hWnd, NULL, 0, 0, sx, sy, (SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOMOVE));

		CRect rcClient;
		pWnd->GetWindowRect(rcClient);
		ScreenToClient(rcClient);
		m_pGraphWnd->MoveWindow(rcClient);

		CRect rcClientMain;
		GetClientRect(&rcClientMain);
		m_bfClientSize = rcClientMain;
	}
	m_pGraphWnd->OnPaint();
}

/// <summary>
/// スペクトルグラフの表示
/// </summary>
/// <param name="data">表示データ群</param>
void CSpectralDlg::draw(std::vector<std::vector<double>> data)
{
	m_data = data;
	CRect rect;
	m_pGraphWnd->GetWindowRect(rect);
	m_pGraphWnd->SetVirticalRange(0.0, 1.2);
	double h = (double)rect.Height() / 2.0;
	double offset = (int)(h / (double)data.size() + 0.5);
	m_pGraphWnd->Erase();
	m_pGraphWnd->Draw(m_data, offset, false);
}
