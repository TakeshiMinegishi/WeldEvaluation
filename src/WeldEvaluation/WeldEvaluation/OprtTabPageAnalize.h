#pragma once

#include "SpectralDlg.h"

// COprtTabPageAnalize �_�C�A���O

/// <summary>
/// ����^�u��̓y�[�W�Ǘ��N���X
/// </summary>
class COprtTabPageAnalize : public CDialogEx
{
	DECLARE_DYNAMIC(COprtTabPageAnalize)

private:

public:
	int				m_ResinAnalizeType;			///< �����ʉ�̓^�C�v
	int				m_MetalAnalizeType;			///< �����ʉ�̓^�C�v
	int				m_JointAnalizeType;			///< �ڍ����ʉ�̓^�C�v
	int				m_ResinDisplayMode;			///< �����ʕ\���^�C�v
	int				m_MetalDisplayMode;			///< �����ʕ\���^�C�v
	int				m_ResultDisplayMode;		///< �ڍ����ʕ\���^�C�v
	bool			m_bChangedAnalize;			///< ��͕ύX�L���t���O
	bool			m_bChangedDisplayMode;		///< �\�����@�ύX�L���t���O

public:
	COprtTabPageAnalize(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~COprtTabPageAnalize();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_OPERTAB_PAGE_ANALIZE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

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
