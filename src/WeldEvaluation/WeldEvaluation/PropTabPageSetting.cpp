// PropTabPageSetting.cpp : 実装ファイル
//

#include "stdafx.h"
#include "WeldEvaluation.h"
#include "PropTabPageSetting.h"
#include "afxdialogex.h"
#include "WeldEvaluationDoc.h"
#include "FileUtil.h"

#pragma warning(disable:4100)

// CPropTabPageSetting ダイアログ

IMPLEMENT_DYNAMIC(CPropTabPageSetting, CDialogEx)

/// <summary>
/// コンストラクタ
/// </summary>
CPropTabPageSetting::CPropTabPageSetting(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPropTabPageSetting::IDD, pParent)
	, m_RegistFolder(_T(""))
	, m_NumberOfOverridePixel(1)
	, m_IntegrationTimeMs(100)
	, m_ResolutionHolizontal(1000)
	, m_ResolutionVertical(100)
	, m_WBFileName(_T(""))
	, m_ScrollBerPos(0)
{
}

/// <summary>
/// デストラクタ
/// </summary>
CPropTabPageSetting::~CPropTabPageSetting()
{
}

/// <summary>
/// データエクスチェンジ処理
/// </summary>
/// <param name="pDX">CDataExchangeオブジェクト</param>
void CPropTabPageSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_REGFOLDER, m_RegistFolder);
	DDX_Text(pDX, IDC_EDT_NUMBEROFOVERRAPPINGPIXEL, m_NumberOfOverridePixel);
	DDX_Text(pDX, IDC_EDT_INTEGRATIONTIME, m_IntegrationTimeMs);
	DDX_Text(pDX, IDC_EDT_RESOLUTION_HOLIZONTAL, m_ResolutionHolizontal);
	DDX_Text(pDX, IDC_EDT_RESOLUTION_VERTICAL, m_ResolutionVertical);
	DDX_Text(pDX, IDC_EDT_WBFILENAME, m_WBFileName);
	DDX_Control(pDX, IDC_SBR_SETTING, m_sbrSettingDlg);
	DDX_Scroll(pDX, IDC_SBR_SETTING, m_ScrollBerPos);
}

/// <summary>
/// ダイアログの初期化
/// </summary>
BOOL CPropTabPageSetting::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	return TRUE;
}

BEGIN_MESSAGE_MAP(CPropTabPageSetting, CDialogEx)
	ON_BN_CLICKED(IDC_BUN_REGFOLDER_FIND, &CPropTabPageSetting::OnBnClickedBunRegfolderFind)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPN__NUMBEROFOVERRAPPINGPIXEL, &CPropTabPageSetting::OnDeltaposSpn)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPN_INTEGRATIONTIME, &CPropTabPageSetting::OnDeltaposSpnIntegrationtime)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPN_RESOLUTION_HOLIZONTAL, &CPropTabPageSetting::OnDeltaposSpnResolutionHolizontal)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPN_RESOLUTION_VERTICAL, &CPropTabPageSetting::OnDeltaposSpnResolutionVertical)
	ON_WM_ACTIVATE()
	ON_EN_KILLFOCUS(IDC_EDT_REGFOLDER, &CPropTabPageSetting::OnEnKillfocusEdtRegfolder)
	ON_EN_KILLFOCUS(IDC_EDT_NUMBEROFOVERRAPPINGPIXEL, &CPropTabPageSetting::OnEnKillfocusEdtNumberofoverrappingpixel)
	ON_EN_KILLFOCUS(IDC_EDT_INTEGRATIONTIME, &CPropTabPageSetting::OnEnKillfocusEdtIntegrationtime)
	ON_EN_KILLFOCUS(IDC_EDT_RESOLUTION_HOLIZONTAL, &CPropTabPageSetting::OnEnKillfocusEdtResolutionHolizontal)
	ON_EN_KILLFOCUS(IDC_EDT_RESOLUTION_VERTICAL, &CPropTabPageSetting::OnEnKillfocusEdtResolutionVertical)
	ON_EN_KILLFOCUS(IDC_EDT_WBFILENAME, &CPropTabPageSetting::OnEnKillfocusEdtWbfilename)
	ON_NOTIFY(NM_THEMECHANGED, IDC_SBR_SETTING, &CPropTabPageSetting::OnNMThemeChangedSbrSetting)
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


