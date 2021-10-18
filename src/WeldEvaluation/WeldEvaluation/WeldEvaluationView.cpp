
// WeldEvaluationView.cpp : CWeldEvaluationView クラスの実装
//

#include "stdafx.h"
#include "FileUtil.h"
// SHARED_HANDLERS は、プレビュー、サムネイル、および検索フィルター ハンドラーを実装している ATL プロジェクトで定義でき、
// そのプロジェクトとのドキュメント コードの共有を可能にします。
#ifndef SHARED_HANDLERS
#include "WeldEvaluation.h"
#endif

#include "WeldEvaluationDoc.h"
#include "WeldEvaluationView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma warning(disable:4800)
#pragma warning(disable:4100)

using namespace std;

// CWeldEvaluationView

IMPLEMENT_DYNCREATE(CWeldEvaluationView, CFormView)

BEGIN_MESSAGE_MAP(CWeldEvaluationView, CFormView)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_PROPATY, &CWeldEvaluationView::OnTcnSelchangeTabPropaty)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_OPERATION, &CWeldEvaluationView::OnTcnSelchangeTabOperation)
	ON_BN_CLICKED(IDC_BTN_PROP_UPDATE, &CWeldEvaluationView::OnBnClickedBtnPropUpdate)
	ON_BN_CLICKED(IDC_BTN_PROP_CANCEL, &CWeldEvaluationView::OnBnClickedBtnPropCancel)
	ON_NOTIFY(NM_DBLCLK, IDC_LST_REGIST_TEST, &CWeldEvaluationView::OnNMDblclkLstRegistTest)
	ON_BN_CLICKED(IDC_BTN_NEWPRJ, &CWeldEvaluationView::OnBnClickedBtnNewprj)
	ON_MESSAGE(WM_UPDATEREQUEST_PROPPAGE, OnUpdateRequestPrpoTab)
	ON_MESSAGE(WM_SCAN_REQUEST, OnScanRequest)
	ON_MESSAGE(WM_VIEW_CHANGE_REQUEST, OnViewChangeRequest)
	ON_MESSAGE(WM_CHANGE_REGIST, OnChangeResistFolder)
	ON_MESSAGE(WM_ANALYSE_REQUEST, OnAnalyzeRequest)
	ON_MESSAGE(WM_RESIST_REGIST, OnProjectResistRequest)
	ON_MESSAGE(WM_IMAGE_OUTPUT_REGIST, OnImageOutputRequest)
	ON_MESSAGE(WM_READ_RESULT_STATUS, OnReadResultFileStatus)
	ON_MESSAGE(WM_SPECTRUM_GRAPH_REQUEST, OnSpectrumGraphRequest)
	ON_MESSAGE(WM_AREA_SPECTRUM_GRAPH_REQUEST, OnAreaSpectrumGraphRequest)
	ON_MESSAGE(WM_SPECTRUME_CLOSE_REQUEST, OnSpectrumeCloseRequest)
	ON_MESSAGE(WM_AREA_SPECTRUM_GRAPH_SET, OnAreaSpectrumeGraphSet)

	ON_WM_NCDESTROY()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CWeldEvaluationView コンストラクション/デストラクション

/// <summary>
/// コンストラクタ
/// </summary>
CWeldEvaluationView::CWeldEvaluationView()
	: CFormView(CWeldEvaluationView::IDD)
{
	m_SelPropPageId = 0;
	m_SelOprtPageId = 0;
	m_bUpdateOperation	= false;
	m_bReadMode			= false;
	m_bUpdatePropaty	= false;
	m_pGraphWnd			= nullptr;
	m_pReginWnd			= nullptr;
	m_pMetalWnd			= nullptr;
	m_pResultWnd		= nullptr;

	m_pProgress			= nullptr;
	m_pSpectralDlg		= nullptr;
	m_SpectralDlgRect = CRect(0, 0, 0, 0);
}

/// <summary>
/// デストラクタ
/// </summary>
CWeldEvaluationView::~CWeldEvaluationView()
{

}

/// <summary>
/// データエクスチェンジ処理
/// </summary>
/// <param name="pDX">CDataExchangeオブジェクト</param>
void CWeldEvaluationView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LST_REGIST_TEST, m_lstRegTest);
	DDX_Control(pDX, IDC_TAB_PROPATY, m_tabPropaty);
	DDX_Control(pDX, IDC_TAB_OPERATION, m_tabOperation);
	DDX_Control(pDX, IDC_STC_PROPTABPAG_CLIENT, m_stcPropTabPageClient);
	DDX_Control(pDX, IDC_STC_OPRTABPAG_CLIENT, m_stcOperationTabPageClient);
	DDX_Control(pDX, IDC_BTN_PROP_CANCEL, m_btnPropTabCancel);
	DDX_Control(pDX, IDC_BTN_PROP_UPDATE, m_btnPropTabOK);
}

BOOL CWeldEvaluationView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	return CFormView::PreCreateWindow(cs);
}

