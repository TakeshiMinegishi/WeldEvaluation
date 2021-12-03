// OprtTabPageScan.cpp : 実装ファイル
//

#include "stdafx.h"
#include "WeldEvaluation.h"
#include "OprtTabPageScan.h"
#include "afxdialogex.h"
#include "WeldEvaluationDoc.h"
#include "message.h"

#pragma warning(disable:4100)

// COprtTabPageScan ダイアログ

IMPLEMENT_DYNAMIC(COprtTabPageScan, CDialogEx)

/// <summary>
/// 初期化コンストラクタ
/// </summary>
/// <param name="pParent">親ウインドへのポインタ</param>
COprtTabPageScan::COprtTabPageScan(CWnd* pParent /*=NULL*/)
	: CDialogEx(COprtTabPageScan::IDD, pParent)
	, m_ScanTarget(0)
{

}

/// <summary>
/// デストラクタ
/// </summary>
COprtTabPageScan::~COprtTabPageScan()
{
}

/// <summary>
/// データエクスチェンジ処理
/// </summary>
/// <param name="pDX">CDataExchangeオブジェクト</param>
void COprtTabPageScan::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RDO_REGIN, m_ScanTarget);
}


BEGIN_MESSAGE_MAP(COprtTabPageScan, CDialogEx)
	ON_WM_ACTIVATE()
	ON_BN_CLICKED(IDC_BTN_SCAN, &COprtTabPageScan::OnBnClickedBtnScan)
	ON_BN_CLICKED(IDC_BTN_INVERS, &COprtTabPageScan::OnBnClickedBtnInvers)
	ON_BN_CLICKED(IDC_RDO_REGIN, &COprtTabPageScan::OnBnClickedRdoRegin)
	ON_BN_CLICKED(IDC_RDO_METAL, &COprtTabPageScan::OnBnClickedRdoMetal)
	ON_BN_CLICKED(IDC_RSO_RESULT, &COprtTabPageScan::OnBnClickedRsoResult)
END_MESSAGE_MAP()


// COprtTabPageScan メッセージ ハンドラー


/// <summary>
/// アイテムのアクティブ/ディアクティブ化
/// </summary>
/// <param name="ItemID">アイテムID</param>
/// <param name="bActive">アクティブフラグ</param>
void COprtTabPageScan::ItemEnable(int ItemID,bool bActive)
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
void COprtTabPageScan::ItemActive(bool bActive)
{
	ItemEnable(IDC_STC_SCANFRAME,bActive);
	ItemEnable(IDC_RDO_REGIN,bActive);
	ItemEnable(IDC_RDO_METAL,bActive);
	ItemEnable(IDC_RSO_RESULT,bActive);
	ItemEnable(IDC_BTN_SCAN,bActive);
	ItemEnable(IDC_BTN_CANCEL,bActive);
	if (bActive) {
		UpdateInversBtnState();
	}
}

/// <summary>
/// パラメータの読み込み
/// </summary>
void COprtTabPageScan::LoadParamater(void)
{
}

/// <summary>
/// スキャンされていない対象の取得
/// </summary>
/// <returns>スキャンされていない対象のIDを返す</returns>
int COprtTabPageScan::GetDeactiveScan()
{
	int targetID = -1;
	CFormView *pWnd = (CFormView *)GetParent()->GetParent();
	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)pWnd->GetDocument();
	if (!pDoc->IsOpen()) {
		return targetID;
	}
	if (pDoc->ExistScanFile(CWeldEvaluationDoc::eResinSurface)) {
		if (pDoc->ExistScanFile(CWeldEvaluationDoc::eMetalSurface)) {
			if (pDoc->ExistScanFile(CWeldEvaluationDoc::eJoiningResult)) {
				if (pDoc->IsNew()) {
					targetID = 0;
				} else {
					targetID = -1;
				}
			} else {
				targetID = 2;
			}
		} else {
			targetID = 1;
		}
	} else {
		targetID = 0;
	}
	return targetID;
}

/// <summary>
/// ウインドアクティブ時処理
/// </summary>
/// <param name="nState">アクティブフラグ</param>
/// <param name="pWndOther">対象ウインドへのポインタ</param>
/// <param name="bMinimized">最小化フラグ</param>
void COprtTabPageScan::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);

	int id = GetDeactiveScan();
	if (id < 0) {
		id = 2;
	}
	m_ScanTarget = id;
	UpdateData(false);
	UpdateInversBtnState();
}

