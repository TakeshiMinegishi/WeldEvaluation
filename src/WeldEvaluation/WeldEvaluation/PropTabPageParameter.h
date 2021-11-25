#pragma once
#include "message.h"
#include "afxwin.h"
#include "WeldEvaluationDoc.h"


// CPropTabPageParameter ダイアログ

/// <summary>
/// プロパティタブ　パラメータ管理クラス
/// </summary>
class CPropTabPageParameter : public CDialogEx
{
	DECLARE_DYNAMIC(CPropTabPageParameter)

public:
	int			m_PageID;					///< ページID
	UINT		m_NumberOfClass;			///< 分類数
	CString		m_JointRatio;				///< 接合割合
	CComboBox	m_cmbJoinratioTarget;		///< 分類用コンボボックス
	CString		m_strJoinratioTarget;		///< 選択分類値
	CStatic		m_stcIDColor;				///< 分類ID色

public:
	CPropTabPageParameter(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CPropTabPageParameter();

// ダイアログ データ
	enum { IDD = IDD_PROPTAB_PAGE_PARAM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
private:
	void ItemEnable(int ItemID,bool bActive);
	void UpdateCmbJoinratioTargetLabel(bool renew);
	void UpdateIDColor(COLORREF color);
	CWeldEvaluationDoc *GetDocument();


public:
	afx_msg void OnDeltaposSpinNumofclass(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnSelchangeCmbJoinratioTargetLabel();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnCbnKillfocusCmbJoinratioTargetLabel();
	afx_msg void OnEnKillfocusEdtNumofclass();

public:
	void setPageID(int PageID);
	int getPageID();
	void ItemActive(bool bActive);
	void LoadParamater(int id);
	void ViewJointRatio(int method, int id, int ViewJointRatioNo);
	bool Update(int index);
	bool ConfirmChange();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
