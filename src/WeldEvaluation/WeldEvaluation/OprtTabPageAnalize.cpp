// OprtTabPageAnalize.cpp : 実装ファイル
//

#include "stdafx.h"
#include "WeldEvaluation.h"
#include "OprtTabPageAnalize.h"
#include "afxdialogex.h"
#include "WeldEvaluationDoc.h"
#include "message.h"


// COprtTabPageAnalize ダイアログ

IMPLEMENT_DYNAMIC(COprtTabPageAnalize, CDialogEx)

/// <summary>
/// コンストラクタ
/// </summary>
COprtTabPageAnalize::COprtTabPageAnalize(CWnd* pParent /*=NULL*/)
	: CDialogEx(COprtTabPageAnalize::IDD, pParent)
	, m_ResinAnalizeType(0)
	, m_MetalAnalizeType(0)
	, m_JointAnalizeType(0)
	, m_ResinDisplayMode(0)
	, m_MetalDisplayMode(0)
	, m_ResultDisplayMode(0)
{
}

/// <summary>
/// デストラクタ
/// </summary>
COprtTabPageAnalize::~COprtTabPageAnalize()
{
}

/// <summary>
/// データエクスチェンジ処理
/// </summary>
/// <param name="pDX">CDataExchangeオブジェクト</param>
void COprtTabPageAnalize::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RDO_RESIN_METHOD1, m_ResinAnalizeType);
	DDX_Radio(pDX, IDC_RDO_METAL_METHOD1, m_MetalAnalizeType);
	DDX_Radio(pDX, IDC_RDO_RESINSCAN, m_ResinDisplayMode);
	DDX_Radio(pDX, IDC_RDO_METALSCAN, m_MetalDisplayMode);
	DDX_Radio(pDX, IDC_RDO_JOINTRESULTSCAN, m_ResultDisplayMode);
	DDX_Radio(pDX, IDC_RDO_JOINT_METHOD1, m_JointAnalizeType);
}


BEGIN_MESSAGE_MAP(COprtTabPageAnalize, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_RESINANALIZE, &COprtTabPageAnalize::OnBnClickedBtnResinanalize)
	ON_BN_CLICKED(IDC_BTN_METALANALIZE, &COprtTabPageAnalize::OnBnClickedBtnMetalanalize)
	ON_BN_CLICKED(IDC_BTN_SECTIONSPECTRUMDISPLAY, &COprtTabPageAnalize::OnBnClickedBtnSectionspectrumdisplay)
	ON_BN_CLICKED(IDC_RDO_RESINSCAN, &COprtTabPageAnalize::OnBnClickedRdoResinscan)
	ON_BN_CLICKED(IDC_RDO_RESINCLASRESULT, &COprtTabPageAnalize::OnBnClickedRdoResinclasresult)
	ON_BN_CLICKED(IDC_RDO_METALSCAN, &COprtTabPageAnalize::OnBnClickedRdoMetalscan)
	ON_BN_CLICKED(IDC_RDO_METALCLASRESULT, &COprtTabPageAnalize::OnBnClickedRdoMetalclasresult)
	ON_BN_CLICKED(IDC_RDO_JOINTRESULTSCAN, &COprtTabPageAnalize::OnBnClickedRdoJointresultscan)
	ON_BN_CLICKED(IDC_RDO_JOINTRESULTCLASRESULT, &COprtTabPageAnalize::OnBnClickedRdoJointresultclasresult)
	ON_BN_CLICKED(IDC_RDO_RESIN_METHOD1, &COprtTabPageAnalize::OnBnClickedRdoResinMethod1)
	ON_BN_CLICKED(IDC_RDO_RESIN_METHOD2, &COprtTabPageAnalize::OnBnClickedRdoResinMethod2)
	ON_BN_CLICKED(IDC_RDO_METAL_METHOD1, &COprtTabPageAnalize::OnBnClickedRdoMetalMethod1)
	ON_BN_CLICKED(IDC_RDO_METAL_METHOD2, &COprtTabPageAnalize::OnBnClickedRdoMetalMethod2)
	ON_BN_CLICKED(IDC_RDO_JOINT_METHOD1, &COprtTabPageAnalize::OnBnClickedRdoJointMethod1)
	ON_BN_CLICKED(IDC_RDO_JOINT_METHOD2, &COprtTabPageAnalize::OnBnClickedRdoJointMethod2)
	ON_BN_CLICKED(IDC_BTN_JOINTANALIZE, &COprtTabPageAnalize::OnBnClickedBtnJointanalize)
