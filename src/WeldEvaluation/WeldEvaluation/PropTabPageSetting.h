#pragma once
#include "message.h"
#include "afxwin.h"

// CPropTabPageSetting ダイアログ

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
	int			m_MinScrollPos;
	int			m_MaxScrollPos;
	int			m_ScrollDelta;
public:
	CString		m_RegistFolder;
	CString		m_WBFileName;
	UINT		m_NumberOfOverridePixel;
	UINT		m_IntegrationTimeMs;
	UINT		m_ResolutionHolizontal;
	UINT		m_ResolutionVertical;
	CScrollBar	m_sbrSettingDlg;
	int			m_ScrollBerPos;

public:
	afx_msg void OnBnClickedBunRegfolderFind();
	afx_msg void OnDeltaposSpn(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpnIntegrationtime(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpnResolutionHolizontal(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpnResolutionVertical(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);

	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
private:
	void ItemEnable(int ItemID,bool bActive);
	void VMove(int ItemId, int offset);
	void MoveItem(int pos);

public:
	void LoadParamater(void);
	void ItemActive(bool bActive);
	afx_msg void OnEnKillfocusEdtRegfolder();
	afx_msg void OnEnKillfocusEdtNumberofoverrappingpixel();
	afx_msg void OnEnKillfocusEdtIntegrationtime();
	afx_msg void OnEnKillfocusEdtResolutionHolizontal();
	afx_msg void OnEnKillfocusEdtResolutionVertical();
	afx_msg void OnEnKillfocusEdtWbfilename();
	afx_msg void OnNMThemeChangedSbrSetting(NMHDR *pNMHDR, LRESULT *pResult);
	void FitRect(CRect rect);
	bool Update(void);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};
