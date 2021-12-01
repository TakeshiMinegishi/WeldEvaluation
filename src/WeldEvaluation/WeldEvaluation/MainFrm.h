
// MainFrm.h : CMainFrame クラスのインターフェイス
//

#pragma once

/// <summary>
/// CMainFrame クラス
/// </summary>
class CMainFrame : public CFrameWnd
{
	
protected: // シリアル化からのみ作成します。
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 属性
public:

// 操作
public:

// オーバーライド
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);

// 実装
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // コントロール バー用メンバー
	CStatusBar        m_wndStatusBar;		///< ステータスバーオブジェクト

// 生成された、メッセージ割り当て関数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();

	DECLARE_MESSAGE_MAP()

};


