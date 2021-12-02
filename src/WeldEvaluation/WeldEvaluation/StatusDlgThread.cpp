#include "stdafx.h"
#include "StatusDlgThread.h"
#include "message.h"

IMPLEMENT_DYNCREATE(CStatusDlgThread, CWinThread)

/// <summary>
/// コンストラクタ
/// </summary>
CStatusDlgThread::CStatusDlgThread(void) : m_Valid(true)
{
	m_bResult = false;
}

/// <summary>
/// デストラクタ
/// </summary>
CStatusDlgThread::~CStatusDlgThread(void)
{
}

/// <summary>
/// インスタンスの初期化
/// </summary>
BOOL CStatusDlgThread::InitInstance()
{
	m_pMainWnd = &m_Dlg;
	m_Dlg.DoModal();
	m_Valid = false;  // ダイアログが閉じられたことを通知。m_Dlg.m_hWndでも代用可能か。
	return TRUE;
}

/// <summary>
/// ステータスの更新
/// </summary>
/// <param name="status">ステータス文字列</param>
void CStatusDlgThread::UpdateStatus(CString status)
{
	m_Dlg.m_Status = status;
	if (m_Dlg.m_hWnd) { // Statusダイアログが閉じられてしまっていないかチェック後post
		m_Dlg.PostMessage(WM_COMMAND, ID_UPDATE_STATUS);
	}
}

BEGIN_MESSAGE_MAP(CStatusDlgThread, CWinThread)
END_MESSAGE_MAP()