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
	m_Valid = false;  // �_�C�A���O������ꂽ���Ƃ�ʒm�Bm_Dlg.m_hWnd�ł���p�\���B
	return TRUE;
}

void CStatusDlgThread::UpdateStatus(CString status)
{
	m_Dlg.m_Status = status;
	if (m_Dlg.m_hWnd) { // Status�_�C�A���O�������Ă��܂��Ă��Ȃ����`�F�b�N��post
		m_Dlg.PostMessage(WM_COMMAND, ID_UPDATE_STATUS);
	}
}

BEGIN_MESSAGE_MAP(CStatusDlgThread, CWinThread)
END_MESSAGE_MAP()