// CPropTabPageSetting メッセージ ハンドラー


void CPropTabPageSetting::OnBnClickedBunRegfolderFind()
{
	UpdateData(true);
	CFolderPickerDialog dlg;
	if (!m_RegistFolder.IsEmpty()) {
		dlg.m_ofn.lpstrInitialDir = m_RegistFolder;
	}
	if( dlg.DoModal() != IDOK )
		return;

	CString folder = dlg.GetPathName() ;
    if (m_RegistFolder != folder) {
		m_RegistFolder = folder;
		UpdateData(false);
		// 更新ボタンの更新
		CWnd *pWnd = GetParent()->GetParent();
		pWnd->SendMessage(WM_UPDATEREQUEST_PROPPAGE,(WPARAM)true,(LPARAM)0);
	}
}

void CPropTabPageSetting::OnEnKillfocusEdtRegfolder()
{
	CString folder = m_RegistFolder;
	UpdateData(true);
	if (m_RegistFolder != folder) {
		// 更新ボタンの更新
		UpdateData(false);
		CWnd *pWnd = GetParent()->GetParent();
		pWnd->SendMessage(WM_UPDATEREQUEST_PROPPAGE,(WPARAM)true,(LPARAM)0);
	}
}


void CPropTabPageSetting::OnEnKillfocusEdtWbfilename()
{
	CString org = m_WBFileName;
	UpdateData(true);
	if (m_WBFileName != org) {
		UpdateData(false);
		// 更新ボタンの更新
		CWnd *pWnd = GetParent()->GetParent();
		pWnd->SendMessage(WM_UPDATEREQUEST_PROPPAGE,(WPARAM)true,(LPARAM)0);
	}
}


void CPropTabPageSetting::OnDeltaposSpn(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	*pResult = 0;
	UpdateData(true);
	int Min = 1;
	int Max = 100;
	int ival = (int)m_NumberOfOverridePixel;
	ival -= pNMUpDown->iDelta;
	if (ival < Min) {
		ival = Min;
	} else if (ival > Max) {
		ival = Max;
	}
	m_NumberOfOverridePixel = (UINT)ival;
	UpdateData(false);
	// 更新ボタンの更新
	CWnd *pWnd = GetParent()->GetParent();
	pWnd->SendMessage(WM_UPDATEREQUEST_PROPPAGE,(WPARAM)true,(LPARAM)0);
}

void CPropTabPageSetting::OnEnKillfocusEdtNumberofoverrappingpixel()
{
	UINT org = m_NumberOfOverridePixel;
	UpdateData(true);
	if (m_NumberOfOverridePixel != org) {
		UpdateData(false);
		// 更新ボタンの更新
		CWnd *pWnd = GetParent()->GetParent();
		pWnd->SendMessage(WM_UPDATEREQUEST_PROPPAGE,(WPARAM)true,(LPARAM)0);
	}
}


void CPropTabPageSetting::OnDeltaposSpnIntegrationtime(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	*pResult = 0;
	UpdateData(true);
	int Min = 1;
	int Max = 60000;
	int ival = (int)m_IntegrationTimeMs;
	ival -= pNMUpDown->iDelta;
	if (ival < Min) {
		ival = Min;
	} else if (ival > Max) {
		ival = Max;
	}
	m_IntegrationTimeMs = (UINT)ival;
	UpdateData(false);
	// 更新ボタンの更新
	CWnd *pWnd = GetParent()->GetParent();
	pWnd->SendMessage(WM_UPDATEREQUEST_PROPPAGE,(WPARAM)true,(LPARAM)0);
}

void CPropTabPageSetting::OnEnKillfocusEdtIntegrationtime()
{
	UINT org = m_IntegrationTimeMs;
	UpdateData(true);
	if (m_IntegrationTimeMs != org) {
		UpdateData(false);
		// 更新ボタンの更新
		CWnd *pWnd = GetParent()->GetParent();
		pWnd->SendMessage(WM_UPDATEREQUEST_PROPPAGE,(WPARAM)true,(LPARAM)0);
	}
}

