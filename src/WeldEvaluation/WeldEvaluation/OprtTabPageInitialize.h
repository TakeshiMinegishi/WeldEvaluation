#pragma once
#include "StatusDlgThread.h"

// COprtTabPageInitiaize ダイアログ

/// <summary>
/// 操作タブ　初期化ページ管理クラス
/// </summary>
class COprtTabPageInitialize : public CDialogEx
{
	DECLARE_DYNAMIC(COprtTabPageInitialize)

public:
	CString m_message;

public:
	COprtTabPageInitialize(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~COprtTabPageInitialize();

// ダイアログ データ
	enum { IDD = IDD_OPERTAB_PAGE_INITIALIZE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

private:
	void ItemEnable(int ItemID,bool bActive);

public:
	void ItemActive(bool bActive);
	afx_msg void OnBnClickedBtnWhitebarance();
	void AddNode(CStatusDlgThread* pStatus);
	void LoadParamater();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};
