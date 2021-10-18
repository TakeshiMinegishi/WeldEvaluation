// OprtTabPageInitialize.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "WeldEvaluation.h"
#include "OprtTabPageInitialize.h"
#include "afxdialogex.h"


// COprtTabPageInitialize �_�C�A���O

IMPLEMENT_DYNAMIC(COprtTabPageInitialize, CDialogEx)

COprtTabPageInitialize::COprtTabPageInitialize(CWnd* pParent /*=NULL*/)
	: CDialogEx(COprtTabPageInitialize::IDD, pParent)
{

}

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


void COprtTabPageInitialize::OnBnClickedBtnWhitebarance()
{
	CStatusDlgThread* pThread = DYNAMIC_DOWNCAST(CStatusDlgThread, AfxBeginThread(RUNTIME_CLASS(CStatusDlgThread) , 0, 0, CREATE_SUSPENDED));
	pThread->m_bAutoDelete = false;			// �����ȃA�N�Z�X�h�~�̂��ߎ����폜�͖�����
	pThread->ResumeThread();
	while (pThread->m_Dlg.m_hWnd == 0) {	// m_Dlg�̃E�B���h�E�����������܂őҋ@
		Sleep(0);
	}
	pThread->UpdateStatus(_T("Start White Balance"));
	AddNode(pThread);  // �������d������

	if (!pThread->m_Valid) {  // �L�����Z���{�^���������ꂽ�ꍇ
//		InitData();
	}
	else {
//	    ShowData(); // ����I�����̌y������
		pThread->m_Dlg.PostMessage(WM_COMMAND, IDOK); // Status�_�C�A���O�����
	}

	WaitForSingleObject(pThread->m_hThread, 30000);	// �X���b�h�̏I����30�b�����҂��Ă��i�ق�0�b�̂͂�
	delete pThread;
}

void COprtTabPageInitialize::AddNode(CStatusDlgThread* pStatus)
{
	if (!pStatus->m_Valid) {  // �L�����Z���{�^���������ꂽ�ꍇ�͉��������ɏI��
		return;
	}

	Sleep(3000);
//	for each (auto child in pItem->matName)
//	{
//		// ��������
//		AddNode(&m_Item[child], hTree, pStatus);  // �ċA�Ăяo��(�d�������̌���)
//	}
		
    // Status��\��
	CString buff;
	buff.Format(_T("Adding item to tree: %d"), 1);
	pStatus->UpdateStatus(buff);
}

void COprtTabPageInitialize::LoadParamater(void)
{
}
