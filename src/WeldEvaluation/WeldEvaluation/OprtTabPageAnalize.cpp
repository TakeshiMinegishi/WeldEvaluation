// OprtTabPageAnalize.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "WeldEvaluation.h"
#include "OprtTabPageAnalize.h"
#include "afxdialogex.h"
#include "WeldEvaluationDoc.h"
#include "message.h"


// COprtTabPageAnalize �_�C�A���O

IMPLEMENT_DYNAMIC(COprtTabPageAnalize, CDialogEx)

/// <summary>
/// �R���X�g���N�^
/// </summary>
COprtTabPageAnalize::COprtTabPageAnalize(CWnd* pParent /*=NULL*/)
	: CDialogEx(COprtTabPageAnalize::IDD, pParent)
	, m_ResinAnalizeType(0)
	, m_MetalAnalizeType(0)
	, m_JointAnalizeType(0)
	, m_ResinDisplayMode(0)
	, m_MetalDisplayMode(0)
	, m_ResultDisplayMode(0)
{
	m_bChangedAnalize = false;
	m_bChangedDisplayMode = false;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
COprtTabPageAnalize::~COprtTabPageAnalize()
{
}

/// <summary>
/// �f�[�^�G�N�X�`�F���W����
/// </summary>
/// <param name="pDX">CDataExchange�I�u�W�F�N�g</param>
void COprtTabPageAnalize::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RDO_RESIN_METHOD1, m_ResinAnalizeType);
	DDX_Radio(pDX, IDC_RDO_METAL_METHOD1, m_MetalAnalizeType);
	DDX_Radio(pDX, IDC_RDO_RESINSCAN, m_ResinDisplayMode);
	DDX_Radio(pDX, IDC_RDO_METALSCAN, m_MetalDisplayMode);
	DDX_Radio(pDX, IDC_RDO_JOINTRESULTSCAN, m_ResultDisplayMode);
	DDX_Radio(pDX, IDC_RDO_JOINT_METHOD1, m_JointAnalizeType);
}


BEGIN_MESSAGE_MAP(COprtTabPageAnalize, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_RESINANALIZE, &COprtTabPageAnalize::OnBnClickedBtnResinanalize)
	ON_BN_CLICKED(IDC_BTN_METALANALIZE, &COprtTabPageAnalize::OnBnClickedBtnMetalanalize)
	ON_BN_CLICKED(IDC_BTN_SECTIONSPECTRUMDISPLAY, &COprtTabPageAnalize::OnBnClickedBtnSectionspectrumdisplay)
	ON_BN_CLICKED(IDC_RDO_RESINSCAN, &COprtTabPageAnalize::OnBnClickedRdoResinscan)
	ON_BN_CLICKED(IDC_RDO_RESINCLASRESULT, &COprtTabPageAnalize::OnBnClickedRdoResinclasresult)
	ON_BN_CLICKED(IDC_RDO_METALSCAN, &COprtTabPageAnalize::OnBnClickedRdoMetalscan)
	ON_BN_CLICKED(IDC_RDO_METALCLASRESULT, &COprtTabPageAnalize::OnBnClickedRdoMetalclasresult)
	ON_BN_CLICKED(IDC_RDO_JOINTRESULTSCAN, &COprtTabPageAnalize::OnBnClickedRdoJointresultscan)
	ON_BN_CLICKED(IDC_RDO_JOINTRESULTCLASRESULT, &COprtTabPageAnalize::OnBnClickedRdoJointresultclasresult)
	ON_BN_CLICKED(IDC_RDO_RESIN_METHOD1, &COprtTabPageAnalize::OnBnClickedRdoResinMethod1)
	ON_BN_CLICKED(IDC_RDO_RESIN_METHOD2, &COprtTabPageAnalize::OnBnClickedRdoResinMethod2)
	ON_BN_CLICKED(IDC_RDO_METAL_METHOD1, &COprtTabPageAnalize::OnBnClickedRdoMetalMethod1)
	ON_BN_CLICKED(IDC_RDO_METAL_METHOD2, &COprtTabPageAnalize::OnBnClickedRdoMetalMethod2)
	ON_BN_CLICKED(IDC_RDO_JOINT_METHOD1, &COprtTabPageAnalize::OnBnClickedRdoJointMethod1)
	ON_BN_CLICKED(IDC_RDO_JOINT_METHOD2, &COprtTabPageAnalize::OnBnClickedRdoJointMethod2)
	ON_BN_CLICKED(IDC_BTN_JOINTANALIZE, &COprtTabPageAnalize::OnBnClickedBtnJointanalize)