void CWeldEvaluationView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();

	CRect formRect;
	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)GetDocument();
	int top,left,width,hight;
	if (!pDoc->GetWindRect(formRect)) {
		top = 0;
		left = 0;
		width = (int)(GetSystemMetrics(SM_CXSCREEN)*0.8);
		hight = (int)(GetSystemMetrics(SM_CYSCREEN)*0.8);
	} else {
		top = formRect.top;
		left = formRect.left;
		width = formRect.Width();
		hight = formRect.Height();
		if ((width == 0) || (hight == 0)) {
			width = (int)(GetSystemMetrics(SM_CXSCREEN)*0.8);
			hight = (int)(GetSystemMetrics(SM_CYSCREEN)*0.8);
		}
	}

	GetClientRect( formRect );
	formRect.top    = 0;
	formRect.left   = 0;
	formRect.bottom = hight;
	formRect.right  = width;
	MoveWindow( formRect );
	GetClientRect( formRect );
	GetParent()->MoveWindow( formRect );

	//////////////////////////////////////
	//	リストコントロールの設定
	m_lstRegTest.DeleteAllItems();
	CString label;
	TCHAR szMsg [256];
	LoadString(GetModuleHandle(NULL),IDS_REGTEST_LABEL,szMsg,sizeof(szMsg)/sizeof(szMsg[0]) );
	m_lstRegTest.InsertColumn(0,szMsg);

	// カラム幅を整える
	CRect rect;
	m_lstRegTest.GetClientRect(&rect);
	m_lstRegTest.SetColumnWidth(0, rect.Width());
	m_lstRegTest.ShowWindow(SW_SHOW);

	//行選択モードにしておく
	DWORD	dwStyle = ListView_GetExtendedListViewStyle(m_lstRegTest.m_hWnd);
	ListView_SetExtendedListViewStyle(m_lstRegTest.m_hWnd, dwStyle | LVS_EX_FULLROWSELECT);	
	UpdateRegistFolderList();

	CString strTitle;
	//////////////////////////////////////
	//	操作パネルタブの設定
	strTitle.LoadString(IDS_OPRTAB_INIT);
	m_tabOperation.InsertItem(0,strTitle);
	strTitle.LoadString(IDS_OPRTAB_SETTING);
	m_tabOperation.InsertItem(1,strTitle);
	strTitle.LoadString(IDS_OPRTAB_SCAN);
	m_tabOperation.InsertItem(2,strTitle);
	strTitle.LoadString(IDS_OPRTAB_ANARIZE);
	m_tabOperation.InsertItem(3,strTitle);

	bool bResult;
	bResult = m_OprtInitialize.Create(COprtTabPageInitialize::IDD,&m_stcOperationTabPageClient);
	m_OprtTab.Add(&m_OprtInitialize);
	bResult = m_OprtSetting.Create(COprtTabPageSetting::IDD,&m_stcOperationTabPageClient);
	m_OprtTab.Add(&m_OprtSetting);
	m_OprtSetting.ItemActive(false);
	bResult = m_OprtScan.Create(COprtTabPageScan::IDD,&m_stcOperationTabPageClient);
	m_OprtTab.Add(&m_OprtScan);
	m_OprtScan.ItemActive(false);
	bResult = m_OprtAnalize.Create(COprtTabPageAnalize::IDD,&m_stcOperationTabPageClient);
	m_OprtTab.Add(&m_OprtAnalize);
	m_OprtAnalize.ItemActive(false);
	OnTcnSelchangeTabOperation(NULL, NULL);

	//////////////////////////////////////
	//	プロパティタブの設定
	int propId = 0;
	strTitle.LoadString(IDS_PROPTAB_RESIN);
	m_tabPropaty.InsertItem(propId++,strTitle);
	strTitle.LoadString(IDS_PROPTAB_METAL);
	m_tabPropaty.InsertItem(propId++,strTitle);
//	strTitle.LoadString(IDS_PROPTAB_RESULT);
//	m_tabPropaty.InsertItem(propId++,strTitle);
	strTitle.LoadString(IDS_PROPTAB_OVERALL);
	m_tabPropaty.InsertItem(propId++,strTitle);

	bResult = m_PropResinPage.Create(CPropTabPageParameter::IDD,&m_stcPropTabPageClient);
	m_PropTab.Add(&m_PropResinPage);
	m_PropResinPage.setPageID(CWeldEvaluationDoc::eResinSurface);
	bResult = m_PropMetalPage.Create(CPropTabPageParameter::IDD,&m_stcPropTabPageClient);
	m_PropTab.Add(&m_PropMetalPage);
	m_PropMetalPage.setPageID(CWeldEvaluationDoc::eMetalSurface);
//	bResult = m_PropResultPage.Create(CPropTabPageParameter::IDD,&m_stcPropTabPageClient);
//	m_PropTab.Add(&m_PropResultPage);
//	m_PropResultPage.setPageID(CWeldEvaluationDoc::eJoiningResult);
	bResult = m_PropSettingPage.Create(CPropTabPageSetting::IDD,&m_stcPropTabPageClient);
	m_PropTab.Add(&m_PropSettingPage);
	OnTcnSelchangeTabPropaty(NULL, NULL);

	OnUpdateRequestPrpoTab(false,0);

	// グラフウインド構築
	if (m_pGraphWnd == nullptr) {
		try{
			m_pGraphWnd = new CGraphWind;
			if( m_pGraphWnd->Create(IDD_GRAPH_WIND, this) == 0 ){
				return;
			}
		}catch(CException *e){
			e->Delete();
			return;
		}
	}

	// ここはクラフウインドの表示確認用のダミーです
	CWnd *pWnd = GetDlgItem(IDC_STC_GRAPH_WND);
	if (pWnd) {
		CRect rcClient;
		pWnd->GetWindowRect(rcClient);
		ScreenToClient(rcClient);
		m_pGraphWnd->MoveWindow(rcClient);
		pWnd->ShowWindow(SW_HIDE);
		m_pGraphWnd->ShowWindow(SW_SHOW);
	}

	// 樹脂面画像用ウインドの構築
	if (m_pReginWnd == nullptr) {
		try{
			m_pReginWnd = new CImageWind;
			if( m_pReginWnd->Create(IDD_IMAGE_WIND, this) == 0 ){
				return;
			}
		}catch(CException *e){
			e->Delete();
			return;
		}
	}
	m_pReginWnd->setType(CImageWind::eResinSurface);
	pWnd = GetDlgItem(IDC_STC_RESINIMG_VIEW);
	if (pWnd) {
		CRect rcClient;
		pWnd->GetWindowRect(rcClient);
		ScreenToClient(rcClient);
		m_pReginWnd->MoveWindow(rcClient);
		pWnd->ShowWindow(SW_HIDE);
		m_pReginWnd->ShowWindow(SW_SHOW);
	}

	// 金属面画像用ウインドの構築
	if (m_pMetalWnd == nullptr) {
		try{
			m_pMetalWnd = new CImageWind;
			if( m_pMetalWnd->Create(IDD_IMAGE_WIND, this) == 0 ){
				return;
			}
		}catch(CException *e){
			e->Delete();
			return;
		}
	}
	m_pMetalWnd->setType(CImageWind::eMetalSurface);
	pWnd = GetDlgItem(IDC_STC_METALIMG_VIEW);
	if (pWnd) {
		CRect rcClient;
		pWnd->GetWindowRect(rcClient);
		ScreenToClient(rcClient);
		m_pMetalWnd->MoveWindow(rcClient);
		pWnd->ShowWindow(SW_HIDE);
		m_pMetalWnd->ShowWindow(SW_SHOW);
	}

	// 結果画像用ウインドの構築
	if (m_pResultWnd == nullptr) {
		try{
			m_pResultWnd = new CImageWind;
			if( m_pResultWnd->Create(IDD_IMAGE_WIND, this) == 0 ){
				return;
			}
		}catch(CException *e){
			e->Delete();
			return;
		}
	}
	m_pResultWnd->setType(CImageWind::eJoiningResult);
	pWnd = GetDlgItem(IDC_STC_RESULTIMG_VIEW);
	if (pWnd) {
		CRect rcClient;
		pWnd->GetWindowRect(rcClient);
		ScreenToClient(rcClient);
		m_pResultWnd->MoveWindow(rcClient);
		pWnd->ShowWindow(SW_HIDE);
		m_pResultWnd->ShowWindow(SW_SHOW);
	}

	FitItem();
	UnSellectProject();

	ResizeParentToFit(FALSE);

	if (!pDoc->GetSpectralDlgRect(m_SpectralDlgRect)) {
		m_SpectralDlgRect = CRect(0, 0, 0, 0);
	}

}

