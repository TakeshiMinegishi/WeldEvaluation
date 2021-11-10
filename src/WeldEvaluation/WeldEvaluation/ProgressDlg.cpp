// ProgressDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "WeldEvaluation.h"
#include "ProgressDlg.h"
#include "afxdialogex.h"
#include "message.h"


// CProgressDlg �_�C�A���O

IMPLEMENT_DYNAMIC(CProgressDlg, CDialogEx)

/// <summary>
/// �R���X�g���N�^
/// </summary>
CProgressDlg::CProgressDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CProgressDlg::IDD, pParent)
	, m_stcStatusLabel(_T(""))
{
	m_title		= _T("");
	m_label		= _T("");
	m_rangeMin	= 0;
	m_rangeMax	= 100;
	m_pos		= 0;
	m_resolution = 0.0;
	m_pParentWnd	= pParent;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
CProgressDlg::~CProgressDlg()
{
}

/// <summary>
/// �f�[�^�G�N�X�`�F���W����
/// </summary>
/// <param name="pDX">CDataExchange�I�u�W�F�N�g</param>
void CProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STC_STATUS, m_stcStatusLabel);
	DDX_Control(pDX, IDC_PRO_STATUS, m_prgress);
}


BEGIN_MESSAGE_MAP(CProgressDlg, CDialogEx)
	ON_BN_CLICKED(IDCANCEL, &CProgressDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CProgressDlg ���b�Z�[�W �n���h���[
/// <summary>
/// �_�C�A���O�̏�����
/// </summary>
BOOL CProgressDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetWindowText(m_title);
	m_stcStatusLabel = m_label;
	m_resolution = 100.0 / (double)(m_rangeMax - m_rangeMin);
		
	m_prgress.SetRange(0,100);
	m_prgress.SetPos((int)(m_pos * m_resolution));

	return TRUE;
}

/// <summary>
/// �^�C�g���̐ݒ�
/// </summary>
/// <param name="title">�^�C�g��</param>
void CProgressDlg::setTitle(CString title)
{
	m_title = title;
}

/// <summary>
/// �X�e�[�^�X���b�Z�[�W�̐ݒ�
/// </summary>
/// <param name="Statuslabel">���b�Z�[�W</param>
void CProgressDlg::setlabel(CString Statuslabel)
{
	m_label = Statuslabel;
	if (IsWindow(m_hWnd)) {
		m_stcStatusLabel = m_label;
		UpdateData(false);
	}
}

/// <summary>
/// �����W�̐ݒ�
/// </summary>
/// <param name="min">�ŏ��l</param>
/// <param name="max">�ŗ���</param>
void CProgressDlg::setRange(UINT min, UINT max)
{
	m_rangeMin	= min;
	m_rangeMax	= max;
}

/// <summary>
/// �����W�̎擾
/// </summary>
/// <param name="min">�ŏ��l</param>
/// <param name="max">�ő�l</param>
void CProgressDlg::getRange(int &min, int & max)
{
	min = m_rangeMin;
	max = m_rangeMax;
}

/// <summary>
/// �v���O���X�o�[�ʒu�̐ݒ�
/// </summary>
/// <param name="pos">�ʒu</param>
void CProgressDlg::setPosition(UINT pos)
{
	m_pos		= pos;
	if (IsWindow(m_hWnd)) {
		int lpos = (int)(m_pos * m_resolution + 0.5);
		m_prgress.SetPos(lpos);
	}
	MSG msg;
	while (PeekMessage(&msg, m_hWnd,  0, 0, PM_REMOVE)) {
		TranslateMessage(&msg); 
		DispatchMessage(&msg); 
	}
}

/// <summary>
/// �L�����Z���{�^����������
/// </summary>
void CProgressDlg::OnBnClickedCancel()
{
//	CWnd *pWnd = GetParent();
	m_pParentWnd->PostMessage(WM_READ_RESULT_STATUS,READ_RESULT_STOP,NULL);
	//	CDialogEx::OnCancel();
}
