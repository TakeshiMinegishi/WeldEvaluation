
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
#pragma warning(disable:26454)

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
	ON_MESSAGE(WM_RESIST_PROJECT, OnProjectResistRequest)
	ON_MESSAGE(WM_IMAGE_OUTPUT_REGIST, OnImageOutputRequest)
	ON_MESSAGE(WM_IMAGE_SCALING, OnImageScaling)
	ON_MESSAGE(WM_IMAGE_MOVEING, OnImageMoveing)
	ON_MESSAGE(WM_READ_RESULT_STATUS, OnReadResultFileStatus)
	ON_MESSAGE(WM_SPECTRUM_GRAPH_REQUEST, OnSpectrumGraphRequest)
	ON_MESSAGE(WM_AREA_SPECTRUM_GRAPH_REQUEST, OnAreaSpectrumGraphRequest)
	ON_MESSAGE(WM_SPECTRUME_CLOSE_REQUEST, OnSpectrumeCloseRequest)
	ON_MESSAGE(WM_AREA_SPECTRUM_GRAPH_SET, OnAreaSpectrumeGraphSet)
	ON_MESSAGE(WM_VIEW_CLER, OnImageErace)
	ON_MESSAGE(WM_INVERS_REQUEST, OnInversRequest)
	ON_MESSAGE(WM_INVERS_STATUS, OnInversStatus)

	ON_WM_NCDESTROY()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_WM_CLOSE()
	ON_NOTIFY(NM_RCLICK, IDC_LST_REGIST_TEST, &CWeldEvaluationView::OnNMRClickLstRegistTest)
	ON_COMMAND(ID_PROJECT_OPEN, &CWeldEvaluationView::OnProjectOpen)
	ON_COMMAND(ID_PROJECT_DELETE, &CWeldEvaluationView::OnProjectDelete)
	ON_COMMAND(ID_FILE_SAVE, &CWeldEvaluationView::OnFileSave)
	ON_COMMAND(ID_FILE_NEW, &CWeldEvaluationView::OnProjectNew)
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
	m_SpectralDlgRect	= CRect(0, 0, 0, 0);
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
	CWnd* pWnd = nullptr;
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
/*
	formRect.top    = 0;
	formRect.left   = 0;
	formRect.bottom = hight;
	formRect.right  = width;
*/
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
	if (!strTitle.LoadString(IDS_OPRTAB_INIT)) {
		strTitle = _T("初期化");
	}
	m_tabOperation.InsertItem(0,strTitle);
	if (!strTitle.LoadString(IDS_OPRTAB_SETTING)) {
		strTitle = _T("設定");
	}
	m_tabOperation.InsertItem(1,strTitle);
	if (!strTitle.LoadString(IDS_OPRTAB_SCAN)) {
		strTitle = _T("スキャン");
	}
	m_tabOperation.InsertItem(2,strTitle);
	if (!strTitle.LoadString(IDS_OPRTAB_ANARIZE)) {
		strTitle = _T("解析");
	}
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
	if (!strTitle.LoadString(IDS_PROPTAB_RESIN)) {
		strTitle = _T("樹脂面");
	}
	m_tabPropaty.InsertItem(propId++,strTitle);
	if (!strTitle.LoadString(IDS_PROPTAB_METAL)) {
		strTitle = _T("金属面");
	}
	m_tabPropaty.InsertItem(propId++,strTitle);
	if (!strTitle.LoadString(IDS_PROPTAB_RESULT)) {
		strTitle = _T("接合結果");
	}
	m_tabPropaty.InsertItem(propId++,strTitle);
	if (!strTitle.LoadString(IDS_PROPTAB_OVERALL)) {
		strTitle = _T("全体");
	}
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
	pWnd = GetDlgItem(IDC_STC_GRAPH_WND);
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

	GetWindowRect(rect);
	CSize minRect = CSize(rect.Width(), rect.Height());
	int w = GetSystemMetrics(SM_CXSIZEFRAME) + GetSystemMetrics(SM_CXEDGE);;
	int h = GetSystemMetrics(SM_CYSIZEFRAME) + GetSystemMetrics(SM_CYEDGE);
	minRect.cx += w * 2;
	minRect.cy += h * 2;

	w += GetSystemMetrics(SM_CYHSCROLL) + GetSystemMetrics(SM_CXHTHUMB) * 2;
	h += GetSystemMetrics(SM_CYHSCROLL) + GetSystemMetrics(SM_CXHTHUMB) * 2;

	h = GetSystemMetrics(SM_CYCAPTION);
	minRect.cy += h*4;
	minRect.cx += w;

	pDoc->SetMinWndSize(minRect);


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
//	GetClientRect(lpRect);
	GetWindowRect(lpRect);

	CWnd* pWnd = GetDlgItem(IDD_WELDEVALUATION_FORM);
	if( pWnd && pWnd->GetSafeHwnd() ) {
		pWnd->MoveWindow(lpRect);
	}
	else {
		int top = 0,bottom = 0,leftsps=0,sps2=0;

		CRect rect;
		pWnd = GetDlgItem(IDC_BTN_NEWPRJ);
		if (pWnd) {
			pWnd->GetWindowRect(rect);
			top = rect.top;
			leftsps = rect.left - lpRect.left;
		}

		if (::IsWindowEnabled(this->m_hWnd)) {
			if ((m_lstRegTest.m_hWnd != NULL) && (m_lstRegTest.IsWindowEnabled())) {
				m_lstRegTest.GetWindowRect(rect);
				rect.bottom = rect.top + lpRect.Height() - 60;		// 登録リスト下端の算出
				bottom = rect.bottom;								// bottom=登録リスト下端
				ScreenToClient(rect);
				m_lstRegTest.MoveWindow(rect);
			}
		}

		if (((m_pReginWnd != NULL) && (m_pReginWnd->IsWindowEnabled())) && ((m_pMetalWnd != NULL) && (m_pMetalWnd->IsWindowEnabled())) && ((m_pResultWnd != NULL) && (m_pResultWnd->IsWindowEnabled()))) {
			CRect rect2,rect3, rect4;
			int sps;
			m_pReginWnd->GetWindowRect(rect);				// 樹脂面ウインド
			m_pMetalWnd->GetWindowRect(rect2);				// 金属面ウインド
			m_pResultWnd->GetWindowRect(rect3);				// 結果ウインド

			// 樹脂面ウインドと金属面ウインド間の縦方向のスペースを取得
			sps = rect2.top - rect.bottom;
			int hight = ((bottom - top) - sps*2) / 3;		// スキャン表示ウインドの高さを算出
			rect.bottom = rect.top + hight;					// 樹脂面ウインドの下端算出
			rect2.top		= rect.bottom	+ sps;			// 金属面ウインドの上端算出
			rect2.bottom	= rect2.top		+ hight;		// 金属面ウインドの下端算出
			rect3.top		= rect2.bottom	+ sps;			// 結果ウインドの上端算出
			rect3.bottom	= rect3.top		+ hight;		// 結果ウインドの下端算出

			int w = 0, movew = 0, moveh = 0;
			if ((m_pGraphWnd != NULL) && (m_pGraphWnd->IsWindowEnabled())) {
				m_pGraphWnd->GetWindowRect(rect4);
				movew = rect4.left;
				sps2 = rect4.left - rect.right;				// スキャンウインドとグラフウインド間の水平幅取得
				w = rect4.Width();
				rect4.right = lpRect.right - leftsps;
				rect4.left = rect4.right - w;
				movew = rect4.left - movew;
			}

			CRect rect5;
			m_tabOperation.GetWindowRect(rect5);
			rect5.right = lpRect.right - leftsps;
			rect5.left = rect5.right - w;

			CRect rect6;
			m_tabPropaty.GetWindowRect(rect6);
			rect6.right = lpRect.right - leftsps;
			rect6.left = rect6.right - w;
			moveh = rect3.bottom - rect6.bottom;
//			int hsps = rect5.top - rect4.bottom;
			rect4.bottom += moveh;


			rect.right = rect4.left - sps2;
			ScreenToClient(rect);
			m_pReginWnd->MoveWindow(rect);
			rect2.right = rect4.left - sps2;
			ScreenToClient(rect2);
			m_pMetalWnd->MoveWindow(rect2);
			rect3.right = rect4.left - sps2;
			ScreenToClient(rect3);
			m_pResultWnd->MoveWindow(rect3);

			ScreenToClient(rect4);
			m_pGraphWnd->MoveWindow(rect4);

			ScreenToClient(rect5);
			m_tabOperation.MoveWindow(rect5);

			ScreenToClient(rect6);
			m_tabPropaty.MoveWindow(rect6);

			MoveItem(&m_tabOperation, 0, moveh);
			MoveItem(&m_tabPropaty, 0, moveh);

			MoveItem(&m_stcOperationTabPageClient, movew, moveh);
			MoveItem(&m_stcPropTabPageClient, movew, moveh);

			MoveItem(&m_btnPropTabCancel, movew, moveh);
			MoveItem(&m_btnPropTabOK, movew, moveh);

			FitItem();
		}
	}
}

