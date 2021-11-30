
// MainFrm.cpp : CMainFrame �N���X�̎���
//

#include "stdafx.h"
#include "WeldEvaluation.h"
#include "WeldEvaluationDoc.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // �X�e�[�^�X ���C�� �C���W�P�[�^�[
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/// <summary>
/// CMainFrame �R���X�g���N�V����
/// </summary>
CMainFrame::CMainFrame()
{
	// TODO: �����o�[�������R�[�h�������ɒǉ����Ă��������B
}

/// <summary>
/// CMainFrame �f�X�g���N�V����
/// </summary>
CMainFrame::~CMainFrame()
{
}

/// <summary>
/// �E�C���h�쐬����
/// </summary>
/// <param name="lpCreateStruct">CREATESTRUCT�I�u�W�F�N�g�ւ̃|�C���^</param>
/// <returns>���������ꍇ��0�A���s�����ꍇ��-1��Ԃ�</returns>
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("�X�e�[�^�X �o�[�̍쐬�Ɏ��s���܂����B\n");
		return -1;      // �쐬�ł��Ȃ��ꍇ
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	return 0;
}

/// <summary>
/// �E�C���h�쐬�O����
/// </summary>
/// <param name="cs">CREATESTRUCT�\����</param>
/// <returns>�쐬���p������ꍇ��TRUE�A����ȊO��FALSE��Ԃ�</returns>
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	::DestroyMenu(cs.hMenu);
	//DeleteObject(cs.hMenu);
	cs.hMenu = NULL;
#ifndef _DEBUG
	cs.style &= ~(WS_MAXIMIZEBOX | WS_THICKFRAME);
#endif
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: ���̈ʒu�� CREATESTRUCT cs ���C������ Window �N���X�܂��̓X�^�C����
	//  �C�����Ă��������B

	return TRUE;
}

// CMainFrame �f�f

#ifdef _DEBUG
/// <summary>
/// �I�u�W�F�N�g�̑Ó��������̎��{
/// </summary>
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}


/// <summary>
/// CDumpContext�I�u�W�F�N�g�̓��e���_���v
/// </summary>
/// <param name="dc">afxDump�f�f�_���v �R���e�L�X�g</param>
void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG

/// <summary>
/// �N���[�Y����
/// </summary>
void CMainFrame::OnClose()
{
	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)GetActiveDocument();
	if (pDoc->IsWorkProjectUpdated()) {
		CString msg;
		msg.LoadString(DM_PRJREGIST_EXISTUPDATE);
		if (AfxMessageBox(msg, MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON1) == IDNO) {
			return;
		}
		pDoc->ClrWorkProject();
	}
	CFrameWnd::OnClose();
}
// CMainFrame ���b�Z�[�W �n���h���[


/// <summary>
/// �E�C���h�̍ő�ŏ��l�̎擾
/// </summary>
/// <param name="lpMMI">�E�B���h�E�̍ő�T�C�Y�ƈʒu�A����эŏ�����эő�̒ǐՃT�C�Y�Ɋւ�����</param>
void CMainFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)GetActiveDocument();
	if (pDoc) {
		CSize min;
		if (pDoc->GetMinWndSize(min)) {
			lpMMI->ptMinTrackSize.x = min.cx;
			lpMMI->ptMinTrackSize.y = min.cy;
		}
	}
}

