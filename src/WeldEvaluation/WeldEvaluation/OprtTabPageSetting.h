#pragma once


// COprtTabPageSetting �_�C�A���O

/// <summary>
/// ����^�u�@�ݒ�y�[�W�Ǘ��N���X
/// </summary>
class COprtTabPageSetting : public CDialogEx
{
	DECLARE_DYNAMIC(COprtTabPageSetting)

public:
	CString m_strTestName;		///< ������


public:
	COprtTabPageSetting(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~COprtTabPageSetting();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_OPERTAB_PAGE_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()

private:
	void ItemEnable(int ItemID,bool bActive);

public:
	void ItemActive(bool bActive);
	void UpddateResist(bool bEnable, bool bReadMode);
	void Update();
	void LoadParamater();
	afx_msg void OnBnClickedBtnRegist();
	afx_msg void OnBnClickedBtnImageout();
};
