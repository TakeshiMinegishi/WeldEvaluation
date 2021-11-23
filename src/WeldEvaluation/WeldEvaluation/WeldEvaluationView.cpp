
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
#include "CDeviceIO.h"
#include "CCameraIO.h"
#include "CLOg.h"

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
	ON_MESSAGE(WM_WBSCAN_EXISTCHECK, OnWBScanExistCheck)
	ON_MESSAGE(WM_WBSCAN_REQUES, OnWBScanRequest)
	ON_MESSAGE(WM_VIEW_CHANGE_REQUEST, OnViewChangeRequest)
	ON_MESSAGE(WM_CHANGE_REGIST, OnChangeResistFolder)
	ON_MESSAGE(WM_ANALYSE_REQUEST, OnAnalyzeRequest)
	ON_MESSAGE(WM_RESIST_REGIST, OnProjectResistRequest)
	ON_MESSAGE(WM_IMAGE_OUTPUT_REGIST, OnImageOutputRequest)
	ON_MESSAGE(WM_IMAGE_SCALING, OnImageScaling)
	ON_MESSAGE(WM_IMAGE_MOVEING, OnImageMoveing)
	ON_MESSAGE(WM_READ_RESULT_STATUS, OnReadResultFileStatus)
	ON_MESSAGE(WM_SPECTRUM_GRAPH_REQUEST, OnSpectrumGraphRequest)
	ON_MESSAGE(WM_AREA_SPECTRUM_GRAPH_REQUEST, OnAreaSpectrumGraphRequest)
	ON_MESSAGE(WM_SPECTRUME_CLOSE_REQUEST, OnSpectrumeCloseRequest)
	ON_MESSAGE(WM_AREA_SPECTRUM_GRAPH_SET, OnAreaSpectrumeGraphSet)
	

	ON_WM_NCDESTROY()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_WM_CLOSE()
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


void CWeldEvaluationView::logOut(CString filePath, long lineNo, CString msg)
{
	CLog log;
	CString ErrMsg;
	ErrMsg.Format(_T(" File:%s Line:%ld:%s"), (LPCTSTR)filePath, lineNo, (LPCTSTR)msg);
	log.logWrite(CLog::LOGLEVEL::Error, ErrMsg);
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

/// <summary>
/// ウインド作成前処理
/// </summary>
/// <param name="cs">CREATESTRUCT構造体</param>
/// <returns>作成を継続する場合はTRUE、それ以外はFALSEを返す</returns>
BOOL CWeldEvaluationView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	return CFormView::PreCreateWindow(cs);
}

/// <summary>
/// 初期化処理
/// </summary>
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
	strTitle.LoadString(IDS_PROPTAB_RESULT);
	m_tabPropaty.InsertItem(propId++,strTitle);
	strTitle.LoadString(IDS_PROPTAB_OVERALL);
	m_tabPropaty.InsertItem(propId++,strTitle);

	bResult = m_PropResinPage.Create(CPropTabPageParameter::IDD,&m_stcPropTabPageClient);
	m_PropTab.Add(&m_PropResinPage);
	m_PropResinPage.setPageID(CWeldEvaluationDoc::eResinSurface);
	bResult = m_PropMetalPage.Create(CPropTabPageParameter::IDD,&m_stcPropTabPageClient);
	m_PropTab.Add(&m_PropMetalPage);
	m_PropMetalPage.setPageID(CWeldEvaluationDoc::eMetalSurface);
	bResult = m_PropResultPage.Create(CPropTabPageParameter::IDD,&m_stcPropTabPageClient);
	m_PropTab.Add(&m_PropResultPage);
	m_PropResultPage.setPageID(CWeldEvaluationDoc::eJoiningResult);
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

	pDoc->SetVisible(true);
	OnBnClickedBtnNewprj();
}

// CWeldEvaluationView 診断

#ifdef _DEBUG
/// <summary>
/// オブジェクトの妥当性検査の実施
/// </summary>
void CWeldEvaluationView::AssertValid() const
{
	CFormView::AssertValid();
}

/// <summary>
/// CDumpContextオブジェクトの内容をダンプ
/// </summary>
/// <param name="dc">afxDump診断ダンプ コンテキスト</param>
void CWeldEvaluationView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

/// <summary>
/// ドキュメントの取得
/// </summary>
/// <returns>CWeldEvaluationDocへのポインタを返す</returns>
CWeldEvaluationDoc* CWeldEvaluationView::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWeldEvaluationDoc)));
	return (CWeldEvaluationDoc*)m_pDocument;
}
#endif //_DEBUG


// CWeldEvaluationView メッセージ ハンドラー

