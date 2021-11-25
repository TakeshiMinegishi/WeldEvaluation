#pragma once
#include "message.h"
#include "afxwin.h"
#include "WeldEvaluationDoc.h"


// CPropTabPageParameter �_�C�A���O

/// <summary>
/// �v���p�e�B�^�u�@�p�����[�^�Ǘ��N���X
/// </summary>
class CPropTabPageParameter : public CDialogEx
{
	DECLARE_DYNAMIC(CPropTabPageParameter)

public:
	int			m_PageID;					///< �y�[�WID
	UINT		m_NumberOfClass;			///< ���ސ�
	CString		m_JointRatio;				///< �ڍ�����
	CComboBox	m_cmbJoinratioTarget;		///< ���ޗp�R���{�{�b�N�X
	CString		m_strJoinratioTarget;		///< �I�𕪗ޒl
	CStatic		m_stcIDColor;				///< ����ID�F

public:
	CPropTabPageParameter(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CPropTabPageParameter();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_PROPTAB_PAGE_PARAM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

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