void CWeldEvaluationView::MoveItem(CWnd *pWnd, int movex, int movey)
{
	CRect wrct;
	pWnd->GetWindowRect(wrct);
	wrct.left	+= movex;
	wrct.right	+= movex;
	wrct.top	+= movey;
	wrct.bottom += movey;
	ScreenToClient(wrct);
	pWnd->MoveWindow(wrct);
}


/// <summary>
/// ウインド破棄時処理
/// </summary>
void CWeldEvaluationView::OnDestroy()
{
	CFormView::OnDestroy();
	if (!AfxGetMainWnd()->IsZoomed() && !AfxGetMainWnd()->IsIconic()) {
		CRect rect;
		GetWindowRect(rect);
		CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)GetDocument();
		pDoc->SetWindRect(rect);
	}
	
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

	OnProjectOpen();

#if 0
	CString str = m_lstRegTest.GetItemText(pNMItemActivate->iItem,0);
	if (str.IsEmpty()) {
		return;
	}

	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)GetDocument();

	if (pDoc->IsWorkProjectUpdated()) {
		CString msg;
		if (!msg.LoadString(IDM_PRJREGIST_EXISTUPDATE)) {
			msg = _T("データが更新されています。\n更新されているデータを破棄しますか?");
		}
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
		if (!AppName.LoadString(AFX_IDS_APP_TITLE)) {
			AppName = _T("WeldEvaluation");
		}
		msg.Format(_T("%s - %s"), (LPCWSTR)str, (LPCWSTR)AppName);
		GetParentFrame()->SetWindowText(msg);
		pDoc->SetWorkProjectUpdteStatus(false);
	} else {
		CString msg,fmt;
		if (!fmt.LoadString(IDM_ERR_NOTOPEN_REGTEST)) {
			fmt = _T("登録済み試験 [%s] を開くことができませんでした。");
		}
		msg.Format(fmt, (LPCWSTR)str);
		AfxMessageBox(msg,MB_OK|MB_ICONSTOP);
	}
#endif
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
#if false	// 2022.01.18 S.Kaneko
		if (m_SelOprtPageId == 3) {
			if (m_OprtAnalize.IsSettingAnalizeArea()) {
				CString msg;
				if (!msg.LoadString(IDM_ERR_ENDANALIZEAREASETTING)) {
					msg = _T("解析領域の設定を終了してください。");
				}
				AfxMessageBox(msg, MB_OK | MB_ICONSTOP);
				m_tabOperation.SetCurSel(m_SelOprtPageId);
				return;
			}
		}
