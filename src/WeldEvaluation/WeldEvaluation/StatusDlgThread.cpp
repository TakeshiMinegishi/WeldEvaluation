#include "stdafx.h"
#include "StatusDlgThread.h"
#include "message.h"

IMPLEMENT_DYNCREATE(CStatusDlgThread, CWinThread)

CStatusDlgThread::CStatusDlgThread(void) : m_Valid(true)
{
}


CStatusDlgThread::~CStatusDlgThread(void)
{
}

BOOL CStatusDlgThread::InitInstance()
{
	m_pMainWnd = &m_Dlg;
	m_Dlg.DoModal();
	m_Valid = false;  // ダイアログが閉じられたことを通知。m_Dlg.m_hWndでも代用可能か。
	return TRUE;
}

void CStatusDlgThread::UpdateStatus(CString status)
{
	m_Dlg.m_Status = status;
	if (m_Dlg.m_hWnd) { // Statusダイアログが閉じられてしまっていないかチェック後post
		m_Dlg.PostMessage(WM_COMMAND, ID_UPDATE_STATUS);
	}
}

BEGIN_MESSAGE_MAP(CStatusDlgThread, CWinThread)
END_MESSAGE_MAP()