// CWeldEvaluationView 診断

#ifdef _DEBUG
void CWeldEvaluationView::AssertValid() const
{
	CFormView::AssertValid();
}

void CWeldEvaluationView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CWeldEvaluationDoc* CWeldEvaluationView::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWeldEvaluationDoc)));
	return (CWeldEvaluationDoc*)m_pDocument;
}
#endif //_DEBUG


// CWeldEvaluationView メッセージ ハンドラー


void CWeldEvaluationView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	CRect   lpRect;
	GetClientRect(lpRect);

	CWnd* pWnd = GetDlgItem( IDD_WELDEVALUATION_FORM );
	if( pWnd && pWnd->GetSafeHwnd() ) {
		pWnd->MoveWindow(lpRect);
	}
}

/// <summary>
/// ウインド破棄時処理
/// </summary>
void CWeldEvaluationView::OnDestroy()
{
	CFormView::OnDestroy();

	if (m_pGraphWnd) {
		delete m_pGraphWnd;
		m_pGraphWnd = nullptr;
	}
	if (m_pReginWnd) {
		delete m_pReginWnd;
		m_pReginWnd = nullptr;
	}
	if (m_pMetalWnd) {
		delete m_pMetalWnd;
		m_pMetalWnd = nullptr;
	}
	if (m_pResultWnd) {
		delete m_pResultWnd;
		m_pResultWnd = nullptr;
	}
	if (m_pSpectralDlg) {
		delete m_pSpectralDlg;
		m_pSpectralDlg = nullptr;
	}
}


///////////////////////////////////////////////////////////////////////////////////
// 初期化・画面配置

/// <summary>
/// アイテムのフィッティング処理
/// </summary>
void CWeldEvaluationView::FitItem()
{
	CRect rect;
	m_stcOperationTabPageClient.GetClientRect(rect);

	m_OprtInitialize.MoveWindow(rect);
	m_OprtSetting.MoveWindow(rect);
	m_OprtScan.MoveWindow(rect);
	m_OprtAnalize.MoveWindow(rect);

	m_stcPropTabPageClient.GetClientRect(rect);
	m_PropResinPage.MoveWindow(rect);
	m_PropMetalPage.MoveWindow(rect);
//	m_PropResultPage.MoveWindow(rect);

	CRect srect;
	m_PropSettingPage.GetWindowRect(srect);
	CRect mrect = rect;
	mrect.bottom = mrect.top + srect.Height();
	m_PropSettingPage.MoveWindow(mrect);
	m_PropSettingPage.FitRect(rect);

}

///////////////////////////////////////////////////////////////////////////////////
// リストビュー処理関連
//

/// <summary>
/// 登録済み検査リストの更新
/// </summary>
void CWeldEvaluationView::UpdateRegistFolderList()
{
	m_lstRegTest.DeleteAllItems();
	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)GetDocument();
	CStringArray list;
	if (pDoc->GetRegistTestList(list)) {
		int count = (int)list.GetCount();
		for (int i = 0; i < count; i++) {
			CString name = list[i];
			int j = m_lstRegTest.InsertItem (i, name);
			m_lstRegTest.SetItemData(j, (unsigned long)i);
		}
	}
}

/// <summary>
/// 登録済み検査選択時処理
/// </summary>
/// <param name="pNMHDR">LPNMITEMACTIVATEオブジェクトへのポインタ</param>
/// <param name="pResult">結果</param>
/// <returns>表示モードを返す</returns>

