#pragma once
#include "message.h"
#include "afxwin.h"

// CPropTabPageSetting �_�C�A���O

/// <summary>
/// �v���p�e�B�^�u�@�ݒ�y�[�W�N���X
/// </summary>
class CPropTabPageSetting : public CDialogEx
{
	DECLARE_DYNAMIC(CPropTabPageSetting)

public:
	CPropTabPageSetting(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CPropTabPageSetting();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_PROPTAB_PAGE_SETTING };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()

private:
	int			m_MinScrollPos;					///< �X�N���[���ŏ��l
	int			m_MaxScrollPos;					///< �X�N���[���ő�l
	int			m_ScrollDelta;					///< �X�N���[������\
public:
	CString		m_RegistFolder;					///< �o�^�ӂ��ӂ�
	CString		m_WBFileName;					///< �z���C�g�o�����X�t�@�C����
	UINT		m_NumberOfOverridePixel;		///< �d�Ȃ�s�N�Z����
	UINT		m_IntegrationTimeMs;			///< Integration Time
	UINT		m_ResolutionHolizontal;			///< �����𑜓x
	UINT		m_ResolutionVertical;			///< �����𑜓x
	UINT		m_ShutterSpeed;					///< �V���b�^�[�X�s�[�h
	CScrollBar	m_sbrSettingDlg;				///< �X�N���[���o�[
	int			m_ScrollBerPos;					///< �X�N���[���o�[�ʒu

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
