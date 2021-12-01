// PropTabPageParameter.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "WeldEvaluation.h"
#include "PropTabPageParameter.h"
#include "afxdialogex.h"


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
	DDX_Control(pDX, IDC_MSG_FEILD, m_stcMsgFeild);
}


BEGIN_MESSAGE_MAP(CPropTabPageParameter, CDialogEx)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_NUMOFCLASS, &CPropTabPageParameter::OnDeltaposSpinNumofclass)
	ON_CBN_SELCHANGE(IDC_CMB_JOINRATIO_TARGET_LABEL, &CPropTabPageParameter::OnCbnSelchangeCmbJoinratioTargetLabel)
	ON_WM_ACTIVATE()
	ON_CBN_KILLFOCUS(IDC_CMB_JOINRATIO_TARGET_LABEL, &CPropTabPageParameter::OnCbnKillfocusCmbJoinratioTargetLabel)
	ON_EN_KILLFOCUS(IDC_EDT_NUMOFCLASS, &CPropTabPageParameter::OnEnKillfocusEdtNumofclass)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
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

	UpdateCmbJoinratioTargetLabel(true);

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
	UpdateCmbJoinratioTargetLabel(false);
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
		UpdateCmbJoinratioTargetLabel(false);
		CWnd *pWnd = GetParent()->GetParent();
		pWnd->SendMessage(WM_UPDATEREQUEST_PROPPAGE,(WPARAM)true,(LPARAM)m_PageID);
	}
}

/// <summary>
/// �ݒ蒆�̃N���X���̎擾
/// </summary>
/// <returns>���X����Ԃ�</returns>
int CPropTabPageParameter::GetNumbetOfClass()
{
	return m_NumberOfClass;
}

/// <summary>
/// �p�����[�^���t�@�C������ǂݍ���
/// </summary>
/// <param name="id">�X�V�΃C���f�b�N�X</param>
void CPropTabPageParameter::LoadParamater(int id)
{
	CWeldEvaluationDoc *pDoc = GetDocument();
	UpdateData(true);
	int AnalyzeMethod = pDoc->GetAnalysisMethod(id);

	switch(id) {
	case	CWeldEvaluationDoc::eResinSurface	:	// ����
		{
			m_NumberOfClass = pDoc->ResinGetNumberOfClass(AnalyzeMethod);
		}
		break;
	case	CWeldEvaluationDoc::eMetalSurface	:	// ����
		{
			m_NumberOfClass = pDoc->MetalGetNumberOfClass(AnalyzeMethod);
		}
		break;
	case	CWeldEvaluationDoc::eJoiningResult	:	// �ڍ�����
		{
			m_NumberOfClass = pDoc->ResultGetNumberOfClass(AnalyzeMethod);
		}
		break;
	}
	int dataid  = m_cmbJoinratioTarget.GetCurSel();
	if (dataid < 0) {
		ViewJointRatio(AnalyzeMethod, dataid, -1);
	} else {
		ViewJointRatio(AnalyzeMethod, id, (int)m_cmbJoinratioTarget.GetItemData(dataid));
	}
	UpdateData(false);
}

