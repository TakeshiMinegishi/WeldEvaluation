#pragma once
#include	"resource.h"


// CStatusDlg �_�C�A���O

/// <summary>
/// �X�e�[�^�X�_�C�A���O�N���X
/// </summary>
class CStatusDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStatusDlg)

public:
	CString m_Status;		///< �X�e�[�^�X

public:
	CStatusDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CStatusDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_STATUS_DLG};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void UpdateStatus();

private:
	CEdit m_CtrlEditStatus;
public:
	afx_msg void OnBnClickedCancel();
};
