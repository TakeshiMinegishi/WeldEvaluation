// PropTabPageParameter.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "WeldEvaluation.h"
#include "PropTabPageParameter.h"
#include "afxdialogex.h"
#include "WeldEvaluationDoc.h"


// CPropTabPageParameter �_�C�A���O

IMPLEMENT_DYNAMIC(CPropTabPageParameter, CDialogEx)

/// <summary>
/// �R���X�g���N�^
/// </summary>
CPropTabPageParameter::CPropTabPageParameter(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPropTabPageParameter::IDD, pParent)
	, m_NumberOfClass(0)
	, m_JointRatio(_T(""))
	, m_strJoinratioTarget(_T(""))
{
	m_PageID = CWeldEvaluationDoc::eResinSurface;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
CPropTabPageParameter::~CPropTabPageParameter()
{
}

/// <summary>
/// �f�[�^�G�N�X�`�F���W����
/// </summary>
/// <param name="pDX">CDataExchange�I�u�W�F�N�g</param>
void CPropTabPageParameter::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_NUMOFCLASS, m_NumberOfClass);
	DDX_Text(pDX, IDC_EDT_JOINRATIO, m_JointRatio);
	DDX_Control(pDX, IDC_CMB_JOINRATIO_TARGET_LABEL, m_cmbJoinratioTarget);
	DDX_CBString(pDX, IDC_CMB_JOINRATIO_TARGET_LABEL, m_strJoinratioTarget);
	DDX_Control(pDX, IDC_STC_IDCOLOR, m_stcIDColor);
}


BEGIN_MESSAGE_MAP(CPropTabPageParameter, CDialogEx)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_NUMOFCLASS, &CPropTabPageParameter::OnDeltaposSpinNumofclass)
	ON_CBN_SELCHANGE(IDC_CMB_JOINRATIO_TARGET_LABEL, &CPropTabPageParameter::OnCbnSelchangeCmbJoinratioTargetLabel)
	ON_WM_ACTIVATE()
	ON_CBN_KILLFOCUS(IDC_CMB_JOINRATIO_TARGET_LABEL, &CPropTabPageParameter::OnCbnKillfocusCmbJoinratioTargetLabel)
	ON_EN_KILLFOCUS(IDC_EDT_NUMOFCLASS, &CPropTabPageParameter::OnEnKillfocusEdtNumofclass)
END_MESSAGE_MAP()


// CPropTabPageParameter ���b�Z�[�W �n���h���[

/// <summary>
/// �y�[�WID�̐ݒ�
/// </summary>
/// <param name="PageID">�y�[�WID</param>
void CPropTabPageParameter::setPageID(int PageID)
{
	m_PageID = PageID;
}

/// <summary>
/// �y�[�WID�̎擾
/// </summary>
int CPropTabPageParameter::getPageID()
{
	return m_PageID;
}

/// <summary>
/// �_�C�A���O�A�C�e���̂̃A�N�e�B�u/�f�B�A�N�e�B�u��
/// </summary>
/// <param name="ItemID">�A�C�e���h�c</param>
/// <param name="bActive">�A�N�e�B�u�t���O</param>
void CPropTabPageParameter::ItemEnable(int ItemID,bool bActive)
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
void CPropTabPageParameter::ItemActive(bool bActive)
{
	ItemEnable(IDC_STC_NUMOFCLASS_LABEL,bActive);
	ItemEnable(IDC_EDT_NUMOFCLASS,bActive);
	ItemEnable(IDC_SPIN_NUMOFCLASS,bActive);

	ItemEnable(IDC_STC_JOINRATIO_LABEL,bActive);
	ItemEnable(IDC_EDT_JOINRATIO,bActive);
	ItemEnable(IDC_STC_JOINRATIO_UNIT,bActive);

	ItemEnable(IDC_STC_JOINRATIO_TARGET_LABEL,bActive);
	ItemEnable(IDC_CMB_JOINRATIO_TARGET_LABEL,bActive);

	ItemEnable(IDC_STC_IDCOLOR, bActive);

	UpdateCmbJoinratioTargetLabel();

}

/// <summary>
/// ���ސ��ҏW�̃X�s���h�����쎞�̏���
/// </summary>
/// <param name="pNMHDR">LPNMUPDOWN�I�u�W�F�N�g</param>
/// <param name="pResult">����</param>
void CPropTabPageParameter::OnDeltaposSpinNumofclass(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	*pResult = 0;

	UpdateData(true);
	int Min = 1;
	int Max = 100;
	int ival = (int)m_NumberOfClass;
	ival -= pNMUpDown->iDelta;
	if (ival < Min) {
		ival = Min;
	} else if (ival > Max) {
		ival = Max;
	}
	m_NumberOfClass = (UINT)ival;
	UpdateData(false);
	UpdateCmbJoinratioTargetLabel();
	// �X�V�{�^���̍X�V
	CWnd *pWnd = GetParent()->GetParent();
	pWnd->SendMessage(WM_UPDATEREQUEST_PROPPAGE,(WPARAM)true,(LPARAM)0);
}


