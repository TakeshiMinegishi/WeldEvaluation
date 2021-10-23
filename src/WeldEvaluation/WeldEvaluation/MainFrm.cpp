
// MainFrm.cpp : CMainFrame クラスの実装
//

#include "stdafx.h"
#include "WeldEvaluation.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
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


// CMainFrame メッセージ ハンドラー