END_MESSAGE_MAP()


// COprtTabPageAnalize ���b�Z�[�W �n���h���[

/// <summary>
/// �A�C�e���̃A�N�e�B�u/�f�B�A�N�e�B�u��
/// </summary>
/// <param name="ItemID">�A�C�e��ID</param>
/// <param name="bActive">�A�N�e�B�u�t���O</param>
void COprtTabPageAnalize::ItemEnable(int ItemID,bool bActive)
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
void COprtTabPageAnalize::ItemActive(bool bActive)
{
	ItemEnable(IDC_STC_RESIN_FRAME,bActive);
	ItemEnable(IDC_RDO_RESIN_METHOD1,bActive);
	ItemEnable(IDC_RDO_RESIN_METHOD2,bActive);
	ItemEnable(IDC_BTN_RESINANALIZE,bActive);

	ItemEnable(IDC_STC_METAL_FRAME,bActive);
	ItemEnable(IDC_RDO_METAL_METHOD1,bActive);
	ItemEnable(IDC_RDO_METAL_METHOD2,bActive);
	ItemEnable(IDC_BTN_METALANALIZE,bActive);

	ItemEnable(IDC_STC_JOINT_FRAME, bActive);
	ItemEnable(IDC_RDO_JOINT_METHOD1, bActive);
	ItemEnable(IDC_RDO_JOINT_METHOD2, bActive);
	ItemEnable(IDC_BTN_JOINTANALIZE, bActive);

	ItemEnable(IDC_STC_VIEWCHANGE_FRAME,bActive);
	ItemEnable(IDC_STC_REGIN_LABEL,bActive);
	ItemEnable(IDC_RDO_RESINSCAN,bActive);
	ItemEnable(IDC_RDO_RESINCLASRESULT,bActive);

	ItemEnable(IDC_STC_METAL_LABEL,bActive);
	ItemEnable(IDC_RDO_METALSCAN,bActive);
	ItemEnable(IDC_RDO_METALCLASRESULT,bActive);

	ItemEnable(IDC_STC_JOINTRESULT_LABEL,bActive);
	ItemEnable(IDC_RDO_JOINTRESULTSCAN,bActive);
	ItemEnable(IDC_RDO_JOINTRESULTCLASRESULT,bActive);

	ItemEnable(IDC_BTN_SECTIONSPECTRUMDISPLAY,bActive);
}

/// <summary>
/// �p�����[�^�̓ǂݍ���
/// </summary>
void COprtTabPageAnalize::LoadParamater(void)
{
	CFormView *pWnd = (CFormView *)GetParent()->GetParent();
	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)pWnd->GetDocument();

	m_ResinAnalizeType = (pDoc->GetAnalysisMethod(CWeldEvaluationDoc::eResinSurface) == CWeldEvaluationDoc::AnalizeTypeKMeans?0:1);
	m_MetalAnalizeType = (pDoc->GetAnalysisMethod(CWeldEvaluationDoc::eMetalSurface) == CWeldEvaluationDoc::AnalizeTypeKMeans?0:1);

	m_ResinDisplayMode = (pDoc->GetDisplayMode(CWeldEvaluationDoc::eResinSurface) == CWeldEvaluationDoc::DisplayModeScan?0:1);
	m_MetalDisplayMode = (pDoc->GetDisplayMode(CWeldEvaluationDoc::eMetalSurface) == CWeldEvaluationDoc::DisplayModeScan?0:1);
	m_ResultDisplayMode = (pDoc->GetDisplayMode(CWeldEvaluationDoc::eJoiningResult) == CWeldEvaluationDoc::DisplayModeScan?0:1);
	UpdateData(false);
}

