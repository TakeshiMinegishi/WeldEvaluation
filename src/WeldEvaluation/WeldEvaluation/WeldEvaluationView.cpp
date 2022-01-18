
// WeldEvaluationView.cpp : CWeldEvaluationView �N���X�̎���
//

#include "stdafx.h"
#include "FileUtil.h"
// SHARED_HANDLERS �́A�v���r���[�A�T���l�C���A����ь����t�B���^�[ �n���h���[���������Ă��� ATL �v���W�F�N�g�Œ�`�ł��A
// ���̃v���W�F�N�g�Ƃ̃h�L�������g �R�[�h�̋��L���\�ɂ��܂��B
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

// CWeldEvaluationView �R���X�g���N�V����/�f�X�g���N�V����

/// <summary>
/// �R���X�g���N�^
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
/// �f�X�g���N�^
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
/// �f�[�^�G�N�X�`�F���W����
/// </summary>
/// <param name="pDX">CDataExchange�I�u�W�F�N�g</param>
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
/// �E�C���h�쐬�O����
/// </summary>
/// <param name="cs">CREATESTRUCT�\����</param>
/// <returns>�쐬���p������ꍇ��TRUE�A����ȊO��FALSE��Ԃ�</returns>
BOOL CWeldEvaluationView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: ���̈ʒu�� CREATESTRUCT cs ���C������ Window �N���X�܂��̓X�^�C����
	//  �C�����Ă��������B

	return CFormView::PreCreateWindow(cs);
}

/// <summary>
/// ����������
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
	//	���X�g�R���g���[���̐ݒ�
	m_lstRegTest.DeleteAllItems();
	CString label;
	TCHAR szMsg [256];
	LoadString(GetModuleHandle(NULL),IDS_REGTEST_LABEL,szMsg,sizeof(szMsg)/sizeof(szMsg[0]) );
	m_lstRegTest.InsertColumn(0,szMsg);

	// �J�������𐮂���
	CRect rect;
	m_lstRegTest.GetClientRect(&rect);
	m_lstRegTest.SetColumnWidth(0, rect.Width());
	m_lstRegTest.ShowWindow(SW_SHOW);

	//�s�I�����[�h�ɂ��Ă���
	DWORD	dwStyle = ListView_GetExtendedListViewStyle(m_lstRegTest.m_hWnd);
	ListView_SetExtendedListViewStyle(m_lstRegTest.m_hWnd, dwStyle | LVS_EX_FULLROWSELECT);	
	UpdateRegistFolderList();

	CString strTitle;
	//////////////////////////////////////
	//	����p�l���^�u�̐ݒ�
	if (!strTitle.LoadString(IDS_OPRTAB_INIT)) {
		strTitle = _T("������");
	}
	m_tabOperation.InsertItem(0,strTitle);
	if (!strTitle.LoadString(IDS_OPRTAB_SETTING)) {
		strTitle = _T("�ݒ�");
	}
	m_tabOperation.InsertItem(1,strTitle);
	if (!strTitle.LoadString(IDS_OPRTAB_SCAN)) {
		strTitle = _T("�X�L����");
	}
	m_tabOperation.InsertItem(2,strTitle);
	if (!strTitle.LoadString(IDS_OPRTAB_ANARIZE)) {
		strTitle = _T("���");
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
	//	�v���p�e�B�^�u�̐ݒ�
	int propId = 0;
	if (!strTitle.LoadString(IDS_PROPTAB_RESIN)) {
		strTitle = _T("������");
	}
	m_tabPropaty.InsertItem(propId++,strTitle);
	if (!strTitle.LoadString(IDS_PROPTAB_METAL)) {
		strTitle = _T("������");
	}
	m_tabPropaty.InsertItem(propId++,strTitle);
	if (!strTitle.LoadString(IDS_PROPTAB_RESULT)) {
		strTitle = _T("�ڍ�����");
	}
	m_tabPropaty.InsertItem(propId++,strTitle);
	if (!strTitle.LoadString(IDS_PROPTAB_OVERALL)) {
		strTitle = _T("�S��");
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

	// �O���t�E�C���h�\�z
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

	// �����̓N���t�E�C���h�̕\���m�F�p�̃_�~�[�ł�
	pWnd = GetDlgItem(IDC_STC_GRAPH_WND);
	if (pWnd) {
		CRect rcClient;
		pWnd->GetWindowRect(rcClient);
		ScreenToClient(rcClient);
		m_pGraphWnd->MoveWindow(rcClient);
		pWnd->ShowWindow(SW_HIDE);
		m_pGraphWnd->ShowWindow(SW_SHOW);
	}

	// �����ʉ摜�p�E�C���h�̍\�z
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

	// �����ʉ摜�p�E�C���h�̍\�z
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

	// ���ʉ摜�p�E�C���h�̍\�z
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

// CWeldEvaluationView �f�f

#ifdef _DEBUG
/// <summary>
/// �I�u�W�F�N�g�̑Ó��������̎��{
/// </summary>
void CWeldEvaluationView::AssertValid() const
{
	CFormView::AssertValid();
}

/// <summary>
/// CDumpContext�I�u�W�F�N�g�̓��e���_���v
/// </summary>
/// <param name="dc">afxDump�f�f�_���v �R���e�L�X�g</param>
void CWeldEvaluationView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

/// <summary>
/// �h�L�������g�̎擾
/// </summary>
/// <returns>CWeldEvaluationDoc�ւ̃|�C���^��Ԃ�</returns>
CWeldEvaluationDoc* CWeldEvaluationView::GetDocument() const // �f�o�b�O�ȊO�̃o�[�W�����̓C�����C���ł��B
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWeldEvaluationDoc)));
	return (CWeldEvaluationDoc*)m_pDocument;
}
#endif //_DEBUG


// CWeldEvaluationView ���b�Z�[�W �n���h���[

