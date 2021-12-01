#pragma once


// COprtTabPageScan ダイアログ

/// <summary>
/// 操作タブ　スキャンページ管理クラス
/// </summary>
class COprtTabPageScan : public CDialogEx
{
	DECLARE_DYNAMIC(COprtTabPageScan)

public:
	COprtTabPageScan(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~COprtTabPageScan();

// ダイアログ データ
	enum { IDD = IDD_OPERTAB_PAGE_SCAN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

private:
	void ItemEnable(int ItemID,bool bActive);
	int GetDeactiveScan();

public:
	int m_ScanTarget;	///< スキャンターゲット

public:
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnBnClickedBtnScan();
	afx_msg void OnBnClickedBtnInvers();

public:
	void ItemActive(bool bActive);
	void LoadParamater();
	void FitRect(CRect rect);
	void COprtTabPageScan::UpdateInversBtnState();

	afx_msg void OnBnClickedRdoRegin();
	afx_msg void OnBnClickedRdoMetal();
	afx_msg void OnBnClickedRsoResult();
};