#endif
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
			if (!msg.LoadString(IDM_PROPUPDATE)) {
				msg = _T("プロパティが変更されています。");
			}
			if (!sub.LoadStringW(IDM_CHECKOFUPDATE)) {
				sub = _T("更新しますか？");
			}
			msg += _T("\n") + sub;
			if (AfxMessageBox(msg,MB_YESNO| MB_ICONWARNING |MB_DEFBUTTON1) == IDYES) {
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
		m_PropResinPage.UpdateStatus();
		
	}
	break;
	case	1:	// 金属面
	{
		pDoc->SetWorkProjectUpdteStatus(true);
		m_PropMetalPage.UpdateStatus();
	}
	break;
	case	2:	// 結果
	{
		pDoc->SetWorkProjectUpdteStatus(true);
		m_PropResultPage.UpdateStatus();
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
/// プロパティ更新情報の設定
/// </summary>
/// <param name="wparam"> 更新フラグ</param>
/// <param name="lparam"> ページID</param>
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

/// <summary>
/// 更新の有無確認
/// </summary>
/// <param name="propatyID">プロパティタブID</param>
/// <returns>更新されている場合はtrue、失敗場合はfalseを返す</returns>
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

/// <summary>
/// オペレーションタブの有効化
/// </summary>
/// <param name="bActive"> 有効フラグ</param>
void CWeldEvaluationView::EnableOperation(bool bActive)
{
	m_OprtInitialize.ItemActive(bActive);
	m_OprtSetting.ItemActive(bActive);
	m_OprtScan.ItemActive(bActive);
	m_OprtAnalize.ItemActive(bActive);
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
	EnableOperation(false);

	int ScanID = (int)wparam;
	int *Result = (int *)lparam;
	int type = m_OprtAnalize.GetAnalizeType(ScanID);

	m_OprtAnalize.SetDisplayMode(ScanID, CWeldEvaluationDoc::DisplayModeScan);
	switch (ScanID) {
	case	CWeldEvaluationDoc::eResinSurface:		// 樹脂
		m_PropResinPage.ViewJointRatio(type, ScanID, -1);
		break;
	case	CWeldEvaluationDoc::eMetalSurface:		// 金属
		m_PropMetalPage.ViewJointRatio(type, ScanID, -1);
		break;
	case	CWeldEvaluationDoc::eJoiningResult:		// 接合結果
		m_PropResultPage.ViewJointRatio(type, ScanID, -1);
		break;
	}

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
					m_pMetalWnd->Invalidate();
				}
				break;
			case	CWeldEvaluationDoc::eJoiningResult:
				DisplayMode = pDoc->GetDisplayMode(CWeldEvaluationDoc::eJoiningResult);
				if (DisplayMode == CWeldEvaluationDoc::DisplayModeScan) {
					ViewChangeRequest(CWeldEvaluationDoc::eJoiningResult, DisplayMode, true);
					m_pResultWnd->Invalidate();
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
	EnableOperation(true);
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

	bool			bResult = true;
	CString			fname				= _T("");
	CString			SnapscanFile		= _T("");
	CString			WBFileName			= _T("");
	CString			registedFolde		= _T("");
	CString			spectralFilePath	= _T("");
	CScanDataIO		scn;
	float *			p = nullptr;			// 線形補間結果格納用
	vector<CPoint>	vOrign, vTrans;			// 線形補間用パラメータ
	double *		pWavelength			= nullptr;
//	CubeFloat *		cube				= nullptr;
	CubeFloat *		cube_corrected		= nullptr;
	CubeFloat *		reference_corrected = nullptr;
	float ***		dst					= nullptr;
	float ***		outData				= nullptr;
	int				outH = 0, outW = 0;
	int				dstW = 0, dstH = 0;

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

	// カメラ設定
	int offset = pDoc->NumberOfOverridePixel();
	int band = pDoc->NumberOfBand();
	int width = pDoc->GetShootingWidth();
	int height = pDoc->GetShootingHeight();
	double speed = pDoc->GetIntegrationTimeMs();
	if ((band <= 0) || (width <= 0) || (height <= 0)) {
		CString msg;
		msg.Format(_T("ScanImage():パラメータが異常です。band=%d, widht=%d,height=%d"), band, width, height);
		logOut(CString(__FILE__), __LINE__,msg );
		return false;
	}

	CCameraIO cam(width, height, band);

	CDeviceIO device;
	int ID = device.Init(deviceName);
	if (ID < 0) {
		logOut(CString(__FILE__), __LINE__,_T("ScanImage():デバイスの初期化に失敗した"));
		return false;
	}
	// キャンセルのチェック
	if (!pStatus->m_Valid) {
		bResult = true;
		goto SCanFinalize;
	}
	if (!device.ToHome(ID)) {
		logOut(CString(__FILE__), __LINE__, _T("ScanImage():ホームポジションへの移動に失敗した"));
		return false;
	}
	// キャンセルのチェック
	if (!pStatus->m_Valid) {
		bResult = true;
		goto SCanFinalize;
	}


	SnapscanFile = pDoc->getSnapscanFile();
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
	// キャンセルのチェック
	if (!pStatus->m_Valid) {
		bResult = true;
		goto SCanFinalize;
	}
	if (!cam.setIntegrationTime(speed)) {
		CString msg;
		msg.Format(_T("ScanImage():IntegrationTimeの設定に失敗しました[%lf]"), speed);
		logOut(CString(__FILE__), __LINE__, msg);
		return false;
	}
	// キャンセルのチェック
	if (!pStatus->m_Valid) {
		bResult = true;
		goto SCanFinalize;
	}

	registedFolde = pDoc->GetRegistedFolder();
	WBFileName = pDoc->GetWBFileName();
	reference_corrected = new CubeFloat();
	if (!cam.LoadReference(*reference_corrected, registedFolde, WBFileName)) {
		logOut(CString(__FILE__), __LINE__, _T("ScanImage():ホワイトバランスデータのロードに失敗しました"));
		return false;
	}

	// 一時ファイル保存先パス設定
	spectralFilePath = pDoc->GetTmpFolderPath();
	if (!CFileUtil::fileExists(spectralFilePath)) {
		if (!CreateDirectory(spectralFilePath, NULL)) {
			spectralFilePath = pDoc->GetRegistedFolder();
		}
	}

	// キャンセルのチェック
	if (!pStatus->m_Valid) {
		bResult = true;
		goto SCanFinalize;
	}
	if (!cam.StartScan()) {
		logOut(CString(__FILE__), __LINE__, _T("ScanImage():スキャンの開始に失敗しました"));
		return false;
	}
	// キャンセルのチェック
	if (!pStatus->m_Valid) {
		cam.StopScan();
		bResult = true;
		goto SCanFinalize;
	}

	cube_corrected = new CubeFloat();
	*cube_corrected = { 0 };

	double hscale = pDoc->GetHorizontalScale();
	double vscale = pDoc->GetVerticalScale();
	// 変換後のデータは90度回転させるので、縦横が入れ替わる
	dstW = (int)(height * vscale);
	dstH = (int)(width * hscale);
	if ((dstW <= 0) || (dstH <= 0)) {
		cam.StopScan();
		bResult = false;
		goto SCanFinalize;
	}

	//////////////////////////////////////////////////////////////////////////////
//	cube = new CubeFloat();
	int DivisionNumber = pDoc->GetDivisionNumber();
	int ScanStartID = pDoc->GetScanStartID();
	int ScanEndID = pDoc->GetScanEndID();

	if ((ScanStartID < 0) || (ScanEndID < ScanStartID) || (ScanEndID >= DivisionNumber)) {
		CString msg;
		msg.Format(_T("ScanImage():分割数(%d)、スキャン開始ID(%d)、スキャン終了ID(%d)のいずれかに不正な値が設定されています"), DivisionNumber, ScanStartID, ScanEndID);
		logOut(CString(__FILE__), __LINE__, msg);

		cam.StopScan();
		bResult = false;
		goto SCanFinalize;
	}

	outH = dstH;
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
	if (outData != NULL) {
		for (int b = 0; b < band; b++) {
			outData[b] = (float**)malloc(sizeof(float **)*dstH);
			if (outData[b]) {
				for (int h = 0; h < dstH; h++) {
					outData[b][h] = (float*)malloc(sizeof(float *)*outW);
					if (outData[b][h]) {
						ZeroMemory(outData[b][h], sizeof(float *)*outW);
					}
					else {
						bResult = false;
						goto SCanFinalize;
					}
				}
			}
			else {
				bResult = false;
				goto SCanFinalize;
			}
		}
	} else {
		return false;
	}

	bool bHomography = true;		// 射影変換可否フラグ
	double aParam[8];				// 射影変換パラメータ
	p = new float[band];		// 線形補間結果格納用
	if (!pDoc->GetHomographyPoint(vOrign, vTrans)) {
		bHomography = false;
	}
	else {
		scn.Calc_ProjectionParam(vOrign, vTrans, aParam);
	}

	dst = new float**[band];
	for (int b = 0; b < band; b++) {
		dst[b] = new float*[height];
		for (int y = 0; y < height; y++) {
			dst[b][y] = new float[width];
			ZeroMemory(dst[b][y], sizeof(float)*width);
		}
	}

	if (ScanStartID > 0) {
		// スキャン開始IDが0で無い場合は結合位置を調整
		jointPos = (dstW - offset) * ScanStartID;
	}

//	for (int pos = 0; pos < DivisionNumber; pos++) {
	for (int pos = ScanStartID; pos <= ScanEndID; pos++) {
		// キャンセルのチェック
		if (!pStatus->m_Valid) {
			cam.StopScan();
			bResult = true;
			goto SCanFinalize;
		}

		if (!device.Move(ID, pos)) {
			break;
		}
		Sleep(100);
		CString buff;
		buff.Format(_T("Scaning : %d/%d(%d %%)"), pos+1, DivisionNumber, (int)((pos+1) * 100 / DivisionNumber));
		pStatus->UpdateStatus(buff);

		// キャンセルのチェック
		if (!pStatus->m_Valid) {
			cam.StopScan();
			bResult = true;
			goto SCanFinalize;
		}
		/////////////////////////////////////////////////////////
		// スキャン実施
		/////////////////////////////////////////////////////////
		fname.Format(_T("normal%02d"), pos+1);
		*cube_corrected = { 0 };
		if (!cam.AcquireSpectralCube(spectralFilePath,fname, *reference_corrected, *cube_corrected)) {
			bResult = false;
			break;
		}
		// キャンセルのチェック
		if (!pStatus->m_Valid) {
			cam.StopScan();
			commonDeallocateCube(cube_corrected);
			bResult = true;
			goto SCanFinalize;
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

		float ***pTmp = nullptr;
		scn.ScanDataConvert(width, height, band, (float ***)cube_corrected->ppp_data, hscale, vscale, dstW, dstH, pTmp,false);

		bool bBicubic = true; //線形補間使用フラグ
		if (bHomography) {	// 射影変換
			for (int h = 0; h < dstH; h++) {
				for (int w = 0; w < dstW; w++) {
					double sx, sy;
					scn.ProjectionInvPos(w, h, aParam, sx, sy);
					if (sx < 0.0) {
						sx = 0.0;
					}
					else if (sx >= dstW) {
						sx = (double)((__int64)dstW - 1);
					}
					if (sy < 0.0) {
						sy = 0.0;
					}
					else if (sy >= dstH) {
						sy = (double)((__int64)dstH - 1);
					}
					// @note #要検討
					// ScanStartID==0でない場合、ScanStartIDのブロックは線形補間は不要か？
					if (bBicubic) {
						scn.bicubic(pTmp, dstW, dstH, band, (float)sx, (float)sy, p);
						for (int b = 0; b < band; b++) {
							dst[b][h][w] = p[b];
						}
					}
					else {
						for (int b = 0; b < band; b++) {
							dst[b][h][w] = pTmp[b][(int)sy][(int)sx];
						}
					}
				}
			}
		}

		if (pTmp) {
			for (int b = 0; b < band; b++) {
				for (int h = 0; h < dstH; h++) {
					if (pTmp[b][h]) {
						delete[] pTmp[b][h];
						pTmp[b][h] = nullptr;
					}
				}
				delete[] pTmp[b];
				pTmp[b] = nullptr;
			}
			delete[] pTmp;
			pTmp = nullptr;
		}

		// キャンセルのチェック
		if (!pStatus->m_Valid) {
			cam.StopScan();
			commonDeallocateCube(cube_corrected);
			bResult = true;
			goto SCanFinalize;
		}

		/////////////////////////////////////////////////////////////////
		// スキャンデータ結合処理
		if (outH < dstH) {
			float **pptmp = nullptr;
			for (int b = 0; b < band; b++) {
				pptmp = (float**)realloc(outData[b],(sizeof(float **)*dstH));
				if (pptmp) {
					outData[b] = pptmp;
					for (int h = outH; h < dstH; h++) {
						outData[b][h] = (float*)malloc(sizeof(float *)*outW);
						if (outData[b][h]) {
							ZeroMemory(outData[b][h], sizeof(float *)*outW);
						}
					}
				}
				else {
					cam.StopScan();
					commonDeallocateCube(cube_corrected);
					bResult = false;
					goto SCanFinalize;
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
				float *ptmp = nullptr;
				for (int b = 0; b < band; b++) {
					for (int h = 0; h < outH; h++) {
						ptmp = (float *)realloc(outData[b][h], (__int64)jointPos + dstW);
						if (ptmp) {
							outData[b][h] = ptmp;
						}
						else {
							cam.StopScan();
							commonDeallocateCube(cube_corrected);
							bResult = false;
							goto SCanFinalize;
						}
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
					memcpy(&outData[b][h][jointPos + offset], &dst[b][h][offset], sizeof(float)*((__int64)dstW - offset));
				}
			}
			jointPos += dstW - offset;
		}
		commonDeallocateCube(cube_corrected);

#if 0
		if (ScanID == CWeldEvaluationDoc::eMetalSurface) {
			if (!scn.FlipUpsideDown(outW, outH, band, outData)) {
				bResult = false;
			}
		}
#endif
	}

SCanFinalize:
	if (dst) {
		for (int b = 0; b < band; b++) {
			for (int h = 0; h < height; h++) {
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

	if (p) {
		delete[] p;
		p = nullptr;
	}

	if (cube_corrected) {
		delete  cube_corrected;
		cube_corrected = nullptr;
	}
	cam.Close();
	device.Close(ID);
	if (reference_corrected) {
		commonDeallocateCube(reference_corrected);
		if (reference_corrected) {
			delete reference_corrected;
		}
	}

	pStatus->UpdateStatus(_T("Closing..."));
	///////////////////////////////////////////////////////////////////////////////////
	// データの保存
	if (bResult && pStatus->m_Valid) {
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
			buf.Format(_T("data type = %d"), (int)sizeof(float));
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

			CString tmp;
			buf.Format(_T("%.8lf"), pWavelength[0]);
			for (int b = 1; b < band; b++) {
				tmp.Format(_T("%s,%.8lf"), (LPCWSTR)buf, pWavelength[b]);
				buf = tmp;
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
			if (outData[b]) {
				for (int h = 0; h < dstH; h++) {
					if (outData[b][h]) {
						free(outData[b][h]);
						outData[b][h] = (float*)nullptr;
					}
				}
				free(outData[b]);
				outData[b] = nullptr;
			}
		}
		free(outData);
		outData = nullptr;
	}

	return bResult;
}

/// <summary>
/// データ反転の要求
/// </summary>
/// <param name="wparam"> スキャンID</param>
/// <param name="lparam"> 結果</param>
/// <returns>成功した場合は0、失敗した場合は-1を返す</returns>
LRESULT CWeldEvaluationView::OnInversRequest(WPARAM wparam, LPARAM lparam)
{
	int ScanID = (int)wparam;
	int *Result = (int *)lparam;
	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)GetDocument();

//	int type = m_OprtAnalize.GetAnalizeType(ScanID);
	OnInversStatus(PROGRESS_INIT,100);

	m_OprtAnalize.SetDisplayMode(ScanID, CWeldEvaluationDoc::DisplayModeScan);

	if (!pDoc->ExistScanFile(ScanID)) {
		OnInversStatus(PROGRESS_END, NULL);
		*Result = -1;
		return -1;
	}

	if (!pDoc->InversScanData(ScanID)) {
		OnInversStatus(PROGRESS_END, NULL);
		*Result = -1;
		return -1;
	}
	OnInversStatus(PROGRESS_UPDATE, 100);

	ViewChangeRequest(ScanID, CWeldEvaluationDoc::DisplayModeScan, true);
	m_pReginWnd->Invalidate();

	switch (ScanID) {
	case	CWeldEvaluationDoc::eResinSurface:
		m_pReginWnd->Invalidate();
		break;
	case	CWeldEvaluationDoc::eMetalSurface:
		m_pMetalWnd->Invalidate();
		break;
	case	CWeldEvaluationDoc::eJoiningResult:
		m_pResultWnd->Invalidate();
		break;
	}
	pDoc->SetWorkProjectUpdteStatus(true);

#if 1
	* Result = 0;
	if (pDoc->IsInversAnalizeData(ScanID)) {
		CString str = _T("解析結果の反転処理");
		OnInversStatus(PROGRESS_SET_TITLE, (LPARAM)((LPCTSTR)str));
		OnInversStatus(PROGRESS_UPDATE, 0);
		if (!pDoc->InversAnalizeData(ScanID)) {
			// 解析データの反転に失敗
			*Result = 1;
		}
		else {
			m_OprtAnalize.SetChangedAnalizeFlag(true);
		}
	}
	else {
		// 反転できない解析データ
		*Result = 1;
	}
#else
	*Result = 1;
#endif
	OnInversStatus(PROGRESS_END, NULL);
	// 解析データが反転できない場合は解析データを削除
	if (*Result == 1) {
		pDoc->DeleteAnalizeData(ScanID);
	}

	return 0;
}

/// <summary>
/// データ反転状態通知
/// </summary>
/// <param name="wparam">メッセージタイプ</param>
/// <param name="lparam">状態の値</param>
/// <returns>成功した場合は0、失敗した場合はそれ以外を返す</returns>
LRESULT CWeldEvaluationView::OnInversStatus(WPARAM wparam, LPARAM lparam)
{
	// メッセージがキューにたまってしまうのでメッセージポンプして処理を通す
	MSG msg;
	while (PeekMessage(&msg, m_hWnd, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	UINT type = (UINT)wparam;
	switch (type) {
	case	PROGRESS_SET_TITLE:
		{
			if (m_pProgress != nullptr) {
				CString str = (CString)((LPCTSTR)lparam);
				m_pProgress->SetWindowTextW(str);
			}
		}
		break;
	case	PROGRESS_INIT:		// 処理開始
		{
			UINT value = (UINT)lparam;
			if (m_pProgress == nullptr) {
				m_pProgress = new CProgressDlg(false,this);
			}
			CString str;
			if (!str.LoadString(IDM_INVERS_SCANDATA_TITLE)) {
				str = _T("スキャンデータ反転処理");
			}
			m_pProgress->setTitle(str);
			if (!str.LoadString(IDM_INVERS_INIT)) {
				str = _T("反転処理準備中・・・");
			}
			m_pProgress->setlabel(str);

			m_pProgress->setRange(0, value);
			m_pProgress->setPosition(0);
			m_pProgress->Create(CProgressDlg::IDD, this);
			m_pProgress->ShowWindow(SW_SHOW);
			m_pProgress->UpdateWindow();
		}
		break;
	case	PROGRESS_UPDATE:		// 処理進行状況
		{
			UINT value = (UINT)lparam;
			if (m_pProgress != nullptr) {
				int min, max;
				m_pProgress->getRange(min, max);
				double per = (double)value / (double)((__int64)max - min);
				CString str, fmt;
				if (!fmt.LoadString(IDM_INVERSING)) {
					fmt = _T("変換中：%d/%d (%.2lf[%%])");
				}
				str.Format(fmt, value, (max - min), per * 100);
				m_pProgress->setlabel(str);
				m_pProgress->setPosition(value);
				m_pProgress->UpdateWindow();
			}
		}
		break;
	case	PROGRESS_STOP:		// 処理停止リクエスト
		{
			CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)GetDocument();
			pDoc->CalcResultStopRequest();
			break;
		}
	case	PROGRESS_END:		// 処理終了
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

/// <summary>
/// ホワイトバランススキャンの実施有無判定
/// </summary>
/// <param name="wparam"> 未使用</param>
/// <param name="lparam"> 未使用</param>
/// <returns>実施する場合は0、しない場合は1を返す</returns>
LRESULT CWeldEvaluationView::OnWBScanExistCheck(WPARAM wparam, LPARAM lparam)
{
	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)GetDocument();
	CString registedFolde = pDoc->GetRegistedFolder();
	CString WBFileName = pDoc->GetWBFileName();
	CString fname = WBFileName + _T(".hdr");
	CString chkPath = CFileUtil::FilePathCombine(registedFolde, fname);
	if (CFileUtil::fileExists(chkPath)) {
		CString msg;
		if (!msg.LoadString(IDM_EXISTWBSCANFILE)) {
			msg = _T("ホワイトバランスデータは取得済みです。\n取得しなおしますか？");
		}
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
		if (!pStatus->m_Valid) {  // キャンセルボタンが押さた
			goto WBScanRequestCancel;
		}

		if (device.ToHome(ID)) {
			if (!pStatus->m_Valid) {  // キャンセルボタンが押さた
				goto WBScanRequestCancel;
			}
			if (device.Move(ID, 1)) {
				if (!pStatus->m_Valid) {  // キャンセルボタンが押さた
					goto WBScanRequestCancel;
				}

				if (!buff.LoadString(IDS_PREPANING)) {
					buff = _T("準備中...");
				}
				pStatus->UpdateStatus(buff);
				/////////////////////////////////////////////////////////
				// ホワイトバランスデータの撮影
				/////////////////////////////////////////////////////////
				int band = pDoc->NumberOfBand();
				int width = pDoc->GetShootingWidth();
				int height = pDoc->GetShootingHeight();
				double speed = pDoc->GetIntegrationTimeMs();
				CCameraIO cam(width, height,band);
				if (!pStatus->m_Valid) {  // キャンセルボタンが押さた
					goto WBScanRequestCancel;
				}
				CString SnapscanFile = pDoc->getSnapscanFile();
				if (CFileUtil::fileExists(SnapscanFile)) {
					bool dummyApi = pDoc->IsCameraDummyApi();
					if (cam.Open(SnapscanFile, dummyApi)) {
						if (!buff.LoadString(IDM_SCANNING)) {
							buff = _T("スキャン中...");
						}
						if (!pStatus->m_Valid) {  // キャンセルボタンが押さた
							goto WBScanRequestCancel;
						}
						pStatus->UpdateStatus(buff);
						if (cam.setIntegrationTime(speed)) {
							if (cam.StartScan()) {
								if (!pStatus->m_Valid) {  // キャンセルボタンが押さた
									cam.Close();
									goto WBScanRequestCancel;
								}
								if (cam.AcquireReference(registedFolde,WBFileName)) {
									if (!pStatus->m_Valid) {  // キャンセルボタンが押さた
										pDoc->DeleteWBFile(WBFileName);
										cam.Close();
										goto WBScanRequestCancel;
									}
									*Result = 0;
									if (!buff.LoadString(IDM_SCAN_SUCCESS)) {
										buff = _T("スキャン処理が正常に終了しました。");
									}
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
		}
	}
	return 0;
WBScanRequestCancel:
	if (!buff.LoadString(IDM_SCAN_CANCELD)) {
		buff = _T("スキャン処理がキャンセルされました。");
	}
	AfxMessageBox(buff, MB_OK | MB_ICONWARNING);
	*Result = 1;
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
		if (!msg.LoadString(IDM_ERR_PROJECT_SAVE)) {
			msg = _T("登録に失敗しました。");
		}
		AfxMessageBox(msg,MB_OK|MB_ICONSTOP);
		iResult = -1;
	} else {
		CString AppName, Title,sub;
		sub = pDoc->GetTestName();
		if (!AppName.LoadString(AFX_IDS_APP_TITLE)) {
			AppName = _T("WeldEvaluation");
		}
		Title.Format(_T("%s - %s"), (LPCWSTR)sub, (LPCWSTR)AppName);
		GetParentFrame()->SetWindowText(Title);

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
	if (buff == nullptr) {
		return iResult;
	}
	int size;
	while (1) {
		size = GetCurrentDirectoryW(buffSize, buff);
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
	if (!label.LoadString(IDS_LBL_FOLDERSELECT)) {
		label = _T("フォルダ選択");
	}

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

/// <summary>
/// 表示変更リクエスト
/// </summary>
/// <param name="ScanID">表示ターゲットID</param>
/// <param name="DisplayMode">ディスプレイモード(0:スキャンデータ、1:解析データ)</param>
/// <param name="renew">再描画有無フラグ(true：再描画、false：再描画せず)</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CWeldEvaluationView::ViewChangeRequest(int ScanID, int DisplayMode, bool renew/*=false*/)
{
	CWaitCursor wcursol;
	bool bResult = true;
	int NumberOfClass = 0;
	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)GetDocument();
	int AnalyzeMethod = m_OprtAnalize.GetAnalizeType(ScanID);
	int BeforAnalyzeMethod;
	int BeforNumberOfClass = 0;
	if (AnalyzeMethod == CWeldEvaluationDoc::AnalizeTypeHiClustering) {
		BeforAnalyzeMethod = CWeldEvaluationDoc::AnalizeTypeKMeans;
	}
	else {
		BeforAnalyzeMethod = CWeldEvaluationDoc::AnalizeTypeHiClustering;
	}

	CPropTabPageParameter *pPropPage = nullptr;
	CImageWind *pImageWnd = nullptr;
	switch (ScanID) {
	case	CWeldEvaluationDoc::eResinSurface:		// 樹脂
		pImageWnd = m_pReginWnd;
		pPropPage = &m_PropResinPage;
		NumberOfClass = pDoc->ResinGetNumberOfClass(AnalyzeMethod);
		BeforNumberOfClass = pDoc->ResinGetNumberOfClass(BeforAnalyzeMethod);
		break;
	case	CWeldEvaluationDoc::eMetalSurface:		// 金属
		pImageWnd = m_pMetalWnd;
		pPropPage = &m_PropMetalPage;
		NumberOfClass = pDoc->MetalGetNumberOfClass(AnalyzeMethod);
		BeforNumberOfClass = pDoc->MetalGetNumberOfClass(BeforAnalyzeMethod);
		break;
	case	CWeldEvaluationDoc::eJoiningResult:		// 接合結果
		pImageWnd = m_pResultWnd;
		pPropPage = &m_PropResultPage;
		NumberOfClass = pDoc->ResultGetNumberOfClass(AnalyzeMethod);
		BeforNumberOfClass = pDoc->ResultGetNumberOfClass(BeforAnalyzeMethod);
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
		bool bRenew = renew;
		if (m_OprtAnalize.IsChangedAnalize()) {
			int newNCluss = pPropPage->GetNumbetOfClass();
			if (BeforNumberOfClass != newNCluss) {
				CString msg;
				if (!msg.LoadString(IDM_CHANGED_NUMBEROFCLASS)) {
					msg = _T("クラス数が更新されています。\n更新を破棄しますか？");
				}
				if (AfxMessageBox(msg, MB_YESNO | MB_ICONWARNING | MB_DEFBUTTON1) == IDNO) {
					m_OprtAnalize.ChangeAnalizeType(ScanID);
					return true;
				}
			}
			if (NumberOfClass != newNCluss) {
				pPropPage->UpdateCmbJoinratioTargetLabel(true, NumberOfClass);
			}
			bRenew = true;
		}
		m_OprtAnalize.SetChangedAnalizeFlag(false);
		int type = m_OprtAnalize.GetAnalizeType(ScanID);
		if (pDoc->IsExistClassificationResultFile(ScanID, type)) {
			CImage *pImg = pImageWnd->GetImage();
			if (pDoc->LoadClassificationResultImage(ScanID, type, *pImg, bRenew)) {
				pImageWnd->Draw();
			}
			else {
				pImageWnd->Erase();
			}
		}
		else {
			pImageWnd->Erase(false);
		}
		pPropPage->LoadParamater(ScanID);
		pPropPage->ViewJointRatio(type, ScanID, -1);
		pPropPage->UpdateStatus();
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

/// <summary>
/// 表示削除
/// </summary>
/// <param name="wparam">表示ターゲットID</param>
/// <param name="lparam">未使用</param>
/// <returns>成功場合は0、失敗場合は-1を返す</returns>
LRESULT CWeldEvaluationView::OnImageErace(WPARAM wparam, LPARAM lparam)
{
	int ScanID = (int)wparam;

//	CImageWind *pImageWnd = nullptr;
	switch (ScanID) {
	case	CWeldEvaluationDoc::eResinSurface:		// 樹脂
		m_pReginWnd->Erase();
		m_pMetalWnd->Reset();
		m_pResultWnd->Reset();
		break;
	case	CWeldEvaluationDoc::eMetalSurface:		// 金属
		m_pReginWnd->Reset();
		m_pMetalWnd->Erase();
		m_pResultWnd->Reset();
		break;
	case	CWeldEvaluationDoc::eJoiningResult:		// 接合結果
		m_pReginWnd->Reset();
		m_pMetalWnd->Reset();
		m_pResultWnd->Erase();
		break;
	default:
		return -1;
	}
	return 0;
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
	CWaitCursor wcursol;
	int targetID = (int)wparam;
	int AnalyzeMethod = (int)lparam;
	int iResult = 0;
	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)GetDocument();
	EnablePropaty(false);
	EnableOperation(false);

	CString ScanDataFilePath = pDoc->getScanDataFilePath(targetID);
	// 改正対象が存在するかをチェック
	if (!CFileUtil::fileExists(ScanDataFilePath)) {
		CString msg;
		if (!msg.LoadString(IDM_ERR_NOTEXIST_ANALIZEDATA)) {
			msg = _T("解析対象が存在しません");
		}
		AfxMessageBox(msg, MB_OK | MB_ICONSTOP);
		EnablePropaty(true);
		EnableOperation(true);
		return -1;
	}

	if (m_bUpdatePropaty) {
		bool bUpdate = false;
		if ((m_SelPropPageId == 0) && (targetID == CWeldEvaluationDoc::eResinSurface)) {
			bUpdate = true;
		} else if ((m_SelPropPageId == 1) && (targetID == CWeldEvaluationDoc::eMetalSurface)) {
			bUpdate = true;
		}
		else if ((m_SelPropPageId == 1) && (targetID == CWeldEvaluationDoc::eJoiningResult)) {
			bUpdate = true;
		}
		if (bUpdate) {
			CString msg;
			if (!msg.LoadString(IDM_PRJREGIST_EXISTUPDATE)) {
				msg = _T("データが更新されています。\n更新されているデータを破棄しますか?");
			}
			if (AfxMessageBox(msg, MB_YESNO | MB_ICONWARNING | MB_DEFBUTTON1) == IDNO) {
				EnablePropaty(true);
				EnableOperation(true);
				return -1;
			}
			OnBnClickedBtnPropCancel();
		}
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
		if (!msg.LoadString(IDM_ERR_NOT_ANALYZE)) {
			msg = _T("解析処理に失敗しました");
		}
		AfxMessageBox(msg,MB_OK|MB_ICONSTOP);
	}
	else {
		// 表示モードをスキャン
		ViewChangeRequest(targetID, CWeldEvaluationDoc::DisplayModeResult,true);
		m_pReginWnd->Invalidate();

		m_OprtAnalize.LoadParamater();

		// プロパティページを更新
		CDialogEx * pDlg = nullptr;
		switch (targetID) {
		case	CWeldEvaluationDoc::eResinSurface:		// 樹脂
			m_PropResinPage.LoadParamater(CWeldEvaluationDoc::eResinSurface);
			m_PropResinPage.UpdateStatus();
			pDlg = (CDialogEx *)&m_PropResinPage;
			break;
		case	CWeldEvaluationDoc::eMetalSurface:		// 金属
			m_PropMetalPage.LoadParamater(CWeldEvaluationDoc::eMetalSurface);
			m_PropMetalPage.UpdateStatus();
			pDlg = (CDialogEx *)&m_PropMetalPage;
			break;
		case	CWeldEvaluationDoc::eJoiningResult:		// 結合結果
			m_PropResultPage.LoadParamater(CWeldEvaluationDoc::eJoiningResult);
			m_PropResultPage.UpdateStatus();
			pDlg = (CDialogEx *)&m_PropResultPage;
			break;
		}
		// ワークのステータス更新
		pDoc->SetWorkProjectUpdteStatus(true);

		// プロパティタブを解析対象のページに切り替える
		if (pDlg != nullptr) {
			int iCnt = (int)m_PropTab.GetSize();
			for (int i = 0; i < iCnt; i++) {
				if (pDlg == m_PropTab.GetAt(i)) {
					WPARAM lwparam = WA_ACTIVE & 0xFFFF;
					LPARAM llparam = (LPARAM)pDlg->m_hWnd;
					m_PropTab.GetAt(i)->SendMessage(WM_ACTIVATE, lwparam, llparam);
					m_PropTab.GetAt(i)->ShowWindow(SW_SHOW);
					m_tabPropaty.SetCurSel(i);
				}
				else {
					m_PropTab.GetAt(i)->ShowWindow(SW_HIDE);
				}
			}
		}

		// 解析終了メッセージ
		CString	msg;
		if (!msg.LoadString(IDM_ANALYZE_SUCCESS)) {
			msg = _T("解析処理が正常終了しました");
		}
		AfxMessageBox(msg, MB_OK | MB_ICONINFORMATION);
	}
	EnablePropaty(true);
	EnableOperation(true);

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
	case	PROGRESS_INIT:		// 処理開始
		{
			if (m_pProgress == nullptr) {
				m_pProgress = new CProgressDlg(true,this);
			}
			CString str;
			if (!str.LoadString(IDM_RESULT_READ_TITLE)) {
				str = _T("結果ファイルの読み込");
			}
			m_pProgress->setTitle(str);
			if (!str.LoadString(IDM_RESULT_READ_INIT)) {
				str = _T("読み込み準備中・・・");
			}
			m_pProgress->setlabel(str);
			m_pProgress->setRange(0,value);
			m_pProgress->setPosition(0);
			m_pProgress->Create(CProgressDlg::IDD, this);
			m_pProgress->ShowWindow(SW_SHOW);
			m_pProgress->UpdateWindow();
		}
		break;
	case	PROGRESS_UPDATE:		// 処理進行状況
		{
			if (m_pProgress != nullptr) {
				int min,max;
				m_pProgress->getRange(min,max);
				double per = (double)value / (double)((__int64)max - min);
				CString str,fmt;
				if (!fmt.LoadString(IDM_RESULT_READING)) {
					fmt = _T("読み込み中：%d/%d (%.2lf[%%])");
				}
				str.Format(fmt,value, (max - min),per*100);
				m_pProgress->setlabel(str);
				m_pProgress->setPosition(value);
				m_pProgress->UpdateWindow();
			}
		}
		break;
	case	PROGRESS_STOP:		// 処理停止リクエスト
		{
			CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)GetDocument();
			pDoc->CalcResultStopRequest();
			break;
		}
	case	PROGRESS_END:		// 処理終了
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
		double min, max;
		if (pDoc->GetSpectrumRange(ScanID, min, max)) {
			CString maxlabel, milabel;
			milabel.Format(_T("%.1lfnm"), min);
			maxlabel.Format(_T("%.1lfnm"), max);
			m_pGraphWnd->SetXLabel(milabel, maxlabel);
		}
		else {
			m_pGraphWnd->SetXLabel(_T(""), _T(""));
		}
		m_pGraphWnd->SetVirticalRange(0.0, 1.2);
		m_pGraphWnd->SetYLabel(_T("0.0"), _T("1.0"));
		m_pGraphWnd->Draw(data, 0, false);
#ifdef _DEBUG
		{
			CPoint p(100, 100);
			CString txt;
			txt.Format(_T("(%4d,%4d)"),pos.x,pos.y);
			m_pGraphWnd->DrawTxt(p, txt);
		}
#endif
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
#if false	// 2022.01.18 S.Kaneko
	m_OprtAnalize.EnableSetAnalizeArea(false);
#endif
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
#if false	// 2022.01.18 S.Kaneko
	m_OprtAnalize.EnableSetAnalizeArea(true);
#endif
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
		bool bInv = false;
		if (pos[0].x > pos[1].x) {
			CPoint p = pos[0];
			pos[0] = pos[1];
			pos[1] = p;
			bInv = true;
		}
		int sz = pos[1].x - pos[0].x;
		data.resize(sz);
		double a = (double)((__int64)pos[1].y - pos[0].y) / (double)sz;
		double b = (double)((__int64)((__int64)pos[1].x * pos[0].y) - (__int64)pos[0].x * pos[1].y) / (double)sz;
		int x;
		if (bInv) {
			int blockid = sz - 1;
			for (int i = 0; i < sz; i++) {
				x = pos[0].x + i;
				point = CPoint(x, (int)(a*x + b));
				if (pDoc->GetSpectrumData(ScanID, point, data[(__int64)blockid-i])) {
				}
			}
		}
		else {
			for (int i = 0; i < sz; i++) {
				x = pos[0].x + i;
				point = CPoint(x, (int)(a*x + b));
				if (pDoc->GetSpectrumData(ScanID, point, data[i])) {
				}
			}
		}
	}
	else {
		// y軸基準
		bool bInv = false;
		if (pos[0].y > pos[1].y) {
			CPoint p = pos[0];
			pos[0] = pos[1];
			pos[1] = p;
			bInv = true;
		}
		int sz = pos[1].y - pos[0].y;
		data.resize(sz);
		double a = (double)((__int64)pos[1].x - pos[0].x) / (double)sz;
		double b = (double)((__int64)((__int64)pos[1].y * pos[0].x) - (__int64)pos[0].y * pos[1].x) / (double)sz;
		int y;
		if (bInv) {
			int blockid = sz - 1;
			for (int i = 0; i < sz; i++) {
				y = pos[0].y + i;
				point = CPoint((int)(a*y + b), y);
				if (pDoc->GetSpectrumData(ScanID, point, data[(__int64)blockid -i])) {
				}
			}
		}
		else {
			for (int i = 0; i < sz; i++) {
				y = pos[0].y + i;
				point = CPoint((int)(a*y + b), y);
				if (pDoc->GetSpectrumData(ScanID, point, data[i])) {
				}
			}
		}
	}

	double min, max;
	if (pDoc->GetSpectrumRange(ScanID, min, max)) {
		CString maxlabel, milabel;
		milabel.Format(_T("%.1lfnm"), min);
		maxlabel.Format(_T("%.1lfnm"), max);
		m_pSpectralDlg->SetXLabel(milabel, maxlabel);
	}
	else {
		m_pSpectralDlg->SetXLabel(_T(""), _T(""));
	}
	m_pSpectralDlg->SetYLabel(_T(""), _T(""));
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
	if (!AppName.LoadString(AFX_IDS_APP_TITLE)) {
		AppName = _T("WeldEvaluation");
	}
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
		if (!msg.LoadString(IDM_PRJREGIST_EXISTUPDATE)) {
			msg = _T("データが更新されています。\n更新されているデータを破棄しますか?");
		}
		if (AfxMessageBox(msg, MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON1) == IDNO) {
			return;
		}
	}

	pDoc->CloseProject();
	UnSellectProject();

	pDoc->OnNewDocument();
	CString msg,sub,AppName;
	if (!sub.LoadString(IDS_NEW)) {
		sub = _T("新規");
	}
	if (!AppName.LoadString(AFX_IDS_APP_TITLE)) {
		AppName = _T("WeldEvaluation");
	}
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
	}
	return bResult;
}

/// <summary>
/// クローズ処理
/// </summary>
void CWeldEvaluationView::OnClose()
{
	CFormView::OnClose();
}

/// <summary>
/// リスト項目の右ボタン押下時処理
/// </summary>
/// <param name="pNMHDR">NMITEMACTIVATEへのポインタ</param>
/// <param name="pResult">結果</param>
void CWeldEvaluationView::OnNMRClickLstRegistTest(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	if(pNMItemActivate->iItem == -1) {
		*pResult = 0;
		return;
	}

	CMenu *popupMenuP = NULL;
	CMenu cMenu;
	CPoint posMouse;
	int err = 0;

	if (!err) {
		if (!cMenu.LoadMenu(IDR_POPUP_MENU)) {
			err = 1;
		}
	}
	// サブメニューを取得
	if (!err) {
		if ((popupMenuP = cMenu.GetSubMenu(0)) == NULL) {
			err = 1;
		}
	}
	// メニューをポップアップ
	if (!err) {
		GetCursorPos(&posMouse);
		if (popupMenuP->TrackPopupMenu(TPM_LEFTBUTTON, posMouse.x, posMouse.y, this))
			err = 1;
	}	

	*pResult = 0;
}


/// <summary>
/// プロジェクトのオープン
/// </summary>
void CWeldEvaluationView::OnProjectOpen()
{
	POSITION pos = m_lstRegTest.GetFirstSelectedItemPosition();
	int iItem = m_lstRegTest.GetNextSelectedItem(pos);

	CString str = m_lstRegTest.GetItemText((int)iItem, 0);
	if (str.IsEmpty()) {
		return;
	}

	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)GetDocument();

	if (pDoc->IsWorkProjectUpdated()) {
		CString msg;
		if (!msg.LoadString(IDM_PRJREGIST_EXISTUPDATE)) {
			msg = _T("データが更新されています。\n更新されているデータを破棄しますか?");
		}
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
		pDoc->GetSpectralGraphPointPosition(holizontal, vertical);

		CString msg, AppName;
		if (!AppName.LoadString(AFX_IDS_APP_TITLE)) {
			AppName = _T("WeldEvaluation");
		}
		msg.Format(_T("%s - %s"), (LPCWSTR)str, (LPCWSTR)AppName);
		GetParentFrame()->SetWindowText(msg);
		pDoc->SetWorkProjectUpdteStatus(false);
	}
	else {
		CString msg, fmt;
		if (!fmt.LoadString(IDM_ERR_NOTOPEN_REGTEST)) {
			fmt = _T("登録済み試験 [%s] を開くことができませんでした。");
		}
		msg.Format(fmt, (LPCWSTR)str);
		AfxMessageBox(msg, MB_OK | MB_ICONSTOP);
	}
}

/// <summary>
/// プロジェクト削除
/// </summary>
void CWeldEvaluationView::OnProjectDelete()
{
	POSITION pos = m_lstRegTest.GetFirstSelectedItemPosition();
	int iItem = m_lstRegTest.GetNextSelectedItem(pos);
	CString str = m_lstRegTest.GetItemText((int)iItem,0);
	if (str.IsEmpty()) {
		return;
	}
	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)GetDocument();
	CString strTestName = pDoc->GetTestName();
	if (str.Compare(strTestName) == 0) {
		CString msg;
		if (!msg.LoadString(IDM_DELETE_ACTIVEPROJECT)) {
			msg = _T("編集中のデータです。\n削除しますか？");
		}
		if (AfxMessageBox(msg, MB_YESNO | MB_ICONWARNING | MB_DEFBUTTON1) == IDNO) {
			return;
		}
	}
	else {
		CString msg;
		if (!msg.LoadString(IDM_DELETE)) {
			msg = _T("削除してよろしいですか？ ");
		}
		if (AfxMessageBox(msg, MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON1) == IDNO) {
			return;
		}
	}

	if (pDoc->DeleteProject(str)) {
		m_lstRegTest.DeleteItem(iItem);
	}
}

/// <summary>
/// プロジェクトの更新
/// </summary>
void CWeldEvaluationView::OnFileSave()
{
	OnProjectResistRequest(NULL, NULL);
}

/// <summary>
/// プロジェクト新規作成
/// </summary>
void CWeldEvaluationView::OnProjectNew()
{
	OnBnClickedBtnNewprj();
}