/// <summary>
/// �\�����[�h���Z�b�g
/// </summary>
void COprtTabPageAnalize::ViewModeReset(void)
{
	m_ResinDisplayMode = 0;
	m_MetalDisplayMode = 0;
	m_ResultDisplayMode = 0;
	UpdateData(false);
}

/// <summary>
/// �����ʉ�͕��@k-means�I��������
/// </summary>
void COprtTabPageAnalize::OnBnClickedRdoResinMethod1()
{
	int org = m_ResinAnalizeType;
	UpdateData(true);
	if (org == m_ResinAnalizeType) {
		return;
	}
	CFormView *pWnd = (CFormView *)GetParent()->GetParent();
	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)pWnd->GetDocument();
	pDoc->SetAnalysisMethod(CWeldEvaluationDoc::eResinSurface,CWeldEvaluationDoc::AnalizeTypeKMeans);
	if (m_ResinAnalizeType == 0) {
		SetChangedAnalizeFlag(true);
		if (pWnd) {
			if (pDoc->GetDisplayMode(CWeldEvaluationDoc::eResinSurface) != CWeldEvaluationDoc::DisplayModeScan) {
				pWnd->SendMessage(WM_VIEW_CHANGE_REQUEST, CWeldEvaluationDoc::eResinSurface, CWeldEvaluationDoc::DisplayModeResult);
			}
		}
	}
}

/// <summary>
/// �����ʉ�͕��@�K�w�N���X�^�����O�I��������
/// </summary>
void COprtTabPageAnalize::OnBnClickedRdoResinMethod2()
{
	int org = m_ResinAnalizeType;
	UpdateData(true);
	if (org == m_ResinAnalizeType) {
		return;
	}
	CFormView *pWnd = (CFormView *)GetParent()->GetParent();
	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)pWnd->GetDocument();
	pDoc->SetAnalysisMethod(CWeldEvaluationDoc::eResinSurface,CWeldEvaluationDoc::AnalizeTypeHiClustering);
	if (m_ResinAnalizeType == 1) {
		SetChangedAnalizeFlag(true);
		if (pWnd) {
			if (pDoc->GetDisplayMode(CWeldEvaluationDoc::eResinSurface) != CWeldEvaluationDoc::DisplayModeScan) {
				pWnd->SendMessage(WM_VIEW_CHANGE_REQUEST, CWeldEvaluationDoc::eResinSurface, CWeldEvaluationDoc::DisplayModeResult);
			}
		}
	}
}

/// <summary>
/// �����ʉ�͕��@k-means�I��������
/// </summary>
void COprtTabPageAnalize::OnBnClickedRdoMetalMethod1()
{
	int org = m_MetalAnalizeType;
	UpdateData(true);
	if (org == m_MetalAnalizeType) {
		return;
	}
	CFormView *pWnd = (CFormView *)GetParent()->GetParent();
	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)pWnd->GetDocument();
	pDoc->SetAnalysisMethod(CWeldEvaluationDoc::eMetalSurface,CWeldEvaluationDoc::AnalizeTypeKMeans);
	if (m_MetalAnalizeType == 0) {
		SetChangedAnalizeFlag(true);
		if (pWnd) {
			if (pDoc->GetDisplayMode(CWeldEvaluationDoc::eMetalSurface) != CWeldEvaluationDoc::DisplayModeScan) {
				pWnd->SendMessage(WM_VIEW_CHANGE_REQUEST, CWeldEvaluationDoc::eMetalSurface, CWeldEvaluationDoc::DisplayModeResult);
			}
		}
	}
}

