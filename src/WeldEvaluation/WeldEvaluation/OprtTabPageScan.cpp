// OprtTabPageScan.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "WeldEvaluation.h"
#include "OprtTabPageScan.h"
#include "afxdialogex.h"
#include "WeldEvaluationDoc.h"
#include "message.h"

#pragma warning(disable:4100)

// COprtTabPageScan �_�C�A���O

IMPLEMENT_DYNAMIC(COprtTabPageScan, CDialogEx)

/// <summary>
/// �������R���X�g���N�^
/// </summary>
/// <param name="pParent">�e�E�C���h�ւ̃|�C���^</param>
COprtTabPageScan::COprtTabPageScan(CWnd* pParent /*=NULL*/)
	: CDialogEx(COprtTabPageScan::IDD, pParent)
	, m_ScanTarget(0)
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
COprtTabPageScan::~COprtTabPageScan()
{
}

/// <summary>
/// �f�[�^�G�N�X�`�F���W����
/// </summary>
/// <param name="pDX">CDataExchange�I�u�W�F�N�g</param>
void COprtTabPageScan::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RDO_REGIN, m_ScanTarget);
}


BEGIN_MESSAGE_MAP(COprtTabPageScan, CDialogEx)
	ON_WM_ACTIVATE()
	ON_BN_CLICKED(IDC_BTN_SCAN, &COprtTabPageScan::OnBnClickedBtnScan)
	ON_BN_CLICKED(IDC_BTN_INVERS, &COprtTabPageScan::OnBnClickedBtnInvers)
	ON_BN_CLICKED(IDC_RDO_REGIN, &COprtTabPageScan::OnBnClickedRdoRegin)
	ON_BN_CLICKED(IDC_RDO_METAL, &COprtTabPageScan::OnBnClickedRdoMetal)
	ON_BN_CLICKED(IDC_RSO_RESULT, &COprtTabPageScan::OnBnClickedRsoResult)
END_MESSAGE_MAP()


// COprtTabPageScan ���b�Z�[�W �n���h���[


/// <summary>
/// �A�C�e���̃A�N�e�B�u/�f�B�A�N�e�B�u��
/// </summary>
/// <param name="ItemID">�A�C�e��ID</param>
/// <param name="bActive">�A�N�e�B�u�t���O</param>
void COprtTabPageScan::ItemEnable(int ItemID,bool bActive)
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
void COprtTabPageScan::ItemActive(bool bActive)
{
	ItemEnable(IDC_STC_SCANFRAME,bActive);
	ItemEnable(IDC_RDO_REGIN,bActive);
	ItemEnable(IDC_RDO_METAL,bActive);
	ItemEnable(IDC_RSO_RESULT,bActive);
	ItemEnable(IDC_BTN_SCAN,bActive);
	ItemEnable(IDC_BTN_CANCEL,bActive);
	if (bActive) {
		UpdateInversBtnState();
	}
}

/// <summary>
/// �p�����[�^�̓ǂݍ���
/// </summary>
void COprtTabPageScan::LoadParamater(void)
{
}

/// <summary>
/// �X�L��������Ă��Ȃ��Ώۂ̎擾
/// </summary>
/// <returns>�X�L��������Ă��Ȃ��Ώۂ�ID��Ԃ�</returns>
int COprtTabPageScan::GetDeactiveScan()
{
	int targetID = -1;
	CFormView *pWnd = (CFormView *)GetParent()->GetParent();
	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)pWnd->GetDocument();
	if (!pDoc->IsOpen()) {
		return targetID;
	}
	if (pDoc->ExistScanFile(CWeldEvaluationDoc::eResinSurface)) {
		if (pDoc->ExistScanFile(CWeldEvaluationDoc::eMetalSurface)) {
			if (pDoc->ExistScanFile(CWeldEvaluationDoc::eJoiningResult)) {
				if (pDoc->IsNew()) {
					targetID = 0;
				} else {
					targetID = -1;
				}
			} else {
				targetID = 2;
			}
		} else {
			targetID = 1;
		}
	} else {
		targetID = 0;
	}
	return targetID;
}