/// <summary>
/// ウインドサイズの変更
/// </summary>
/// <param name="nType">ウインドタイプ</param>
/// <param name="cx">変更後のウインド幅</param>
/// <param name="cy">変更後のウインド高さ</param>
void CWeldEvaluationView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	CRect   lpRect;
	GetClientRect(lpRect);

	CWnd* pWnd = GetDlgItem(IDD_WELDEVALUATION_FORM);
	if( pWnd && pWnd->GetSafeHwnd() ) {
		pWnd->MoveWindow(lpRect);
	}
	else {
		if (::IsWindowEnabled(this->m_hWnd)) {
//			MoveWindow(lpRect);
		}
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
	m_PropResultPage.MoveWindow(rect);

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

	// 現在のプロジェクトを選択状態にする
	int count = (int)list.GetCount();
	for (int i = 0; i < count; i++) {
		if (!list[i].CompareNoCase(pDoc->GetActiveRegistedTest())) {
			m_lstRegTest.SetItemState(i, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
			break;
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

	if (pDoc->IsWorkProjectUpdated()) {
		CString msg;
		msg.LoadString(DM_PRJREGIST_EXISTUPDATE);
		if (AfxMessageBox(msg, MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON1) == IDNO) {
			return;
		}
	}
	pDoc->ClrWorkProject();

	m_pGraphWnd->Erase();
	m_pReginWnd->Erase();
	m_pMetalWnd->Erase();
	m_pResultWnd->Erase();

	CWaitCursor wcursol;
	if (pDoc->OpenProject(str)) {

		// 設定データの読み込み
		LoadSetting();
		m_OprtInitialize.ItemActive(true);
		m_bReadMode = true;
//		m_OprtSetting.UpddateResist(false,m_bReadMode);	// 設定データをリードモードにする
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
		ViewChangeRequest(CWeldEvaluationDoc::eResinSurface, DisplayMode);
		m_pReginWnd->Invalidate();

		// 金属面画像
		DisplayMode = pDoc->GetDisplayMode(CWeldEvaluationDoc::eMetalSurface);
		ViewChangeRequest(CWeldEvaluationDoc::eMetalSurface, DisplayMode);
		m_pReginWnd->Invalidate();

		// 結果画像
		DisplayMode = pDoc->GetDisplayMode(CWeldEvaluationDoc::eJoiningResult);
		ViewChangeRequest(CWeldEvaluationDoc::eJoiningResult, DisplayMode);
		m_pReginWnd->Invalidate();

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
		pDoc->SetWorkProjectUpdteStatus(false);
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

/// <summary>
/// 操作タブ切り替え時処理
/// </summary>
/// <param name="pNMHDR"> NMHDR 構造体へのポインター</param>
/// <param name="pResult">結果</param>
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

/// <summary>
/// プロパティタブ切り替え時処理
/// </summary>
/// <param name="pNMHDR"> NMHDR 構造体へのポインター</param>
/// <param name="pResult">結果</param>
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
		ConfirmChange(m_SelPropPageId);
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
					bResult = (bool)pDlg->ShowWindow(SW_SHOW);
					pDlg->SendMessage(WM_ACTIVATE,wparam,lparam);
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

/// <summary>
/// プロパティ更新ボタン押下時処理
/// </summary>
void CWeldEvaluationView::OnBnClickedBtnPropUpdate()
{
	int SelIdx = -1;
	SelIdx = m_tabPropaty.GetCurSel();
	UpdatePropaty(SelIdx);
	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)GetDocument();

	switch (SelIdx) {
	case	0:	// 樹脂面
	{
		pDoc->SetWorkProjectUpdteStatus(true);
	}
	break;
	case	1:	// 金属面
	{
		pDoc->SetWorkProjectUpdteStatus(true);
	}
	break;
	case	2:	// 結果
	{
		pDoc->SetWorkProjectUpdteStatus(true);
	}
	break;
	case	3:	// 設定
	{
		m_OprtInitialize.LoadParamater();
		pDoc->SetWorkProjectUpdteStatus(true);
	}
	break;
	}

	OnUpdateRequestPrpoTab(false,0);
}

/// <summary>
/// プロパティキャンセルボタン押下時処理
/// </summary>
void CWeldEvaluationView::OnBnClickedBtnPropCancel()
{
	OnUpdateRequestPrpoTab(false,0);
	ReloadPropaty(m_SelPropPageId);
}

/// <summary>
/// プロパティキャンセルボタン押下時処理
/// </summary>
/// <param name="wparam"> 更新フラグ</param>
/// <param name="lparam"> 未使用</param>
/// <returns>成功した場合は0、失敗した場合は0以外を返す</returns>
LRESULT CWeldEvaluationView::OnUpdateRequestPrpoTab(WPARAM wparam, LPARAM lparam)
{
	bool bUpdate = (bool)wparam;
	m_btnPropTabOK.EnableWindow(bUpdate);
	m_btnPropTabCancel.EnableWindow(bUpdate);
	m_bUpdatePropaty = bUpdate;
	return 0;
}

/// <summary>
/// プロパティ読み込み
/// </summary>
void CWeldEvaluationView::LoadPropaty()
{
	int iCnt = (int)m_PropTab.GetSize();
	for (int i = 0; i < iCnt; i++) {
		ReloadPropaty(i);
	}
}

/// <summary>
/// プロパティの有効化
/// </summary>
/// <param name="bActive"> 有効フラグ</param>
void CWeldEvaluationView::EnablePropaty(bool bActive)
{
	m_PropResinPage.ItemActive(bActive);
	m_PropMetalPage.ItemActive(bActive);
	m_PropResultPage.ItemActive(bActive);
	m_PropSettingPage.ItemActive(bActive);
}

bool CWeldEvaluationView::ConfirmChange(int propatyID)
{
	bool bResult = false;
	switch (propatyID) {
	case	0:
	{
		CPropTabPageParameter *pDlg = (CPropTabPageParameter *)m_PropTab.GetAt(propatyID);
		bResult = pDlg->ConfirmChange();
	}
	break;
	case	1:
	{
		CPropTabPageParameter *pDlg = (CPropTabPageParameter *)m_PropTab.GetAt(propatyID);
		bResult = pDlg->ConfirmChange();
	}
	break;
	case	2:
	{
		CPropTabPageParameter *pDlg = (CPropTabPageParameter *)m_PropTab.GetAt(propatyID);
		bResult = pDlg->ConfirmChange();
	}
	break;
	case	3:
	{
		CPropTabPageSetting *pDlg = (CPropTabPageSetting *)m_PropTab.GetAt(propatyID);
		bResult = pDlg->ConfirmChange();
	}
	break;
	}
	return bResult;
}

/// <summary>
/// プロパティページの更新
/// </summary>
/// <param name="propatyID"> プロパティID</param>
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

/// <summary>
/// プロパティページの再読込
/// </summary>
/// <param name="propatyID"> プロパティID</param>
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
	case	2	:
		{
			CPropTabPageParameter *pDlg = (CPropTabPageParameter *)m_PropTab.GetAt(propatyID);
			pDlg->LoadParamater(propatyID);
		}
		break;
	case	3	:
		{
			CPropTabPageSetting *pDlg = (CPropTabPageSetting *)m_PropTab.GetAt(propatyID);
			pDlg->LoadParamater();
		}
		break;
	}
}

/// <summary>
/// 設定の読込
/// </summary>
void CWeldEvaluationView::LoadSetting()
{
	int iCnt = (int)m_OprtTab.GetSize();
	for (int i = 0; i < iCnt; i++) {
		ReloadSetting(i);
	}
}

/// <summary>
/// 設定の有効化
/// </summary>
/// <param name="bActive"> 有効フラグ</param>
void CWeldEvaluationView::EnableSetting(bool bActive)
{

}

/// <summary>
/// 設定ページの再読込
/// </summary>
/// <param name="settingID"> 設定ページID</param>
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

/// <summary>
/// スキャンの要求
/// </summary>
/// <param name="wparam"> スキャンID</param>
/// <param name="lparam"> スキャン結果</param>
/// <returns>成功した場合は0、失敗した場合は0以外を返す</returns>
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
	pThread->UpdateStatus(_T("Preparing to start scanning ..."));
	if (!ScanImage(pThread, ScanID)) {
		pThread->m_Dlg.PostMessage(WM_COMMAND, IDCANCEL); // Statusダイアログを閉じる
		*Result = -1;
	}
	else {
		CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)GetDocument();
		if (!pThread->m_Valid) {  // キャンセルボタンが押された場合
			// キャンセルの処理
			*Result = 1;
		}
		else {
			// 正常終了処理
			pThread->m_Dlg.PostMessage(WM_COMMAND, IDOK); // Statusダイアログを閉じる
			// スキャン画面更新
			// もし、スキャンした画像の表示モードが解析ならスキャンに変更
			int DisplayMode = 0;
			pDoc->DeleteScanImageFilePath(ScanID);
			switch (ScanID) {
			case	CWeldEvaluationDoc::eResinSurface:
				DisplayMode = pDoc->GetDisplayMode(CWeldEvaluationDoc::eResinSurface);
				if (DisplayMode == CWeldEvaluationDoc::DisplayModeScan) {
					ViewChangeRequest(CWeldEvaluationDoc::eResinSurface, DisplayMode,true);
					m_pReginWnd->Invalidate();
				}
				break;
			case	CWeldEvaluationDoc::eMetalSurface:
				DisplayMode = pDoc->GetDisplayMode(CWeldEvaluationDoc::eMetalSurface);
				if (DisplayMode == CWeldEvaluationDoc::DisplayModeScan) {
					ViewChangeRequest(CWeldEvaluationDoc::eMetalSurface, DisplayMode, true);
					m_pReginWnd->Invalidate();
				}
				break;
			case	CWeldEvaluationDoc::eJoiningResult:
				DisplayMode = pDoc->GetDisplayMode(CWeldEvaluationDoc::eJoiningResult);
				if (DisplayMode == CWeldEvaluationDoc::DisplayModeScan) {
					ViewChangeRequest(CWeldEvaluationDoc::eJoiningResult, DisplayMode, true);
					m_pReginWnd->Invalidate();
				}
				break;
			}
			pDoc->SetWorkProjectUpdteStatus(true);
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

// #define _LocalCHechk_
/// <summary>
/// スキャンの実施
/// </summary>
/// <param name="pStatus"> スレッドオブジェクト</param>
/// <param name="ScanID"> スキャンID</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CWeldEvaluationView::ScanImage(CStatusDlgThread* pStatus, int ScanID)
{
	CWaitCursor waitCursol;
	CDeviceIO device;

	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)GetDocument();
	CString deviceName = pDoc->GetDeviceName();
	CString ProjectName = pDoc->GetProjectName();
	CString ProjectPath;
	if (ProjectName.IsEmpty()) {
		// プロジェクト登録がまだ
		ProjectPath = pDoc->GetWorkProjectPath();
		if (!CFileUtil::fileExists(ProjectPath)) {
			if (!CreateDirectory(ProjectPath,NULL)) {
				return false;
			}
		}
		ProjectPath = CFileUtil::FilePathCombine(pDoc->GetWorkProjectPath(), pDoc->GetScanDataName(ScanID, _T("")));
	}
	else {
		// 既存プロジェクト
		ProjectPath = pDoc->getScanDataPath(ScanID);
	}

	int ID = device.Init(deviceName);
	if (ID < 0) {
		logOut(CString(__FILE__), __LINE__,_T("ScanImage():デバイスの初期化に失敗した"));
		return false;
	}
	if (!device.ToHome(ID)) {
		logOut(CString(__FILE__), __LINE__, _T("ScanImage():ホームポジションへの移動に失敗した"));
		return false;
	}
	
	int HorizontalResolution = pDoc->GetHorizontalResolution();
	int VerticalResolution = pDoc->GetVerticalResolution();

	// カメラ設定
	int band	= pDoc->NumberOfBand();
	int width	= pDoc->GetShootingWidth();
	int height	= pDoc->GetShootingHeight();
	int offset	= pDoc->NumberOfOverridePixel();
	double speed = pDoc->GetIntegrationTimeMs();
	CCameraIO cam(width, height, band);

	CString SnapscanFile = pDoc->getSnapscanFile();
	if (!CFileUtil::fileExists(SnapscanFile)) {
		CString msg;
		msg.Format(_T("ScanImage():SnapscanFileが見つからない[%s]"), (LPCTSTR)SnapscanFile);
		logOut(CString(__FILE__), __LINE__, msg);
		return false;
	}
	bool dummyApi = pDoc->IsCameraDummyApi();
	if (!cam.Open(SnapscanFile,dummyApi)) {
		logOut(CString(__FILE__), __LINE__, _T("ScanImage():カメラをオープンできませんでした"));
		return false;
	}
	if (!cam.setIntegrationTime(speed)) {
		CString msg;
		msg.Format(_T("ScanImage():IntegrationTimeの設定に失敗しました[%lf]"), speed);
		logOut(CString(__FILE__), __LINE__, msg);
		return false;
	}
	if (!cam.StartScan()) {
		logOut(CString(__FILE__), __LINE__, _T("ScanImage():スキャンの開始に失敗しました"));
		return false;
	}

	CString registedFolde = pDoc->GetRegistedFolder();
	CString WBFileName = pDoc->GetWBFileName();
	CubeFloat *reference_corrected = new CubeFloat();
	if (!cam.LoadReference(*reference_corrected, registedFolde, WBFileName)) {
		logOut(CString(__FILE__), __LINE__, _T("ScanImage():ホワイトバランスデータのロードに失敗しました"));
		return false;
	}

	// 一時ファイル保存先パス設定
	bool bResult = true;
	CString spectralFilePath = pDoc->GetTmpFolderPath();
	if (!CFileUtil::fileExists(spectralFilePath)) {
		if (!CreateDirectory(spectralFilePath, NULL)) {
			spectralFilePath = pDoc->GetRegistedFolder();
		}
	}

	CubeFloat  *cube_corrected = new CubeFloat();
	CScanDataIO scn;
	double hscale = pDoc->GetHorizontalScale();
	double vscale = pDoc->GetVerticalScale();
	// 変換後のデータは90度回転させるので、縦横が入れ替わる
	int dstW = (int)(height * vscale);
	int dstH = (int)(width * hscale);
	float ***dst = nullptr;
	float ***outData = nullptr;

	//////////////////////////////////////////////////////////////////////////////
	CubeFloat *cube = new CubeFloat();
	CString fname;
	int DivisionNumber = pDoc->GetDivisionNumber();
	int outH = dstH;
	int outW;
	if (dstW < offset) {
		return false;
	}
	if (dstW < (offset * 2)) {
		outW = dstW + (dstW - offset) * (DivisionNumber - 1);
	}
	else {
		outW = (dstW * DivisionNumber) - (DivisionNumber - 1)*offset;
	}
	int jointPos = 0;
	outData = (float ***)malloc(sizeof(float **)*band);
	for (int b = 0; b < band; b++) {
		outData[b] = (float**)malloc(sizeof(float **)*dstH);
		for (int h = 0; h < dstH; h++) {
			outData[b][h] = (float*)malloc(sizeof(float *)*outW);
			ZeroMemory(outData[b][h], sizeof(float *)*outW);
		}
	}

	double *pWavelength = nullptr;
	for (int pos = 0; pos < DivisionNumber; pos++) {
		if (!pStatus->m_Valid) {  // キャンセルボタンが押された場合は何もせずに終了
			cam.StopScan();
			commonDeallocateCube(reference_corrected);
			if (reference_corrected) {
				delete[] reference_corrected;
			}
			bResult = true;
			break;
		}
		if (!device.Move(ID, pos)) {
			break;
		}
		Sleep(100);
		CString buff;
		buff.Format(_T("Scaning : %d/%d(%d %%)"), pos+1, DivisionNumber, (int)((pos+1) * 100 / DivisionNumber));
		pStatus->UpdateStatus(buff);
		/////////////////////////////////////////////////////////
		// スキャン実施
		/////////////////////////////////////////////////////////
		fname.Format(_T("normal%02d"), pos+1);
		*cube_corrected = { 0 };
		if (!cam.AcquireSpectralCube(spectralFilePath,fname, *reference_corrected, *cube_corrected)) {
			bResult = false;
			break;
		}
		if (!pWavelength) {
			pWavelength = new double[band];
			for (int b = 0; b < band; b++) {
				pWavelength[b] = cube_corrected->format.band_names[b];
			}
		}

		/////////////////////////////////////////////////////////////////
		// スキャンデータ変換処理
		buff.Format(_T("Scaning : %d/%d(%d %%) Converting..."), pos + 1, DivisionNumber, (int)((pos + 1) * 100 / DivisionNumber));
		pStatus->UpdateStatus(buff);
		scn.ScanDataConvert(width, height, band, (float ***)cube_corrected->ppp_data, hscale, vscale, dstW, dstH, dst,false);

		/////////////////////////////////////////////////////////////////
		// スキャンデータ結合処理
		if (outH < dstH) {
			for (int b = 0; b < band; b++) {
				outData[b] = (float**)realloc(outData[b],(sizeof(float **)*dstH));
				for (int h = outH; h < dstH; h++) {
					outData[b][h] = (float*)malloc(sizeof(float *)*outW);
					ZeroMemory(outData[b][h], sizeof(float *)*outW);
				}
			}
			outH = dstH;
		}
		if (jointPos == 0) {
			for (int b = 0; b < band; b++) {
				buff.Format(_T("Scaning : %d/%d(%d %%) Joint : %d/%d"), pos + 1, DivisionNumber, (int)((pos + 1) * 100 / DivisionNumber),b+1,band);
				pStatus->UpdateStatus(buff);
				for (int h = 0; h < dstH; h++) {
					memcpy(outData[b][h], dst[b][h], sizeof(float)*dstW);
				}
			}
			jointPos = dstW - offset;
		}
		else {
			if (jointPos + dstW > outW) {
				for (int b = 0; b < band; b++) {
					for (int h = 0; h < outH; h++) {
						outData[b][h] = (float *)realloc(outData[b][h], jointPos + dstW);
					}
				}
				outW = jointPos + dstW;
			}

			// offset分結合
			for (int b = 0; b < band; b++) {
				buff.Format(_T("Scaning : %d/%d(%d %%) Joint : %d/%d"), pos + 1, DivisionNumber, (int)((pos + 1) * 100 / DivisionNumber), b + 1, band);
				pStatus->UpdateStatus(buff);
				for (int h = 0; h < outH; h++) {
					for (int w = 0; w < offset; w++) {
						float delta = (float)w / (float)offset;
						outData[b][h][jointPos + w] = outData[b][h][jointPos + w]*(1.0f-delta) + dst[b][h][w]*delta;
					}
					memcpy(&outData[b][h][jointPos + offset], &dst[b][h][offset], sizeof(float)*(dstW - offset));
				}
			}
			jointPos += dstW - offset;
		}

		if (dst) {
			for (int b = 0; b < band; b++) {
				for (int h = 0; h < dstH; h++) {
					if (dst[b][h]) {
						delete[] dst[b][h];
						dst[b][h] = nullptr;
					}
				}
				delete[] dst[b];
				dst[b] = nullptr;
			}
			delete[] dst;
			dst = nullptr;
		}
		commonDeallocateCube(cube_corrected);
		commonDeallocateCube(cube);
	}
	if (cube_corrected) {
		delete  cube_corrected;
		cube_corrected = nullptr;
	}
	cam.Close();
	device.Close(ID);
	commonDeallocateCube(reference_corrected);
	if (reference_corrected) {
		delete[] reference_corrected;
	}

	pStatus->UpdateStatus(_T("Closing..."));
	///////////////////////////////////////////////////////////////////////////////////
	// データの保存
	if (bResult) {
		CString headerFile = ProjectPath + _T(".hdr");
		CString rawFile = ProjectPath + _T(".raw");

		CStdioFile tfd;
		// headerデータ出力
		if (tfd.Open(headerFile, CFile::modeWrite | CFile::modeCreate | CFile::typeText)) {
			CString buf;
			buf = _T("ENVI");
			CFileUtil::WriteUTF8ToSJIS(tfd, buf);
			buf.Format(_T("bands = %d"), band);
			CFileUtil::WriteUTF8ToSJIS(tfd, buf);
			buf.Format(_T("byte order = %d"), 0);
			CFileUtil::WriteUTF8ToSJIS(tfd, buf);
			buf.Format(_T("data type = %d"), sizeof(float));
			CFileUtil::WriteUTF8ToSJIS(tfd, buf);
			buf.Format(_T("file type = %s"), _T("ENVI Standard"));
			CFileUtil::WriteUTF8ToSJIS(tfd, buf);
			buf.Format(_T("header offset = %d"), 0);
			CFileUtil::WriteUTF8ToSJIS(tfd, buf);
			buf.Format(_T("interleave = %s"), _T("BSQ"));
			CFileUtil::WriteUTF8ToSJIS(tfd, buf);
			buf.Format(_T("lines = %d"), outH);
			CFileUtil::WriteUTF8ToSJIS(tfd, buf);
			buf.Format(_T("samples = %d"), outW);
			CFileUtil::WriteUTF8ToSJIS(tfd, buf);

			buf = _T("wavelength = {");
			CFileUtil::WriteUTF8ToSJIS(tfd, buf);

			buf.Format(_T("%.8lf"), pWavelength[0]);
			for (int b = 1; b < band; b++) {
				buf.Format(_T("%s,%.8lf"), buf, pWavelength[b]);
			}
			CFileUtil::WriteUTF8ToSJIS(tfd, buf);
			buf = _T("}");
			CFileUtil::WriteUTF8ToSJIS(tfd, buf);

			tfd.Close();
			pDoc->SetScanDataSize(outW,outH);
		}

		// rawデータ出力
		CFile fd;
		if (fd.Open(rawFile, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary)) {
			for (int b = 0; b < band; b++) {
				for (int h = 0; h < outH; h++) {
					fd.Write(outData[b][h], sizeof(float)*outW);
				}
			}
			fd.Close();
		}
	}

	if (pWavelength) {
		delete[] pWavelength;
		pWavelength = nullptr;
	}

	if (outData) {
		for (int b = 0; b < band; b++) {
			for (int h = 0; h < dstH; h++) {
				free(outData[b][h]);
				outData[b][h] = nullptr;
			}
			free(outData[b]);
			outData[b] = nullptr;
		}
		free(outData);
		outData = nullptr;
	}

	if (cube) {
		delete cube;
		cube = nullptr;
	}
	return bResult;
}

LRESULT CWeldEvaluationView::OnWBScanExistCheck(WPARAM wparam, LPARAM lparam)
{
	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)GetDocument();
	CString registedFolde = pDoc->GetRegistedFolder();
	CString WBFileName = pDoc->GetWBFileName();
	CString fname = WBFileName + _T(".hdr");
	CString chkPath = CFileUtil::FilePathCombine(registedFolde, fname);
	if (CFileUtil::fileExists(chkPath)) {
		CString msg;
		msg.LoadString(IDM_EXISTWBSCANFILE);
		if (AfxMessageBox(msg, MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON1) != IDYES) {
			return 1;
		}
	}
	return 0;
}

/// <summary>
/// ホワイトバランススキャンの要求
/// </summary>
/// <param name="wparam"> スレッドオブジェクト</param>
/// <param name="lparam"> スキャン結果</param>
/// <returns>成功した場合は0、失敗した場合は0以外を返す</returns>
LRESULT CWeldEvaluationView::OnWBScanRequest(WPARAM wparam, LPARAM lparam)
{
	CStatusDlgThread* pStatus = (CStatusDlgThread*)wparam;
	int *Result = (int *)lparam;
	*Result = -1;

	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)GetDocument();
	CString registedFolde = pDoc->GetRegistedFolder();
	CString WBFileName = pDoc->GetWBFileName();
	CString deviceName = pDoc->GetDeviceName();
	CString buff;
	CDeviceIO device;
	int ID = device.Init(deviceName);
	if (ID >= 0) {
		if (pStatus->m_Valid) {  // キャンセルボタンが押されていない
			if (device.ToHome(ID)) {
				if (device.Move(ID, 1)) {
					if (pStatus->m_Valid) {  // キャンセルボタンが押されていない
						buff.LoadString(IDS_PREPANING);
						pStatus->UpdateStatus(buff);
						/////////////////////////////////////////////////////////
						// ホワイトバランスデータの撮影
						/////////////////////////////////////////////////////////
						int band = pDoc->NumberOfBand();
//						int width = pDoc->GetHorizontalResolution();
//						int height = pDoc->GetVerticalResolution();
						int width = pDoc->GetShootingWidth();
						int height = pDoc->GetShootingHeight();
						double speed = pDoc->GetIntegrationTimeMs();
						CCameraIO cam(width, height,band);

						CString SnapscanFile = pDoc->getSnapscanFile();
						if (CFileUtil::fileExists(SnapscanFile)) {
							bool dummyApi = pDoc->IsCameraDummyApi();
							if (cam.Open(SnapscanFile, dummyApi)) {
								buff.LoadString(IDM_SCANNING);
								pStatus->UpdateStatus(buff);
								if (cam.setIntegrationTime(speed)) {
									if (cam.StartScan()) {
										if (cam.AcquireReference(registedFolde,WBFileName)) {
											*Result = 0;
											buff.LoadString(IDM_SCAN_SUCCESS);
											pStatus->UpdateStatus(buff);
										}
										else {
											logOut(CString(__FILE__), __LINE__, _T("OnWBScanRequest():ホワイトバランスデータの取得に失敗しました"));
										}
									}
									else {
										logOut(CString(__FILE__), __LINE__, _T("OnWBScanRequest():スキャンの開始に失敗しました"));
									}
								}
								else {
									CString msg;
									msg.Format(_T("OnWBScanRequest():IntegrationTimeの設定に失敗しました[%lf]"), speed);
									logOut(CString(__FILE__), __LINE__, msg);
								}
								cam.Close();
							}
							else {
								logOut(CString(__FILE__), __LINE__, _T("OnWBScanRequest():カメラをオープンできませんでした"));
							}
						}
						else {
							CString msg;
							msg.Format(_T("OnWBScanRequest():SnapscanFileが見つからない[%s]"), (LPCTSTR)SnapscanFile);
							logOut(CString(__FILE__), __LINE__, msg);
						}
					}
					else {
						buff.LoadString(IDM_SCAN_CANCELD);
						AfxMessageBox(buff, MB_OK | MB_ICONWARNING);
					}
				}
			}
		}
		else {
			buff.LoadString(IDM_SCAN_CANCELD);
			AfxMessageBox(buff, MB_OK | MB_ICONWARNING);
		}
	}

	return 0;
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

	CWaitCursor waitCursol;
	if (!pDoc->SaveProject()) {
		CString msg;
		msg.LoadString(IDM_ERR_PROJECT_SAVE);
		AfxMessageBox(msg,MB_OK|MB_ICONSTOP);
		iResult = -1;
	} else {
		m_bUpdateOperation = false;
		m_OprtSetting.UpddateResist(true,m_bReadMode);
//		m_OprtSetting.UpddateResist(m_bUpdateOperation,m_bReadMode);
		m_OprtSetting.Update();
		UpdateRegistFolderList();
	}
	return iResult;
}

