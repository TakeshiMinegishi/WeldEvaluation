#pragma once

#include "SpectralDlg.h"

// COprtTabPageAnalize ダイアログ

/// <summary>
/// 操作タブ解析ページ管理クラス
/// </summary>
class COprtTabPageAnalize : public CDialogEx
{
	DECLARE_DYNAMIC(COprtTabPageAnalize)

private:

public:
	int				m_ResinAnalizeType;			///< 樹脂面解析タイプ
	int				m_MetalAnalizeType;			///< 金属面解析タイプ
	int				m_JointAnalizeType;			///< 接合結果解析タイプ
	int				m_ResinDisplayMode;			///< 樹脂面表示タイプ
	int				m_MetalDisplayMode;			///< 金属面表示タイプ
	int				m_ResultDisplayMode;		///< 接合結果表示タイプ
	bool			m_bChangedAnalize;			///< 解析変更有無フラグ
	bool			m_bChangedDisplayMode;		///< 表示方法変更有無フラグ

public:
	COprtTabPageAnalize(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~COprtTabPageAnalize();

// ダイアログ データ
	enum { IDD = IDD_OPERTAB_PAGE_ANALIZE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedBtnResinanalize();
	afx_msg void OnBnClickedBtnMetalanalize();
	afx_msg void OnBnClickedBtnSectionspectrumdisplay();
	afx_msg void OnBnClickedRdoResinscan();
	afx_msg void OnBnClickedRdoResinclasresult();
	afx_msg void OnBnClickedRdoMetalscan();
	afx_msg void OnBnClickedRdoMetalclasresult();
	afx_msg void OnBnClickedRdoJointresultscan();
	afx_msg void OnBnClickedRdoJointresultclasresult();
	afx_msg void OnBnClickedRdoResinMethod1();
	afx_msg void OnBnClickedRdoResinMethod2();
	afx_msg void OnBnClickedRdoMetalMethod1();
	afx_msg void OnBnClickedRdoMetalMethod2();
	afx_msg void OnBnClickedRdoJointMethod1();
	afx_msg void OnBnClickedRdoJointMethod2();
	afx_msg void OnBnClickedBtnJointanalize();

private:
	void ItemEnable(int ItemID, bool bActive);

public:
	void ItemActive(bool bActive);
	void LoadParamater();
	void ViewModeReset(void);
	int GetAnalizeType(int ItemID);
	void ChangeAnalizeType(int ScanID);
	int SetDisplayMode(int ScanID, int mode);
	bool IsChangedAnalize();
	void SetChangedAnalizeFlag(bool bFlag);
	bool IsChangedDisplayMode();
};