/// <summary>
/// �E�C���h�T�C�Y�̕ύX
/// </summary>
/// <param name="nType">�E�C���h�^�C�v</param>
/// <param name="cx">�ύX��̃E�C���h��</param>
/// <param name="cy">�ύX��̃E�C���h����</param>
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
				rect.bottom = rect.top + lpRect.Height() - 60;		// �o�^���X�g���[�̎Z�o
				bottom = rect.bottom;								// bottom=�o�^���X�g���[
				ScreenToClient(rect);
				m_lstRegTest.MoveWindow(rect);
			}
		}

		if (((m_pReginWnd != NULL) && (m_pReginWnd->IsWindowEnabled())) && ((m_pMetalWnd != NULL) && (m_pMetalWnd->IsWindowEnabled())) && ((m_pResultWnd != NULL) && (m_pResultWnd->IsWindowEnabled()))) {
			CRect rect2,rect3, rect4;
			int sps;
			m_pReginWnd->GetWindowRect(rect);				// �����ʃE�C���h
			m_pMetalWnd->GetWindowRect(rect2);				// �����ʃE�C���h
			m_pResultWnd->GetWindowRect(rect3);				// ���ʃE�C���h

			// �����ʃE�C���h�Ƌ����ʃE�C���h�Ԃ̏c�����̃X�y�[�X���擾
			sps = rect2.top - rect.bottom;
			int hight = ((bottom - top) - sps*2) / 3;		// �X�L�����\���E�C���h�̍������Z�o
			rect.bottom = rect.top + hight;					// �����ʃE�C���h�̉��[�Z�o
			rect2.top		= rect.bottom	+ sps;			// �����ʃE�C���h�̏�[�Z�o
			rect2.bottom	= rect2.top		+ hight;		// �����ʃE�C���h�̉��[�Z�o
			rect3.top		= rect2.bottom	+ sps;			// ���ʃE�C���h�̏�[�Z�o
			rect3.bottom	= rect3.top		+ hight;		// ���ʃE�C���h�̉��[�Z�o

			int w = 0, movew = 0, moveh = 0;
			if ((m_pGraphWnd != NULL) && (m_pGraphWnd->IsWindowEnabled())) {
				m_pGraphWnd->GetWindowRect(rect4);
				movew = rect4.left;
				sps2 = rect4.left - rect.right;				// �X�L�����E�C���h�ƃO���t�E�C���h�Ԃ̐������擾
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
/// �E�C���h�j��������
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
// �������E��ʔz�u

/// <summary>
/// �A�C�e���̃t�B�b�e�B���O����
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
// ���X�g�r���[�����֘A
//

/// <summary>
/// �o�^�ς݌������X�g�̍X�V
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

	// ���݂̃v���W�F�N�g��I����Ԃɂ���
	int count = (int)list.GetCount();
	for (int i = 0; i < count; i++) {
		if (!list[i].CompareNoCase(pDoc->GetActiveRegistedTest())) {
			m_lstRegTest.SetItemState(i, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
			break;
		}
	}
}

/// <summary>
/// �o�^�ς݌����I��������
/// </summary>
/// <param name="pNMHDR">LPNMITEMACTIVATE�I�u�W�F�N�g�ւ̃|�C���^</param>
/// <param name="pResult">����</param>
/// <returns>�\�����[�h��Ԃ�</returns>
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
			msg = _T("�f�[�^���X�V����Ă��܂��B\n�X�V����Ă���f�[�^��j�����܂���?");
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

		// �ݒ�f�[�^�̓ǂݍ���
		LoadSetting();
		m_OprtInitialize.ItemActive(true);
		m_bReadMode = true;
//		m_OprtSetting.UpddateResist(false,m_bReadMode);	// �ݒ�f�[�^�����[�h���[�h�ɂ���
		m_OprtSetting.Update();
		m_OprtScan.ItemActive(true);;
		m_OprtAnalize.ItemActive(true);;

		// �v���p�e�B�f�[�^�̓ǂݍ���
		LoadPropaty();
		EnablePropaty(true);

		// �摜�\��
		int DisplayMode = 0;
		// �����ʉ摜
		DisplayMode = pDoc->GetDisplayMode(CWeldEvaluationDoc::eResinSurface);
		ViewChangeRequest(CWeldEvaluationDoc::eResinSurface, DisplayMode);
		m_pReginWnd->Invalidate();

		// �����ʉ摜
		DisplayMode = pDoc->GetDisplayMode(CWeldEvaluationDoc::eMetalSurface);
		ViewChangeRequest(CWeldEvaluationDoc::eMetalSurface, DisplayMode);
		m_pReginWnd->Invalidate();

		// ���ʉ摜
		DisplayMode = pDoc->GetDisplayMode(CWeldEvaluationDoc::eJoiningResult);
		ViewChangeRequest(CWeldEvaluationDoc::eJoiningResult, DisplayMode);
		m_pReginWnd->Invalidate();

		//#######################################################
		//#
		//# �O���t�̕\�����K�v
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
			fmt = _T("�o�^�ςݎ��� [%s] ���J�����Ƃ��ł��܂���ł����B");
		}
		msg.Format(fmt, (LPCWSTR)str);
		AfxMessageBox(msg,MB_OK|MB_ICONSTOP);
	}
#endif
}


///////////////////////////////////////////////////////////////////////////////////
// ����^�u����֘A
//

/// <summary>
/// ����^�u�؂�ւ�������
/// </summary>
/// <param name="pNMHDR"> NMHDR �\���̂ւ̃|�C���^�[</param>
/// <param name="pResult">����</param>
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
					msg = _T("��͗̈�̐ݒ���I�����Ă��������B");
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
// �v���p�e�B�^�u����֘A
//

/// <summary>
/// �v���p�e�B�^�u�؂�ւ�������
/// </summary>
/// <param name="pNMHDR"> NMHDR �\���̂ւ̃|�C���^�[</param>
/// <param name="pResult">����</param>
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
				msg = _T("�v���p�e�B���ύX����Ă��܂��B");
			}
			if (!sub.LoadStringW(IDM_CHECKOFUPDATE)) {
				sub = _T("�X�V���܂����H");
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
/// �v���p�e�B�X�V�{�^������������
/// </summary>
void CWeldEvaluationView::OnBnClickedBtnPropUpdate()
{
	int SelIdx = -1;
	SelIdx = m_tabPropaty.GetCurSel();
	UpdatePropaty(SelIdx);
	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)GetDocument();
	switch (SelIdx) {
	case	0:	// ������
	{
		pDoc->SetWorkProjectUpdteStatus(true);
		m_PropResinPage.UpdateStatus();
		
	}
	break;
	case	1:	// ������
	{
		pDoc->SetWorkProjectUpdteStatus(true);
		m_PropMetalPage.UpdateStatus();
	}
	break;
	case	2:	// ����
	{
		pDoc->SetWorkProjectUpdteStatus(true);
		m_PropResultPage.UpdateStatus();
	}
	break;
	case	3:	// �ݒ�
	{
		m_OprtInitialize.LoadParamater();
		pDoc->SetWorkProjectUpdteStatus(true);
	}
	break;
	}

	OnUpdateRequestPrpoTab(false,0);
}

/// <summary>
/// �v���p�e�B�L�����Z���{�^������������
/// </summary>
void CWeldEvaluationView::OnBnClickedBtnPropCancel()
{
	OnUpdateRequestPrpoTab(false,0);
	ReloadPropaty(m_SelPropPageId);
}

/// <summary>
/// �v���p�e�B�X�V���̐ݒ�
/// </summary>
/// <param name="wparam"> �X�V�t���O</param>
/// <param name="lparam"> �y�[�WID</param>
/// <returns>���������ꍇ��0�A���s�����ꍇ��0�ȊO��Ԃ�</returns>
LRESULT CWeldEvaluationView::OnUpdateRequestPrpoTab(WPARAM wparam, LPARAM lparam)
{
	bool bUpdate = (bool)wparam;
	m_btnPropTabOK.EnableWindow(bUpdate);
	m_btnPropTabCancel.EnableWindow(bUpdate);
	m_bUpdatePropaty = bUpdate;
	return 0;
}

/// <summary>
/// �v���p�e�B�ǂݍ���
/// </summary>
void CWeldEvaluationView::LoadPropaty()
{
	int iCnt = (int)m_PropTab.GetSize();
	for (int i = 0; i < iCnt; i++) {
		ReloadPropaty(i);
	}
}

