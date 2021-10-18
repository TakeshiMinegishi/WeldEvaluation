// OprtTabPageSetting.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "WeldEvaluation.h"
#include "OprtTabPageSetting.h"
#include "afxdialogex.h"
#include "WeldEvaluationDoc.h"
#include "message.h"


// COprtTabPageSetting �_�C�A���O

IMPLEMENT_DYNAMIC(COprtTabPageSetting, CDialogEx)

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="pParent">�e�E�C���h�ւ̃|�C���^</param>
COprtTabPageSetting::COprtTabPageSetting(CWnd* pParent /*=NULL*/)
	: CDialogEx(COprtTabPageSetting::IDD, pParent)
	, m_strTestName(_T(""))
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
COprtTabPageSetting::~COprtTabPageSetting()
{
}

/// <summary>
/// �f�[�^�G�N�X�`�F���W����
/// </summary>
/// <param name="pDX">CDataExchange�I�u�W�F�N�g</param>
void COprtTabPageSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_TESTNAME, m_strTestName);
}


BEGIN_MESSAGE_MAP(COprtTabPageSetting, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_REGIST, &COprtTabPageSetting::OnBnClickedBtnRegist)
	ON_BN_CLICKED(IDC_BTN_IMAGEOUT, &COprtTabPageSetting::OnBnClickedBtnImageout)
END_MESSAGE_MAP()


// COprtTabPageSetting ���b�Z�[�W �n���h���[

/// <summary>
/// �A�C�e���̃A�N�e�B�u/�f�B�A�N�e�B�u��
/// </summary>
/// <param name="ItemID">�A�C�e��ID</param>
/// <param name="bActive">�A�N�e�B�u�t���O</param>
void COprtTabPageSetting::ItemEnable(int ItemID,bool bActive)
{
	CWnd *pWnd;
	pWnd = GetDlgItem(ItemID);
	if (pWnd) {
		pWnd->EnableWindow(bActive);
	}
}

/// <summary>
/// �A�C�e���̃A�N�e�B�u/�f�B�A�N�e�B�u��
/// </summary>
/// <param name="bActive">�A�N�e�B�u�t���O</param>
void COprtTabPageSetting::ItemActive(bool bActive)
{
	ItemEnable(IDC_STC_TESTNAME,bActive);
	ItemEnable(IDC_EDT_TESTNAME,bActive);
	ItemEnable(IDC_BTN_REGIST,bActive);
	ItemEnable(IDC_BTN_IMAGEOUT,bActive);
}

/// <summary>
/// �A�C�e���̗L��/������
/// </summary>
/// <param name="bEnable">�L�������t���O</param>
/// <param name="bReadMode">���[�h���[�h</param>
void COprtTabPageSetting::UpddateResist(bool bEnable, bool bReadMode)
{
	if (bEnable) {
		if (bReadMode) {
			ItemEnable(IDC_STC_TESTNAME,FALSE);
			ItemEnable(IDC_EDT_TESTNAME,FALSE);
		} else {
			ItemEnable(IDC_STC_TESTNAME,bEnable);
			ItemEnable(IDC_EDT_TESTNAME,bEnable);
		}
		Update();
	} else {
		ItemEnable(IDC_STC_TESTNAME,bEnable);
		ItemEnable(IDC_EDT_TESTNAME,bEnable);
		ItemEnable(IDC_BTN_IMAGEOUT,bEnable);
	}
	ItemEnable(IDC_BTN_REGIST,bEnable);
}

void COprtTabPageSetting::Update()
{
	CFormView *pWnd = (CFormView *)GetParent()->GetParent();
	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)pWnd->GetDocument();

	CString ResinScan	= pDoc->GetResinScanImagePath();
	CString ResinResult = pDoc->GetResinClassificationResultPath();

	CString MetalScan	= pDoc->GetMetalScanImagePath();
	CString MetalResult = pDoc->GetMetalClassificationResultPath();

	CString ClassScan	= pDoc->GetClassScanImagePath();

	if (!ResinScan.IsEmpty() || !MetalScan.IsEmpty() || !ClassScan.IsEmpty() ||
		!ResinResult.IsEmpty() || !MetalResult.IsEmpty()) {
		ItemEnable(IDC_BTN_IMAGEOUT,TRUE);
	} else {
		ItemEnable(IDC_BTN_IMAGEOUT,FALSE);
	}
}

/// <summary>
/// �p�����[�^�̎擾
/// </summary>
void COprtTabPageSetting::LoadParamater(void)
{
	CFormView *pWnd = (CFormView *)GetParent()->GetParent();
	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)pWnd->GetDocument();

	CString str = pDoc->GetTestName();
	m_strTestName = str;
	UpdateData(false);
}

/// <summary>
/// �o�^�{�^����������
/// </summary>
void COprtTabPageSetting::OnBnClickedBtnRegist()
{
	UpdateData(true);

	CFormView *pWnd = (CFormView *)GetParent()->GetParent();
	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)pWnd->GetDocument();
	if (!pDoc->SetTestName(m_strTestName)) {
		// Error
	}

	if (pWnd->SendMessage(WM_RESIST_REGIST,NULL,NULL) != 0) {
		// Error
	}
}

/// <summary>
/// �摜�o�̓{�^����������
/// </summary>
void COprtTabPageSetting::OnBnClickedBtnImageout()
{
	CFormView *pWnd = (CFormView *)GetParent()->GetParent();
	if (pWnd->SendMessage(WM_IMAGE_OUTPUT_REGIST,NULL,NULL) != 0) {
	}
}