/// <summary>
/// �ڍ������̐ݒ�
/// </summary>
/// <param name="method">��͕��@</param>
/// <param name="id">�X�V�΃C���f�b�N�X</param>
/// <param name="ViewJointRatioNo">�ڍ��ʔԍ�</param>
void CPropTabPageParameter::ViewJointRatio(int method, int id, int ViewJointRatioNo)
{
	if (ViewJointRatioNo < 0) {
		if (m_cmbJoinratioTarget.GetCount() <= 0) {
			ViewJointRatioNo = -1;
		}
		else {
			if (!m_strJoinratioTarget.IsEmpty()) {
				int selid = m_cmbJoinratioTarget.SelectString(-1, m_strJoinratioTarget);
				if (id == CB_ERR) {
					ViewJointRatioNo = 0;
					selid = m_cmbJoinratioTarget.SetCurSel(0);
				}
				else {
					ViewJointRatioNo = selid;
				}
			}
			else {
				m_strJoinratioTarget = _T("1");
				ViewJointRatioNo = 0;
				int selid = m_cmbJoinratioTarget.SelectString(-1, m_strJoinratioTarget);
				if (id == CB_ERR) {
					selid = m_cmbJoinratioTarget.SetCurSel(0);
				}
			}

		}
	}

	if (ViewJointRatioNo < 0) {
		m_JointRatio.Format(_T("%.2lf"),0.0);
	} else {
		CWeldEvaluationDoc *pDoc = GetDocument();

		switch(id) {
		case	CWeldEvaluationDoc::eResinSurface	:	// ����
			{
				m_JointRatio.Format(_T("%.2lf"),pDoc->ResinGetJointRetio(method,ViewJointRatioNo));
				COLORREF col = pDoc->ResinGetJointColor(method,ViewJointRatioNo);
				if (col == 0) {
					col = pDoc->GetClassColor(ViewJointRatioNo, m_NumberOfClass);
				}
				UpdateIDColor(col);
			}
			break;
		case	CWeldEvaluationDoc::eMetalSurface	:	// ����
			{
				m_JointRatio.Format(_T("%.2lf"),pDoc->MetalGetJointRetio(method,ViewJointRatioNo));
				COLORREF col = pDoc->MetalGetJointColor(method,ViewJointRatioNo);
				if (col == 0) {
					col = pDoc->GetClassColor(ViewJointRatioNo, m_NumberOfClass);
				}
				UpdateIDColor(col);
		}
			break;
		case	CWeldEvaluationDoc::eJoiningResult	:	// �ڍ�����
			{
				m_JointRatio.Format(_T("%.2lf"),pDoc->ResultGetJointRetio(method,ViewJointRatioNo));
				COLORREF col = pDoc->ResultGetJointColor(method,ViewJointRatioNo);
				if (col == 0) {
					col = pDoc->GetClassColor(ViewJointRatioNo, m_NumberOfClass);
				}
				UpdateIDColor(col);
		}
			break;
		}
#if 1
		if (m_cmbJoinratioTarget.GetCount() > 0) {
			int selid = m_cmbJoinratioTarget.SetCurSel(ViewJointRatioNo);
			CString sel;
			m_cmbJoinratioTarget.GetLBText(selid, sel);
			selid = m_cmbJoinratioTarget.SelectString(-1, sel);
			m_strJoinratioTarget = sel;
			m_cmbJoinratioTarget.SetWindowTextW(sel);
		}
#endif

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
		return;
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
	CWeldEvaluationDoc *pDoc = GetDocument();
	int AnarizeMethod = pDoc->GetAnalysisMethod(m_PageID);

	switch(index) {
	case	CWeldEvaluationDoc::eResinSurface	:	// ����
		{
		bResult = pDoc->ResinSetNumberOfClass(AnarizeMethod,m_NumberOfClass);
		}
		break;
	case	CWeldEvaluationDoc::eMetalSurface	:	// ����
		{
			bResult = pDoc->MetalSetNumberOfClass(AnarizeMethod,m_NumberOfClass);
		}
		break;
	case	CWeldEvaluationDoc::eJoiningResult	:	// �ڍ�����
		{
			bResult = pDoc->ResultSetNumberOfClass(AnarizeMethod,m_NumberOfClass);
		}
		break;
	}
	UpdateData(false);
	return bResult;
}

/// <summary>
/// �X�V�̗L���m�F
/// </summary>
/// <returns>�X�V����Ă���ꍇ��true�A���s�ꍇ��false��Ԃ�</returns>
bool CPropTabPageParameter::ConfirmChange()
{
	bool bResult = false;
	UINT org = m_NumberOfClass;
	UpdateData(true);
	if (m_NumberOfClass != org) {
		UpdateData(false);
		// �X�V�{�^���̍X�V
		UpdateCmbJoinratioTargetLabel(false);
		CWnd *pWnd = GetParent()->GetParent();
		pWnd->SendMessage(WM_UPDATEREQUEST_PROPPAGE, (WPARAM)true, (LPARAM)0);
		bResult = true;
	}
	return bResult;
}

/// <summary>
/// CWeldEvaluationDoc�̎擾
/// </summary>
/// <returns>CWeldEvaluationDoc�ւ̃|�C���^��Ԃ�</returns>
CWeldEvaluationDoc *CPropTabPageParameter::GetDocument()
{
	CFormView *pWnd = (CFormView *)GetParent()->GetParent();
	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)pWnd->GetDocument();
	return pDoc;
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

	CWeldEvaluationDoc *pDoc = GetDocument();
	int AnarizeMethod = pDoc->GetAnalysisMethod(m_PageID);
	CPropTabPageParameter::ViewJointRatio(AnarizeMethod, m_PageID, (int)m_cmbJoinratioTarget.GetItemData(dataid));
}

/// <summary>
/// �ΏۃR���{�{�b�N�X���x���ύX������
/// </summary>
void CPropTabPageParameter::UpdateCmbJoinratioTargetLabel(bool renew, int nClass/* = -1*/)
{
	UpdateData(true);
	if (nClass > 0) {
		m_NumberOfClass = nClass;
	}
	int itemCount = m_cmbJoinratioTarget.GetCount();
	if ((renew) || (m_cmbJoinratioTarget.GetCount() != (int)m_NumberOfClass)) {
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
					int id = m_cmbJoinratioTarget.SelectString(-1,m_strJoinratioTarget);
					if (id != CB_ERR ) {
						CWeldEvaluationDoc *pDoc = GetDocument();
						int AnarizeMethod = pDoc->GetAnalysisMethod(m_PageID);
						ViewJointRatio(AnarizeMethod, m_PageID, id);
					}
				}
			}
		} else if (itemCount < (int)m_NumberOfClass) {
			m_cmbJoinratioTarget.ResetContent();
			for (int i =0 ; i < (int)m_NumberOfClass; i++) {
				TCHAR sval[255];
				if (_itot_s(i+1,sval,255,10) == 0) {
					CString strLabel = sval;
					int id = m_cmbJoinratioTarget.AddString(strLabel);
					m_cmbJoinratioTarget.SetItemData(id,i);
				}
			}
			itemCount = m_NumberOfClass;
			m_cmbJoinratioTarget.SetCurSel(0);
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

	CWeldEvaluationDoc *pDoc = GetDocument();
	int AnarizeMethod = pDoc->GetAnalysisMethod(m_PageID);

	UpdateCmbJoinratioTargetLabel(true);

	CString strJoinratioTarget = m_strJoinratioTarget;
	int selid = m_cmbJoinratioTarget.SelectString(-1, strJoinratioTarget);
	if (selid != CB_ERR) {
		CPropTabPageParameter::ViewJointRatio(AnarizeMethod, m_PageID, (int)m_cmbJoinratioTarget.GetItemData(selid));
	}

	UpdateStatus();

	UINT timerID = 1;
	UINT interval = 50;
	SetTimer(timerID, interval, NULL);
}