void CPropTabPageSetting::OnDeltaposSpnResolutionHolizontal(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	*pResult = 0;
	UpdateData(true);
	int Min = 1;
	int Max = 1024;
	int ival = (int)m_ResolutionHolizontal;
	ival -= pNMUpDown->iDelta;
	if (ival < Min) {
		ival = Min;
	} else if (ival > Max) {
		ival = Max;
	}
	m_ResolutionHolizontal = (UINT)ival;
	UpdateData(false);
	// 更新ボタンの更新
	CWnd *pWnd = GetParent()->GetParent();
	pWnd->SendMessage(WM_UPDATEREQUEST_PROPPAGE,(WPARAM)true,(LPARAM)0);
}

void CPropTabPageSetting::OnEnKillfocusEdtResolutionHolizontal()
{
	UINT org = m_ResolutionHolizontal;
	UpdateData(true);
	if (m_ResolutionHolizontal != org) {
		UpdateData(false);
		// 更新ボタンの更新
		CWnd *pWnd = GetParent()->GetParent();
		pWnd->SendMessage(WM_UPDATEREQUEST_PROPPAGE,(WPARAM)true,(LPARAM)0);
	}
}


void CPropTabPageSetting::OnDeltaposSpnResolutionVertical(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	*pResult = 0;
	UpdateData(true);
	int Min = 1;
	int Max = 1024;
	int ival = (int)m_ResolutionVertical;
	ival -= pNMUpDown->iDelta;
	if (ival < Min) {
		ival = Min;
	} else if (ival > Max) {
		ival = Max;
	}
	m_ResolutionVertical = (UINT)ival;
	UpdateData(false);
	// 更新ボタンの更新
	CWnd *pWnd = GetParent()->GetParent();
	pWnd->SendMessage(WM_UPDATEREQUEST_PROPPAGE,(WPARAM)true,(LPARAM)0);
}

void CPropTabPageSetting::OnEnKillfocusEdtResolutionVertical()
{
	UINT org = m_ResolutionVertical;
	UpdateData(true);
	if (m_ResolutionVertical != org) {
		UpdateData(false);
		// 更新ボタンの更新
		CWnd *pWnd = GetParent()->GetParent();
		pWnd->SendMessage(WM_UPDATEREQUEST_PROPPAGE,(WPARAM)true,(LPARAM)0);
	}
}


void CPropTabPageSetting::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);
	LoadParamater();
}


void CPropTabPageSetting::LoadParamater(void)
{
	CFormView *pWnd = (CFormView *)GetParent()->GetParent();
	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)pWnd->GetDocument();

	m_RegistFolder = pDoc->GetRegistedFolder();
	m_WBFileName = pDoc->GetWBFileName();
	m_NumberOfOverridePixel = pDoc->NumberOfOverridePixel();
	m_IntegrationTimeMs = pDoc->GetIntegrationTimeMs();
	m_ResolutionHolizontal = pDoc->GetHorizontalResolution();
	m_ResolutionVertical = pDoc->GetVerticalResolution();
	UpdateData(false);
}

/// <summary>
/// アイテムのアクティブ/ディアクティブ化
/// </summary>
/// <param name="ItemID">アイテムID</param>
/// <param name="bActive">アクティブフラグ</param>
void CPropTabPageSetting::ItemEnable(int ItemID,bool bActive)
{
	CWnd *pWnd;
	pWnd = GetDlgItem(ItemID);
	if (pWnd) {
		pWnd->EnableWindow(bActive);
	}
}

