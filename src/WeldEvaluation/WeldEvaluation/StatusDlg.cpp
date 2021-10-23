// StatusDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "WeldEvaluation.h"
#include "StatusDlg.h"
#include "afxdialogex.h"
#include "message.h"


// CStatusDlg ダイアログ

IMPLEMENT_DYNAMIC(CStatusDlg, CDialogEx)

/// <summary>
/// コンストラクタ
/// </summary>
CStatusDlg::CStatusDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CStatusDlg::IDD, pParent)
{

}

/// <summary>
/// デストラクタ
/// </summary>
CStatusDlg::~CStatusDlg()
{
}

/// <summary>
/// データエクスチェンジ処理
/// </summary>
/// <param name="pDX">CDataExchangeオブジェクト</param>
void CStatusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDT_STATUS, m_CtrlEditStatus);
}


BEGIN_MESSAGE_MAP(CStatusDlg, CDialogEx)
	ON_COMMAND(ID_UPDATE_STATUS, UpdateStatus)
	ON_BN_CLICKED(IDCANCEL, &CStatusDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CStatusDlg メッセージ ハンドラー
/// <summary>
/// ステータスの更新
/// </summary>
void CStatusDlg::UpdateStatus()
{
	m_CtrlEditStatus.SetWindowText(m_Status);
}

/// <summary>
/// キャンセルボタン押下時処理
/// </summary>
void CStatusDlg::OnBnClickedCancel()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CDialogEx::OnCancel();
}
