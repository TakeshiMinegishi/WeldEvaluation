#pragma once


// COprtTabPageScan �_�C�A���O

/// <summary>
/// ����^�u�@�X�L�����y�[�W�Ǘ��N���X
/// </summary>
class COprtTabPageScan : public CDialogEx
{
	DECLARE_DYNAMIC(COprtTabPageScan)

public:
	COprtTabPageScan(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~COprtTabPageScan();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_OPERTAB_PAGE_SCAN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()

private:
	void ItemEnable(int ItemID,bool bActive);
	int GetDeactiveScan();

public:
	int m_ScanTarget;	///< �X�L�����^�[�Q�b�g

public:
	void ItemActive(bool bActive);
	void LoadParamater();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnBnClickedBtnScan();
	void FitRect(CRect rect);
};