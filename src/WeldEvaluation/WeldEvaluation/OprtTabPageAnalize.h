#pragma once

#include "SpectralDlg.h"

// COprtTabPageAnalize �_�C�A���O

class COprtTabPageAnalize : public CDialogEx
{
	DECLARE_DYNAMIC(COprtTabPageAnalize)

private:

public:
	int				m_ResinAnalizeType;			///< �����ʉ�̓^�C�v
	int				m_MetalAnalizeType;			///< �����ʉ�̓^�C�v
	int				m_ResinDisplayMode;			///< �����ʕ\���^�C�v
	int				m_MetalDisplayMode;			///< �����ʕ\���^�C�v
	int				m_ResultDisplayMode;		///< �ڍ����ʕ\���^�C�v

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

private:
	void ItemEnable(int ItemID, bool bActive);

public:
	void ItemActive(bool bActive);
	void LoadParamater();
	void ViewModeReset(void);
};