/// <summary>
/// スキャンボタン押下時処理
/// </summary>
void COprtTabPageScan::OnBnClickedBtnScan()
{
	CFormView *pWnd = (CFormView *)GetParent()->GetParent();
	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)pWnd->GetDocument();
	if (!pDoc->IsOpen()) {
		return;
	}

	int ScanID;
	bool bExist = false;
	UpdateData(true);
	switch(m_ScanTarget) {
	case	0	:
		ScanID = CWeldEvaluationDoc::eResinSurface;
		break;
	case	1	:
		ScanID = CWeldEvaluationDoc::eMetalSurface;
		break;
	case	2	:
		ScanID = CWeldEvaluationDoc::eJoiningResult;
		break;
	default:
		return;
	}

	if (pDoc->ExistScanFile(ScanID)) {
		bExist = true;
	}

	ItemActive(false);
	CString	msg;
	if (bExist) {
		if (!msg.LoadString(IDM_EXISTSCANIMAGE)) {
			msg = _T("スキャン画像が存在します。\n再取得をおこないますか？");
		}
		if (AfxMessageBox(msg,MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON1) != IDYES) {
			ItemActive(true);
			return;
		}
		else {
			// 関連データの削除
			pDoc->DeleteContents(ScanID);
			pWnd->SendMessage(WM_VIEW_CLER, (WPARAM)ScanID, NULL);
		}
	}
	
	int result;
	if (pWnd->SendMessage(WM_SCAN_REQUEST,(WPARAM)ScanID,(LPARAM)&result) == 0) {
		switch(result) {
		case	0	:		// スキャン成功
			if (pDoc->ExistScanFile(ScanID)) {
				int id = GetDeactiveScan();
				if (id >= 0) {
					m_ScanTarget = id;
					UpdateData(false);
				}
			}
			if (!msg.LoadString(IDM_SCAN_SUCCESS)) {
				msg = _T("スキャン処理が正常に終了しました。");
			}
			AfxMessageBox(msg,MB_OK|MB_ICONINFORMATION);
			break;
		case	1	:		// スキャンキャンセル
			if (!msg.LoadString(IDM_SCAN_CANCELD)) {
				msg = _T("スキャン処理がキャンセルされました。");
			}
			AfxMessageBox(msg,MB_OK|MB_ICONWARNING);
			break;
		default	:
			if (!msg.LoadString(IDM_ERR_SCAN)) {
				msg = _T("スキャンに失敗しました");
			}
			AfxMessageBox(msg,MB_OK|MB_ICONSTOP);
			break;
		}
	} else {
		if (!msg.LoadString(IDM_ERR_SCAN)) {
			msg = _T("スキャンに失敗しました");
		}
		AfxMessageBox(msg,MB_OK|MB_ICONSTOP);
	}
	ItemActive(true);
}

/// <summary>
/// 指定サイズにフィッティング
/// </summary>
/// <param name="rect">フィッティング領域</param>
void COprtTabPageScan::FitRect(CRect rect)
{
}

/// <summary>
/// 「上下反転」ボタンの状態更新
/// </summary>
void COprtTabPageScan::UpdateInversBtnState()
{
	UpdateData(true);
	int ScanID = -1;
	bool status = false;
	switch (m_ScanTarget) {
	case	0:
		ScanID = CWeldEvaluationDoc::eResinSurface;
		break;
	case	1:
		ScanID = CWeldEvaluationDoc::eMetalSurface;
		break;
	case	2:
		ScanID = CWeldEvaluationDoc::eJoiningResult;
		break;
	default:
		return;
	}
	
	if (ScanID >= 0) {
		CFormView *pWnd = (CFormView *)GetParent()->GetParent();
		CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)pWnd->GetDocument();
		if (pDoc->ExistScanFile(ScanID)) {
			status = true;
		}
	}
	ItemEnable(IDC_BTN_INVERS, status);
}

/// <summary>
/// 「上下反転」ボタン押下時処理
/// </summary>
void COprtTabPageScan::OnBnClickedBtnInvers()
{
	CWaitCursor wcursol;
	bool bResult = false;
	CFormView *pWnd = (CFormView *)GetParent()->GetParent();
	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)pWnd->GetDocument();
	bool bDeltedAnalizeData = false;
	if (pDoc->IsOpen()) {

		int ScanID = -1;
		UpdateData(true);
		switch (m_ScanTarget) {
		case	0:
			ScanID = CWeldEvaluationDoc::eResinSurface;
			break;
		case	1:
			ScanID = CWeldEvaluationDoc::eMetalSurface;
			break;
		case	2:
			ScanID = CWeldEvaluationDoc::eJoiningResult;
			break;
		default:
			return;
		}
		if (ScanID >= 0) {
			if (pDoc->ExistScanFile(ScanID)) {
				int result;
				if (pWnd->SendMessage(WM_INVERS_REQUEST, (WPARAM)ScanID, (LPARAM)&result) == 0) {
					if (result == 1) {
						bDeltedAnalizeData = true;
					}
					else {
						bDeltedAnalizeData = false;
					}
					bResult = true;
				}
			}
		}
	}

	CString msg;
	if (bResult) {
		if (!bDeltedAnalizeData) {
			if (!msg.LoadString(IDM_INVERS_SUCCESS)) {
				msg = _T("データの反転が完了しました");
			}
			AfxMessageBox(msg, MB_OK | MB_ICONINFORMATION);
		}
		else {
			// 解析データが削除されている
			CString s1, s2, s3;
			if (!s1.LoadString(IDM_INVERS_SUCCESS)) {
				s1 = _T("データの反転が完了しました");
			}
			if (!s2.LoadString(IDM_DELETED_ANALIZEDATA)) {
				s2 = _T("解析データが削除されました");
			}
			if (!s3.LoadString(IDM_REANALIZE_REQUEST)) {
				s3 = _T("解析の実施を行ってください");
			}
			msg.Format(_T("%s。\n%s。 %s。"), static_cast<LPCWSTR>(s1), static_cast<LPCWSTR>(s2), static_cast<LPCWSTR>(s3));
			AfxMessageBox(msg, MB_OK | MB_ICONWARNING);
		}
	}
	else {
		if (!msg.LoadString(IDM_ERR_INVERS_FAIL)) {
			msg = _T("データの反転に失敗しました");
		}
		AfxMessageBox(msg, MB_OK | MB_ICONSTOP);
	}
}

/// <summary>
/// 「樹脂面」ラジオボタンボタン押下時処理
/// </summary>
void COprtTabPageScan::OnBnClickedRdoRegin()
{
	UpdateInversBtnState();
}

/// <summary>
/// 「金属面」ラジオボタンボタン押下時処理
/// </summary>
void COprtTabPageScan::OnBnClickedRdoMetal()
{
	UpdateInversBtnState();
}

/// <summary>
/// 「接合結果」ラジオボタンボタン押下時処理
/// </summary>
void COprtTabPageScan::OnBnClickedRsoResult()
{
	UpdateInversBtnState();
}