/// <summary>
/// �����ʉ�͕��@�K�w�N���X�^�����O�I��������
/// </summary>
void COprtTabPageAnalize::OnBnClickedRdoMetalMethod2()
{
	int org = m_MetalAnalizeType;
	UpdateData(true);
	if (org == m_MetalAnalizeType) {
		return;
	}
	CFormView *pWnd = (CFormView *)GetParent()->GetParent();
	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)pWnd->GetDocument();
	pDoc->SetAnalysisMethod(CWeldEvaluationDoc::eMetalSurface,CWeldEvaluationDoc::AnalizeTypeHiClustering);
	if (m_MetalAnalizeType == 1) {
		SetChangedAnalizeFlag(true);
		if (pWnd) {
			if (pDoc->GetDisplayMode(CWeldEvaluationDoc::eMetalSurface) != CWeldEvaluationDoc::DisplayModeScan) {
				pWnd->SendMessage(WM_VIEW_CHANGE_REQUEST, CWeldEvaluationDoc::eMetalSurface, CWeldEvaluationDoc::DisplayModeResult);
			}
		}
	}
}

/// <summary>
/// �ڑ����ʉ�͕��@k-means�I��������
/// </summary>
void COprtTabPageAnalize::OnBnClickedRdoJointMethod1()
{
	CFormView *pWnd = (CFormView *)GetParent()->GetParent();
	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)pWnd->GetDocument();

	int org = m_JointAnalizeType;
	UpdateData(true);
	if (org == m_JointAnalizeType) {
		return;
	}
	pDoc->SetAnalysisMethod(CWeldEvaluationDoc::eJoiningResult, CWeldEvaluationDoc::AnalizeTypeKMeans);
	if (m_JointAnalizeType == 0) {
		SetChangedAnalizeFlag(true);
		if (pWnd) {
			if (pDoc->GetDisplayMode(CWeldEvaluationDoc::eJoiningResult) != CWeldEvaluationDoc::DisplayModeScan) {
				pWnd->SendMessage(WM_VIEW_CHANGE_REQUEST, CWeldEvaluationDoc::eJoiningResult, CWeldEvaluationDoc::DisplayModeResult);
			}
		}
	}
}

/// <summary>
/// �ڑ����ʉ�͕��@�K�w�N���X�^�����O�I��������
/// </summary>
void COprtTabPageAnalize::OnBnClickedRdoJointMethod2()
{
	CFormView *pWnd = (CFormView *)GetParent()->GetParent();
	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)pWnd->GetDocument();

	int org = m_JointAnalizeType;
	UpdateData(true);
	if (org == m_JointAnalizeType) {
		return;
	}
	pDoc->SetAnalysisMethod(CWeldEvaluationDoc::eJoiningResult, CWeldEvaluationDoc::AnalizeTypeHiClustering);
	if (m_JointAnalizeType == 1) {
		SetChangedAnalizeFlag(true);
		if (pWnd) {
			if (pDoc->GetDisplayMode(CWeldEvaluationDoc::eJoiningResult) != CWeldEvaluationDoc::DisplayModeScan) {
				pWnd->SendMessage(WM_VIEW_CHANGE_REQUEST, CWeldEvaluationDoc::eJoiningResult, CWeldEvaluationDoc::DisplayModeResult);
			}
		}
	}
}

/// <summary>
/// �����ʕ��͎��{�I��������
/// </summary>
void COprtTabPageAnalize::OnBnClickedBtnResinanalize()
{
	UpdateData(true);
	WPARAM targetItemID = CWeldEvaluationDoc::eResinSurface;
	LPARAM type = m_ResinAnalizeType==0?CWeldEvaluationDoc::AnalizeTypeKMeans:CWeldEvaluationDoc::AnalizeTypeHiClustering;
	CFormView *pWnd = (CFormView *)GetParent()->GetParent();
	if (pWnd->SendMessage(WM_ANALYSE_REQUEST,targetItemID,type) != 0) {
	}
}

