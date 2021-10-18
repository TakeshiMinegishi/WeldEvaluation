// OprtTabPageSetting.cpp : 実装ファイル
//

#include "stdafx.h"
#include "WeldEvaluation.h"
#include "OprtTabPageSetting.h"
#include "afxdialogex.h"
#include "WeldEvaluationDoc.h"
#include "message.h"


// COprtTabPageSetting ダイアログ

IMPLEMENT_DYNAMIC(COprtTabPageSetting, CDialogEx)

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="pParent">親ウインドへのポインタ</param>
COprtTabPageSetting::COprtTabPageSetting(CWnd* pParent /*=NULL*/)
	: CDialogEx(COprtTabPageSetting::IDD, pParent)
	, m_strTestName(_T(""))
{

}

/// <summary>
/// デストラクタ
/// </summary>
COprtTabPageSetting::~COprtTabPageSetting()
{
}

/// <summary>
/// データエクスチェンジ処理
/// </summary>
/// <param name="pDX">CDataExchangeオブジェクト</param>
void COprtTabPageSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_TESTNAME, m_strTestName);
}


BEGIN_MESSAGE_MAP(COprtTabPageSetting, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_REGIST, &COprtTabPageSetting::OnBnClickedBtnRegist)
	ON_BN_CLICKED(IDC_BTN_IMAGEOUT, &COprtTabPageSetting::OnBnClickedBtnImageout)
END_MESSAGE_MAP()


// COprtTabPageSetting メッセージ ハンドラー

/// <summary>
/// アイテムのアクティブ/ディアクティブ化
/// </summary>
/// <param name="ItemID">アイテムID</param>
/// <param name="bActive">アクティブフラグ</param>
void COprtTabPageSetting::ItemEnable(int ItemID,bool bActive)
{
	CWnd *pWnd;
	pWnd = GetDlgItem(ItemID);
	if (pWnd) {
		pWnd->EnableWindow(bActive);
	}
}

/// <summary>
/// アイテムのアクティブ/ディアクティブ化
/// </summary>
/// <param name="bActive">アクティブフラグ</param>
void COprtTabPageSetting::ItemActive(bool bActive)
{
	ItemEnable(IDC_STC_TESTNAME,bActive);
	ItemEnable(IDC_EDT_TESTNAME,bActive);
	ItemEnable(IDC_BTN_REGIST,bActive);
	ItemEnable(IDC_BTN_IMAGEOUT,bActive);
}

/// <summary>
/// アイテムの有効/無効化
/// </summary>
/// <param name="bEnable">有効無効フラグ</param>
/// <param name="bReadMode">リードモード</param>
void COprtTabPageSetting::UpddateResist(bool bEnable, bool bReadMode)
{
	if (bEnable) {
		if (bReadMode) {
			ItemEnable(IDC_STC_TESTNAME,FALSE);
			ItemEnable(IDC_EDT_TESTNAME,FALSE);
		} else {
			ItemEnable(IDC_STC_TESTNAME,bEnable);
			ItemEnable(IDC_EDT_TESTNAME,bEnable);
		}
		Update();
	} else {
		ItemEnable(IDC_STC_TESTNAME,bEnable);
		ItemEnable(IDC_EDT_TESTNAME,bEnable);
		ItemEnable(IDC_BTN_IMAGEOUT,bEnable);
	}
	ItemEnable(IDC_BTN_REGIST,bEnable);
}

void COprtTabPageSetting::Update()
{
	CFormView *pWnd = (CFormView *)GetParent()->GetParent();
	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)pWnd->GetDocument();

	CString ResinScan	= pDoc->GetResinScanImagePath();
	CString ResinResult = pDoc->GetResinClassificationResultPath();

	CString MetalScan	= pDoc->GetMetalScanImagePath();
	CString MetalResult = pDoc->GetMetalClassificationResultPath();

	CString ClassScan	= pDoc->GetClassScanImagePath();

	if (!ResinScan.IsEmpty() || !MetalScan.IsEmpty() || !ClassScan.IsEmpty() ||
		!ResinResult.IsEmpty() || !MetalResult.IsEmpty()) {
		ItemEnable(IDC_BTN_IMAGEOUT,TRUE);
	} else {
		ItemEnable(IDC_BTN_IMAGEOUT,FALSE);
	}
}

/// <summary>
/// パラメータの取得
/// </summary>
void COprtTabPageSetting::LoadParamater(void)
{
	CFormView *pWnd = (CFormView *)GetParent()->GetParent();
	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)pWnd->GetDocument();

	CString str = pDoc->GetTestName();
	m_strTestName = str;
	UpdateData(false);
}

/// <summary>
/// 登録ボタン押下処理
/// </summary>
void COprtTabPageSetting::OnBnClickedBtnRegist()
{
	UpdateData(true);

	CFormView *pWnd = (CFormView *)GetParent()->GetParent();
	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)pWnd->GetDocument();
	if (!pDoc->SetTestName(m_strTestName)) {
		// Error
	}

	if (pWnd->SendMessage(WM_RESIST_REGIST,NULL,NULL) != 0) {
		// Error
	}
}

/// <summary>
/// 画像出力ボタン押下処理
/// </summary>
void COprtTabPageSetting::OnBnClickedBtnImageout()
{
	CFormView *pWnd = (CFormView *)GetParent()->GetParent();
	if (pWnd->SendMessage(WM_IMAGE_OUTPUT_REGIST,NULL,NULL) != 0) {
	}
}
