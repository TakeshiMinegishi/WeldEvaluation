#pragma once
#include	"resource.h"


// CStatusDlg ダイアログ

class CStatusDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStatusDlg)

public:
	CStatusDlg(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CStatusDlg();

// ダイアログ データ
	enum { IDD = IDD_STATUS_DLG};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void UpdateStatus();
	CString m_Status;

private:
	CEdit m_CtrlEditStatus;
public:
	afx_msg void OnBnClickedCancel();
};
