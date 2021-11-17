// OprtTabPageInitialize.cpp : 実装ファイル
//

#include "stdafx.h"
#include "WeldEvaluation.h"
#include "OprtTabPageInitialize.h"
#include "afxdialogex.h"
#include "message.h"
#include "CDeviceIO.h"


// COprtTabPageInitialize ダイアログ

IMPLEMENT_DYNAMIC(COprtTabPageInitialize, CDialogEx)

/// <summary>
/// 初期化コンストラクタ
/// </summary>
/// <param name="pParent">親ウインドへのポインタ</param>
COprtTabPageInitialize::COprtTabPageInitialize(CWnd* pParent /*=NULL*/)
	: CDialogEx(COprtTabPageInitialize::IDD, pParent)
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
}


BEGIN_MESSAGE_MAP(COprtTabPageInitialize, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_WHITEBARANCE, &COprtTabPageInitialize::OnBnClickedBtnWhitebarance)
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
}

#include "ScanDataIO.h"
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
	msg.LoadString(IDS_PREPANING);
	pThread->UpdateStatus(msg);
	AddNode(pThread);  // すごく重い処理

	if (pThread->m_Valid) {  // キャンセルボタンが押された場合
		pThread->m_Dlg.PostMessage(WM_COMMAND, IDOK); // Statusダイアログを閉じる
	}

	WaitForSingleObject(pThread->m_hThread, 30000);	// スレッドの終了を30秒だけ待ってやる（ほぼ0秒のはず

	if (!pThread->m_bResult) {
		msg.LoadString(IDM_ERR_SCAN);
		AfxMessageBox(msg, MB_OK | MB_ICONSTOP);
	}
	else {
		msg.LoadString(IDM_SCAN_SUCCESS);
		AfxMessageBox(msg, MB_OK | MB_ICONINFORMATION);
	}
	delete pThread;
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
}
