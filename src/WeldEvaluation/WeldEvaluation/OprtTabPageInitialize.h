#pragma once
#include "StatusDlgThread.h"

// COprtTabPageInitiaize �_�C�A���O

class COprtTabPageInitialize : public CDialogEx
{
	DECLARE_DYNAMIC(COprtTabPageInitialize)

public:
	COprtTabPageInitialize(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~COprtTabPageInitialize();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_OPERTAB_PAGE_INITIALIZE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()

private:
	void ItemEnable(int ItemID,bool bActive);

public:
	void ItemActive(bool bActive);
	afx_msg void OnBnClickedBtnWhitebarance();
	void AddNode(CStatusDlgThread* pStatus);
	void LoadParamater();
};