END_MESSAGE_MAP()


// COprtTabPageAnalize メッセージ ハンドラー

/// <summary>
/// アイテムのアクティブ/ディアクティブ化
/// </summary>
/// <param name="ItemID">アイテムID</param>
/// <param name="bActive">アクティブフラグ</param>
void COprtTabPageAnalize::ItemEnable(int ItemID,bool bActive)
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
void COprtTabPageAnalize::ItemActive(bool bActive)
{
	ItemEnable(IDC_STC_RESIN_FRAME,bActive);
	ItemEnable(IDC_RDO_RESIN_METHOD1,bActive);
	ItemEnable(IDC_RDO_RESIN_METHOD2,bActive);
	ItemEnable(IDC_BTN_RESINANALIZE,bActive);

	ItemEnable(IDC_STC_METAL_FRAME,bActive);
	ItemEnable(IDC_RDO_METAL_METHOD1,bActive);
	ItemEnable(IDC_RDO_METAL_METHOD2,bActive);
	ItemEnable(IDC_BTN_METALANALIZE,bActive);

	ItemEnable(IDC_STC_VIEWCHANGE_FRAME,bActive);
	ItemEnable(IDC_STC_REGIN_LABEL,bActive);
	ItemEnable(IDC_RDO_RESINSCAN,bActive);
	ItemEnable(IDC_RDO_RESINCLASRESULT,bActive);

	ItemEnable(IDC_STC_METAL_LABEL,bActive);
	ItemEnable(IDC_RDO_METALSCAN,bActive);
	ItemEnable(IDC_RDO_METALCLASRESULT,bActive);

	ItemEnable(IDC_STC_JOINTRESULT_LABEL,bActive);
	ItemEnable(IDC_RDO_JOINTRESULTSCAN,bActive);
	ItemEnable(IDC_RDO_JOINTRESULTCLASRESULT,bActive);

	ItemEnable(IDC_BTN_SECTIONSPECTRUMDISPLAY,bActive);
}

/// <summary>
/// パラメータの読み込み
/// </summary>
void COprtTabPageAnalize::LoadParamater(void)
{
	CFormView *pWnd = (CFormView *)GetParent()->GetParent();
	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)pWnd->GetDocument();

	m_ResinAnalizeType = (pDoc->GetAnalysisMethod(CWeldEvaluationDoc::eResinSurface) == CWeldEvaluationDoc::AnalizeTypeKMeans?0:1);
	m_MetalAnalizeType = (pDoc->GetAnalysisMethod(CWeldEvaluationDoc::eMetalSurface) == CWeldEvaluationDoc::AnalizeTypeKMeans?0:1);

	m_ResinDisplayMode = (pDoc->GetDisplayMode(CWeldEvaluationDoc::eResinSurface) == CWeldEvaluationDoc::DisplayModeScan?0:1);
	m_MetalDisplayMode = (pDoc->GetDisplayMode(CWeldEvaluationDoc::eMetalSurface) == CWeldEvaluationDoc::DisplayModeScan?0:1);
	m_ResultDisplayMode = (pDoc->GetDisplayMode(CWeldEvaluationDoc::eJoiningResult) == CWeldEvaluationDoc::DisplayModeScan?0:1);
	UpdateData(false);
}

/// <summary>
/// 表示モードリセット
/// </summary>
void COprtTabPageAnalize::ViewModeReset(void)
{
	m_ResinDisplayMode = 0;
	m_MetalDisplayMode = 0;
	m_ResultDisplayMode = 0;
	UpdateData(false);
}

/// <summary>
/// 樹脂面解析方法k-means選択時処理
/// </summary>
void COprtTabPageAnalize::OnBnClickedRdoResinMethod1()
{
	int org = m_ResinAnalizeType;
	UpdateData(true);
	if (org == m_ResinAnalizeType) {
		return;
	}
	CFormView *pWnd = (CFormView *)GetParent()->GetParent();
	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)pWnd->GetDocument();
	pDoc->SetAnalysisMethod(CWeldEvaluationDoc::eResinSurface,CWeldEvaluationDoc::AnalizeTypeKMeans);
	if (m_ResinDisplayMode == 1) {
		CFormView *pWnd = (CFormView *)GetParent()->GetParent();
		if (pWnd) {
			pWnd->SendMessage(WM_VIEW_CHANGE_REQUEST, CWeldEvaluationDoc::eResinSurface, CWeldEvaluationDoc::DisplayModeResult);
		}
	}
}