void CWeldEvaluationView::OnNMDblclkLstRegistTest(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	*pResult = 0;

	if (pNMItemActivate->iItem < 0) {
		return;
	}

	CString str = m_lstRegTest.GetItemText(pNMItemActivate->iItem,0);
	if (str.IsEmpty()) {
		return;
	}

	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)GetDocument();

	CWaitCursor wcursol;
	if (pDoc->OpenProject(str)) {

		// 設定データの読み込み
		LoadSetting();
		m_OprtInitialize.ItemActive(true);
		m_bReadMode = true;
		m_OprtSetting.UpddateResist(false,m_bReadMode);	// 設定データをリードモードにする
		m_OprtSetting.Update();
		m_OprtScan.ItemActive(true);;
		m_OprtAnalize.ItemActive(true);;

		// プロパティデータの読み込み
		LoadPropaty();
		EnablePropaty(true);

		// 画像表示
		int DisplayMode = 0;
		// 樹脂面画像
		DisplayMode = pDoc->GetDisplayMode(CWeldEvaluationDoc::eResinSurface);
		OnViewChangeRequest(CWeldEvaluationDoc::eResinSurface, DisplayMode);
		m_pReginWnd->UpdateWindow();
		// 金属面画像
		DisplayMode = pDoc->GetDisplayMode(CWeldEvaluationDoc::eMetalSurface);
		OnViewChangeRequest(CWeldEvaluationDoc::eMetalSurface, DisplayMode);
		m_pMetalWnd->UpdateWindow();
		// 結果画像
		DisplayMode = pDoc->GetDisplayMode(CWeldEvaluationDoc::eJoiningResult);
		OnViewChangeRequest(CWeldEvaluationDoc::eJoiningResult, DisplayMode);

		//#######################################################
		//#
		//# グラフの表示が必要
		//#
		//#######################################################
		int holizontal, vertical;
		pDoc->GetSpectralGraphPointPosition(holizontal,vertical);

		CString msg,AppName;
		AppName.LoadString(AFX_IDS_APP_TITLE);
		msg.Format(_T("%s - %s"), (LPCWSTR)str, (LPCWSTR)AppName);
		GetParentFrame()->SetWindowText(msg);

	} else {
		CString msg,fmt;
		fmt.LoadString(IDM_ERR_NOTOPEN_REGTEST);
		msg.Format(fmt, (LPCWSTR)str);
		AfxMessageBox(msg,MB_OK|MB_ICONSTOP);
	}
}


///////////////////////////////////////////////////////////////////////////////////
// 操作タブ操作関連
//

void CWeldEvaluationView::OnTcnSelchangeTabOperation(NMHDR *pNMHDR, LRESULT *pResult)
{
	int	SelIdx;
	if ((pNMHDR != NULL) && (pNMHDR->code != TCN_SELCHANGE)) {
		return;
	}
	SelIdx = m_tabOperation.GetCurSel();
	CDialogEx *pDlg;

	bool bResult;
	int iCnt;
	if (SelIdx < 0) {
		iCnt = (int)m_OprtTab.GetSize();
		for (int i = 0; i < iCnt; i++) {
			pDlg = m_OprtTab.GetAt(i);
			if (pDlg) {
				pDlg->ShowWindow(SW_HIDE);
			}
		}
	} else {
		iCnt = (int)m_OprtTab.GetSize();
		for (int i = 0; i < iCnt; i++) {
			pDlg = m_OprtTab.GetAt(i);
			if (pDlg) {
				if (SelIdx == i) {
					WPARAM wparam = WA_ACTIVE & 0xFFFF;
					LPARAM lparam = (LPARAM)pDlg->m_hWnd;
					pDlg->SendMessage(WM_ACTIVATE,wparam,lparam);
					bResult = (bool)pDlg->ShowWindow(SW_SHOW);
				} else {
					bResult = (bool)pDlg->ShowWindow(SW_HIDE);
				}
			}
		}
	}

	m_SelOprtPageId = SelIdx;
	if (pResult != NULL) {
		*pResult = 0;
	}
}

///////////////////////////////////////////////////////////////////////////////////
// プロパティタブ操作関連
//

void CWeldEvaluationView::OnTcnSelchangeTabPropaty(NMHDR *pNMHDR, LRESULT *pResult)
{
	int	SelIdx;
	if ((pNMHDR != NULL) && (pNMHDR->code != TCN_SELCHANGE)) {
		return;
	}
	SelIdx = m_tabPropaty.GetCurSel();
	CDialogEx *pDlg;

	bool bResult;
	int iCnt;
	if (SelIdx < 0) {
		iCnt = (int)m_PropTab.GetSize();
		for (int i = 0; i < iCnt; i++) {
			pDlg = m_PropTab.GetAt(i);
			if (pDlg) {
				pDlg->ShowWindow(SW_HIDE);
			}
		}
	} else {
		if ((m_SelPropPageId != SelIdx) && (m_bUpdatePropaty)) {
			CString	msg,sub;
			msg.LoadString(IDM_PROPUPDATE);
			sub.LoadStringW(IDM_CHECKOFUPDATE);
			msg += _T("\n") + sub;
			if (AfxMessageBox(msg,MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON1) == IDYES) {
				UpdatePropaty(m_SelPropPageId);
			} else {
				ReloadPropaty(m_SelPropPageId);
			}
			OnUpdateRequestPrpoTab(false,0);
		}

		iCnt = (int)m_PropTab.GetSize();
		for (int i = 0; i < iCnt; i++) {
			pDlg = m_PropTab.GetAt(i);
			if (pDlg) {
				if (SelIdx == i) {
					WPARAM wparam = WA_ACTIVE & 0xFFFF;
					LPARAM lparam = (LPARAM)pDlg->m_hWnd;
					pDlg->SendMessage(WM_ACTIVATE,wparam,lparam);
					bResult = (bool)pDlg->ShowWindow(SW_SHOW);
				} else {
					bResult = (bool)pDlg->ShowWindow(SW_HIDE);
				}
			}
		}
	}

	m_SelPropPageId = SelIdx;
	if (pResult != NULL) {
		*pResult = 0;
	}
}

void CWeldEvaluationView::OnBnClickedBtnPropUpdate()
{
	int SelIdx = -1;
	SelIdx = m_tabPropaty.GetCurSel();
	UpdatePropaty(SelIdx);

	OnUpdateRequestPrpoTab(false,0);
}


void CWeldEvaluationView::OnBnClickedBtnPropCancel()
{
	OnUpdateRequestPrpoTab(false,0);
	ReloadPropaty(m_SelPropPageId);
}

LRESULT CWeldEvaluationView::OnUpdateRequestPrpoTab(WPARAM wparam, LPARAM lparam)
{
	bool bUpdate = (bool)wparam;
	m_btnPropTabOK.EnableWindow(bUpdate);
	m_btnPropTabCancel.EnableWindow(bUpdate);
	m_bUpdatePropaty = bUpdate;
	return 0;
}

void CWeldEvaluationView::LoadPropaty()
{
	int iCnt = (int)m_PropTab.GetSize();
	for (int i = 0; i < iCnt; i++) {
		ReloadPropaty(i);
	}
}