/// <summary>
/// アイテムのアクティブ/ディアクティブ化
/// </summary>
/// <param name="bActive">アクティブフラグ</param>
void CPropTabPageSetting::ItemActive(bool bActive)
{
	ItemEnable(IDC_STC_NUMBEROFOVERRAPPINGPIXEL_LABEL,bActive);
	ItemEnable(IDC_EDT_NUMBEROFOVERRAPPINGPIXEL,bActive);
	ItemEnable(IDC_SPN__NUMBEROFOVERRAPPINGPIXEL,bActive);

	ItemEnable(IDC_STC_INTEGRATIONTIME_LABEL,bActive);
	ItemEnable(IDC_EDT_INTEGRATIONTIME,bActive);
	ItemEnable(IDC_SPN_INTEGRATIONTIME,bActive);

	ItemEnable(IDC_STC_RESOLUTION_LABEL,bActive);
	ItemEnable(IDC_STC_RESOLUTION_HOLIZONTAL_LABEL,bActive);
	ItemEnable(IDC_EDT_RESOLUTION_HOLIZONTAL,bActive);
	ItemEnable(IDC_SPN_RESOLUTION_HOLIZONTAL,bActive);

	ItemEnable(IDC_STC_RESOLUTION_VERTICAL_LABEL,bActive);
	ItemEnable(IDC_EDT_RESOLUTION_VERTICAL,bActive);
	ItemEnable(IDC_SPN_RESOLUTION_VERTICAL,bActive);
}


void CPropTabPageSetting::OnNMThemeChangedSbrSetting(NMHDR *pNMHDR, LRESULT *pResult)
{
	// この機能は Windows XP それ以降のバージョンを必要とします。
	// シンボル _WIN32_WINNT は >= 0x0501にならなければなりません。
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	*pResult = 0;
}

void CPropTabPageSetting::FitRect(CRect rect)
{
	CRect rr;
	m_sbrSettingDlg.GetWindowRect(rr);
	
	CRect sRect = rect;
	sRect.left = sRect.right - rr.Width();

	m_sbrSettingDlg.MoveWindow(sRect);

	CRect wr;
	GetWindowRect(wr);

	int h = GetSystemMetrics(SM_CYVSCROLL);

	m_MinScrollPos = 0;
	h = sRect.Height() - h*2;
	m_MaxScrollPos = (int)((double)h * (double)h/(double)wr.Height());
	m_ScrollDelta = 0;
	m_sbrSettingDlg.SetScrollRange(m_MinScrollPos,m_MaxScrollPos);
}

void CPropTabPageSetting::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int nID = pScrollBar->GetDlgCtrlID();
	int pos = pScrollBar->GetScrollPos();
	BOOL bIsChange = FALSE;

	if (nID == IDC_SBR_SETTING) {
		int min = m_MinScrollPos;
		int max = m_MaxScrollPos;
		switch (nSBCode)
		{
		case SB_LINEUP:
			if (pos > min)
			{
				bIsChange = TRUE;
				--pos;
			}
			break;
		case SB_LINEDOWN:
			if (pos < max)
			{
				bIsChange = TRUE;
				++pos;
			}
			break;
		case SB_THUMBPOSITION:
			bIsChange = TRUE;
			pos = nPos;
			break;
		case SB_PAGEUP:
			if (pos > min)
			{
				int step = (max - min)/10;
				bIsChange = TRUE;
				pos -= step;
				if (pos <= min) {
					pos = min;
				}
			}
			break;
		case SB_PAGEDOWN:
			if (pos < max)
			{
				int step = (max - min)/10;
				bIsChange = TRUE;
				pos += step;
				if (pos >= max) {
					pos = max;
				}
			}
			break;
		case WM_MOUSEWHEEL:
			{
			}
			break;
		default:
			break;
		}
		// スクロール位置に変更があった場合
		if (bIsChange)
		{
			// スクロール位置を更新します。
			SCROLLINFO si;
			ZeroMemory(&si, sizeof(si));
			pScrollBar->GetScrollInfo(&si);
			si.nPos = pos;
			si.fMask = SIF_POS;
			pScrollBar->SetScrollInfo(&si);

			MoveItem(pos);
			InvalidateRect(NULL,TRUE);
		}
	}
}

void CPropTabPageSetting::VMove(int ItemId, int offset)
{
	CRect rect;
	CWnd *pWnd;
	pWnd = GetDlgItem(ItemId);
	if (pWnd == NULL) {
		return;
	}
	pWnd->GetClientRect(rect);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	rect.top -= offset;
	pWnd->SetWindowPos(NULL,rect.left,rect.top,NULL,NULL,SWP_NOSIZE|SWP_NOZORDER);
}

