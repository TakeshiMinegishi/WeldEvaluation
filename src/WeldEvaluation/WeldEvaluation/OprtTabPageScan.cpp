// OprtTabPageScan.cpp : 実装ファイル
//

#include "stdafx.h"
#include "WeldEvaluation.h"
#include "OprtTabPageScan.h"
#include "afxdialogex.h"
#include "WeldEvaluationDoc.h"
#include "message.h"


// COprtTabPageScan ダイアログ

IMPLEMENT_DYNAMIC(COprtTabPageScan, CDialogEx)

COprtTabPageScan::COprtTabPageScan(CWnd* pParent /*=NULL*/)
	: CDialogEx(COprtTabPageScan::IDD, pParent)
	, m_ScanTarget(0)
{

}

COprtTabPageScan::~COprtTabPageScan()
{
}

/// <summary>
/// データエクスチェンジ処理
/// </summary>
/// <param name="pDX">CDataExchangeオブジェクト</param>
void COprtTabPageScan::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RDO_REGIN, m_ScanTarget);
}


BEGIN_MESSAGE_MAP(COprtTabPageScan, CDialogEx)
	ON_WM_ACTIVATE()
	ON_BN_CLICKED(IDC_BTN_SCAN, &COprtTabPageScan::OnBnClickedBtnScan)
END_MESSAGE_MAP()


// COprtTabPageScan メッセージ ハンドラー


/// <summary>
/// アイテムのアクティブ/ディアクティブ化
/// </summary>
/// <param name="ItemID">アイテムID</param>
/// <param name="bActive">アクティブフラグ</param>
void COprtTabPageScan::ItemEnable(int ItemID,bool bActive)
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
void COprtTabPageScan::ItemActive(bool bActive)
{
	ItemEnable(IDC_STC_SCANFRAME,bActive);
	ItemEnable(IDC_RDO_REGIN,bActive);
	ItemEnable(IDC_RDO_METAL,bActive);
	ItemEnable(IDC_RSO_RESULT,bActive);
	ItemEnable(IDC_BTN_SCAN,bActive);
	ItemEnable(IDC_BTN_CANCEL,bActive);
}

void COprtTabPageScan::LoadParamater(void)
{
}

int COprtTabPageScan::GetDeactiveScan()
{
	int targetID = -1;
	CFormView *pWnd = (CFormView *)GetParent()->GetParent();
	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)pWnd->GetDocument();
	if (!pDoc->IsOpen()) {
		return targetID;
	}
	if (pDoc->ExistScanFile(CWeldEvaluationDoc::eResinSurface)) {
		if (pDoc->ExistScanFile(CWeldEvaluationDoc::eMetalSurface)) {
			if (pDoc->ExistScanFile(CWeldEvaluationDoc::eJoiningResult)) {
				if (pDoc->IsNew()) {
					targetID = 0;
				} else {
					targetID = -1;
				}
			} else {
				targetID = 2;
			}
		} else {
			targetID = 1;
		}
	} else {
		targetID = 0;
	}
	return targetID;
}

void COprtTabPageScan::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);

	int id = GetDeactiveScan();
	if (id < 0) {
		id = 2;
	}
	m_ScanTarget = id;
	UpdateData(false);
}


void COprtTabPageScan::OnBnClickedBtnScan()
{
	CFormView *pWnd = (CFormView *)GetParent()->GetParent();
	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)pWnd->GetDocument();
	if (!pDoc->IsOpen()) {
		return;
	}

	int ScanID;
	bool bExist = false;
	UpdateData(true);
	switch(m_ScanTarget) {
	case	0	:
		ScanID = CWeldEvaluationDoc::eResinSurface;
		break;
	case	1	:
		ScanID = CWeldEvaluationDoc::eMetalSurface;
		break;
	case	2	:
		ScanID = CWeldEvaluationDoc::eJoiningResult;
		break;
	default:
		return;
	}

	if (pDoc->ExistScanFile(ScanID)) {
		bExist = true;
	}

	ItemActive(false);
	CString	msg;
	if (bExist) {
		msg.LoadString(IDM_EXISTSCANIMAGE);
		if (AfxMessageBox(msg,MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON1) != IDYES) {
			ItemActive(true);
			return;
		}
	}
	
	LPARAM result;
	if (pWnd->SendMessage(WM_SCAN_REQUEST,(WPARAM)ScanID,(LPARAM)&result) == 0) {
		switch(result) {
		case	0	:		// スキャン成功
			if (pDoc->ExistScanFile(ScanID)) {
				int id = GetDeactiveScan();
				if (id >= 0) {
					m_ScanTarget = id;
					UpdateData(false);
				}
			}
			msg.LoadString(IDM_SCAN_SUCCESS);
			AfxMessageBox(msg,MB_OK|MB_ICONINFORMATION);
			break;
		case	1	:		// スキャンキャンセル
			msg.LoadString(IDM_SCAN_CANCELD);
			AfxMessageBox(msg,MB_OK|MB_ICONWARNING);
			break;
		default	:
			msg.LoadString(IDM_ERR_SCAN);
			AfxMessageBox(msg,MB_OK|MB_ICONSTOP);
			break;
		}
	} else {
		msg.LoadString(IDM_ERR_SCAN);
		AfxMessageBox(msg,MB_OK|MB_ICONSTOP);
	}
	ItemActive(true);
}

void COprtTabPageScan::FitRect(CRect rect)
{
}