/// <summary>
/// 画像出力要求
/// </summary>
/// <param name="wparam">未使用</param>
/// <param name="lparam">未使用</param>
//// <returns>成功場合は0、失敗場合は-1を返す</returns>
LRESULT CWeldEvaluationView::OnImageOutputRequest(WPARAM wparam, LPARAM lparam)
{
	int iResult = -1;

	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)GetDocument();

	CWaitCursor waitCursol;
	CString ImagePath = pDoc->GetScanImagePath(CWeldEvaluationDoc::eResinSurface);
	int buffSize = 1024;
	TCHAR *buff = new TCHAR[buffSize];
	int size;
	while (1) {
		size = GetCurrentDirectory(sizeof(TCHAR)*buffSize, buff);
		if (size < buffSize) {
			ImagePath = buff;
			break;
		}
		if (buff) {
			delete[] buff;
		}
		buffSize *= 2;
		if (buffSize > 16384) {
			break;
		}
		buff = new TCHAR[buffSize];
	}

	CString label;
	label.LoadString(IDS_LBL_FOLDERSELECT);

	if (CFileUtil::SelectFolder(this->m_hWnd, ImagePath, buff, BIF_RETURNONLYFSDIRS, label)) {
		CString writePath = buff;
		pDoc->WriteImage(writePath);
	}
	if (buff) {
		delete[] buff;
	}

	return iResult;
}

