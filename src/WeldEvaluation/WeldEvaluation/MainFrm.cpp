
// MainFrm.cpp : CMainFrame �N���X�̎���
//

#include "stdafx.h"
#include "WeldEvaluation.h"
#include "WeldEvaluationDoc.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#define _FREE_WIND_SIZE_
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_GETMINMAXINFO()
	ON_WM_NCLBUTTONDBLCLK()
	ON_WM_NCMOUSEMOVE()
	ON_WM_NCLBUTTONUP()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_WINDOWPOSCHANGED()
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
#ifndef _FREE_WIND_SIZE_
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
		if (!msg.LoadString(IDM_PRJREGIST_EXISTUPDATE)) {
			msg = _T("�f�[�^���X�V����Ă��܂��B\n�X�V����Ă���f�[�^��j�����܂���?");
		}
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

/// <summary>
/// ��N���C�A���g�̈�����{�^���_�u���N���b�N���̏���
/// </summary>
/// <param name="nHitTest">�q�b�g�e�X�g�R�[�h</param>
/// <param name="point">�J�[�\���ʒu</param>
void CMainFrame::OnNcLButtonDblClk(UINT nHitTest, CPoint point)
{
#ifndef _FREE_WIND_SIZE_
	if (HTCAPTION != nHitTest) {
		CFrameWnd::OnNcLButtonDblClk(nHitTest, point);
	}
#else
	CFrameWnd::OnNcLButtonDblClk(nHitTest, point);
#endif
}

/// <summary>
/// ��N���C�A���g�̈���}�E�X�ړ����̏���
/// </summary>
/// <param name="nHitTest">�q�b�g�e�X�g�R�[�h</param>
/// <param name="point">�J�[�\���ʒu</param>
void CMainFrame::OnNcMouseMove(UINT nHitTest, CPoint point)
{
#ifndef _FREE_WIND_SIZE_
	if (HTCAPTION != nHitTest) {
		CFrameWnd::OnNcMouseMove(nHitTest, point);
	}
#else
	CFrameWnd::OnNcMouseMove(nHitTest, point);
#endif
}

/// <summary>
/// ��N���C�A���g�̈���}�E�X���{�^���������̏���
/// </summary>
/// <param name="nHitTest">�q�b�g�e�X�g�R�[�h</param>
/// <param name="point">�J�[�\���ʒu</param>
void CMainFrame::OnNcLButtonUp(UINT nHitTest, CPoint point)
{
#ifndef _FREE_WIND_SIZE_
	if (HTCAPTION != nHitTest) {
		CFrameWnd::OnNcLButtonUp(nHitTest, point);
	}
#else
	CFrameWnd::OnNcLButtonUp(nHitTest, point);
#endif
}

/// <summary>
/// ��N���C�A���g�̈���}�E�X���{�^�����㎞�̏���
/// </summary>
/// <param name="nHitTest">�q�b�g�e�X�g�R�[�h</param>
/// <param name="point">�J�[�\���ʒu</param>
void CMainFrame::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
#ifndef _FREE_WIND_SIZE_
	if (HTCAPTION != nHitTest) {
		CFrameWnd::OnNcLButtonUp(nHitTest, point);
	}
#else
	CFrameWnd::OnNcLButtonUp(nHitTest, point);
#endif
}

/// <summary>
/// �E�C���h���b�Z�[�W�f�B�X�p�b�`�O�̏���
/// </summary>
/// <param name="pMsg">���b�Z�[�W</param>
/// <returns>�f�B�X�p�b�`���ׂ��łȂ��ꍇ��true�A�f�B�X�p�b�`���K�v�ȏꍇ��false��Ԃ�</returns>
BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
#ifndef _FREE_WIND_SIZE_
	switch (pMsg->message)
	{
	case WM_SYSCOMMAND:
		{
			int command = (__int32)(pMsg->wParam) & 0xfff0;
			// �ŏ�������߂�Ȃ��s��C���hcommand == SC_RESTORE�h���R�����g��
			if (command == SC_MOVE || command == SC_TASKLIST ||/* command == SC_RESTORE ||*/ command == SC_VSCROLL || command == SC_HSCROLL) {
				return true;
			}
		}
		break;
	}

	if ((GetKeyState(VK_LWIN) < 0) || (GetKeyState(VK_RWIN) < 0)) {
		return true;
	}
#endif
	return CFrameWnd::PreTranslateMessage(pMsg);
}

/// <summary>
/// �E�C���h�̈ʒu�A�T�C�Y���ύX���ꂽ���̏���
/// </summary>
/// <param name="lpwndpos">WINDOWPOS�\���̂ւ̃|�C���^</param>
void CMainFrame::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
#ifndef _FREE_WIND_SIZE_
	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)GetActiveDocument();
	if (pDoc) {
		if (pDoc->IsNew() || pDoc->IsOpen()) {
			if ((!IsIconic()) && (!IsZoomed())) {
				lpwndpos->x = 0;
				lpwndpos->y = 0;
				lpwndpos->cx = 0;
				lpwndpos->cy = 0;
				lpwndpos->flags = SWP_NOMOVE | SWP_NOSIZE;
			}
		}
	}
#endif
	CFrameWnd::OnWindowPosChanged(lpwndpos);
}