/// <summary>
/// 樹脂面解析方法階層クラスタリング選択時処理
/// </summary>
void COprtTabPageAnalize::OnBnClickedRdoResinMethod2()
{
	int org = m_ResinAnalizeType;
	UpdateData(true);
	if (org == m_ResinAnalizeType) {
		return;
	}
	CFormView *pWnd = (CFormView *)GetParent()->GetParent();
	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)pWnd->GetDocument();
	pDoc->SetAnalysisMethod(CWeldEvaluationDoc::eResinSurface,CWeldEvaluationDoc::AnalizeTypeHiClustering);
	if (m_ResinDisplayMode == 1) {
		CFormView *pWnd = (CFormView *)GetParent()->GetParent();
		if (pWnd) {
			pWnd->SendMessage(WM_VIEW_CHANGE_REQUEST, CWeldEvaluationDoc::eResinSurface, CWeldEvaluationDoc::DisplayModeResult);
		}
	}
}

/// <summary>
/// 金属面解析方法k-means選択時処理
/// </summary>
void COprtTabPageAnalize::OnBnClickedRdoMetalMethod1()
{
	int org = m_MetalAnalizeType;
	UpdateData(true);
	if (org == m_MetalAnalizeType) {
		return;
	}
	CFormView *pWnd = (CFormView *)GetParent()->GetParent();
	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)pWnd->GetDocument();
	pDoc->SetAnalysisMethod(CWeldEvaluationDoc::eMetalSurface,CWeldEvaluationDoc::AnalizeTypeKMeans);
	if (m_MetalDisplayMode == 1) {
		CFormView *pWnd = (CFormView *)GetParent()->GetParent();
		if (pWnd) {
			pWnd->SendMessage(WM_VIEW_CHANGE_REQUEST, CWeldEvaluationDoc::eMetalSurface, CWeldEvaluationDoc::DisplayModeResult);
		}
	}
}

/// <summary>
/// 金属面解析方法階層選択時処理
/// </summary>
void COprtTabPageAnalize::OnBnClickedRdoMetalMethod2()
{
	int org = m_MetalAnalizeType;
	UpdateData(true);
	if (org == m_MetalAnalizeType) {
		return;
	}
	CFormView *pWnd = (CFormView *)GetParent()->GetParent();
	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)pWnd->GetDocument();
	pDoc->SetAnalysisMethod(CWeldEvaluationDoc::eMetalSurface,CWeldEvaluationDoc::AnalizeTypeHiClustering);
	if (m_MetalDisplayMode == 1) {
		CFormView *pWnd = (CFormView *)GetParent()->GetParent();
		if (pWnd) {
			pWnd->SendMessage(WM_VIEW_CHANGE_REQUEST, CWeldEvaluationDoc::eMetalSurface, CWeldEvaluationDoc::DisplayModeResult);
		}
	}
}

/// <summary>
/// 接続結果解析方法k-means選択時処理
/// </summary>
void COprtTabPageAnalize::OnBnClickedRdoJointMethod1()
{
	int org = m_JointAnalizeType;
	UpdateData(true);
	if (org == m_JointAnalizeType) {
		return;
	}
	CFormView *pWnd = (CFormView *)GetParent()->GetParent();
	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)pWnd->GetDocument();
	pDoc->SetAnalysisMethod(CWeldEvaluationDoc::eJoiningResult, CWeldEvaluationDoc::AnalizeTypeKMeans);
	if (m_JointAnalizeType == 1) {
		CFormView *pWnd = (CFormView *)GetParent()->GetParent();
		if (pWnd) {
			pWnd->SendMessage(WM_VIEW_CHANGE_REQUEST, CWeldEvaluationDoc::eJoiningResult, CWeldEvaluationDoc::DisplayModeResult);
		}
	}
}

/// <summary>
/// 接続結果解析方法階層選択時処理
/// </summary>
void COprtTabPageAnalize::OnBnClickedRdoJointMethod2()
{
	int org = m_JointAnalizeType;
	UpdateData(true);
	if (org == m_JointAnalizeType) {
		return;
	}
	CFormView *pWnd = (CFormView *)GetParent()->GetParent();
	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)pWnd->GetDocument();
	pDoc->SetAnalysisMethod(CWeldEvaluationDoc::eJoiningResult, CWeldEvaluationDoc::AnalizeTypeHiClustering);
	if (m_JointAnalizeType == 1) {
		CFormView *pWnd = (CFormView *)GetParent()->GetParent();
		if (pWnd) {
			pWnd->SendMessage(WM_VIEW_CHANGE_REQUEST, CWeldEvaluationDoc::eJoiningResult, CWeldEvaluationDoc::DisplayModeResult);
		}
	}
}


