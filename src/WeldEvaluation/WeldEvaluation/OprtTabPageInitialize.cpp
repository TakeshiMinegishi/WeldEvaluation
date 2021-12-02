// OprtTabPageInitialize.cpp : 実装ファイル
//

#include "stdafx.h"
#include "WeldEvaluation.h"
#include "OprtTabPageInitialize.h"
#include "afxdialogex.h"
#include "message.h"
#include "CDeviceIO.h"
#include "WeldEvaluationDoc.h"
#include "FileUtil.h"
#include "ScanDataIO.h"


// COprtTabPageInitialize ダイアログ

IMPLEMENT_DYNAMIC(COprtTabPageInitialize, CDialogEx)

/// <summary>
/// 初期化コンストラクタ
/// </summary>
/// <param name="pParent">親ウインドへのポインタ</param>
COprtTabPageInitialize::COprtTabPageInitialize(CWnd* pParent /*=NULL*/)
	: CDialogEx(COprtTabPageInitialize::IDD, pParent)
	, m_message(_T(""))
{

}

/// <summary>
/// デストラクタ
/// </summary>
COprtTabPageInitialize::~COprtTabPageInitialize()
{
}

/// <summary>
/// データエクスチェンジ処理
/// </summary>
/// <param name="pDX">CDataExchangeオブジェクト</param>
void COprtTabPageInitialize::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STC_MESSAGE, m_message);
}


BEGIN_MESSAGE_MAP(COprtTabPageInitialize, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_WHITEBARANCE, &COprtTabPageInitialize::OnBnClickedBtnWhitebarance)
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()


// COprtTabPageInitiaize メッセージ ハンドラー

/// <summary>
/// アイテムのアクティブ/ディアクティブ化
/// </summary>
/// <param name="ItemID">アイテムID</param>
/// <param name="bActive">アクティブフラグ</param>
void COprtTabPageInitialize::ItemEnable(int ItemID,bool bActive)
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
void COprtTabPageInitialize::ItemActive(bool bActive)
{
	ItemEnable(IDC_STC_WHITEBARANCE,bActive);
	ItemEnable(IDC_BTN_WHITEBARANCE,bActive);
	LoadParamater();
}

/// <summary>
/// ホワイトバランスボタン押下時処理
/// </summary>
void COprtTabPageInitialize::OnBnClickedBtnWhitebarance()
{
	CFormView *pWnd = (CFormView *)GetParent()->GetParent();
	LRESULT iret = pWnd->SendMessage(WM_WBSCAN_EXISTCHECK, (WPARAM)NULL, (LPARAM)NULL);
	if (iret == 1) {
		// ホワイトバランス用のスキャンを行わない。
		return;
	}

	CWaitCursor waitCursor;
	CStatusDlgThread* pThread = DYNAMIC_DOWNCAST(CStatusDlgThread, AfxBeginThread(RUNTIME_CLASS(CStatusDlgThread) , 0, 0, CREATE_SUSPENDED));
	pThread->m_bAutoDelete = false;			// 無効なアクセス防止のため自動削除は無効化
	pThread->ResumeThread();
	while (pThread->m_Dlg.m_hWnd == 0) {	// m_Dlgのウィンドウが生成されるまで待機
		Sleep(0);
	}
	CString msg;
	if (!msg.LoadString(IDS_PREPANING)) {
		msg = _T("準備中...");
	}
	pThread->UpdateStatus(msg);
	AddNode(pThread);  // すごく重い処理

	if (pThread->m_Valid) {  // キャンセルボタンが押された場合
		pThread->m_Dlg.PostMessage(WM_COMMAND, IDOK); // Statusダイアログを閉じる
	}

	WaitForSingleObject(pThread->m_hThread, 30000);	// スレッドの終了を30秒だけ待ってやる（ほぼ0秒のはず

	if (!pThread->m_bResult) {
		if (!msg.LoadString(IDM_ERR_SCAN)) {
			msg = _T("スキャンに失敗しました");
		}
		AfxMessageBox(msg, MB_OK | MB_ICONSTOP);
	}
	else {
		if (!msg.LoadString(IDM_SCAN_SUCCESS)) {
			msg = _T("スキャン処理が正常に終了しました。");
		}
		AfxMessageBox(msg, MB_OK | MB_ICONINFORMATION);
	}
	delete pThread;
	LoadParamater();
}

/// <summary>
/// ホワイトバランス用画像撮影処理
/// </summary>
/// <param name="pStatus">スレッドオブジェクト</param>
void COprtTabPageInitialize::AddNode(CStatusDlgThread* pStatus)
{
	if (!pStatus->m_Valid) {  // キャンセルボタンが押された場合は何もせずに終了
		return;
	}

	CFormView *pWnd = (CFormView *)GetParent()->GetParent();
	int result;
	LRESULT iret = pWnd->SendMessage(WM_WBSCAN_REQUES, (WPARAM)pStatus, (LPARAM)&result);
	if ((iret != 0) || (result != 0)) {
		pStatus->m_bResult = false;
	}
	else {
		pStatus->m_bResult = true;
	}

#if false
	CString buff;
	CDeviceIO device;
	int ID = device.Init(_T("DIO000"));
	if (ID >= 0) {
		if (pStatus->m_Valid) {  // キャンセルボタンが押されていない
			if (device.ToHome(ID)) {
				if (!device.Move(ID, 1)) {
					return;
				}
				else {
					if (pStatus->m_Valid) {  // キャンセルボタンが押されていない
						buff.Format(_T("Scan Start"));
						pStatus->UpdateStatus(buff);

						/////////////////////////////////////////////////////////
						// ホワイトバランスデータの撮影
						/////////////////////////////////////////////////////////
					}
				}
			}
		}
	}
#endif
}

/// <summary>
/// パラメータの読み込み
/// </summary>
void COprtTabPageInitialize::LoadParamater(void)
{
	bool bWBExist = false;
	CFormView *pWnd = (CFormView *)GetParent()->GetParent();
	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)pWnd->GetDocument();
	CString registedFolde = pDoc->GetRegistedFolder();
	CString WBFileName = pDoc->GetWBFileName();
	if (!WBFileName.IsEmpty()) {
		WBFileName = WBFileName + _T(".hdr");
		CString path = CFileUtil::FilePathCombine(registedFolde, WBFileName);
		int width, height;
		if (!CScanDataIO::GetHeaderFilePrm(path, width, height)) {
			bWBExist = false;
		}
		else {
			if ((width != pDoc->GetShootingWidth()) || (height != pDoc->GetShootingHeight())) {
				bWBExist = false;
			}
			else {
				bWBExist = true;
			}
		}
	}

	if (bWBExist) {
		m_message = _T("ホワイトバランス設定済み");
	}
	else {
		m_message = _T("ホワイトバランス未設定");
	}
	UpdateData(false);
}

/// <summary>
/// アクティブ時処理
/// </summary>
/// <param name="nState">ウインドステータス</param>
/// <param name="pWndOther">対象ウインドへのポインタ</param>
/// <param name="bMinimized">最小化フラグ（対象ウインドが最小化されているかどうか）</param>
/// @remark nStateはWA_INACTIVE (非アクティブ化)、WA_ACTIVE (マウスクリック以外でのアクティブ化)、WA_CLICKACTIVE （マウスクリックでのアクティブ化）
void COprtTabPageInitialize::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);

	LoadParamater();
}
