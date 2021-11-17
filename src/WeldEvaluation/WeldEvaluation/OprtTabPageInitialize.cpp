// OprtTabPageInitialize.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "WeldEvaluation.h"
#include "OprtTabPageInitialize.h"
#include "afxdialogex.h"
#include "message.h"
#include "CDeviceIO.h"


// COprtTabPageInitialize �_�C�A���O

IMPLEMENT_DYNAMIC(COprtTabPageInitialize, CDialogEx)

/// <summary>
/// �������R���X�g���N�^
/// </summary>
/// <param name="pParent">�e�E�C���h�ւ̃|�C���^</param>
COprtTabPageInitialize::COprtTabPageInitialize(CWnd* pParent /*=NULL*/)
	: CDialogEx(COprtTabPageInitialize::IDD, pParent)
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
COprtTabPageInitialize::~COprtTabPageInitialize()
{
}

/// <summary>
/// �f�[�^�G�N�X�`�F���W����
/// </summary>
/// <param name="pDX">CDataExchange�I�u�W�F�N�g</param>
void COprtTabPageInitialize::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COprtTabPageInitialize, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_WHITEBARANCE, &COprtTabPageInitialize::OnBnClickedBtnWhitebarance)
END_MESSAGE_MAP()


// COprtTabPageInitiaize ���b�Z�[�W �n���h���[

/// <summary>
/// �A�C�e���̃A�N�e�B�u/�f�B�A�N�e�B�u��
/// </summary>
/// <param name="ItemID">�A�C�e��ID</param>
/// <param name="bActive">�A�N�e�B�u�t���O</param>
void COprtTabPageInitialize::ItemEnable(int ItemID,bool bActive)
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
void COprtTabPageInitialize::ItemActive(bool bActive)
{
	ItemEnable(IDC_STC_WHITEBARANCE,bActive);
	ItemEnable(IDC_BTN_WHITEBARANCE,bActive);
}

#include "ScanDataIO.h"
/// <summary>
/// �z���C�g�o�����X�{�^������������
/// </summary>
void COprtTabPageInitialize::OnBnClickedBtnWhitebarance()
{
	CFormView *pWnd = (CFormView *)GetParent()->GetParent();
	LRESULT iret = pWnd->SendMessage(WM_WBSCAN_EXISTCHECK, (WPARAM)NULL, (LPARAM)NULL);
	if (iret == 1) {
		// �z���C�g�o�����X�p�̃X�L�������s��Ȃ��B
		return;
	}

	CWaitCursor waitCursor;
	CStatusDlgThread* pThread = DYNAMIC_DOWNCAST(CStatusDlgThread, AfxBeginThread(RUNTIME_CLASS(CStatusDlgThread) , 0, 0, CREATE_SUSPENDED));
	pThread->m_bAutoDelete = false;			// �����ȃA�N�Z�X�h�~�̂��ߎ����폜�͖�����
	pThread->ResumeThread();
	while (pThread->m_Dlg.m_hWnd == 0) {	// m_Dlg�̃E�B���h�E�����������܂őҋ@
		Sleep(0);
	}
	CString msg;
	msg.LoadString(IDS_PREPANING);
	pThread->UpdateStatus(msg);
	AddNode(pThread);  // �������d������

	if (pThread->m_Valid) {  // �L�����Z���{�^���������ꂽ�ꍇ
		pThread->m_Dlg.PostMessage(WM_COMMAND, IDOK); // Status�_�C�A���O�����
	}

	WaitForSingleObject(pThread->m_hThread, 30000);	// �X���b�h�̏I����30�b�����҂��Ă��i�ق�0�b�̂͂�

	if (!pThread->m_bResult) {
		msg.LoadString(IDM_ERR_SCAN);
		AfxMessageBox(msg, MB_OK | MB_ICONSTOP);
	}
	else {
		msg.LoadString(IDM_SCAN_SUCCESS);
		AfxMessageBox(msg, MB_OK | MB_ICONINFORMATION);
	}
	delete pThread;
}

/// <summary>
/// �z���C�g�o�����X�p�摜�B�e����
/// </summary>
/// <param name="pStatus">�X���b�h�I�u�W�F�N�g</param>
void COprtTabPageInitialize::AddNode(CStatusDlgThread* pStatus)
{
	if (!pStatus->m_Valid) {  // �L�����Z���{�^���������ꂽ�ꍇ�͉��������ɏI��
		return;
	}

	CFormView *pWnd = (CFormView *)GetParent()->GetParent();
	int result;
	LRESULT iret = pWnd->SendMessage(WM_WBSCAN_REQUES, (WPARAM)pStatus, (LPARAM)&result);
	if ((iret != 0) || (result != 0)) {
		pStatus->m_bResult = false;
	}
	else {
		pStatus->m_bResult = true;
	}

#if false
	CString buff;
	CDeviceIO device;
	int ID = device.Init(_T("DIO000"));
	if (ID >= 0) {
		if (pStatus->m_Valid) {  // �L�����Z���{�^����������Ă��Ȃ�
			if (device.ToHome(ID)) {
				if (!device.Move(ID, 1)) {
					return;
				}
				else {
					if (pStatus->m_Valid) {  // �L�����Z���{�^����������Ă��Ȃ�
						buff.Format(_T("Scan Start"));
						pStatus->UpdateStatus(buff);

						/////////////////////////////////////////////////////////
						// �z���C�g�o�����X�f�[�^�̎B�e
						/////////////////////////////////////////////////////////
					}
				}
			}
		}
	}
#endif
}

/// <summary>
/// �p�����[�^�̓ǂݍ���
/// </summary>
void COprtTabPageInitialize::LoadParamater(void)
{
}