void CWeldEvaluationView::EnablePropaty(bool bActive)
{
	m_PropResinPage.ItemActive(bActive);
	m_PropMetalPage.ItemActive(bActive);
//	m_PropResultPage.ItemActive(bActive);
	m_PropSettingPage.ItemActive(bActive);
}

void CWeldEvaluationView::UpdatePropaty(int propatyID)
{
	switch(propatyID) {
	case	0	:
		{
			CPropTabPageParameter *pDlg = (CPropTabPageParameter *)m_PropTab.GetAt(propatyID);
			pDlg->Update(propatyID);
		}
		break;
	case	1	:
		{
			CPropTabPageParameter *pDlg = (CPropTabPageParameter *)m_PropTab.GetAt(propatyID);
			pDlg->Update(propatyID);
		}
		break;
	case	2	:
		{
			CPropTabPageParameter *pDlg = (CPropTabPageParameter *)m_PropTab.GetAt(propatyID);
			pDlg->Update(propatyID);
		}
		break;
	case	3	:
		{
			CPropTabPageSetting *pDlg = (CPropTabPageSetting *)m_PropTab.GetAt(propatyID);
			pDlg->Update();
		}
		break;
	}
}

void CWeldEvaluationView::ReloadPropaty(int propatyID)
{
	switch(propatyID) {
	case	0	:
		{
			CPropTabPageParameter *pDlg = (CPropTabPageParameter *)m_PropTab.GetAt(propatyID);
			pDlg->LoadParamater(propatyID);
		}
		break;
	case	1	:
		{
			CPropTabPageParameter *pDlg = (CPropTabPageParameter *)m_PropTab.GetAt(propatyID);
			pDlg->LoadParamater(propatyID);
		}
		break;
//	case	2	:
//		{
//			CPropTabPageParameter *pDlg = (CPropTabPageParameter *)m_PropTab.GetAt(propatyID);
//			pDlg->LoadParamater(propatyID);
//		}
//		break;
//	case	3	:
	case	2	:
		{
			CPropTabPageSetting *pDlg = (CPropTabPageSetting *)m_PropTab.GetAt(propatyID);
			pDlg->LoadParamater();
		}
		break;
	}
}

void CWeldEvaluationView::LoadSetting()
{
	int iCnt = (int)m_OprtTab.GetSize();
	for (int i = 0; i < iCnt; i++) {
		ReloadSetting(i);
	}
}

void CWeldEvaluationView::EnableSetting(bool bActive)
{

}

void CWeldEvaluationView::ReloadSetting(int settingID)
{
	switch(settingID) {
	case	0	:
		{
			COprtTabPageInitialize *pDlg = (COprtTabPageInitialize *)m_OprtTab.GetAt(settingID);
			pDlg->LoadParamater();
		}
		break;
	case	1	:
		{
			COprtTabPageSetting *pDlg = (COprtTabPageSetting *)m_OprtTab.GetAt(settingID);
			pDlg->LoadParamater();
		}
		break;
	case	2	:
		{
			COprtTabPageScan *pDlg = (COprtTabPageScan *)m_OprtTab.GetAt(settingID);
			pDlg->LoadParamater();
		}
		break;
	case	3	:
		{
			COprtTabPageAnalize *pDlg = (COprtTabPageAnalize *)m_OprtTab.GetAt(settingID);
			pDlg->LoadParamater();
		}
		break;
	}
}

///////////////////////////////////////////////////////////////////////////////////
// スキャン処理関連
//

LRESULT CWeldEvaluationView::OnScanRequest(WPARAM wparam, LPARAM lparam)
{
	EnablePropaty(false);

	int ScanID = (int)wparam;
	int *Result = (int *)lparam;
	CStatusDlgThread* pThread = DYNAMIC_DOWNCAST(CStatusDlgThread, AfxBeginThread(RUNTIME_CLASS(CStatusDlgThread) , 0, 0, CREATE_SUSPENDED));
	pThread->m_bAutoDelete = false;			// 無効なアクセス防止のため自動削除は無効化
	pThread->ResumeThread();
	while (pThread->m_Dlg.m_hWnd == 0) {	// m_Dlgのウィンドウが生成されるまで待機
		Sleep(0);
	}
	pThread->UpdateStatus(_T("Start Scan"));
	if (!ScanImage(pThread, ScanID)) {
		*Result = -1;
	}

	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)GetDocument();
	if (!pThread->m_Valid) {  // キャンセルボタンが押された場合
		// キャンセルの処理
		*Result = 1;
	}
	else {
		// 正常終了処理
		pThread->m_Dlg.PostMessage(WM_COMMAND, IDOK); // Statusダイアログを閉じる
		if (!pDoc->SaveScanImage(ScanID)) {
			*Result = -1;
		} else {
			// スキャン画面更新
			// もし、スキャンした画像の表示モードが解析ならスキャンに変更
			*Result = 0;
		}
	}

	WaitForSingleObject(pThread->m_hThread, 30000);	// スレッドの終了を30秒だけ待ってやる（ほぼ0秒のはず
	delete pThread;
	EnablePropaty(true);
	m_bUpdateOperation = true;
	m_OprtSetting.UpddateResist(m_bUpdateOperation,m_bReadMode);
	m_OprtSetting.Update();
	return 0;
}

bool CWeldEvaluationView::ScanImage(CStatusDlgThread* pStatus, int ScanID)
{
	// ダミーの処理
	for (int i = 0 ; i < 100; i++) {
		if (!pStatus->m_Valid) {  // キャンセルボタンが押された場合は何もせずに終了
			return true;
		}
		Sleep(100);
		CString buff;
		buff.Format(_T("Scaning : %d %%"), i);
		pStatus->UpdateStatus(buff);
		//#######################################################
		//#
		//# スキャン中の画像表示が必要
		//#
		//#######################################################
	}
	//#######################################################
	//#
	//# スキャン結果の全画面表示が必要
	//#
	//#######################################################
	return true;
}

