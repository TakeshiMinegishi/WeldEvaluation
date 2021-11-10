#pragma once
#include "afxwin.h"
#include "StatusDlg.h"

/// <summary>
/// �X�e�[�^�X�_�C�A���O�X���b�h�N���X
/// </summary>
class CStatusDlgThread : public CWinThread
{
	DECLARE_DYNCREATE(CStatusDlgThread)

public:
	CStatusDlgThread(void);
	~CStatusDlgThread(void);

	virtual BOOL InitInstance();
	void UpdateStatus(CString status);

	CStatusDlg	m_Dlg;		///< �X�e�[�^�X�_�C�A���O�I�u�W�F�N�g
	bool		m_Valid;	///< �L���t���O

protected:
	DECLARE_MESSAGE_MAP()
};