/// <summary>
/// 樹脂面分析実施選択時処理
/// </summary>
void COprtTabPageAnalize::OnBnClickedBtnResinanalize()
{
	UpdateData(true);
	WPARAM targetItemID = CWeldEvaluationDoc::eResinSurface;
	LPARAM type = m_ResinAnalizeType==0?CWeldEvaluationDoc::AnalizeTypeKMeans:CWeldEvaluationDoc::AnalizeTypeHiClustering;
	CFormView *pWnd = (CFormView *)GetParent()->GetParent();
	if (pWnd->SendMessage(WM_ANALYSE_REQUEST,targetItemID,type) != 0) {
	}
}

/// <summary>
/// 金属面分析実施選択時処理
/// </summary>
void COprtTabPageAnalize::OnBnClickedBtnMetalanalize()
{
	UpdateData(true);
	WPARAM targetItemID = CWeldEvaluationDoc::eMetalSurface;
	LPARAM type = m_MetalAnalizeType==0?CWeldEvaluationDoc::AnalizeTypeKMeans:CWeldEvaluationDoc::AnalizeTypeHiClustering;
	CFormView *pWnd = (CFormView *)GetParent()->GetParent();
	if (pWnd->SendMessage(WM_ANALYSE_REQUEST,targetItemID,type) != 0) {
	}
}

/// <summary>
/// 接合結果分析実施選択時処理
/// </summary>
void COprtTabPageAnalize::OnBnClickedBtnJointanalize()
{
	UpdateData(true);
	WPARAM targetItemID = CWeldEvaluationDoc::eJoiningResult;
	LPARAM type = m_JointAnalizeType == 0 ? CWeldEvaluationDoc::AnalizeTypeKMeans : CWeldEvaluationDoc::AnalizeTypeHiClustering;
	CFormView *pWnd = (CFormView *)GetParent()->GetParent();
	if (pWnd->SendMessage(WM_ANALYSE_REQUEST, targetItemID, type) != 0) {
	}
}

/// <summary>
/// 樹脂面表示タイプスキャン選択時処理
/// </summary>
void COprtTabPageAnalize::OnBnClickedRdoResinscan()
{
	int org = m_ResinDisplayMode;
	UpdateData(true);
	if (org == m_ResinDisplayMode) {
		return;
	}
	CFormView *pWnd = (CFormView *)GetParent()->GetParent();
	if (pWnd->SendMessage(WM_VIEW_CHANGE_REQUEST,CWeldEvaluationDoc::eResinSurface,CWeldEvaluationDoc::DisplayModeScan) != 0) {
		m_ResinDisplayMode = org;
	} else {
		CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)pWnd->GetDocument();
		pDoc->SetDisplayMode(CWeldEvaluationDoc::eResinSurface,CWeldEvaluationDoc::DisplayModeScan);
	}
	UpdateData(false);
}

/// <summary>
/// 樹脂面表示タイプ分類結果選択時処理
/// </summary>
void COprtTabPageAnalize::OnBnClickedRdoResinclasresult()
{
	int org = m_ResinDisplayMode;
	UpdateData(true);
	if (org == m_ResinDisplayMode) {
		return;
	}
	CFormView *pWnd = (CFormView *)GetParent()->GetParent();
	if (pWnd->SendMessage(WM_VIEW_CHANGE_REQUEST,CWeldEvaluationDoc::eResinSurface,CWeldEvaluationDoc::DisplayModeResult) != 0) {
		m_ResinDisplayMode = org;
	} else {
		CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)pWnd->GetDocument();
		pDoc->SetDisplayMode(CWeldEvaluationDoc::eResinSurface,CWeldEvaluationDoc::DisplayModeResult);
	}
	UpdateData(false);
}

