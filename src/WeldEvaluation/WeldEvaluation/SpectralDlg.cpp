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
		TRACE(_T("Main  %d, %d\n"), rcClientMain.Width(), rcClientMain.Height());

		CRect rcClient;
		pWnd->GetWindowRect(rcClient);
		ScreenToClient(rcClient);
		TRACE(_T("Graph %d, %d\n"), rcClient.Width(), rcClient.Height());

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

	int deltaWidth = ((CRect*)pRect)->Width() - rect.Width();
	int deltaHeight = ((CRect*)pRect)->Height() - rect.Height();

	CWnd *pWnd = GetDlgItem(IDC_STC_GRAPH);
	if (pWnd) {
		pWnd->GetWindowRect(rect);
		int sx = rect.Width() + deltaWidth;
		int sy = rect.Height() + deltaHeight;
		::SetWindowPos(pWnd->m_hWnd, NULL, 0, 0, sx, sy, (SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOMOVE));

		CRect rcClient;
		pWnd->GetWindowRect(rcClient);
		ScreenToClient(rcClient);
		m_pGraphWnd->MoveWindow(rcClient);
		pWnd->ShowWindow(SW_HIDE);
		m_pGraphWnd->ShowWindow(SW_SHOW);

		CRect rcClientMain;
		GetClientRect(&rcClientMain);
		m_orgClientSize = rcClientMain;
		TRACE(_T("OnSizing Main  %d, %d\n"), rcClientMain.Width(), rcClientMain.Height());
		m_orgClientSize = CRect(0, 0, 0, 0);
	}
	CDialog::OnSizing(fwSide, pRect);
}


int CSpectralDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}


void CSpectralDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if ((m_orgClientSize.Width() == 0) || (m_orgClientSize.Height() == 0)) {
		return;
	}
	TRACE(_T("OnSize %d, %d\n"), cx, cy);
	TRACE(_T("       %d, %d\n"), m_orgClientSize.Width(), m_orgClientSize.Height());


	CWnd *pWnd = GetDlgItem(IDC_STC_GRAPH);
	if (pWnd) {
		int deltaWidth = cx - m_orgClientSize.Width();
		int deltaHeight = cy - m_orgClientSize.Height();
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
		m_orgClientSize = rcClientMain;
	}
}

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