/// <summary>
/// �E�C���h�A�N�e�B�u������
/// </summary>
/// <param name="nState">�A�N�e�B�u�t���O</param>
/// <param name="pWndOther">�ΏۃE�C���h�ւ̃|�C���^</param>
/// <param name="bMinimized">�ŏ����t���O</param>
void COprtTabPageScan::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);

	int id = GetDeactiveScan();
	if (id < 0) {
		id = 2;
	}
	m_ScanTarget = id;
	UpdateData(false);
	UpdateInversBtnState();
}

/// <summary>
/// �X�L�����{�^������������
/// </summary>
void COprtTabPageScan::OnBnClickedBtnScan()
{
	CFormView *pWnd = (CFormView *)GetParent()->GetParent();
	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)pWnd->GetDocument();
	if (!pDoc->IsOpen()) {
		return;
	}

	int ScanID;
	bool bExist = false;
	UpdateData(true);
	switch(m_ScanTarget) {
	case	0	:
		ScanID = CWeldEvaluationDoc::eResinSurface;
		break;
	case	1	:
		ScanID = CWeldEvaluationDoc::eMetalSurface;
		break;
	case	2	:
		ScanID = CWeldEvaluationDoc::eJoiningResult;
		break;
	default:
		return;
	}

	if (pDoc->ExistScanFile(ScanID)) {
		bExist = true;
	}

	ItemActive(false);
	CString	msg;
	if (bExist) {
		if (!msg.LoadString(IDM_EXISTSCANIMAGE)) {
			msg = _T("�X�L�����摜�����݂��܂��B\n�Ď擾�������Ȃ��܂����H");
		}
		if (AfxMessageBox(msg,MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON1) != IDYES) {
			ItemActive(true);
			return;
		}
		else {
			// �֘A�f�[�^�̍폜
			pDoc->DeleteContents(ScanID);
			pWnd->SendMessage(WM_VIEW_CLER, (WPARAM)ScanID, NULL);
		}
	}
	
	int result;
	if (pWnd->SendMessage(WM_SCAN_REQUEST,(WPARAM)ScanID,(LPARAM)&result) == 0) {
		switch(result) {
		case	0	:		// �X�L��������
			if (pDoc->ExistScanFile(ScanID)) {
				int id = GetDeactiveScan();
				if (id >= 0) {
					m_ScanTarget = id;
					UpdateData(false);
				}
			}
			if (!msg.LoadString(IDM_SCAN_SUCCESS)) {
				msg = _T("�X�L��������������ɏI�����܂����B");
			}
			AfxMessageBox(msg,MB_OK|MB_ICONINFORMATION);
			break;
		case	1	:		// �X�L�����L�����Z��
			if (!msg.LoadString(IDM_SCAN_CANCELD)) {
				msg = _T("�X�L�����������L�����Z������܂����B");
			}
			AfxMessageBox(msg,MB_OK|MB_ICONWARNING);
			break;
		default	:
			if (!msg.LoadString(IDM_ERR_SCAN)) {
				msg = _T("�X�L�����Ɏ��s���܂���");
			}
			AfxMessageBox(msg,MB_OK|MB_ICONSTOP);
			break;
		}
	} else {
		if (!msg.LoadString(IDM_ERR_SCAN)) {
			msg = _T("�X�L�����Ɏ��s���܂���");
		}
		AfxMessageBox(msg,MB_OK|MB_ICONSTOP);
	}
	ItemActive(true);
}

/// <summary>
/// �w��T�C�Y�Ƀt�B�b�e�B���O
/// </summary>
/// <param name="rect">�t�B�b�e�B���O�̈�</param>
void COprtTabPageScan::FitRect(CRect rect)
{
}

