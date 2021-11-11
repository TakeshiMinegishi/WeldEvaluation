#pragma once
#include "message.h"
#include "afxwin.h"

// CPropTabPageSetting ダイアログ

/// <summary>
/// プロパティタブ　設定ページクラス
/// </summary>
class CPropTabPageSetting : public CDialogEx
{
	DECLARE_DYNAMIC(CPropTabPageSetting)

public:
	CPropTabPageSetting(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CPropTabPageSetting();

// ダイアログ データ
	enum { IDD = IDD_PROPTAB_PAGE_SETTING };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

private:
	int			m_MinScrollPos;					///< スクロール最小値
	int			m_MaxScrollPos;					///< スクロール最大値
	int			m_ScrollDelta;					///< スクロール分解能
public:
	CString		m_RegistFolder;					///< 登録ふぉふだ
	CString		m_WBFileName;					///< ホワイトバランスファイル名
	UINT		m_NumberOfOverridePixel;		///< 重なりピクセル数
	UINT		m_IntegrationTimeMs;			///< Integration Time
	UINT		m_ResolutionHolizontal;			///< 水平解像度
	UINT		m_ResolutionVertical;			///< 垂直解像度
	UINT		m_ShutterSpeed;					///< シャッタースピード
	CScrollBar	m_sbrSettingDlg;				///< スクロールバー
	int			m_ScrollBerPos;					///< スクロールバー位置

public:
	afx_msg void OnBnClickedBunRegfolderFind();
	afx_msg void OnDeltaposSpn(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpnIntegrationtime(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpnResolutionHolizontal(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpnResolutionVertical(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnEnKillfocusEdtRegfolder();
	afx_msg void OnEnKillfocusEdtNumberofoverrappingpixel();
	afx_msg void OnEnKillfocusEdtIntegrationtime();
	afx_msg void OnEnKillfocusEdtResolutionHolizontal();
	afx_msg void OnEnKillfocusEdtResolutionVertical();
	afx_msg void OnEnKillfocusEdtWbfilename();

	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnEnKillfocusEdtShutterspeed();
	afx_msg void OnDeltaposSpnShutterspeed(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

private:
	void ItemEnable(int ItemID,bool bActive);
	void VMove(int ItemId, int offset);
	void MoveItem(int pos);

public:
	void LoadParamater(void);
	void ItemActive(bool bActive);
	void FitRect(CRect rect);
	bool Update(void);
};