/// <summary>
/// ���ސ��ҏW�{�b�N�X���t�H�[�J�X�����������̏���
/// </summary>
void CPropTabPageParameter::OnEnKillfocusEdtNumofclass()
{
	UINT org = m_NumberOfClass;
	UpdateData(true);
	if (m_NumberOfClass != org) {
		UpdateData(false);
		// �X�V�{�^���̍X�V
		UpdateCmbJoinratioTargetLabel();
		CWnd *pWnd = GetParent()->GetParent();
		pWnd->SendMessage(WM_UPDATEREQUEST_PROPPAGE,(WPARAM)true,(LPARAM)0);
	}
}

/// <summary>
/// �p�����[�^���t�@�C������ǂݍ���
/// </summary>
/// <param name="id">�X�V�΃C���f�b�N�X</param>
void CPropTabPageParameter::LoadParamater(int id)
{
	CFormView *pWnd = (CFormView *)GetParent()->GetParent();
	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)pWnd->GetDocument();
	UpdateData(true);

	switch(id) {
	case	CWeldEvaluationDoc::eResinSurface	:	// ����
		{
			m_NumberOfClass = pDoc->ResinGetNumberOfClass();
		}
		break;
	case	CWeldEvaluationDoc::eMetalSurface	:	// ����
		{
			m_NumberOfClass = pDoc->MetalGetNumberOfClass();
		}
		break;
	case	CWeldEvaluationDoc::eJoiningResult	:	// �ڍ�����
		{
			m_NumberOfClass = pDoc->ResultGetNumberOfClass();
		}
		break;
	}
	int dataid  = m_cmbJoinratioTarget.GetCurSel();
	if (dataid < 0) {
		ViewJointRatio(dataid, -1);
	} else {
		ViewJointRatio(id, (int)m_cmbJoinratioTarget.GetItemData(dataid));
	}
	UpdateData(false);
}

/// <summary>
/// �ڍ������̐ݒ�
/// </summary>
/// <param name="id">�X�V�΃C���f�b�N�X</param>
/// <param name="ViewJointRatioNo">�ڍ��ʔԍ�</param>
void CPropTabPageParameter::ViewJointRatio(int id, int ViewJointRatioNo)
{
	if (ViewJointRatioNo < 0) {
		m_JointRatio.Format(_T("%.1lf"),0.0);
	} else {
		CFormView *pWnd = (CFormView *)GetParent()->GetParent();
		CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)pWnd->GetDocument();

		switch(id) {
		case	CWeldEvaluationDoc::eResinSurface	:	// ����
			{
				m_JointRatio.Format(_T("%.1lf"),pDoc->ResinGetJointRetio(ViewJointRatioNo));
				UpdateIDColor(pDoc->ResinGetJointColor(ViewJointRatioNo));
			}
			break;
		case	CWeldEvaluationDoc::eMetalSurface	:	// ����
			{
				m_JointRatio.Format(_T("%.1lf"),pDoc->MetalGetJointRetio(ViewJointRatioNo));
				UpdateIDColor(pDoc->MetalGetJointColor(ViewJointRatioNo));
		}
			break;
		case	CWeldEvaluationDoc::eJoiningResult	:	// �ڍ�����
			{
				m_JointRatio.Format(_T("%.1lf"),pDoc->ResultGetJointRetio(ViewJointRatioNo));
				UpdateIDColor(pDoc->ResultGetJointColor(ViewJointRatioNo));
		}
			break;
		}
	}
	UpdateData(false);
}

/// <summary>
/// ���ސF�𔽉f
/// </summary>
/// <param name="color">�ύX�F</param>
void CPropTabPageParameter::UpdateIDColor(COLORREF color)
{
	CWnd *pWnd;
	pWnd = GetDlgItem(IDC_STC_IDCOLOR);
	if (pWnd) {
		CDC *dc = pWnd->GetDC();
		if (dc == NULL)
		{
			return;
		}
		CRect rect;
		pWnd->GetClientRect(rect);
		CBrush blackBrush(color);
		CBrush *orgBrush;
		orgBrush = dc->SelectObject(&blackBrush);
		dc->FillRect(&rect, &blackBrush);
		dc->SelectObject(orgBrush);
		ReleaseDC(dc);
	}
}


