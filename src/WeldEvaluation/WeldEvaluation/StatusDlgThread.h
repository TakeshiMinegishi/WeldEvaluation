#pragma once
#include "afxwin.h"
#include "StatusDlg.h"

class CStatusDlgThread : public CWinThread
{
	DECLARE_DYNCREATE(CStatusDlgThread)

public:
	CStatusDlgThread(void);
	~CStatusDlgThread(void);

	virtual BOOL InitInstance();
	void UpdateStatus(CString status);

	CStatusDlg m_Dlg;
	bool m_Valid;

protected:
	DECLARE_MESSAGE_MAP()
};

