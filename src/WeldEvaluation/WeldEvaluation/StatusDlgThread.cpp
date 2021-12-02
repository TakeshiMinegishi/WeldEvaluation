#include "stdafx.h"
#include "StatusDlgThread.h"
#include "message.h"

IMPLEMENT_DYNCREATE(CStatusDlgThread, CWinThread)

/// <summary>
/// �R���X�g���N�^
/// </summary>
CStatusDlgThread::CStatusDlgThread(void) : m_Valid(true)
{
	m_bResult = false;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
CStatusDlgThread::~CStatusDlgThread(void)
{
}

/// <summary>
/// �C���X�^���X�̏�����
/// </summary>
BOOL CStatusDlgThread::InitInstance()
{
	m_pMainWnd = &m_Dlg;
	m_Dlg.DoModal();
	m_Valid = false;  // �_�C�A���O������ꂽ���Ƃ�ʒm�Bm_Dlg.m_hWnd�ł���p�\���B
	return TRUE;
}

/// <summary>
/// �X�e�[�^�X�̍X�V
/// </summary>
/// <param name="status">�X�e�[�^�X������</param>
void CStatusDlgThread::UpdateStatus(CString status)
{
	m_Dlg.m_Status = status;
	if (m_Dlg.m_hWnd) { // Status�_�C�A���O�������Ă��܂��Ă��Ȃ����`�F�b�N��post
		m_Dlg.PostMessage(WM_COMMAND, ID_UPDATE_STATUS);
	}
}

BEGIN_MESSAGE_MAP(CStatusDlgThread, CWinThread)
END_MESSAGE_MAP()