/// <summary>
/// 金属面表示タイプスキャン選択時処理
/// </summary>
void COprtTabPageAnalize::OnBnClickedRdoMetalscan()
{
	int org = m_MetalDisplayMode;
	UpdateData(true);
	if (org == m_MetalDisplayMode) {
		return;
	}
	CFormView *pWnd = (CFormView *)GetParent()->GetParent();
	if (pWnd->SendMessage(WM_VIEW_CHANGE_REQUEST,CWeldEvaluationDoc::eMetalSurface,CWeldEvaluationDoc::DisplayModeScan) != 0) {
		m_MetalDisplayMode = org;
	} else {
		CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)pWnd->GetDocument();
		pDoc->SetDisplayMode(CWeldEvaluationDoc::eMetalSurface,CWeldEvaluationDoc::DisplayModeScan);
	}
	UpdateData(false);
}

/// <summary>
/// 金属面表示タイプ分類結果選択時処理
/// </summary>
void COprtTabPageAnalize::OnBnClickedRdoMetalclasresult()
{
	int org = m_MetalDisplayMode;
	UpdateData(true);
	if (org == m_MetalDisplayMode) {
		return;
	}
	CFormView *pWnd = (CFormView *)GetParent()->GetParent();
	if (pWnd->SendMessage(WM_VIEW_CHANGE_REQUEST,CWeldEvaluationDoc::eMetalSurface,CWeldEvaluationDoc::DisplayModeResult) != 0) {
		m_MetalDisplayMode = org;
	} else {
		CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)pWnd->GetDocument();
		pDoc->SetDisplayMode(CWeldEvaluationDoc::eMetalSurface,CWeldEvaluationDoc::DisplayModeResult);
	}
	UpdateData(false);
}

/// <summary>
/// 接合結果表示タイプスキャン選択時処理
/// </summary>
void COprtTabPageAnalize::OnBnClickedRdoJointresultscan()
{
	int org = m_ResultDisplayMode;
	UpdateData(true);
	if (org == m_ResultDisplayMode) {
		return;
	}
	CFormView *pWnd = (CFormView *)GetParent()->GetParent();
	if (pWnd->SendMessage(WM_VIEW_CHANGE_REQUEST,CWeldEvaluationDoc::eJoiningResult,CWeldEvaluationDoc::DisplayModeScan) != 0) {
		m_ResultDisplayMode = org;
	} else {
		CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)pWnd->GetDocument();
		pDoc->SetDisplayMode(CWeldEvaluationDoc::eJoiningResult,CWeldEvaluationDoc::DisplayModeScan);
	}
	UpdateData(false);
}

/// <summary>
/// 接合結果表示タイプ分類結果選択時処理
/// </summary>
void COprtTabPageAnalize::OnBnClickedRdoJointresultclasresult()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	int org = m_ResultDisplayMode;
	UpdateData(true);
	if (org == m_ResultDisplayMode) {
		return;
	}
	CFormView *pWnd = (CFormView *)GetParent()->GetParent();
	if (pWnd->SendMessage(WM_VIEW_CHANGE_REQUEST,CWeldEvaluationDoc::eJoiningResult,CWeldEvaluationDoc::DisplayModeResult) != 0) {
		m_ResultDisplayMode = org;
	} else {
		CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)pWnd->GetDocument();
		pDoc->SetDisplayMode(CWeldEvaluationDoc::eJoiningResult,CWeldEvaluationDoc::DisplayModeResult);
	}
	UpdateData(false);
}

/// <summary>
/// 区間スペクトル表示ボタン選択時処理
/// </summary>
void COprtTabPageAnalize::OnBnClickedBtnSectionspectrumdisplay()
{
	CFormView *pViewWnd = (CFormView *)GetParent()->GetParent();
	pViewWnd->PostMessageW(WM_AREA_SPECTRUM_GRAPH_REQUEST,NULL, NULL);
}

/// <summary>
/// 解析手法の取得
/// </summary>
/// <param name="ItemID">表示ターゲットID</param>
/// <returns>解析手法を返す</returns>
int COprtTabPageAnalize::GetAnalizeType(int ItemID)
{
	int AnalizeType = 0;
	UpdateData(true);
	switch (ItemID) {
	case		CWeldEvaluationDoc::eResinSurface	:
		AnalizeType = m_ResinAnalizeType;
		break;
	case		CWeldEvaluationDoc::eMetalSurface	:
		AnalizeType = m_MetalAnalizeType;
		break;
	case		CWeldEvaluationDoc::eJoiningResult	:
		AnalizeType = m_JointAnalizeType;
		break;
	}
	int type = AnalizeType == 0 ? CWeldEvaluationDoc::AnalizeTypeKMeans : CWeldEvaluationDoc::AnalizeTypeHiClustering;
	return type;
}