/// <summary>
/// プロジェクトの更新要求
/// </summary>
/// <param name="wparam">未使用</param>
/// <param name="lparam">未使用</param>
/// <returns>成功場合は0、失敗場合は-1を返す</returns>
LRESULT CWeldEvaluationView::OnProjectResistRequest(WPARAM wparam, LPARAM lparam)
{
	int iResult = 0;
	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)GetDocument();
	if (!pDoc->SaveProject()) {
		CString msg;
		msg.LoadString(IDM_ERR_PROJECT_SAVE);
		AfxMessageBox(msg,MB_OK|MB_ICONSTOP);
		iResult = -1;
	} else {
		m_bUpdateOperation = false;
		m_OprtSetting.UpddateResist(m_bUpdateOperation,m_bReadMode);
		m_OprtSetting.Update();
	}
	return iResult;
}

/// <summary>
/// 画像出力要求
/// </summary>
// <param name="wparam">未使用</param>
/// <param name="lparam">未使用</param>
//// <returns>成功場合は0、失敗場合は-1を返す</returns>
LRESULT CWeldEvaluationView::OnImageOutputRequest(WPARAM wparam, LPARAM lparam)
{
	int iResult = 0;
	return iResult;
}

/// <summary>
/// 表示切り替え要求
/// </summary>
/// <param name="wparam">表示ターゲットID</param>
/// <param name="lparam">表示モードID</param>
/// <returns>成功場合は0、失敗場合は-1を返す</returns>
LRESULT CWeldEvaluationView::OnViewChangeRequest(WPARAM wparam, LPARAM lparam)
{
	int targetID = (int)wparam;
	int DisplayMode = (int)lparam;
	int iResult = 0;
	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)GetDocument();

	CImageWind *pImageWnd = nullptr;
	switch(targetID) {
	case	CWeldEvaluationDoc::eResinSurface	:		// 樹脂
		pImageWnd = m_pReginWnd;
		break;
	case	CWeldEvaluationDoc::eMetalSurface	:		// 金属
		pImageWnd = m_pMetalWnd;
		break;
	case	CWeldEvaluationDoc::eJoiningResult	:		// 接合結果
		pImageWnd = m_pResultWnd;
		break;
	}

	if (DisplayMode == 0) {
		if (pDoc->ExistScanFile(targetID)) {
			CImage *pImg = pImageWnd->GetImage();
			if (pDoc->LoadScanImage(targetID, *pImg)) {
				pImageWnd->Draw();
			}
			else {
				pImageWnd->Erase();
			}
		}
		else {
			pImageWnd->Erase();
		}
	}
	else {
		if (pDoc->ExistClassificationResultFile(targetID)) {
			CImage *pImg = pImageWnd->GetImage();
			if (pDoc->LoadClassificationResultImage(targetID, *pImg)) {
				pImageWnd->Draw();
			}
			else {
				pImageWnd->Erase();
			}
		}
		else {
			pImageWnd->Erase();
		}
	}

	if (!pDoc->SetDisplayMode(targetID,DisplayMode)) {
		iResult = -1;
	}
	return iResult;
}


///////////////////////////////////////////////////////////////////////////////////
// 解析操作関連
//

/// <summary>
/// 解析実施要求
/// </summary>
/// <param name="wparam">表示ターゲットID</param>
/// <param name="lparam">解析方法ID</param>
/// <returns>成功場合は0、失敗場合は-1を返す</returns>
LRESULT CWeldEvaluationView::OnAnalyzeRequest(WPARAM wparam, LPARAM lparam)
{
	int targetID = (int)wparam;
	int AnalyzeMethod = (int)lparam;
	int iResult = 0;
	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)GetDocument();

	switch(targetID) {
	case	CWeldEvaluationDoc::eResinSurface	:		// 樹脂
		break;
	case	CWeldEvaluationDoc::eMetalSurface	:		// 金属
		break;
	}
	// 解析の実施
	if (!pDoc->Analize(targetID,AnalyzeMethod)) {
		iResult = -1;
	}

	// 解析方法の設定
	if (!pDoc->SetAnalysisMethod(targetID,AnalyzeMethod)) {
		iResult = -1;
	}
	m_bUpdateOperation = true;
	m_OprtSetting.UpddateResist(m_bUpdateOperation,m_bReadMode);
	m_OprtSetting.Update();
	if (iResult < 0) {
		CString msg;
		msg.LoadString(IDM_ERR_NOT_ANALYZE);
		AfxMessageBox(msg,MB_OK|MB_ICONSTOP);
	}
	return iResult;
}

/// <summary>
/// 解析結果ファイル読み込み状態通知
/// </summary>
/// <param name="wparam">メッセージタイプ</param>
/// <param name="lparam">状態の値</param>
/// <returns>成功した場合は0、失敗した場合はそれ以外を返す</returns>
LRESULT CWeldEvaluationView::OnReadResultFileStatus(WPARAM wparam, LPARAM lparam)
{
	// メッセージがキューにたまってしまうのでメッセージポンプして処理を通す
	MSG msg;
	while (PeekMessage(&msg, m_hWnd,  0, 0, PM_REMOVE)) {
		TranslateMessage(&msg); 
		DispatchMessage(&msg); 
	}

	UINT type = (UINT)wparam;
	UINT value = (UINT)lparam;
	switch(type) {
	case	READ_RESULT_INIT	:		// 処理開始
		{
			if (m_pProgress == nullptr) {
				m_pProgress = new CProgressDlg(this);
			}
			CString str;
			str.LoadString(IDM_RESULT_READ_TITLE);
			m_pProgress->setTitle(str);
			str.LoadString(IDM_RESULT_READ_INIT);
			m_pProgress->setlabel(str);
			m_pProgress->setRange(0,value);
			m_pProgress->setPosition(0);
			m_pProgress->Create(CProgressDlg::IDD, this);
			m_pProgress->ShowWindow(SW_SHOW);
			m_pProgress->UpdateWindow();
		}
		break;
	case	READ_RESULT_READ	:		// 処理進行状況
		{
			if (m_pProgress != nullptr) {
				int min,max;
				m_pProgress->getRange(min,max);
				double per = (double)value / (double)(max - min);
				CString str,fmt;
				fmt.LoadString(IDM_RESULT_READING);
				str.Format(fmt,value, (max - min),per*100);
				m_pProgress->setlabel(str);
				m_pProgress->setPosition(value);
				m_pProgress->UpdateWindow();
			}
		}
		break;
	case	READ_RESULT_STOP	:		// 処理停止リクエスト
		{
			CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)GetDocument();
			pDoc->CalcResultStopRequest();
			break;
		}
	case	READ_RESULT_END		:		// 処理終了
		{
			m_pProgress->DestroyWindow();
			if (m_pProgress) {
				delete m_pProgress;
				m_pProgress = nullptr;
			}
		}
		break;
	}
	return 0;
}


