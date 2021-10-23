#pragma once
#include "afxwin.h"
#include "StatusDlg.h"

/// <summary>
/// ステータスダイアログスレッドクラス
/// </summary>
class CStatusDlgThread : public CWinThread
{
	DECLARE_DYNCREATE(CStatusDlgThread)

public:
	CStatusDlgThread(void);
	~CStatusDlgThread(void);

	virtual BOOL InitInstance();
	void UpdateStatus(CString status);

	CStatusDlg	m_Dlg;		///< ステータスダイアログオブジェクト
	bool		m_Valid;	///< 有効フラグ

protected:
	DECLARE_MESSAGE_MAP()
};