/// <summary>
/// �v���p�e�B�̗L����
/// </summary>
/// <param name="bActive"> �L���t���O</param>
void CWeldEvaluationView::EnablePropaty(bool bActive)
{
	m_PropResinPage.ItemActive(bActive);
	m_PropMetalPage.ItemActive(bActive);
	m_PropResultPage.ItemActive(bActive);
	m_PropSettingPage.ItemActive(bActive);
}

/// <summary>
/// �X�V�̗L���m�F
/// </summary>
/// <param name="propatyID">�v���p�e�B�^�uID</param>
/// <returns>�X�V����Ă���ꍇ��true�A���s�ꍇ��false��Ԃ�</returns>
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
/// �v���p�e�B�y�[�W�̍X�V
/// </summary>
/// <param name="propatyID"> �v���p�e�BID</param>
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
/// �v���p�e�B�y�[�W�̍ēǍ�
/// </summary>
/// <param name="propatyID"> �v���p�e�BID</param>
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
/// �ݒ�̓Ǎ�
/// </summary>
void CWeldEvaluationView::LoadSetting()
{
	int iCnt = (int)m_OprtTab.GetSize();
	for (int i = 0; i < iCnt; i++) {
		ReloadSetting(i);
	}
}

/// <summary>
/// �ݒ�̗L����
/// </summary>
/// <param name="bActive"> �L���t���O</param>
void CWeldEvaluationView::EnableSetting(bool bActive)
{

}

/// <summary>
/// �ݒ�y�[�W�̍ēǍ�
/// </summary>
/// <param name="settingID"> �ݒ�y�[�WID</param>
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
/// �I�y���[�V�����^�u�̗L����
/// </summary>
/// <param name="bActive"> �L���t���O</param>
void CWeldEvaluationView::EnableOperation(bool bActive)
{
	m_OprtInitialize.ItemActive(bActive);
	m_OprtSetting.ItemActive(bActive);
	m_OprtScan.ItemActive(bActive);
	m_OprtAnalize.ItemActive(bActive);
}

///////////////////////////////////////////////////////////////////////////////////
// �X�L���������֘A
//

/// <summary>
/// �X�L�����̗v��
/// </summary>
/// <param name="wparam"> �X�L����ID</param>
/// <param name="lparam"> �X�L��������</param>
/// <returns>���������ꍇ��0�A���s�����ꍇ��0�ȊO��Ԃ�</returns>
LRESULT CWeldEvaluationView::OnScanRequest(WPARAM wparam, LPARAM lparam)
{
	EnablePropaty(false);
	EnableOperation(false);

	int ScanID = (int)wparam;
	int *Result = (int *)lparam;
	int type = m_OprtAnalize.GetAnalizeType(ScanID);

	m_OprtAnalize.SetDisplayMode(ScanID, CWeldEvaluationDoc::DisplayModeScan);
	switch (ScanID) {
	case	CWeldEvaluationDoc::eResinSurface:		// ����
		m_PropResinPage.ViewJointRatio(type, ScanID, -1);
		break;
	case	CWeldEvaluationDoc::eMetalSurface:		// ����
		m_PropMetalPage.ViewJointRatio(type, ScanID, -1);
		break;
	case	CWeldEvaluationDoc::eJoiningResult:		// �ڍ�����
		m_PropResultPage.ViewJointRatio(type, ScanID, -1);
		break;
	}

	CStatusDlgThread* pThread = DYNAMIC_DOWNCAST(CStatusDlgThread, AfxBeginThread(RUNTIME_CLASS(CStatusDlgThread) , 0, 0, CREATE_SUSPENDED));
	pThread->m_bAutoDelete = false;			// �����ȃA�N�Z�X�h�~�̂��ߎ����폜�͖�����
	pThread->ResumeThread();
	while (pThread->m_Dlg.m_hWnd == 0) {	// m_Dlg�̃E�B���h�E�����������܂őҋ@
		Sleep(0);
	}
	pThread->UpdateStatus(_T("Preparing to start scanning ..."));
	if (!ScanImage(pThread, ScanID)) {
		pThread->m_Dlg.PostMessage(WM_COMMAND, IDCANCEL); // Status�_�C�A���O�����
		*Result = -1;
	}
	else {
		CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)GetDocument();
		if (!pThread->m_Valid) {  // �L�����Z���{�^���������ꂽ�ꍇ
			// �L�����Z���̏���
			*Result = 1;
		}
		else {
			// ����I������
			pThread->m_Dlg.PostMessage(WM_COMMAND, IDOK); // Status�_�C�A���O�����
			// �X�L������ʍX�V
			// �����A�X�L���������摜�̕\�����[�h����͂Ȃ�X�L�����ɕύX
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

	WaitForSingleObject(pThread->m_hThread, 30000);	// �X���b�h�̏I����30�b�����҂��Ă��i�ق�0�b�̂͂�
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
/// �X�L�����̎��{
/// </summary>
/// <param name="pStatus"> �X���b�h�I�u�W�F�N�g</param>
/// <param name="ScanID"> �X�L����ID</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
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
	float *			p = nullptr;			// ���`��Ԍ��ʊi�[�p
	vector<CPoint>	vOrign, vTrans;			// ���`��ԗp�p�����[�^
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
		// �v���W�F�N�g�o�^���܂�
		ProjectPath = pDoc->GetWorkProjectPath();
		if (!CFileUtil::fileExists(ProjectPath)) {
			if (!CreateDirectory(ProjectPath,NULL)) {
				return false;
			}
		}
		ProjectPath = CFileUtil::FilePathCombine(pDoc->GetWorkProjectPath(), pDoc->GetScanDataName(ScanID, _T("")));
	}
	else {
		// �����v���W�F�N�g
		ProjectPath = pDoc->getScanDataPath(ScanID);
	}

	// �J�����ݒ�
	int offset = pDoc->NumberOfOverridePixel();
	int band = pDoc->NumberOfBand();
	int width = pDoc->GetShootingWidth();
	int height = pDoc->GetShootingHeight();
	double speed = pDoc->GetIntegrationTimeMs();
	if ((band <= 0) || (width <= 0) || (height <= 0)) {
		CString msg;
		msg.Format(_T("ScanImage():�p�����[�^���ُ�ł��Bband=%d, widht=%d,height=%d"), band, width, height);
		logOut(CString(__FILE__), __LINE__,msg );
		return false;
	}

	CCameraIO cam(width, height, band);

	CDeviceIO device;
	int ID = device.Init(deviceName);
	if (ID < 0) {
		logOut(CString(__FILE__), __LINE__,_T("ScanImage():�f�o�C�X�̏������Ɏ��s����"));
		return false;
	}
	// �L�����Z���̃`�F�b�N
	if (!pStatus->m_Valid) {
		bResult = true;
		goto SCanFinalize;
	}
	if (!device.ToHome(ID)) {
		logOut(CString(__FILE__), __LINE__, _T("ScanImage():�z�[���|�W�V�����ւ̈ړ��Ɏ��s����"));
		return false;
	}
	// �L�����Z���̃`�F�b�N
	if (!pStatus->m_Valid) {
		bResult = true;
		goto SCanFinalize;
	}


	SnapscanFile = pDoc->getSnapscanFile();
	if (!CFileUtil::fileExists(SnapscanFile)) {
		CString msg;
		msg.Format(_T("ScanImage():SnapscanFile��������Ȃ�[%s]"), (LPCTSTR)SnapscanFile);
		logOut(CString(__FILE__), __LINE__, msg);
		return false;
	}
	bool dummyApi = pDoc->IsCameraDummyApi();
	if (!cam.Open(SnapscanFile,dummyApi)) {
		logOut(CString(__FILE__), __LINE__, _T("ScanImage():�J�������I�[�v���ł��܂���ł���"));
		return false;
	}
	// �L�����Z���̃`�F�b�N
	if (!pStatus->m_Valid) {
		bResult = true;
		goto SCanFinalize;
	}
	if (!cam.setIntegrationTime(speed)) {
		CString msg;
		msg.Format(_T("ScanImage():IntegrationTime�̐ݒ�Ɏ��s���܂���[%lf]"), speed);
		logOut(CString(__FILE__), __LINE__, msg);
		return false;
	}
	// �L�����Z���̃`�F�b�N
	if (!pStatus->m_Valid) {
		bResult = true;
		goto SCanFinalize;
	}

	registedFolde = pDoc->GetRegistedFolder();
	WBFileName = pDoc->GetWBFileName();
	reference_corrected = new CubeFloat();
	if (!cam.LoadReference(*reference_corrected, registedFolde, WBFileName)) {
		logOut(CString(__FILE__), __LINE__, _T("ScanImage():�z���C�g�o�����X�f�[�^�̃��[�h�Ɏ��s���܂���"));
		return false;
	}

	// �ꎞ�t�@�C���ۑ���p�X�ݒ�
	spectralFilePath = pDoc->GetTmpFolderPath();
	if (!CFileUtil::fileExists(spectralFilePath)) {
		if (!CreateDirectory(spectralFilePath, NULL)) {
			spectralFilePath = pDoc->GetRegistedFolder();
		}
	}

	// �L�����Z���̃`�F�b�N
	if (!pStatus->m_Valid) {
		bResult = true;
		goto SCanFinalize;
	}
	if (!cam.StartScan()) {
		logOut(CString(__FILE__), __LINE__, _T("ScanImage():�X�L�����̊J�n�Ɏ��s���܂���"));
		return false;
	}
	// �L�����Z���̃`�F�b�N
	if (!pStatus->m_Valid) {
		cam.StopScan();
		bResult = true;
		goto SCanFinalize;
	}

	cube_corrected = new CubeFloat();
	*cube_corrected = { 0 };

	double hscale = pDoc->GetHorizontalScale();
	double vscale = pDoc->GetVerticalScale();
	// �ϊ���̃f�[�^��90�x��]������̂ŁA�c��������ւ��
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
		msg.Format(_T("ScanImage():������(%d)�A�X�L�����J�nID(%d)�A�X�L�����I��ID(%d)�̂����ꂩ�ɕs���Ȓl���ݒ肳��Ă��܂�"), DivisionNumber, ScanStartID, ScanEndID);
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

	bool bHomography = true;		// �ˉe�ϊ��ۃt���O
	double aParam[8];				// �ˉe�ϊ��p�����[�^
	p = new float[band];		// ���`��Ԍ��ʊi�[�p
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
		// �X�L�����J�nID��0�Ŗ����ꍇ�͌����ʒu�𒲐�
		jointPos = (dstW - offset) * ScanStartID;
	}

