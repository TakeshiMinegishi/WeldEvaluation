
// WeldEvaluation.h : WeldEvaluation アプリケーションのメイン ヘッダー ファイル
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"       // メイン シンボル


// CWeldEvaluationApp:
// このクラスの実装については、WeldEvaluation.cpp を参照してください。
//

/// <summary>
/// CWeldEvaluatioアプリケーションクラス
/// </summary>
class CWeldEvaluationApp : public CWinApp
{
public:
	CWeldEvaluationApp();


// オーバーライド
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 実装
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CWeldEvaluationApp theApp;
