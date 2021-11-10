#pragma once
#include "afxcmn.h"


// CProgressDlg ダイアログ

/// <summary>
/// プログレスダイアログクラス
/// </summary>
class CProgressDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CProgressDlg)

private:
	CString			m_title;			///< タイトル
	CString			m_label;			///< ステータスメッセージ
	UINT			m_rangeMin;			///< プログレスバー最小値
	UINT			m_rangeMax;			///< プログレスバー最大値
	UINT			m_pos;				///< プログレスバー位置
	double			m_resolution;		///< プログレスバー解像度

public:
	CString			m_stcStatusLabel;	///< ステータスメッセージ
	CProgressCtrl	m_prgress;			///< プログレスバー
	CWnd			*m_pParentWnd;		///< 親ウインドへのポインタ

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
