// StatusDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "WeldEvaluation.h"
#include "StatusDlg.h"
#include "afxdialogex.h"
#include "message.h"


// CStatusDlg �_�C�A���O

IMPLEMENT_DYNAMIC(CStatusDlg, CDialogEx)

/// <summary>
/// �R���X�g���N�^
/// </summary>
CStatusDlg::CStatusDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CStatusDlg::IDD, pParent)
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
CStatusDlg::~CStatusDlg()
{
}

/// <summary>
/// �f�[�^�G�N�X�`�F���W����
/// </summary>
/// <param name="pDX">CDataExchange�I�u�W�F�N�g</param>
void CStatusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDT_STATUS, m_CtrlEditStatus);
}


BEGIN_MESSAGE_MAP(CStatusDlg, CDialogEx)
	ON_COMMAND(ID_UPDATE_STATUS, UpdateStatus)
	ON_BN_CLICKED(IDCANCEL, &CStatusDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CStatusDlg ���b�Z�[�W �n���h���[
/// <summary>
/// �X�e�[�^�X�̍X�V
/// </summary>
void CStatusDlg::UpdateStatus()
{
	m_CtrlEditStatus.SetWindowText(m_Status);
}

/// <summary>
/// �L�����Z���{�^������������
/// </summary>
void CStatusDlg::OnBnClickedCancel()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	CDialogEx::OnCancel();
}
