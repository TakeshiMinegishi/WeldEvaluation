// OprtTabPageInitialize.cpp : 実装ファイル
//

#include "stdafx.h"
#include "WeldEvaluation.h"
#include "OprtTabPageInitialize.h"
#include "afxdialogex.h"


// COprtTabPageInitialize ダイアログ

IMPLEMENT_DYNAMIC(COprtTabPageInitialize, CDialogEx)

COprtTabPageInitialize::COprtTabPageInitialize(CWnd* pParent /*=NULL*/)
	: CDialogEx(COprtTabPageInitialize::IDD, pParent)
{

}

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


void COprtTabPageInitialize::OnBnClickedBtnWhitebarance()
{
	CStatusDlgThread* pThread = DYNAMIC_DOWNCAST(CStatusDlgThread, AfxBeginThread(RUNTIME_CLASS(CStatusDlgThread) , 0, 0, CREATE_SUSPENDED));
	pThread->m_bAutoDelete = false;			// 無効なアクセス防止のため自動削除は無効化
	pThread->ResumeThread();
	while (pThread->m_Dlg.m_hWnd == 0) {	// m_Dlgのウィンドウが生成されるまで待機
		Sleep(0);
	}
	pThread->UpdateStatus(_T("Start White Balance"));
	AddNode(pThread);  // すごく重い処理

	if (!pThread->m_Valid) {  // キャンセルボタンが押された場合
//		InitData();
	}
	else {
//	    ShowData(); // 正常終了時の軽い処理
		pThread->m_Dlg.PostMessage(WM_COMMAND, IDOK); // Statusダイアログを閉じる
	}

	WaitForSingleObject(pThread->m_hThread, 30000);	// スレッドの終了を30秒だけ待ってやる（ほぼ0秒のはず
	delete pThread;
}

void COprtTabPageInitialize::AddNode(CStatusDlgThread* pStatus)
{
	if (!pStatus->m_Valid) {  // キャンセルボタンが押された場合は何もせずに終了
		return;
	}

	Sleep(3000);
//	for each (auto child in pItem->matName)
//	{
//		// 何か処理
//		AddNode(&m_Item[child], hTree, pStatus);  // 再帰呼び出し(重い処理の原因)
//	}
		
    // Statusを表示
	CString buff;
	buff.Format(_T("Adding item to tree: %d"), 1);
	pStatus->UpdateStatus(buff);
}

void COprtTabPageInitialize::LoadParamater(void)
{
}
