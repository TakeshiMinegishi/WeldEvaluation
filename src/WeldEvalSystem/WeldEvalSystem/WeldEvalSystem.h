
// WeldEvalSystem.h : WeldEvalSystem アプリケーションのメイン ヘッダー ファイル
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"       // メイン シンボル


// CWeldEvalSystemApp:
// このクラスの実装については、WeldEvalSystem.cpp を参照してください。
//

class CWeldEvalSystemApp : public CWinApp
{
public:
	CWeldEvalSystemApp();


// オーバーライド
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 実装

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CWeldEvalSystemApp theApp;