//	for (int pos = 0; pos < DivisionNumber; pos++) {
	for (int pos = ScanStartID; pos <= ScanEndID; pos++) {
		// �L�����Z���̃`�F�b�N
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

		// �L�����Z���̃`�F�b�N
		if (!pStatus->m_Valid) {
			cam.StopScan();
			bResult = true;
			goto SCanFinalize;
		}
		/////////////////////////////////////////////////////////
		// �X�L�������{
		/////////////////////////////////////////////////////////
		fname.Format(_T("normal%02d"), pos+1);
		*cube_corrected = { 0 };
		if (!cam.AcquireSpectralCube(spectralFilePath,fname, *reference_corrected, *cube_corrected)) {
			bResult = false;
			break;
		}
		// �L�����Z���̃`�F�b�N
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
		// �X�L�����f�[�^�ϊ�����
		buff.Format(_T("Scaning : %d/%d(%d %%) Converting..."), pos + 1, DivisionNumber, (int)((pos + 1) * 100 / DivisionNumber));
		pStatus->UpdateStatus(buff);

		float ***pTmp = nullptr;
		scn.ScanDataConvert(width, height, band, (float ***)cube_corrected->ppp_data, hscale, vscale, dstW, dstH, pTmp,false);

		bool bBicubic = true; //���`��Ԏg�p�t���O
		if (bHomography) {	// �ˉe�ϊ�
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
					// @note #�v����
					// ScanStartID==0�łȂ��ꍇ�AScanStartID�̃u���b�N�͐��`��Ԃ͕s�v���H
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

		// �L�����Z���̃`�F�b�N
		if (!pStatus->m_Valid) {
			cam.StopScan();
			commonDeallocateCube(cube_corrected);
			bResult = true;
			goto SCanFinalize;
		}

		/////////////////////////////////////////////////////////////////
		// �X�L�����f�[�^��������
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

			// offset������
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
	// �f�[�^�̕ۑ�
	if (bResult && pStatus->m_Valid) {
		CString headerFile = ProjectPath + _T(".hdr");
		CString rawFile = ProjectPath + _T(".raw");

		CStdioFile tfd;
		// header�f�[�^�o��
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

		// raw�f�[�^�o��
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
/// �f�[�^���]�̗v��
/// </summary>
/// <param name="wparam"> �X�L����ID</param>
/// <param name="lparam"> ����</param>
/// <returns>���������ꍇ��0�A���s�����ꍇ��-1��Ԃ�</returns>
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
		CString str = _T("��͌��ʂ̔��]����");
		OnInversStatus(PROGRESS_SET_TITLE, (LPARAM)((LPCTSTR)str));
		OnInversStatus(PROGRESS_UPDATE, 0);
		if (!pDoc->InversAnalizeData(ScanID)) {
			// ��̓f�[�^�̔��]�Ɏ��s
			*Result = 1;
		}
		else {
			m_OprtAnalize.SetChangedAnalizeFlag(true);
		}
	}
	else {
		// ���]�ł��Ȃ���̓f�[�^
		*Result = 1;
	}
#else
	*Result = 1;
#endif
	OnInversStatus(PROGRESS_END, NULL);
	// ��̓f�[�^�����]�ł��Ȃ��ꍇ�͉�̓f�[�^���폜
	if (*Result == 1) {
		pDoc->DeleteAnalizeData(ScanID);
	}

	return 0;
}

/// <summary>
/// �f�[�^���]��Ԓʒm
/// </summary>
/// <param name="wparam">���b�Z�[�W�^�C�v</param>
/// <param name="lparam">��Ԃ̒l</param>
/// <returns>���������ꍇ��0�A���s�����ꍇ�͂���ȊO��Ԃ�</returns>
LRESULT CWeldEvaluationView::OnInversStatus(WPARAM wparam, LPARAM lparam)
{
	// ���b�Z�[�W���L���[�ɂ��܂��Ă��܂��̂Ń��b�Z�[�W�|���v���ď�����ʂ�
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
	case	PROGRESS_INIT:		// �����J�n
		{
			UINT value = (UINT)lparam;
			if (m_pProgress == nullptr) {
				m_pProgress = new CProgressDlg(false,this);
			}
			CString str;
			if (!str.LoadString(IDM_INVERS_SCANDATA_TITLE)) {
				str = _T("�X�L�����f�[�^���]����");
			}
			m_pProgress->setTitle(str);
			if (!str.LoadString(IDM_INVERS_INIT)) {
				str = _T("���]�����������E�E�E");
			}
			m_pProgress->setlabel(str);

			m_pProgress->setRange(0, value);
			m_pProgress->setPosition(0);
			m_pProgress->Create(CProgressDlg::IDD, this);
			m_pProgress->ShowWindow(SW_SHOW);
			m_pProgress->UpdateWindow();
		}
		break;
	case	PROGRESS_UPDATE:		// �����i�s��
		{
			UINT value = (UINT)lparam;
			if (m_pProgress != nullptr) {
				int min, max;
				m_pProgress->getRange(min, max);
				double per = (double)value / (double)((__int64)max - min);
				CString str, fmt;
				if (!fmt.LoadString(IDM_INVERSING)) {
					fmt = _T("�ϊ����F%d/%d (%.2lf[%%])");
				}
				str.Format(fmt, value, (max - min), per * 100);
				m_pProgress->setlabel(str);
				m_pProgress->setPosition(value);
				m_pProgress->UpdateWindow();
			}
		}
		break;
	case	PROGRESS_STOP:		// ������~���N�G�X�g
		{
			CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)GetDocument();
			pDoc->CalcResultStopRequest();
			break;
		}
	case	PROGRESS_END:		// �����I��
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
/// �z���C�g�o�����X�X�L�����̎��{�L������
/// </summary>
/// <param name="wparam"> ���g�p</param>
/// <param name="lparam"> ���g�p</param>
/// <returns>���{����ꍇ��0�A���Ȃ��ꍇ��1��Ԃ�</returns>
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
			msg = _T("�z���C�g�o�����X�f�[�^�͎擾�ς݂ł��B\n�擾���Ȃ����܂����H");
		}
		if (AfxMessageBox(msg, MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON1) != IDYES) {
			return 1;
		}
	}
	return 0;
}

