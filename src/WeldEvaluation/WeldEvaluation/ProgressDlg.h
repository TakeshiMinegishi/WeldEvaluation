#pragma once
#include "afxcmn.h"


// CProgressDlg ダイアログ

class CProgressDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CProgressDlg)

private:
	CString			m_title;
	CString			m_label;
	UINT			m_rangeMin;
	UINT			m_rangeMax;
	UINT			m_pos;
	double			m_resolution;

public:
	CString			m_stcStatusLabel;
	CProgressCtrl	m_prgress;
	CWnd			*m_pParentWnd;

public:
	CProgressDlg(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CProgressDlg();

// ダイアログ データ
	enum { IDD = IDD_PROGRESS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	void setTitle(CString title);
	void setlabel(CString Statuslabel);
	void setRange(UINT min, UINT max);
	void setPosition(UINT pos);
	void getRange(int &min, int & max);
	afx_msg void OnBnClickedCancel();
};