/// <summary>
/// 画像スケーリング実施通知
/// </summary>
/// <param name="wparam">表示ターゲットID</param>
/// <param name="lparam">画像エリア値</param>
/// <returns>成功場合は0、失敗場合は-1を返す</returns>
LRESULT CWeldEvaluationView::OnImageScaling(WPARAM wparam, LPARAM lparam)
{
	int iResult = 0;
#if true
	int targetID = (int)wparam;
	CRect *pRect = (CRect *)lparam;
	if (!CWeldEvaluationView::ImageScaling(targetID, *pRect)) {
		iResult = -1;
	}
#else
	int targetID = (int)wparam;
	CRect *pRect = (CRect *)lparam;
	double scalingRetio = 1.0;
	CPoint pos,pos1,pos2;
	CImageWind *pImageWnd1 = nullptr;
	CImageWind *pImageWnd2 = nullptr;
	switch (targetID) {
	case	CWeldEvaluationDoc::eResinSurface:		// 樹脂
		if (!m_pReginWnd->GetScalingInfo(scalingRetio, pos)) {
			return -1;
		}
		pImageWnd1 = m_pMetalWnd;
		pImageWnd2 = m_pResultWnd;
		pos1 = pImageWnd1->ConvertImagePos(pos);
		pos2 = pos;
		break;
	case	CWeldEvaluationDoc::eMetalSurface:		// 金属
		if (!m_pMetalWnd->GetScalingInfo(scalingRetio, pos)) {
			return -1;
		}
		pImageWnd1 = m_pReginWnd;
		pImageWnd2 = m_pResultWnd;
		pos1 = pImageWnd1->ConvertImagePos(pos);
		pos2 = pImageWnd2->ConvertImagePos(pos);
		break;
	case	CWeldEvaluationDoc::eJoiningResult:		// 接合結果
		if (!m_pResultWnd->GetScalingInfo(scalingRetio, pos)) {
			return -1;
		}
		pImageWnd1 = m_pReginWnd;
		pImageWnd2 = m_pMetalWnd;
		pos1 = pos;
		pos2 = pImageWnd2->ConvertImagePos(pos);
		break;
	default:
		return -1;
	}

	if (pImageWnd1) {
		pImageWnd1->MoveImage(pos1.x,pos1.y, pRect->Width(), pRect->Height(), scalingRetio);
		pImageWnd1->MoveImage(pos1.x, pos1.y, pRect->Width(), pRect->Height(), scalingRetio);
	}
	if (pImageWnd2) {
		pImageWnd2->MoveImage(pos2.x, pos2.y, pRect->Width(), pRect->Height(), scalingRetio);
		pImageWnd1->MoveImage(pos1.x, pos1.y, pRect->Width(), pRect->Height(), scalingRetio);
	}
	TRACE(_T("Pos(%d,%d)->Pos1(%d,%d),Pos2(%d,%d)\n"), pos.x, pos.y, pos1.x, pos1.y, pos2.x, pos2.y);
#endif
	return iResult;
}

