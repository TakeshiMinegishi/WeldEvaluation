#pragma once
#include "afxcmn.h"


// CProgressDlg �_�C�A���O

/// <summary>
/// �v���O���X�_�C�A���O�N���X
/// </summary>
class CProgressDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CProgressDlg)

private:
	CString			m_title;			///< �^�C�g��
	CString			m_label;			///< �X�e�[�^�X���b�Z�[�W
	UINT			m_rangeMin;			///< �v���O���X�o�[�ŏ��l
	UINT			m_rangeMax;			///< �v���O���X�o�[�ő�l
	UINT			m_pos;				///< �v���O���X�o�[�ʒu
	double			m_resolution;		///< �v���O���X�o�[�𑜓x

public:
	CString			m_stcStatusLabel;	///< �X�e�[�^�X���b�Z�[�W
	CProgressCtrl	m_prgress;			///< �v���O���X�o�[
	CWnd			*m_pParentWnd;		///< �e�E�C���h�ւ̃|�C���^

public:
	CProgressDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CProgressDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_PROGRESS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	void setTitle(CString title);
	void setlabel(CString Statuslabel);
	void setRange(UINT min, UINT max);
	void setPosition(UINT pos);
	void getRange(int &min, int & max);
	afx_msg void OnBnClickedCancel();
};
