// CSplashDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "WeldEvaluation.h"
#include "CSplashDlg.h"
#include "afxdialogex.h"


// CSplashDlg ダイアログ

IMPLEMENT_DYNAMIC(CSplashDlg, CDialogEx)

CSplashDlg::CSplashDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SPLASH_DLG, pParent)
{

}

CSplashDlg::~CSplashDlg()
{
}

void CSplashDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSplashDlg, CDialogEx)
END_MESSAGE_MAP()


// CSplashDlg メッセージ ハンドラー