/// <summary>
/// ���ސ��𔽉f
/// </summary>
/// <param name="index">�X�V�΃C���f�b�N�X</param>
/// <returns>�����ꍇ��true�A���s�ꍇ��false��Ԃ�</returns>
bool CPropTabPageParameter::Update(int index)
{
	bool bResult = false;
	UpdateData(true);
	CFormView *pWnd = (CFormView *)GetParent()->GetParent();
	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)pWnd->GetDocument();

	switch(index) {
	case	CWeldEvaluationDoc::eResinSurface	:	// ����
		{
			bResult = pDoc->ResinSetNumberOfClass(m_NumberOfClass);
		}
		break;
	case	CWeldEvaluationDoc::eMetalSurface	:	// ����
		{
			bResult = pDoc->MetalSetNumberOfClass(m_NumberOfClass);
		}
		break;
	case	CWeldEvaluationDoc::eJoiningResult	:	// �ڍ�����
		{
			bResult = pDoc->ResultSetNumberOfClass(m_NumberOfClass);
		}
		break;
	}
	UpdateData(false);
	return false;
}


/// <summary>
/// �ΏۃR���{�{�b�N���ڕύX������
/// </summary>
void CPropTabPageParameter::OnCbnSelchangeCmbJoinratioTargetLabel()
{
	UpdateData(true);
	int dataid = m_cmbJoinratioTarget.GetCurSel();
	if (dataid == CB_ERR) {
		return;
	}
	CString str;
	m_cmbJoinratioTarget.GetLBText(dataid, str);
	m_strJoinratioTarget = str;
	UpdateData(false);
	CPropTabPageParameter::ViewJointRatio(m_PageID, (int)m_cmbJoinratioTarget.GetItemData(dataid));
}

/// <summary>
/// �ΏۃR���{�{�b�N�X���x���ύX������
/// </summary>
void CPropTabPageParameter::UpdateCmbJoinratioTargetLabel()
{
	UpdateData(true);
	int itemCount = m_cmbJoinratioTarget.GetCount();
	if (m_cmbJoinratioTarget.GetCount() != (int)m_NumberOfClass) {
		CString strJoinratioTarget = m_strJoinratioTarget;
		if (itemCount > (int)m_NumberOfClass) {
			for (int i = itemCount-1; i >= (int)m_NumberOfClass; i--) {
				m_cmbJoinratioTarget.DeleteString(i);
			}
			if (m_cmbJoinratioTarget.FindString(0,strJoinratioTarget) == -1) {
				TCHAR sval[255];
				if (_itot_s(m_NumberOfClass,sval,255,10) == 0) {
					m_strJoinratioTarget = sval;
					UpdateData(false);
					int id = m_cmbJoinratioTarget.SelectString(0,m_strJoinratioTarget);
					if (id != CB_ERR ) {
						ViewJointRatio(m_PageID, id);
					}
				}
			}
		} else if (itemCount < (int)m_NumberOfClass) {
			for (int i =itemCount ; i < (int)m_NumberOfClass; i++) {
				TCHAR sval[255];
				if (_itot_s(i+1,sval,255,10) == 0) {
					int id = m_cmbJoinratioTarget.AddString(sval);
					m_cmbJoinratioTarget.SetItemData(id,i);
				}
			}
		}
	}
	UpdateData(false);
}

/// <summary>
/// �E�C���h�A�N�e�B�u������
/// </summary>
/// <param name="nState">�A�N�e�B�u�t���O</param>
/// <param name="pWndOther">�ΏۃE�C���h�ւ̃|�C���^</param>
/// <param name="bMinimized">�ŏ����t���O</param>
void CPropTabPageParameter::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);

	UpdateCmbJoinratioTargetLabel();
}


/// <summary>
/// �ΏۃR���{�{�b�N�X�̃G�f�B�b�g�{�b�N�X�t�H�[�J�X����������
/// </summary>
void CPropTabPageParameter::OnCbnKillfocusCmbJoinratioTargetLabel()
{
	CString str = m_strJoinratioTarget;
	UpdateData(true);
	int dataid = m_cmbJoinratioTarget.SelectString(0,m_strJoinratioTarget);
	if (dataid != CB_ERR) {
		CPropTabPageParameter::ViewJointRatio(m_PageID, (int)m_cmbJoinratioTarget.GetItemData(dataid));
	}
	else {
		m_strJoinratioTarget = str;
	}
	UpdateData(false);
}
