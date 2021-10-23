#pragma once
#include	"resource.h"


// CStatusDlg ダイアログ

/// <summary>
/// ステータスダイアログクラス
/// </summary>
class CStatusDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStatusDlg)

public:
	CString m_Status;		///< ステータス

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

private:
	CEdit m_CtrlEditStatus;
public:
	afx_msg void OnBnClickedCancel();
};