///////////////////////////////////////////////////////////////////////////////////
// グラフ操作関連
//

/// <summary>
/// スペクトルグラフの表示
/// </summary>
/// <param name="wparam">表示データ対象</param>
/// <param name="lparam">表示データ座標</param>
/// <returns>成功した場合は0、失敗した場合はそれ以外を返す</returns>
LRESULT CWeldEvaluationView::OnSpectrumGraphRequest(WPARAM wparam, LPARAM lparam)
{
	CPoint pos = *((CPoint*)lparam);
	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)GetDocument();

	std::vector < std::vector<double>> data;
	data.resize(1);
	int ScanID ;
	switch ((int)wparam) {
	case	CImageWind::eResinSurface	:
		ScanID = CWeldEvaluationDoc::eResinSurface;
		break;
	case	CImageWind::eMetalSurface	:
		ScanID = CWeldEvaluationDoc::eMetalSurface;
		break;
	case	CImageWind::eJoiningResult	:
		ScanID = CWeldEvaluationDoc::eJoiningResult;
		break;
	default:
		return 1;
	}

	if (pDoc->GetSpectrumData(ScanID, pos, data[0])) {
		m_pGraphWnd->SetVirticalRange(0.0, 1.2);
		m_pGraphWnd->Draw(data, 0, false);
	}

	return 0;
}

/// <summary>
/// 区間スペクトル表示ダイアログの表示
/// </summary>
/// <param name="wparam">未使用</param>
/// <param name="lparam">未使用</param>
/// <returns>成功した場合は0、失敗した場合はそれ以外を返す</returns>
LRESULT CWeldEvaluationView::OnAreaSpectrumGraphRequest(WPARAM wparam, LPARAM lparam)
{
	if (m_pSpectralDlg) {
		if (IsWindow(m_pSpectralDlg->m_hWnd)) {
			return 1;
		}
	}
	else {
		m_pSpectralDlg = new CSpectralDlg(this);
	}

	if (m_pSpectralDlg->Create(IDD_SPECTRAL_DLG, this) == 0) {
		return 1;
	}

	int screenH = GetSystemMetrics(SM_CXSCREEN);
	int screenV = GetSystemMetrics(SM_CYSCREEN);
	if (m_SpectralDlgRect.right > screenH) {
		int delta = m_SpectralDlgRect.right - screenH;
		m_SpectralDlgRect.right -= delta;
		m_SpectralDlgRect.left -= delta;
		if (m_SpectralDlgRect.right < 0) {
			m_SpectralDlgRect = CRect(0, 0, 0, 0);
		} else if (m_SpectralDlgRect.left < 0) {
			m_SpectralDlgRect.left = 0;
		}
	}
	else if (m_SpectralDlgRect.left < 0) {
		int delta = m_SpectralDlgRect.left;
		m_SpectralDlgRect.left -= delta;
		m_SpectralDlgRect.right -= delta;
		if (m_SpectralDlgRect.left > screenH) {
			m_SpectralDlgRect = CRect(0, 0, 0, 0);
		}
		else if (m_SpectralDlgRect.right > screenH) {
			m_SpectralDlgRect.right = screenH;
		}
	}

	if (m_SpectralDlgRect.bottom > screenV) {
		int delta = m_SpectralDlgRect.bottom - screenV;
		m_SpectralDlgRect.bottom -= delta;
		m_SpectralDlgRect.top -= delta;
		if (m_SpectralDlgRect.bottom < 0) {
			m_SpectralDlgRect = CRect(0, 0, 0, 0);
		}
		else if (m_SpectralDlgRect.top < 0) {
			m_SpectralDlgRect.left = 0;
		}
	}
	else if (m_SpectralDlgRect.top < 0) {
		int delta = m_SpectralDlgRect.top;
		m_SpectralDlgRect.top -= delta;
		m_SpectralDlgRect.bottom -= delta;
		if (m_SpectralDlgRect.top > screenV) {
			m_SpectralDlgRect = CRect(0, 0, 0, 0);
		}
		else if (m_SpectralDlgRect.bottom > screenV) {
			m_SpectralDlgRect.bottom = screenV;
		}
	}

	ScreenToClient(m_SpectralDlgRect);
	UINT uFlag = SWP_SHOWWINDOW;
	if ((m_SpectralDlgRect.Width() == 0) || (m_SpectralDlgRect.Height() == 0)) {
		uFlag |= SWP_NOMOVE | SWP_NOSIZE;
	}
	::SetWindowPos(m_pSpectralDlg->m_hWnd, HWND_TOPMOST, m_SpectralDlgRect.left, m_SpectralDlgRect.top, m_SpectralDlgRect.Width(), m_SpectralDlgRect.Height(), uFlag);
	m_pReginWnd->setMode(1);
	m_pMetalWnd->setMode(1);

	return 0;
}


/// <summary>
/// 区間スペクトル表示ダイアログのクローズ
/// </summary>
/// <param name="wparam">未使用</param>
/// <param name="lparam">未使用</param>
/// <returns>成功した場合は0、失敗した場合はそれ以外を返す</returns>
/// @remark ダイアログが表示されている場合はクローズする
LRESULT CWeldEvaluationView::OnSpectrumeCloseRequest(WPARAM wparam, LPARAM lparam)
{
	if (m_pSpectralDlg) {
		m_pSpectralDlg->GetWindowRect(&m_SpectralDlgRect);
		ClientToScreen(&m_SpectralDlgRect);
		CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)GetDocument();
		pDoc->SetSpectralDlgRect(m_SpectralDlgRect);
		m_pSpectralDlg->DestroyWindow();
		delete m_pSpectralDlg;
		m_pSpectralDlg = nullptr;
	}
	m_pReginWnd->setMode(0);
	m_pMetalWnd->setMode(0);

	return 0;
}