/// <summary>
/// �z���C�g�o�����X�X�L�����̗v��
/// </summary>
/// <param name="wparam"> �X���b�h�I�u�W�F�N�g</param>
/// <param name="lparam"> �X�L��������</param>
/// <returns>���������ꍇ��0�A���s�����ꍇ��0�ȊO��Ԃ�</returns>
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
		if (!pStatus->m_Valid) {  // �L�����Z���{�^����������
			goto WBScanRequestCancel;
		}

		if (device.ToHome(ID)) {
			if (!pStatus->m_Valid) {  // �L�����Z���{�^����������
				goto WBScanRequestCancel;
			}
			if (device.Move(ID, 1)) {
				if (!pStatus->m_Valid) {  // �L�����Z���{�^����������
					goto WBScanRequestCancel;
				}

				if (!buff.LoadString(IDS_PREPANING)) {
					buff = _T("������...");
				}
				pStatus->UpdateStatus(buff);
				/////////////////////////////////////////////////////////
				// �z���C�g�o�����X�f�[�^�̎B�e
				/////////////////////////////////////////////////////////
				int band = pDoc->NumberOfBand();
				int width = pDoc->GetShootingWidth();
				int height = pDoc->GetShootingHeight();
				double speed = pDoc->GetIntegrationTimeMs();
				CCameraIO cam(width, height,band);
				if (!pStatus->m_Valid) {  // �L�����Z���{�^����������
					goto WBScanRequestCancel;
				}
				CString SnapscanFile = pDoc->getSnapscanFile();
				if (CFileUtil::fileExists(SnapscanFile)) {
					bool dummyApi = pDoc->IsCameraDummyApi();
					if (cam.Open(SnapscanFile, dummyApi)) {
						if (!buff.LoadString(IDM_SCANNING)) {
							buff = _T("�X�L������...");
						}
						if (!pStatus->m_Valid) {  // �L�����Z���{�^����������
							goto WBScanRequestCancel;
						}
						pStatus->UpdateStatus(buff);
						if (cam.setIntegrationTime(speed)) {
							if (cam.StartScan()) {
								if (!pStatus->m_Valid) {  // �L�����Z���{�^����������
									cam.Close();
									goto WBScanRequestCancel;
								}
								if (cam.AcquireReference(registedFolde,WBFileName)) {
									if (!pStatus->m_Valid) {  // �L�����Z���{�^����������
										pDoc->DeleteWBFile(WBFileName);
										cam.Close();
										goto WBScanRequestCancel;
									}
									*Result = 0;
									if (!buff.LoadString(IDM_SCAN_SUCCESS)) {
										buff = _T("�X�L��������������ɏI�����܂����B");
									}
									pStatus->UpdateStatus(buff);
								}
								else {
									logOut(CString(__FILE__), __LINE__, _T("OnWBScanRequest():�z���C�g�o�����X�f�[�^�̎擾�Ɏ��s���܂���"));
								}
							}
							else {
								logOut(CString(__FILE__), __LINE__, _T("OnWBScanRequest():�X�L�����̊J�n�Ɏ��s���܂���"));
							}
						}
						else {
							CString msg;
							msg.Format(_T("OnWBScanRequest():IntegrationTime�̐ݒ�Ɏ��s���܂���[%lf]"), speed);
							logOut(CString(__FILE__), __LINE__, msg);
						}
						cam.Close();
					}
					else {
						logOut(CString(__FILE__), __LINE__, _T("OnWBScanRequest():�J�������I�[�v���ł��܂���ł���"));
					}
				}
				else {
					CString msg;
					msg.Format(_T("OnWBScanRequest():SnapscanFile��������Ȃ�[%s]"), (LPCTSTR)SnapscanFile);
					logOut(CString(__FILE__), __LINE__, msg);
				}
			}
		}
	}
	return 0;
WBScanRequestCancel:
	if (!buff.LoadString(IDM_SCAN_CANCELD)) {
		buff = _T("�X�L�����������L�����Z������܂����B");
	}
	AfxMessageBox(buff, MB_OK | MB_ICONWARNING);
	*Result = 1;
	return 0;
}

