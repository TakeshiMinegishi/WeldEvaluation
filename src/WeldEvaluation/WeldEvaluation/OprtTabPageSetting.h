#pragma once


// COprtTabPageSetting ダイアログ

class COprtTabPageSetting : public CDialogEx
{
	DECLARE_DYNAMIC(COprtTabPageSetting)

public:
	COprtTabPageSetting(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~COprtTabPageSetting();

// ダイアログ データ
	enum { IDD = IDD_OPERTAB_PAGE_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

private:
	void ItemEnable(int ItemID,bool bActive);

public:
	void ItemActive(bool bActive);
	void UpddateResist(bool bEnable, bool bReadMode);
	void Update();
	void LoadParamater();
	CString m_strTestName;
	afx_msg void OnBnClickedBtnRegist();
	afx_msg void OnBnClickedBtnImageout();
};