void CPropTabPageSetting::MoveItem(int pos)
{
	int offset = pos - m_ScrollDelta;
	VMove(IDC_STC_REGFOLDER_LABEL,offset);
	VMove(IDC_EDT_REGFOLDER,offset);
	VMove(IDC_BUN_REGFOLDER_FIND,offset);
	VMove(IDC_STC_WBFILENAME_LABEL,offset);
	VMove(IDC_EDT_WBFILENAME,offset);
	VMove(IDC_STC_NUMBEROFOVERRAPPINGPIXEL_LABEL,offset);
	VMove(IDC_EDT_NUMBEROFOVERRAPPINGPIXEL,offset);
	VMove(IDC_SPN__NUMBEROFOVERRAPPINGPIXEL,offset);
	VMove(IDC_STC_INTEGRATIONTIME_LABEL,offset);
	VMove(IDC_EDT_INTEGRATIONTIME,offset);
	VMove(IDC_SPN_INTEGRATIONTIME,offset);
	VMove(IDC_STC_RESOLUTION_LABEL,offset);
	VMove(IDC_STC_RESOLUTION_HOLIZONTAL_LABEL,offset);
	VMove(IDC_EDT_RESOLUTION_HOLIZONTAL,offset);
	VMove(IDC_SPN_RESOLUTION_HOLIZONTAL,offset);

	VMove(IDC_STC_RESOLUTION_VERTICAL_LABEL,offset);
	VMove(IDC_EDT_RESOLUTION_VERTICAL,offset);
	VMove(IDC_SPN_RESOLUTION_VERTICAL,offset);

	m_ScrollDelta = pos;
}


bool CPropTabPageSetting::Update(void)
{
	bool bResult = true;
	UpdateData(true);

	CFormView *pWnd = (CFormView *)GetParent()->GetParent();
	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)pWnd->GetDocument();

	CString RegistFolder = pDoc->GetRegistedFolder();
	if (m_RegistFolder != RegistFolder) {
		if (!pDoc->SetRegistedFolder(m_RegistFolder)) {
			bResult = false;
		} else {
			pWnd->SendMessage(WM_CHANGE_REGIST,NULL,NULL);
		}
	}

	CString WBFileName = pDoc->GetWBFileName();
	if (m_WBFileName != WBFileName) {
		if (!pDoc->SetWBFileName(m_WBFileName)) {
			bResult = false;
		}
	}

	UINT uprm;
	uprm = pDoc->NumberOfOverridePixel();
	if (m_NumberOfOverridePixel != uprm) {
		if (!pDoc->SetOverridePixelNumber(m_NumberOfOverridePixel)) {
			bResult = false;
		}
	}
	uprm = pDoc->GetIntegrationTimeMs();
	if (m_IntegrationTimeMs != uprm) {
		if (!pDoc->SetIntegrationTimeMs(m_IntegrationTimeMs)) {
			bResult = false;
		}
	}
	uprm = pDoc->GetHorizontalResolution();
	if (m_ResolutionHolizontal != uprm) {
		if (!pDoc->SetHorizontalResolution(m_ResolutionHolizontal)) {
			bResult = false;
		}
	}
	uprm = pDoc->GetVerticalResolution();
	if (m_ResolutionVertical != uprm) {
		if (!pDoc->SetVerticalResolution(m_ResolutionVertical)) {
			bResult = false;
		}
	}
	return bResult;
}

/// <summary>
/// マウスホイールの回転イベント
/// </summary>
/// <param name="nFlags">仮想キーフラグ</param>
/// <param name="zDelta">回転距離</param>
/// <param name="pt">カーソル位置</param>
/// <returns>マウス ホイールのスクロールが有効ならtrue,そうでなければfalseを返す</returns>
/// @remark 独自スクロールバーに回転情報を変化して移動距離を渡す
BOOL CPropTabPageSetting::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if (zDelta < 0) {
		OnVScroll(SB_PAGEDOWN, m_sbrSettingDlg.GetScrollPos(), &m_sbrSettingDlg);
	} else{
		OnVScroll(SB_PAGEUP, m_sbrSettingDlg.GetScrollPos(), &m_sbrSettingDlg);
	}
	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}