/// <summary>
/// 画像移動通知
/// </summary>
/// <param name="wparam">表示ターゲットID</param>
/// <param name="lparam">画像エリア値</param>
/// <returns>成功場合は0、失敗場合は-1を返す</returns>
LRESULT CWeldEvaluationView::OnImageMoveing(WPARAM wparam, LPARAM lparam)
{
	int iResult = 0;
#if true
	int targetID = (int)wparam;
	CRect *pRect = (CRect *)lparam;
	if (!CWeldEvaluationView::ImageScaling(targetID, *pRect)) {
		iResult = -1;
	}
#else
	int targetID = (int)wparam;
	CRect *pRect = (CRect *)lparam;
	double scalingRetio = 1.0;
	CPoint pos, pos1, pos2;
	CImageWind *pImageWnd1 = nullptr;
	CImageWind *pImageWnd2 = nullptr;
	switch (targetID) {
	case	CWeldEvaluationDoc::eResinSurface:		// 樹脂
		if (!m_pReginWnd->GetScalingInfo(scalingRetio, pos)) {
			return -1;
		}
		pImageWnd1 = m_pMetalWnd;
		pImageWnd2 = m_pResultWnd;
		pos1 = m_pReginWnd->ConvertImagePos(pos);
		pos2 = pos;
		break;
	case	CWeldEvaluationDoc::eMetalSurface:		// 金属
		if (!m_pMetalWnd->GetScalingInfo(scalingRetio, pos)) {
			return -1;
		}
		pImageWnd1 = m_pReginWnd;
		pImageWnd2 = m_pResultWnd;
		pos1 = m_pMetalWnd->ConvertImagePos(pos);
		pos2 = m_pMetalWnd->ConvertImagePos(pos);
		break;
	case	CWeldEvaluationDoc::eJoiningResult:		// 接合結果
		if (!m_pResultWnd->GetScalingInfo(scalingRetio, pos)) {
			return -1;
		}
		pImageWnd1 = m_pReginWnd;
		pImageWnd2 = m_pMetalWnd;
		pos1 = pos;
		pos2 = m_pResultWnd->ConvertImagePos(pos);
		break;
	default:
		return -1;
	}

	if (pImageWnd1) {
		pImageWnd1->MoveImage(pos1.x, pos1.y, pRect->Width(), pRect->Height(), scalingRetio);
		pImageWnd1->MoveImage(pos1.x, pos1.y, pRect->Width(), pRect->Height(), scalingRetio);
	}
	if (pImageWnd2) {
		pImageWnd2->MoveImage(pos2.x, pos2.y, pRect->Width(), pRect->Height(), scalingRetio);
		pImageWnd2->MoveImage(pos2.x, pos2.y, pRect->Width(), pRect->Height(), scalingRetio);
	}
	TRACE(_T("Pos(%d,%d)->Pos1(%d,%d),Pos2(%d,%d)\n"),pos.x,pos.y, pos1.x, pos1.y, pos2.x, pos2.y);
#endif
	return iResult;
}