/// <summary>
/// �����ʕ��͎��{�I��������
/// </summary>
void COprtTabPageAnalize::OnBnClickedBtnMetalanalize()
{
	UpdateData(true);
	WPARAM targetItemID = CWeldEvaluationDoc::eMetalSurface;
	LPARAM type = m_MetalAnalizeType==0?CWeldEvaluationDoc::AnalizeTypeKMeans:CWeldEvaluationDoc::AnalizeTypeHiClustering;
	CFormView *pWnd = (CFormView *)GetParent()->GetParent();
	if (pWnd->SendMessage(WM_ANALYSE_REQUEST,targetItemID,type) != 0) {
	}
}

/// <summary>
/// �ڍ����ʕ��͎��{�I��������
/// </summary>
void COprtTabPageAnalize::OnBnClickedBtnJointanalize()
{
	UpdateData(true);
	WPARAM targetItemID = CWeldEvaluationDoc::eJoiningResult;
	LPARAM type = m_JointAnalizeType == 0 ? CWeldEvaluationDoc::AnalizeTypeKMeans : CWeldEvaluationDoc::AnalizeTypeHiClustering;
	CFormView *pWnd = (CFormView *)GetParent()->GetParent();
	if (pWnd->SendMessage(WM_ANALYSE_REQUEST, targetItemID, type) != 0) {
	}
}

/// <summary>
/// �����ʕ\���^�C�v�X�L�����I��������
/// </summary>
void COprtTabPageAnalize::OnBnClickedRdoResinscan()
{
	int org = m_ResinDisplayMode;
	UpdateData(true);
	if (org == m_ResinDisplayMode) {
		return;
	}
	CFormView *pWnd = (CFormView *)GetParent()->GetParent();
	if (pWnd->SendMessage(WM_VIEW_CHANGE_REQUEST,CWeldEvaluationDoc::eResinSurface,CWeldEvaluationDoc::DisplayModeScan) != 0) {
		m_ResinDisplayMode = org;
	} else {
		CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)pWnd->GetDocument();
		pDoc->SetDisplayMode(CWeldEvaluationDoc::eResinSurface,CWeldEvaluationDoc::DisplayModeScan);
	}
	UpdateData(false);
}

/// <summary>
/// �����ʕ\���^�C�v���ތ��ʑI��������
/// </summary>
void COprtTabPageAnalize::OnBnClickedRdoResinclasresult()
{
	int org = m_ResinDisplayMode;
	UpdateData(true);
	if (org == m_ResinDisplayMode) {
		return;
	}
	CFormView *pWnd = (CFormView *)GetParent()->GetParent();
	if (pWnd) {
		m_bChangedDisplayMode = true;
		if (pWnd->SendMessage(WM_VIEW_CHANGE_REQUEST,CWeldEvaluationDoc::eResinSurface,CWeldEvaluationDoc::DisplayModeResult) != 0) {
			m_ResinDisplayMode = org;
		} else {
			CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)pWnd->GetDocument();
			pDoc->SetDisplayMode(CWeldEvaluationDoc::eResinSurface,CWeldEvaluationDoc::DisplayModeResult);
		}
		m_bChangedDisplayMode = false;
	}
	UpdateData(false);
}

/// <summary>
/// �����ʕ\���^�C�v�X�L�����I��������
/// </summary>
void COprtTabPageAnalize::OnBnClickedRdoMetalscan()
{
	int org = m_MetalDisplayMode;
	UpdateData(true);
	if (org == m_MetalDisplayMode) {
		return;
	}
	CFormView *pWnd = (CFormView *)GetParent()->GetParent();
	if (pWnd) {
		m_bChangedDisplayMode = true;
		if (pWnd->SendMessage(WM_VIEW_CHANGE_REQUEST, CWeldEvaluationDoc::eMetalSurface, CWeldEvaluationDoc::DisplayModeScan) != 0) {
			m_MetalDisplayMode = org;
		}
		else {
			CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)pWnd->GetDocument();
			pDoc->SetDisplayMode(CWeldEvaluationDoc::eMetalSurface, CWeldEvaluationDoc::DisplayModeScan);
		}
		m_bChangedDisplayMode = false;
	}
	UpdateData(false);
}

