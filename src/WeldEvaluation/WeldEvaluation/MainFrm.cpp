
// MainFrm.cpp : CMainFrame クラスの実装
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
	ID_SEPARATOR,           // ステータス ライン インジケーター
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/// <summary>
/// CMainFrame コンストラクション
/// </summary>
CMainFrame::CMainFrame()
{
	// TODO: メンバー初期化コードをここに追加してください。
}

/// <summary>
/// CMainFrame デストラクション
/// </summary>
CMainFrame::~CMainFrame()
{
}

/// <summary>
/// ウインド作成処理
/// </summary>
/// <param name="lpCreateStruct">CREATESTRUCTオブジェクトへのポインタ</param>
/// <returns>成功した場合は0、失敗した場合は-1を返す</returns>
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("ステータス バーの作成に失敗しました。\n");
		return -1;      // 作成できない場合
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	return 0;
}

/// <summary>
/// ウインド作成前処理
/// </summary>
/// <param name="cs">CREATESTRUCT構造体</param>
/// <returns>作成を継続する場合はTRUE、それ以外はFALSEを返す</returns>
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
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	return TRUE;
}

// CMainFrame 診断

#ifdef _DEBUG
/// <summary>
/// オブジェクトの妥当性検査の実施
/// </summary>
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}


/// <summary>
/// CDumpContextオブジェクトの内容をダンプ
/// </summary>
/// <param name="dc">afxDump診断ダンプ コンテキスト</param>
void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG

/// <summary>
/// クローズ処理
/// </summary>
void CMainFrame::OnClose()
{
	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)GetActiveDocument();
	if (pDoc->IsWorkProjectUpdated()) {
		CString msg;
		if (!msg.LoadString(IDM_PRJREGIST_EXISTUPDATE)) {
			msg = _T("データが更新されています。\n更新されているデータを破棄しますか?");
		}
		if (AfxMessageBox(msg, MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON1) == IDNO) {
			return;
		}
		pDoc->ClrWorkProject();
	}
	CFrameWnd::OnClose();
}
// CMainFrame メッセージ ハンドラー


/// <summary>
/// ウインドの最大最小値の取得
/// </summary>
/// <param name="lpMMI">ウィンドウの最大サイズと位置、および最小および最大の追跡サイズに関する情報</param>
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
/// 非クライアント領域内左ボタンダブルクリック時の処理
/// </summary>
/// <param name="nHitTest">ヒットテストコード</param>
/// <param name="point">カーソル位置</param>
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
/// 非クライアント領域内マウス移動時の処理
/// </summary>
/// <param name="nHitTest">ヒットテストコード</param>
/// <param name="point">カーソル位置</param>
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
/// 非クライアント領域内マウス左ボタン押下時の処理
/// </summary>
/// <param name="nHitTest">ヒットテストコード</param>
/// <param name="point">カーソル位置</param>
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
/// 非クライアント領域内マウス左ボタン押上時の処理
/// </summary>
/// <param name="nHitTest">ヒットテストコード</param>
/// <param name="point">カーソル位置</param>
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
/// ウインドメッセージディスパッチ前の処理
/// </summary>
/// <param name="pMsg">メッセージ</param>
/// <returns>ディスパッチすべきでない場合はtrue、ディスパッチが必要な場合はfalseを返す</returns>
BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
#ifndef _FREE_WIND_SIZE_
	switch (pMsg->message)
	{
	case WM_SYSCOMMAND:
		{
			int command = (__int32)(pMsg->wParam) & 0xfff0;
			// 最小化から戻らない不具合修正”command == SC_RESTORE”をコメント化
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
/// ウインドの位置、サイズが変更された時の処理
/// </summary>
/// <param name="lpwndpos">WINDOWPOS構造体へのポインタ</param>
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