LRESULT CWeldEvaluationView::OnAreaSpectrumeGraphSet(WPARAM wparam, LPARAM lparam)
{
	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)GetDocument();
	int ScanID;
	switch ((int)wparam) {
	case	CImageWind::eResinSurface:
		ScanID = CWeldEvaluationDoc::eResinSurface;
		break;
	case	CImageWind::eMetalSurface:
		ScanID = CWeldEvaluationDoc::eMetalSurface;
		break;
	case	CImageWind::eJoiningResult:
		ScanID = CWeldEvaluationDoc::eJoiningResult;
		break;
	default:
		return 1;
	}

	vector<int> iata;
	vector<CPoint> pos = *(vector<CPoint>*)lparam;
	std::vector < std::vector<double>> data;
	CPoint point;
	if (abs(pos[1].x - pos[0].x) > abs(pos[1].y - pos[0].y)) {
		// x軸基準
		if (pos[0].x > pos[1].x) {
			CPoint p = pos[0];
			pos[0] = pos[1];
			pos[1] = p;
		}
		int sz = pos[1].x - pos[0].x;
		data.resize(sz);
		double a = (double)(pos[1].y - pos[0].y) / (double)sz;
		double b = (double)(pos[1].x * pos[0].y - pos[0].x * pos[1].y) / (double)sz;
		int x;
		for (int i = 0; i < sz; i++) {
			x = pos[0].x + i;
			point = CPoint(x, (int)(a*x + b));
			if (pDoc->GetSpectrumData(ScanID, point, data[i])) {
			}
		}
	}
	else {
		// y軸基準
		if (pos[0].y > pos[1].y) {
			CPoint p = pos[0];
			pos[0] = pos[1];
			pos[1] = p;
		}
		int sz = pos[1].y - pos[0].y;
		data.resize(sz);
		double a = (double)(pos[1].x - pos[0].x) / (double)sz;
		double b = (double)(pos[1].y * pos[0].x - pos[0].y * pos[1].x) / (double)sz;
		int y;
		for (int i = 0; i < sz; i++) {
			y = pos[0].y + i;
			point = CPoint((int)(a*y + b),y);
			if (pDoc->GetSpectrumData(ScanID, point, data[i])) {
			}
		}
	}
	m_pSpectralDlg->draw(data);

	pos.clear();
	pos.shrink_to_fit();
	data.clear();
	data.shrink_to_fit();
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////
// 
//

/// <summary>
/// 登録済み検査の項目の更新
/// </summary>
/// <param name="wparam">未使用</param>
/// <param name="lparam">未使用</param>
/// <returns>成功場合は0、失敗場合は-1を返す</returns>
LRESULT CWeldEvaluationView::OnChangeResistFolder(WPARAM wparam, LPARAM lparam)
{
	//#######################################################
	//#
	//# プロジェクトが保存されていない場合の処理が必要
	//#
	//#######################################################

	int iResult = 0;
	UpdateRegistFolderList();
	UnSellectProject();
	return iResult;
}

/// <summary>
/// 登録済み検査未選択
/// </summary>
void CWeldEvaluationView::UnSellectProject()
{
	CString msg,AppName;
	AppName.LoadString(AFX_IDS_APP_TITLE);
	msg.Format(_T("%s"), (LPCWSTR)AppName);
	GetParentFrame()->SetWindowText(msg);

	// プロパティの削除
	EnablePropaty(false);
	// 画像、グラフのクリア
	m_pGraphWnd->Erase();
	m_pReginWnd->Erase();
	m_pMetalWnd->Erase();
	m_pResultWnd->Erase();
}


/// <summary>
/// 「新規検査」ボタン押下時処理
/// </summary>
void CWeldEvaluationView::OnBnClickedBtnNewprj()
{
	bool bResult;

	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)GetDocument();
	pDoc->CloseProject();
	UnSellectProject();

	pDoc->OnNewDocument();
	CString msg,sub,AppName;
	sub.LoadString(IDS_NEW);
	AppName.LoadString(AFX_IDS_APP_TITLE);
	msg.Format(_T("%s - %s"), (LPCWSTR)sub, (LPCWSTR)AppName);
	GetParentFrame()->SetWindowText(msg);
	m_bReadMode = false;

	CDialogEx * pDlg;
	int iCnt = (int)m_OprtTab.GetSize();
	for (int i = 0; i < iCnt; i++) {
		pDlg = m_OprtTab.GetAt(i);
		if (pDlg == (CDialogEx *)&m_OprtSetting) {
			WPARAM wparam = WA_ACTIVE & 0xFFFF;
			LPARAM lparam = (LPARAM)pDlg->m_hWnd;
			pDlg->SendMessage(WM_ACTIVATE,wparam,lparam);
			bResult = (bool)pDlg->ShowWindow(SW_SHOW);
			m_tabOperation.SetCurSel(i);
		} else {
			bResult = (bool)pDlg->ShowWindow(SW_HIDE);
		}
	}
	// 設定データの読み込み
	LoadSetting();
	m_OprtInitialize.ItemActive(true);;
	m_OprtSetting.ItemActive(true);
	m_OprtScan.ItemActive(true);;
	m_OprtAnalize.ItemActive(true);
	m_OprtAnalize.ViewModeReset();

	// プロパティデータの読み込み
	LoadPropaty();
	EnablePropaty(true);
}



void CWeldEvaluationView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
	TRACE(_T("MainWind MouseMuve(%d,%d)\n"),point.x,point.y);
	m_pReginWnd->OnMouseLeave();
	m_pMetalWnd->OnMouseLeave();
	m_pResultWnd->OnMouseLeave();
	CFormView::OnMouseMove(nFlags, point);
}