/// <summary>
/// �����ʕ\���^�C�v���ތ��ʑI��������
/// </summary>
void COprtTabPageAnalize::OnBnClickedRdoMetalclasresult()
{
	int org = m_MetalDisplayMode;
	UpdateData(true);
	if (org == m_MetalDisplayMode) {
		return;
	}
	CFormView *pWnd = (CFormView *)GetParent()->GetParent();
	if (pWnd) {
		m_bChangedDisplayMode = true;
		if (pWnd->SendMessage(WM_VIEW_CHANGE_REQUEST, CWeldEvaluationDoc::eMetalSurface, CWeldEvaluationDoc::DisplayModeResult) != 0) {
			m_MetalDisplayMode = org;
		}
		else {
			CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)pWnd->GetDocument();
			pDoc->SetDisplayMode(CWeldEvaluationDoc::eMetalSurface, CWeldEvaluationDoc::DisplayModeResult);
		}
		m_bChangedDisplayMode = false;
	}
	UpdateData(false);
}

/// <summary>
/// �ڍ����ʕ\���^�C�v�X�L�����I��������
/// </summary>
void COprtTabPageAnalize::OnBnClickedRdoJointresultscan()
{
	int org = m_ResultDisplayMode;
	UpdateData(true);
	if (org == m_ResultDisplayMode) {
		return;
	}
	CFormView *pWnd = (CFormView *)GetParent()->GetParent();
	if (pWnd) {
		m_bChangedDisplayMode = true;
		if (pWnd->SendMessage(WM_VIEW_CHANGE_REQUEST, CWeldEvaluationDoc::eJoiningResult, CWeldEvaluationDoc::DisplayModeScan) != 0) {
			m_ResultDisplayMode = org;
		}
		else {
			CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)pWnd->GetDocument();
			pDoc->SetDisplayMode(CWeldEvaluationDoc::eJoiningResult, CWeldEvaluationDoc::DisplayModeScan);
		}
		m_bChangedDisplayMode = false;
	}
	UpdateData(false);
}

/// <summary>
/// �ڍ����ʕ\���^�C�v���ތ��ʑI��������
/// </summary>
void COprtTabPageAnalize::OnBnClickedRdoJointresultclasresult()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	int org = m_ResultDisplayMode;
	UpdateData(true);
	if (org == m_ResultDisplayMode) {
		return;
	}
	CFormView *pWnd = (CFormView *)GetParent()->GetParent();
	if (pWnd) {
		m_bChangedDisplayMode = true;
		if (pWnd->SendMessage(WM_VIEW_CHANGE_REQUEST, CWeldEvaluationDoc::eJoiningResult, CWeldEvaluationDoc::DisplayModeResult) != 0) {
			m_ResultDisplayMode = org;
		}
		else {
			CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)pWnd->GetDocument();
			pDoc->SetDisplayMode(CWeldEvaluationDoc::eJoiningResult, CWeldEvaluationDoc::DisplayModeResult);
		}
		m_bChangedDisplayMode = false;
	}
	UpdateData(false);
}

/// <summary>
/// ��ԃX�y�N�g���\���{�^���I��������
/// </summary>
void COprtTabPageAnalize::OnBnClickedBtnSectionspectrumdisplay()
{
	CFormView *pViewWnd = (CFormView *)GetParent()->GetParent();
	pViewWnd->PostMessageW(WM_AREA_SPECTRUM_GRAPH_REQUEST,NULL, NULL);
}

/// <summary>
/// ��͎�@�̎擾
/// </summary>
/// <param name="ItemID">�\���^�[�Q�b�gID</param>
/// <returns>��͎�@��Ԃ�</returns>
int COprtTabPageAnalize::GetAnalizeType(int ItemID)
{
	int AnalizeType = 0;
	UpdateData(true);
	switch (ItemID) {
	case		CWeldEvaluationDoc::eResinSurface	:
		AnalizeType = m_ResinAnalizeType;
		break;
	case		CWeldEvaluationDoc::eMetalSurface	:
		AnalizeType = m_MetalAnalizeType;
		break;
	case		CWeldEvaluationDoc::eJoiningResult	:
		AnalizeType = m_JointAnalizeType;
		break;
	}
	int type = AnalizeType == 0 ? CWeldEvaluationDoc::AnalizeTypeKMeans : CWeldEvaluationDoc::AnalizeTypeHiClustering;
	return type;
}

