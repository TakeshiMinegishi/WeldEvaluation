#pragma once
#include	"resource.h"


// CStatusDlg �_�C�A���O

class CStatusDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStatusDlg)

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
	CString m_Status;

private:
	CEdit m_CtrlEditStatus;
public:
	afx_msg void OnBnClickedCancel();
};