bool CWeldEvaluationView::ViewChangeRequest(int ScanID, int DisplayMode, bool renew/*=false*/)
{
	bool bResult = true;
	int iResult = 0;
	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)GetDocument();

	CImageWind *pImageWnd = nullptr;
	switch (ScanID) {
	case	CWeldEvaluationDoc::eResinSurface:		// 樹脂
		pImageWnd = m_pReginWnd;
		break;
	case	CWeldEvaluationDoc::eMetalSurface:		// 金属
		pImageWnd = m_pMetalWnd;
		break;
	case	CWeldEvaluationDoc::eJoiningResult:		// 接合結果
		pImageWnd = m_pResultWnd;
		break;
	}

	if (DisplayMode == 0) {
		if (pDoc->ExistScanFile(ScanID)) {
			CImage *pImg = pImageWnd->GetImage();
			if (pDoc->LoadScanImage(ScanID, *pImg, renew)) {
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
		int type = m_OprtAnalize.GetAnalizeType(ScanID);
		if (pDoc->ExistClassificationResultFile(ScanID, type)) {
			CImage *pImg = pImageWnd->GetImage();
			if (pDoc->LoadClassificationResultImage(ScanID, type, *pImg,true)) {
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

	if (!pDoc->SetDisplayMode(ScanID, DisplayMode)) {
		bResult = false;
	}
	pDoc->SetWorkProjectUpdteStatus(true);
	return bResult;
}

/// <summary>
/// 表示切り替え要求
/// </summary>
/// <param name="wparam">表示ターゲットID</param>
/// <param name="lparam">表示モードID</param>
/// <returns>成功場合は0、失敗場合は-1を返す</returns>
LRESULT CWeldEvaluationView::OnViewChangeRequest(WPARAM wparam, LPARAM lparam)
{
	int iResult = 0;
	int ScanID = (int)wparam;
	int DisplayMode = (int)lparam;
	if (!ViewChangeRequest(ScanID, DisplayMode)) {
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

	CString ScanDataFilePath = pDoc->getScanDataFilePath(targetID);
	// 改正対象が存在するかをチェック
	if (!CFileUtil::fileExists(ScanDataFilePath)) {
		AfxMessageBox(_T("解析対象が存在しません。"));
		return false;
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
	else {
		// もし、解析した画像の表示モードがスキャンなら解析に変更
//		if (pDoc->GetDisplayMode(targetID) == CWeldEvaluationDoc::DisplayModeScan) {
			ViewChangeRequest(targetID, CWeldEvaluationDoc::DisplayModeResult,true);
			m_pReginWnd->Invalidate();
//		}

		m_OprtAnalize.LoadParamater();

		// プロパティページを更新
		switch (targetID) {
		case	CWeldEvaluationDoc::eResinSurface:		// 樹脂
			m_PropResinPage.LoadParamater(CWeldEvaluationDoc::eResinSurface);
			break;
		case	CWeldEvaluationDoc::eMetalSurface:		// 金属
			m_PropMetalPage.LoadParamater(CWeldEvaluationDoc::eMetalSurface);
			break;
		case	CWeldEvaluationDoc::eJoiningResult:		// 結合結果
			m_PropResultPage.LoadParamater(CWeldEvaluationDoc::eJoiningResult);
			break;
		}
		pDoc->SetWorkProjectUpdteStatus(true);
		
		CString	msg;
		msg.LoadString(IDM_ANALYZE_SUCCESS);
		AfxMessageBox(msg, MB_OK | MB_ICONINFORMATION);
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
	m_pResultWnd->setMode(1);

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
	m_pResultWnd->setMode(0);

	return 0;
}

/// <summary>
/// 区間スペクトル表示ダイアログの設定
/// </summary>
/// <param name="wparam">対象ID</param>
/// <param name="lparam">表示対象画像位置</param>
/// <returns>成功した場合は0、失敗した場合はそれ以外を返す</returns>
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
	if (pDoc->IsWorkProjectUpdated()) {
		CString msg;
		msg.LoadString(DM_PRJREGIST_EXISTUPDATE);
		if (AfxMessageBox(msg, MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON1) == IDNO) {
			return;
		}
	}

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
			m_OprtSetting.LoadParamater();
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

/// <summary>
/// マウス移動時処理
/// </summary>
/// <param name="nFlags">仮想キーフラグ</param>
/// <param name="point">カーソル座標</param>
/// <returns>成功の場合はTRUE、失敗の場合はFALSEを返す</returns>
/// @remark 仮想キーフラグnFlagsはMK_CONTROL CTRL キー、MK_LBUTTON マウスの左ボタン、MK_MBUTTON マウスの中央ボタン、MK_RBUTTON マウスの右ボタン、MK_SHIFT SHIFT キー
void CWeldEvaluationView::OnMouseMove(UINT nFlags, CPoint point)
{
	m_pReginWnd->OnMouseLeave();
	m_pMetalWnd->OnMouseLeave();
	m_pResultWnd->OnMouseLeave();
	CFormView::OnMouseMove(nFlags, point);
}

/// <summary>
/// 画像のスケーリング・移動
/// </summary>
/// <param name="targetID">表示ターゲットID</param>
/// <param name="rect">画像エリア値</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CWeldEvaluationView::ImageScaling(int targetID, CRect rect)
{
	bool bResult = true;
	double scalingRetio = 1.0;
	CPoint pos, pos1, pos2,tmp;
	CImageWind *pImageWnd1 = nullptr;
	CImageWind *pImageWnd2 = nullptr;
	switch (targetID) {
	case	CWeldEvaluationDoc::eResinSurface:		// 樹脂
		if (!m_pReginWnd->GetScalingInfo(scalingRetio, pos)) {
			return false;
		}
		pImageWnd1 = m_pMetalWnd;
		pImageWnd2 = m_pResultWnd;
		tmp = m_pReginWnd->ConvertImagePos(CPoint(pos.x, 0));
		pos1 = tmp;
		pos2 = pos;
		break;
	case	CWeldEvaluationDoc::eMetalSurface:		// 金属
		if (!m_pMetalWnd->GetScalingInfo(scalingRetio, pos)) {
			return false;
		}
		pImageWnd1 = m_pReginWnd;
		pImageWnd2 = m_pResultWnd;
		tmp = m_pMetalWnd->ConvertImagePos(CPoint(pos.x, 0));
		pos1 = tmp;
		pos2 = tmp;
		break;
	case	CWeldEvaluationDoc::eJoiningResult:		// 接合結果
		if (!m_pResultWnd->GetScalingInfo(scalingRetio, pos)) {
			return false;
		}
		pImageWnd1 = m_pReginWnd;
		pImageWnd2 = m_pMetalWnd;
		tmp = m_pResultWnd->ConvertImagePos(CPoint(pos.x, 0));
		pos1 = pos;
		pos2 = tmp;
		break;
	default:
		return false;
	}

	if (pImageWnd1) {
		pImageWnd1->MoveImage(pos1.x, pos1.y, rect.Width(), rect.Height(), scalingRetio);
		pImageWnd1->MoveImage(pos1.x, pos1.y, rect.Width(), rect.Height(), scalingRetio);
	}
	if (pImageWnd2) {
		pImageWnd2->MoveImage(pos2.x, pos2.y, rect.Width(), rect.Height(), scalingRetio);
//		pImageWnd2->MoveImage(pos2.x, pos2.y, rect.Width(), rect.Height(), scalingRetio);
	}
	TRACE(_T("Pos(%d,%d)->Pos1(%d,%d),Pos2(%d,%d)\n"), pos.x, pos.y, pos1.x, pos1.y, pos2.x, pos2.y);
	return bResult;
}



void CWeldEvaluationView::OnClose()
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	CFormView::OnClose();
}