/// <summary>
/// �u�㉺���]�v�{�^���̏�ԍX�V
/// </summary>
void COprtTabPageScan::UpdateInversBtnState()
{
	UpdateData(true);
	int ScanID = -1;
	bool status = false;
	switch (m_ScanTarget) {
	case	0:
		ScanID = CWeldEvaluationDoc::eResinSurface;
		break;
	case	1:
		ScanID = CWeldEvaluationDoc::eMetalSurface;
		break;
	case	2:
		ScanID = CWeldEvaluationDoc::eJoiningResult;
		break;
	default:
		return;
	}
	
	if (ScanID >= 0) {
		CFormView *pWnd = (CFormView *)GetParent()->GetParent();
		CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)pWnd->GetDocument();
		if (pDoc->ExistScanFile(ScanID)) {
			status = true;
		}
	}
	ItemEnable(IDC_BTN_INVERS, status);
}

/// <summary>
/// �u�㉺���]�v�{�^������������
/// </summary>
void COprtTabPageScan::OnBnClickedBtnInvers()
{
	CWaitCursor wcursol;
	bool bResult = false;
	CFormView *pWnd = (CFormView *)GetParent()->GetParent();
	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)pWnd->GetDocument();
	bool bDeltedAnalizeData = false;
	if (pDoc->IsOpen()) {

		int ScanID = -1;
		UpdateData(true);
		switch (m_ScanTarget) {
		case	0:
			ScanID = CWeldEvaluationDoc::eResinSurface;
			break;
		case	1:
			ScanID = CWeldEvaluationDoc::eMetalSurface;
			break;
		case	2:
			ScanID = CWeldEvaluationDoc::eJoiningResult;
			break;
		default:
			return;
		}
		if (ScanID >= 0) {
			if (pDoc->ExistScanFile(ScanID)) {
				int result;
				if (pWnd->SendMessage(WM_INVERS_REQUEST, (WPARAM)ScanID, (LPARAM)&result) == 0) {
					if (result == 1) {
						bDeltedAnalizeData = true;
					}
					else {
						bDeltedAnalizeData = false;
					}
					bResult = true;
				}
			}
		}
	}

	CString msg;
	if (bResult) {
		if (!bDeltedAnalizeData) {
			if (!msg.LoadString(IDM_INVERS_SUCCESS)) {
				msg = _T("�f�[�^�̔��]���������܂���");
			}
			AfxMessageBox(msg, MB_OK | MB_ICONINFORMATION);
		}
		else {
			// ��̓f�[�^���폜����Ă���
			CString s1, s2, s3;
			if (!s1.LoadString(IDM_INVERS_SUCCESS)) {
				s1 = _T("�f�[�^�̔��]���������܂���");
			}
			if (!s2.LoadString(IDM_DELETED_ANALIZEDATA)) {
				s2 = _T("��̓f�[�^���폜����܂���");
			}
			if (!s3.LoadString(IDM_REANALIZE_REQUEST)) {
				s3 = _T("��͂̎��{���s���Ă�������");
			}
			msg.Format(_T("%s�B\n%s�B %s�B"), static_cast<LPCWSTR>(s1), static_cast<LPCWSTR>(s2), static_cast<LPCWSTR>(s3));
			AfxMessageBox(msg, MB_OK | MB_ICONWARNING);
		}
	}
	else {
		if (!msg.LoadString(IDM_ERR_INVERS_FAIL)) {
			msg = _T("�f�[�^�̔��]�Ɏ��s���܂���");
		}
		AfxMessageBox(msg, MB_OK | MB_ICONSTOP);
	}
}

/// <summary>
/// �u�����ʁv���W�I�{�^���{�^������������
/// </summary>
void COprtTabPageScan::OnBnClickedRdoRegin()
{
	UpdateInversBtnState();
}

/// <summary>
/// �u�����ʁv���W�I�{�^���{�^������������
/// </summary>
void COprtTabPageScan::OnBnClickedRdoMetal()
{
	UpdateInversBtnState();
}

/// <summary>
/// �u�ڍ����ʁv���W�I�{�^���{�^������������
/// </summary>
void COprtTabPageScan::OnBnClickedRsoResult()
{
	UpdateInversBtnState();
}