/// <summary>
/// �X�e�[�^�X�̍X�V
/// </summary>
void CPropTabPageParameter::UpdateStatus()
{
	CWeldEvaluationDoc *pDoc = GetDocument();
	int AnarizeMethod = pDoc->GetAnalysisMethod(m_PageID);

	if (!pDoc->IsExistClassificationResultFile(m_PageID, AnarizeMethod)) {
		ViewJointRatio(AnarizeMethod, m_PageID, m_cmbJoinratioTarget.GetCurSel());
		CString msg = _T("��͂������{�ł��B");
		m_stcMsgFeild.SetWindowText(msg);
		m_stcMsgFeild.ShowWindow(SW_SHOW);
	}
	else {
		if (pDoc->IsUpdateNumberOfClassifications(m_PageID, AnarizeMethod)) {
			CString msg = _T("���ސ����ύX����Ă��܂��B��͂����{���Ă��������B");
			m_stcMsgFeild.SetWindowText(msg);
			m_stcMsgFeild.ShowWindow(SW_SHOW);
		}
		else {
			m_stcMsgFeild.SetWindowText(_T(""));
			m_stcMsgFeild.ShowWindow(SW_HIDE);
		}
	}
}


/// <summary>
/// �ΏۃR���{�{�b�N�X�̃G�f�B�b�g�{�b�N�X�t�H�[�J�X����������
/// </summary>
void CPropTabPageParameter::OnCbnKillfocusCmbJoinratioTargetLabel()
{
	CString str = m_strJoinratioTarget;
	UpdateData(true);
	int dataid = m_cmbJoinratioTarget.SelectString(-1,m_strJoinratioTarget);
	if (dataid != CB_ERR) {
		CWeldEvaluationDoc *pDoc = GetDocument();
		int AnarizeMethod = pDoc->GetAnalysisMethod(m_PageID);
		CPropTabPageParameter::ViewJointRatio(AnarizeMethod, m_PageID, (int)m_cmbJoinratioTarget.GetItemData(dataid));
	}
	else {
		m_strJoinratioTarget = str;
	}
	UpdateData(false);
}

/// <summary>
/// �R���g���[���`�掞����
/// </summary>
/// <param name="pDC">�q�E�B���h�E�̕\���R���e�L�X�g</param>
/// <param name="pWnd">�F�����߂�R���g���[���ւ̃|�C���^</param>
/// <param name="nCtlColor">���g���[���̎��</param>
/// <returns>�R���g���[���̔w�i��`�悷�邽�߂Ɏg�p�����u���V�ւ̃n���h����Ԃ�</returns>
HBRUSH CPropTabPageParameter::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{

	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	int id = pWnd->GetDlgCtrlID();
	switch (id) {
	case IDC_MSG_FEILD:
		pDC->SetTextColor(RGB(255, 0, 0));
		break;

	}
	return hbr;
}

/// <summary>
/// �^�C�}�[
/// </summary>
/// <param name="nIDEvent">�^�C�}�[�̎��ʎq</param>
void CPropTabPageParameter::OnTimer(UINT_PTR nIDEvent)
{
	CWeldEvaluationDoc *pDoc = GetDocument();
	if (!pDoc->ExistScanFile(m_PageID)) {
		m_strJoinratioTarget = _T("");
		m_cmbJoinratioTarget.SetCurSel(-1);
	}
	else {
		if (m_strJoinratioTarget.IsEmpty()) {
			m_strJoinratioTarget = _T("1");
		}
		int selid = m_cmbJoinratioTarget.SelectString(-1, m_strJoinratioTarget);
		if (selid != CB_ERR) {
			int AnarizeMethod = pDoc->GetAnalysisMethod(m_PageID);
			CPropTabPageParameter::ViewJointRatio(AnarizeMethod, m_PageID, (int)m_cmbJoinratioTarget.GetItemData(selid));
		}
	}
	UpdateStatus();

	KillTimer(nIDEvent);
	CDialogEx::OnTimer(nIDEvent);
}