/// <summary>
/// �v���W�F�N�g�̍X�V�v��
/// </summary>
/// <param name="wparam">���g�p</param>
/// <param name="lparam">���g�p</param>
/// <returns>�����ꍇ��0�A���s�ꍇ��-1��Ԃ�</returns>
LRESULT CWeldEvaluationView::OnProjectResistRequest(WPARAM wparam, LPARAM lparam)
{
	int iResult = 0;
	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)GetDocument();

	CWaitCursor waitCursol;
	if (!pDoc->SaveProject()) {
		CString msg;
		if (!msg.LoadString(IDM_ERR_PROJECT_SAVE)) {
			msg = _T("�o�^�Ɏ��s���܂����B");
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
/// �摜�o�͗v��
/// </summary>
/// <param name="wparam">���g�p</param>
/// <param name="lparam">���g�p</param>
//// <returns>�����ꍇ��0�A���s�ꍇ��-1��Ԃ�</returns>
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
		label = _T("�t�H���_�I��");
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
/// �摜�X�P�[�����O���{�ʒm
/// </summary>
/// <param name="wparam">�\���^�[�Q�b�gID</param>
/// <param name="lparam">�摜�G���A�l</param>
/// <returns>�����ꍇ��0�A���s�ꍇ��-1��Ԃ�</returns>
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
	case	CWeldEvaluationDoc::eResinSurface:		// ����
		if (!m_pReginWnd->GetScalingInfo(scalingRetio, pos)) {
			return -1;
		}
		pImageWnd1 = m_pMetalWnd;
		pImageWnd2 = m_pResultWnd;
		pos1 = pImageWnd1->ConvertImagePos(pos);
		pos2 = pos;
		break;
	case	CWeldEvaluationDoc::eMetalSurface:		// ����
		if (!m_pMetalWnd->GetScalingInfo(scalingRetio, pos)) {
			return -1;
		}
		pImageWnd1 = m_pReginWnd;
		pImageWnd2 = m_pResultWnd;
		pos1 = pImageWnd1->ConvertImagePos(pos);
		pos2 = pImageWnd2->ConvertImagePos(pos);
		break;
	case	CWeldEvaluationDoc::eJoiningResult:		// �ڍ�����
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
/// �摜�ړ��ʒm
/// </summary>
/// <param name="wparam">�\���^�[�Q�b�gID</param>
/// <param name="lparam">�摜�G���A�l</param>
/// <returns>�����ꍇ��0�A���s�ꍇ��-1��Ԃ�</returns>
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
	case	CWeldEvaluationDoc::eResinSurface:		// ����
		if (!m_pReginWnd->GetScalingInfo(scalingRetio, pos)) {
			return -1;
		}
		pImageWnd1 = m_pMetalWnd;
		pImageWnd2 = m_pResultWnd;
		pos1 = m_pReginWnd->ConvertImagePos(pos);
		pos2 = pos;
		break;
	case	CWeldEvaluationDoc::eMetalSurface:		// ����
		if (!m_pMetalWnd->GetScalingInfo(scalingRetio, pos)) {
			return -1;
		}
		pImageWnd1 = m_pReginWnd;
		pImageWnd2 = m_pResultWnd;
		pos1 = m_pMetalWnd->ConvertImagePos(pos);
		pos2 = m_pMetalWnd->ConvertImagePos(pos);
		break;
	case	CWeldEvaluationDoc::eJoiningResult:		// �ڍ�����
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
/// �\���ύX���N�G�X�g
/// </summary>
/// <param name="ScanID">�\���^�[�Q�b�gID</param>
/// <param name="DisplayMode">�f�B�X�v���C���[�h(0:�X�L�����f�[�^�A1:��̓f�[�^)</param>
/// <param name="renew">�ĕ`��L���t���O(true�F�ĕ`��Afalse�F�ĕ`�悹��)</param>
/// <returns>�����ꍇ��true�A���s�ꍇ��false��Ԃ�</returns>
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
	case	CWeldEvaluationDoc::eResinSurface:		// ����
		pImageWnd = m_pReginWnd;
		pPropPage = &m_PropResinPage;
		NumberOfClass = pDoc->ResinGetNumberOfClass(AnalyzeMethod);
		BeforNumberOfClass = pDoc->ResinGetNumberOfClass(BeforAnalyzeMethod);
		break;
	case	CWeldEvaluationDoc::eMetalSurface:		// ����
		pImageWnd = m_pMetalWnd;
		pPropPage = &m_PropMetalPage;
		NumberOfClass = pDoc->MetalGetNumberOfClass(AnalyzeMethod);
		BeforNumberOfClass = pDoc->MetalGetNumberOfClass(BeforAnalyzeMethod);
		break;
	case	CWeldEvaluationDoc::eJoiningResult:		// �ڍ�����
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
					msg = _T("�N���X�����X�V����Ă��܂��B\n�X�V��j�����܂����H");
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
/// �\���؂�ւ��v��
/// </summary>
/// <param name="wparam">�\���^�[�Q�b�gID</param>
/// <param name="lparam">�\�����[�hID</param>
/// <returns>�����ꍇ��0�A���s�ꍇ��-1��Ԃ�</returns>
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
/// �\���폜
/// </summary>
/// <param name="wparam">�\���^�[�Q�b�gID</param>
/// <param name="lparam">���g�p</param>
/// <returns>�����ꍇ��0�A���s�ꍇ��-1��Ԃ�</returns>
LRESULT CWeldEvaluationView::OnImageErace(WPARAM wparam, LPARAM lparam)
{
	int ScanID = (int)wparam;

//	CImageWind *pImageWnd = nullptr;
	switch (ScanID) {
	case	CWeldEvaluationDoc::eResinSurface:		// ����
		m_pReginWnd->Erase();
		m_pMetalWnd->Reset();
		m_pResultWnd->Reset();
		break;
	case	CWeldEvaluationDoc::eMetalSurface:		// ����
		m_pReginWnd->Reset();
		m_pMetalWnd->Erase();
		m_pResultWnd->Reset();
		break;
	case	CWeldEvaluationDoc::eJoiningResult:		// �ڍ�����
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
// ��͑���֘A
//

/// <summary>
/// ��͎��{�v��
/// </summary>
/// <param name="wparam">�\���^�[�Q�b�gID</param>
/// <param name="lparam">��͕��@ID</param>
/// <returns>�����ꍇ��0�A���s�ꍇ��-1��Ԃ�</returns>
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
	// �����Ώۂ����݂��邩���`�F�b�N
	if (!CFileUtil::fileExists(ScanDataFilePath)) {
		CString msg;
		if (!msg.LoadString(IDM_ERR_NOTEXIST_ANALIZEDATA)) {
			msg = _T("��͑Ώۂ����݂��܂���");
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
				msg = _T("�f�[�^���X�V����Ă��܂��B\n�X�V����Ă���f�[�^��j�����܂���?");
			}
			if (AfxMessageBox(msg, MB_YESNO | MB_ICONWARNING | MB_DEFBUTTON1) == IDNO) {
				EnablePropaty(true);
				EnableOperation(true);
				return -1;
			}
			OnBnClickedBtnPropCancel();
		}
	}

	// ��͂̎��{
 	if (!pDoc->Analize(targetID,AnalyzeMethod)) {
		iResult = -1;
	}

	// ��͕��@�̐ݒ�
	if (!pDoc->SetAnalysisMethod(targetID,AnalyzeMethod)) {
		iResult = -1;
	}

	m_bUpdateOperation = true;
	m_OprtSetting.UpddateResist(m_bUpdateOperation,m_bReadMode);
	m_OprtSetting.Update();
	if (iResult < 0) {
		CString msg;
		if (!msg.LoadString(IDM_ERR_NOT_ANALYZE)) {
			msg = _T("��͏����Ɏ��s���܂���");
		}
		AfxMessageBox(msg,MB_OK|MB_ICONSTOP);
	}
	else {
		// �\�����[�h���X�L����
		ViewChangeRequest(targetID, CWeldEvaluationDoc::DisplayModeResult,true);
		m_pReginWnd->Invalidate();

		m_OprtAnalize.LoadParamater();

		// �v���p�e�B�y�[�W���X�V
		CDialogEx * pDlg = nullptr;
		switch (targetID) {
		case	CWeldEvaluationDoc::eResinSurface:		// ����
			m_PropResinPage.LoadParamater(CWeldEvaluationDoc::eResinSurface);
			m_PropResinPage.UpdateStatus();
			pDlg = (CDialogEx *)&m_PropResinPage;
			break;
		case	CWeldEvaluationDoc::eMetalSurface:		// ����
			m_PropMetalPage.LoadParamater(CWeldEvaluationDoc::eMetalSurface);
			m_PropMetalPage.UpdateStatus();
			pDlg = (CDialogEx *)&m_PropMetalPage;
			break;
		case	CWeldEvaluationDoc::eJoiningResult:		// ��������
			m_PropResultPage.LoadParamater(CWeldEvaluationDoc::eJoiningResult);
			m_PropResultPage.UpdateStatus();
			pDlg = (CDialogEx *)&m_PropResultPage;
			break;
		}
		// ���[�N�̃X�e�[�^�X�X�V
		pDoc->SetWorkProjectUpdteStatus(true);

		// �v���p�e�B�^�u����͑Ώۂ̃y�[�W�ɐ؂�ւ���
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

		// ��͏I�����b�Z�[�W
		CString	msg;
		if (!msg.LoadString(IDM_ANALYZE_SUCCESS)) {
			msg = _T("��͏���������I�����܂���");
		}
		AfxMessageBox(msg, MB_OK | MB_ICONINFORMATION);
	}
	EnablePropaty(true);
	EnableOperation(true);

	return iResult;
}

/// <summary>
/// ��͌��ʃt�@�C���ǂݍ��ݏ�Ԓʒm
/// </summary>
/// <param name="wparam">���b�Z�[�W�^�C�v</param>
/// <param name="lparam">��Ԃ̒l</param>
/// <returns>���������ꍇ��0�A���s�����ꍇ�͂���ȊO��Ԃ�</returns>
LRESULT CWeldEvaluationView::OnReadResultFileStatus(WPARAM wparam, LPARAM lparam)
{
	// ���b�Z�[�W���L���[�ɂ��܂��Ă��܂��̂Ń��b�Z�[�W�|���v���ď�����ʂ�
	MSG msg;
	while (PeekMessage(&msg, m_hWnd,  0, 0, PM_REMOVE)) {
		TranslateMessage(&msg); 
		DispatchMessage(&msg); 
	}

	UINT type = (UINT)wparam;
	UINT value = (UINT)lparam;
	switch(type) {
	case	PROGRESS_INIT:		// �����J�n
		{
			if (m_pProgress == nullptr) {
				m_pProgress = new CProgressDlg(true,this);
			}
			CString str;
			if (!str.LoadString(IDM_RESULT_READ_TITLE)) {
				str = _T("���ʃt�@�C���̓ǂݍ�");
			}
			m_pProgress->setTitle(str);
			if (!str.LoadString(IDM_RESULT_READ_INIT)) {
				str = _T("�ǂݍ��ݏ������E�E�E");
			}
			m_pProgress->setlabel(str);
			m_pProgress->setRange(0,value);
			m_pProgress->setPosition(0);
			m_pProgress->Create(CProgressDlg::IDD, this);
			m_pProgress->ShowWindow(SW_SHOW);
			m_pProgress->UpdateWindow();
		}
		break;
	case	PROGRESS_UPDATE:		// �����i�s��
		{
			if (m_pProgress != nullptr) {
				int min,max;
				m_pProgress->getRange(min,max);
				double per = (double)value / (double)((__int64)max - min);
				CString str,fmt;
				if (!fmt.LoadString(IDM_RESULT_READING)) {
					fmt = _T("�ǂݍ��ݒ��F%d/%d (%.2lf[%%])");
				}
				str.Format(fmt,value, (max - min),per*100);
				m_pProgress->setlabel(str);
				m_pProgress->setPosition(value);
				m_pProgress->UpdateWindow();
			}
		}
		break;
	case	PROGRESS_STOP:		// ������~���N�G�X�g
		{
			CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)GetDocument();
			pDoc->CalcResultStopRequest();
			break;
		}
	case	PROGRESS_END:		// �����I��
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
// �O���t����֘A
//

/// <summary>
/// �X�y�N�g���O���t�̕\��
/// </summary>
/// <param name="wparam">�\���f�[�^�Ώ�</param>
/// <param name="lparam">�\���f�[�^���W</param>
/// <returns>���������ꍇ��0�A���s�����ꍇ�͂���ȊO��Ԃ�</returns>
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
/// ��ԃX�y�N�g���\���_�C�A���O�̕\��
/// </summary>
/// <param name="wparam">���g�p</param>
/// <param name="lparam">���g�p</param>
/// <returns>���������ꍇ��0�A���s�����ꍇ�͂���ȊO��Ԃ�</returns>
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
/// ��ԃX�y�N�g���\���_�C�A���O�̃N���[�Y
/// </summary>
/// <param name="wparam">���g�p</param>
/// <param name="lparam">���g�p</param>
/// <returns>���������ꍇ��0�A���s�����ꍇ�͂���ȊO��Ԃ�</returns>
/// @remark �_�C�A���O���\������Ă���ꍇ�̓N���[�Y����
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
/// ��ԃX�y�N�g���\���_�C�A���O�̐ݒ�
/// </summary>
/// <param name="wparam">�Ώ�ID</param>
/// <param name="lparam">�\���Ώۉ摜�ʒu</param>
/// <returns>���������ꍇ��0�A���s�����ꍇ�͂���ȊO��Ԃ�</returns>
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
		// x���
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
		// y���
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
/// �o�^�ς݌����̍��ڂ̍X�V
/// </summary>
/// <param name="wparam">���g�p</param>
/// <param name="lparam">���g�p</param>
/// <returns>�����ꍇ��0�A���s�ꍇ��-1��Ԃ�</returns>
LRESULT CWeldEvaluationView::OnChangeResistFolder(WPARAM wparam, LPARAM lparam)
{
	//#######################################################
	//#
	//# �v���W�F�N�g���ۑ�����Ă��Ȃ��ꍇ�̏������K�v
	//#
	//#######################################################

	int iResult = 0;
	UpdateRegistFolderList();
	UnSellectProject();
	return iResult;
}

/// <summary>
/// �o�^�ς݌������I��
/// </summary>
void CWeldEvaluationView::UnSellectProject()
{
	CString msg,AppName;
	if (!AppName.LoadString(AFX_IDS_APP_TITLE)) {
		AppName = _T("WeldEvaluation");
	}
	msg.Format(_T("%s"), (LPCWSTR)AppName);
	GetParentFrame()->SetWindowText(msg);

	// �v���p�e�B�̍폜
	EnablePropaty(false);
	// �摜�A�O���t�̃N���A
	m_pGraphWnd->Erase();
	m_pReginWnd->Erase();
	m_pMetalWnd->Erase();
	m_pResultWnd->Erase();
}


/// <summary>
/// �u�V�K�����v�{�^������������
/// </summary>
void CWeldEvaluationView::OnBnClickedBtnNewprj()
{
	bool bResult;

	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)GetDocument();
	if (pDoc->IsWorkProjectUpdated()) {
		CString msg;
		if (!msg.LoadString(IDM_PRJREGIST_EXISTUPDATE)) {
			msg = _T("�f�[�^���X�V����Ă��܂��B\n�X�V����Ă���f�[�^��j�����܂���?");
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
		sub = _T("�V�K");
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
	// �ݒ�f�[�^�̓ǂݍ���
	LoadSetting();
	m_OprtInitialize.ItemActive(true);;
	m_OprtSetting.ItemActive(true);
	m_OprtScan.ItemActive(true);;
	m_OprtAnalize.ItemActive(true);
	m_OprtAnalize.ViewModeReset();

	// �v���p�e�B�f�[�^�̓ǂݍ���
	LoadPropaty();
	EnablePropaty(true);
}

/// <summary>
/// �}�E�X�ړ�������
/// </summary>
/// <param name="nFlags">���z�L�[�t���O</param>
/// <param name="point">�J�[�\�����W</param>
/// <returns>�����̏ꍇ��TRUE�A���s�̏ꍇ��FALSE��Ԃ�</returns>
/// @remark ���z�L�[�t���OnFlags��MK_CONTROL CTRL �L�[�AMK_LBUTTON �}�E�X�̍��{�^���AMK_MBUTTON �}�E�X�̒����{�^���AMK_RBUTTON �}�E�X�̉E�{�^���AMK_SHIFT SHIFT �L�[
void CWeldEvaluationView::OnMouseMove(UINT nFlags, CPoint point)
{
	m_pReginWnd->OnMouseLeave();
	m_pMetalWnd->OnMouseLeave();
	m_pResultWnd->OnMouseLeave();
	CFormView::OnMouseMove(nFlags, point);
}

/// <summary>
/// �摜�̃X�P�[�����O�E�ړ�
/// </summary>
/// <param name="targetID">�\���^�[�Q�b�gID</param>
/// <param name="rect">�摜�G���A�l</param>
/// <returns>�����ꍇ��true�A���s�ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationView::ImageScaling(int targetID, CRect rect)
{
	bool bResult = true;
	double scalingRetio = 1.0;
	CPoint pos, pos1, pos2,tmp;
	CImageWind *pImageWnd1 = nullptr;
	CImageWind *pImageWnd2 = nullptr;
	switch (targetID) {
	case	CWeldEvaluationDoc::eResinSurface:		// ����
		if (!m_pReginWnd->GetScalingInfo(scalingRetio, pos)) {
			return false;
		}
		pImageWnd1 = m_pMetalWnd;
		pImageWnd2 = m_pResultWnd;
		tmp = m_pReginWnd->ConvertImagePos(CPoint(pos.x, 0));
		pos1 = tmp;
		pos2 = pos;
		break;
	case	CWeldEvaluationDoc::eMetalSurface:		// ����
		if (!m_pMetalWnd->GetScalingInfo(scalingRetio, pos)) {
			return false;
		}
		pImageWnd1 = m_pReginWnd;
		pImageWnd2 = m_pResultWnd;
		tmp = m_pMetalWnd->ConvertImagePos(CPoint(pos.x, 0));
		pos1 = tmp;
		pos2 = tmp;
		break;
	case	CWeldEvaluationDoc::eJoiningResult:		// �ڍ�����
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
/// �N���[�Y����
/// </summary>
void CWeldEvaluationView::OnClose()
{
	CFormView::OnClose();
}

/// <summary>
/// ���X�g���ڂ̉E�{�^������������
/// </summary>
/// <param name="pNMHDR">NMITEMACTIVATE�ւ̃|�C���^</param>
/// <param name="pResult">����</param>
void CWeldEvaluationView::OnNMRClickLstRegistTest(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
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
	// �T�u���j���[���擾
	if (!err) {
		if ((popupMenuP = cMenu.GetSubMenu(0)) == NULL) {
			err = 1;
		}
	}
	// ���j���[���|�b�v�A�b�v
	if (!err) {
		GetCursorPos(&posMouse);
		if (popupMenuP->TrackPopupMenu(TPM_LEFTBUTTON, posMouse.x, posMouse.y, this))
			err = 1;
	}	

	*pResult = 0;
}


/// <summary>
/// �v���W�F�N�g�̃I�[�v��
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
			msg = _T("�f�[�^���X�V����Ă��܂��B\n�X�V����Ă���f�[�^��j�����܂���?");
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

		// �ݒ�f�[�^�̓ǂݍ���
		LoadSetting();
		m_OprtInitialize.ItemActive(true);
		m_bReadMode = true;
		//		m_OprtSetting.UpddateResist(false,m_bReadMode);	// �ݒ�f�[�^�����[�h���[�h�ɂ���
		m_OprtSetting.Update();
		m_OprtScan.ItemActive(true);;
		m_OprtAnalize.ItemActive(true);;

		// �v���p�e�B�f�[�^�̓ǂݍ���
		LoadPropaty();
		EnablePropaty(true);

		// �摜�\��
		int DisplayMode = 0;
		// �����ʉ摜
		DisplayMode = pDoc->GetDisplayMode(CWeldEvaluationDoc::eResinSurface);
		ViewChangeRequest(CWeldEvaluationDoc::eResinSurface, DisplayMode);
		m_pReginWnd->Invalidate();

		// �����ʉ摜
		DisplayMode = pDoc->GetDisplayMode(CWeldEvaluationDoc::eMetalSurface);
		ViewChangeRequest(CWeldEvaluationDoc::eMetalSurface, DisplayMode);
		m_pReginWnd->Invalidate();

		// ���ʉ摜
		DisplayMode = pDoc->GetDisplayMode(CWeldEvaluationDoc::eJoiningResult);
		ViewChangeRequest(CWeldEvaluationDoc::eJoiningResult, DisplayMode);
		m_pReginWnd->Invalidate();

		//#######################################################
		//#
		//# �O���t�̕\�����K�v
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
			fmt = _T("�o�^�ςݎ��� [%s] ���J�����Ƃ��ł��܂���ł����B");
		}
		msg.Format(fmt, (LPCWSTR)str);
		AfxMessageBox(msg, MB_OK | MB_ICONSTOP);
	}
}

/// <summary>
/// �v���W�F�N�g�폜
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
			msg = _T("�ҏW���̃f�[�^�ł��B\n�폜���܂����H");
		}
		if (AfxMessageBox(msg, MB_YESNO | MB_ICONWARNING | MB_DEFBUTTON1) == IDNO) {
			return;
		}
	}
	else {
		CString msg;
		if (!msg.LoadString(IDM_DELETE)) {
			msg = _T("�폜���Ă�낵���ł����H ");
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
/// �v���W�F�N�g�̍X�V
/// </summary>
void CWeldEvaluationView::OnFileSave()
{
	OnProjectResistRequest(NULL, NULL);
}

/// <summary>
/// �v���W�F�N�g�V�K�쐬
/// </summary>
void CWeldEvaluationView::OnProjectNew()
{
	OnBnClickedBtnNewprj();
}