/// <summary>
/// ��͎�@�̐؂芷��
/// </summary>
/// <param name="ScanID">�X�L����ID</param>
/// @remark �`�F�b�N����Ă����@�𖢃`�F�b�N�̎�@�ɐ؂芷����
void COprtTabPageAnalize::ChangeAnalizeType(int ScanID)
{
	UpdateData(true);
	switch (ScanID) {
	case		CWeldEvaluationDoc::eResinSurface:
		if (m_ResinAnalizeType == 0) {
			m_ResinAnalizeType = 1;
		}
		else {
			m_ResinAnalizeType = 0;
		}
		break;
	case		CWeldEvaluationDoc::eMetalSurface:
		if (m_MetalAnalizeType == 0) {
			m_MetalAnalizeType = 1;
		}
		else {
			m_MetalAnalizeType = 0;
		}
		break;
	case		CWeldEvaluationDoc::eJoiningResult:
		if (m_JointAnalizeType == 0) {
			m_JointAnalizeType = 1;
		}
		else {
			m_JointAnalizeType = 0;
		}
		break;
	}
	UpdateData(false);
}

/// <summary>
/// �\�����@�̐ݒ�
/// </summary>
/// <param name="ScanID">�X�L����ID</param>
/// <param name="mode">�\�����[�h</param>
/// <returns>�\�����[�h��Ԃ�</returns>
int COprtTabPageAnalize::SetDisplayMode(int ScanID, int mode)
{
	switch (ScanID) {
	case		CWeldEvaluationDoc::eResinSurface:
		m_ResinDisplayMode = mode;			// �����ʕ\���^�C�v
		OnBnClickedRdoResinscan();
		UpdateData(true);
		m_ResinDisplayMode = mode;			// �����ʕ\���^�C�v
		break;
	case		CWeldEvaluationDoc::eMetalSurface:
		m_MetalDisplayMode = mode;			// �����ʕ\���^�C�v	
		OnBnClickedRdoMetalscan();
		UpdateData(true);
		m_MetalDisplayMode = mode;			// �����ʕ\���^�C�v
		break;
	case		CWeldEvaluationDoc::eJoiningResult:
		m_ResultDisplayMode = mode;			// �ڍ����ʕ\���^�C�v
		OnBnClickedRdoJointresultscan();
		UpdateData(true);
		m_ResultDisplayMode = mode;			// �����ʕ\���^�C�v
		break;
	}
	UpdateData(false);
	return mode;
}

/// <summary>
/// ��͕��@���ύX���ꂽ���̔���
/// </summary>
/// <returns>�ύX����Ă����true�A����Ă��Ȃ����false��Ԃ�</returns>
/// @remark �؂芷�����������{�����܂ŗL��
bool COprtTabPageAnalize::IsChangedAnalize()
{
	return m_bChangedAnalize;
}

/// <summary>
/// ��͕��@�ύX�t���O�̃��Z�b�g
/// </summary>
/// <param name="bFlag">�ύX�L���ݒ�</param>
void COprtTabPageAnalize::SetChangedAnalizeFlag(bool bFlag)
{
	m_bChangedAnalize = bFlag;
}


/// <summary>
/// �\�����@���ύX���ꂽ���̔���
/// </summary>
/// <returns>�ύX����Ă����true�A����Ă��Ȃ����false��Ԃ�</returns>
/// @remark �؂芷�����������{�����܂ŗL��
bool COprtTabPageAnalize::IsChangedDisplayMode()
{
	return m_bChangedDisplayMode;
}
