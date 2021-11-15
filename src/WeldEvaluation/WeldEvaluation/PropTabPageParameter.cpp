// PropTabPageParameter.cpp : 実装ファイル
//

#include "stdafx.h"
#include "WeldEvaluation.h"
#include "PropTabPageParameter.h"
#include "afxdialogex.h"
#include "WeldEvaluationDoc.h"


// CPropTabPageParameter ダイアログ

IMPLEMENT_DYNAMIC(CPropTabPageParameter, CDialogEx)

/// <summary>
/// コンストラクタ
/// </summary>
CPropTabPageParameter::CPropTabPageParameter(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPropTabPageParameter::IDD, pParent)
	, m_NumberOfClass(0)
	, m_JointRatio(_T(""))
	, m_strJoinratioTarget(_T(""))
{
	m_PageID = CWeldEvaluationDoc::eResinSurface;
}

/// <summary>
/// デストラクタ
/// </summary>
CPropTabPageParameter::~CPropTabPageParameter()
{
}

/// <summary>
/// データエクスチェンジ処理
/// </summary>
/// <param name="pDX">CDataExchangeオブジェクト</param>
void CPropTabPageParameter::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_NUMOFCLASS, m_NumberOfClass);
	DDX_Text(pDX, IDC_EDT_JOINRATIO, m_JointRatio);
	DDX_Control(pDX, IDC_CMB_JOINRATIO_TARGET_LABEL, m_cmbJoinratioTarget);
	DDX_CBString(pDX, IDC_CMB_JOINRATIO_TARGET_LABEL, m_strJoinratioTarget);
	DDX_Control(pDX, IDC_STC_IDCOLOR, m_stcIDColor);
}


BEGIN_MESSAGE_MAP(CPropTabPageParameter, CDialogEx)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_NUMOFCLASS, &CPropTabPageParameter::OnDeltaposSpinNumofclass)
	ON_CBN_SELCHANGE(IDC_CMB_JOINRATIO_TARGET_LABEL, &CPropTabPageParameter::OnCbnSelchangeCmbJoinratioTargetLabel)
	ON_WM_ACTIVATE()
	ON_CBN_KILLFOCUS(IDC_CMB_JOINRATIO_TARGET_LABEL, &CPropTabPageParameter::OnCbnKillfocusCmbJoinratioTargetLabel)
	ON_EN_KILLFOCUS(IDC_EDT_NUMOFCLASS, &CPropTabPageParameter::OnEnKillfocusEdtNumofclass)
END_MESSAGE_MAP()


// CPropTabPageParameter メッセージ ハンドラー

/// <summary>
/// ページIDの設定
/// </summary>
/// <param name="PageID">ページID</param>
void CPropTabPageParameter::setPageID(int PageID)
{
	m_PageID = PageID;
}

/// <summary>
/// ページIDの取得
/// </summary>
int CPropTabPageParameter::getPageID()
{
	return m_PageID;
}

/// <summary>
/// ダイアログアイテムののアクティブ/ディアクティブ化
/// </summary>
/// <param name="ItemID">アイテムＩＤ</param>
/// <param name="bActive">アクティブフラグ</param>
void CPropTabPageParameter::ItemEnable(int ItemID,bool bActive)
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
void CPropTabPageParameter::ItemActive(bool bActive)
{
	ItemEnable(IDC_STC_NUMOFCLASS_LABEL,bActive);
	ItemEnable(IDC_EDT_NUMOFCLASS,bActive);
	ItemEnable(IDC_SPIN_NUMOFCLASS,bActive);

	ItemEnable(IDC_STC_JOINRATIO_LABEL,bActive);
	ItemEnable(IDC_EDT_JOINRATIO,bActive);
	ItemEnable(IDC_STC_JOINRATIO_UNIT,bActive);

	ItemEnable(IDC_STC_JOINRATIO_TARGET_LABEL,bActive);
	ItemEnable(IDC_CMB_JOINRATIO_TARGET_LABEL,bActive);

	ItemEnable(IDC_STC_IDCOLOR, bActive);

	UpdateCmbJoinratioTargetLabel();

}

/// <summary>
/// 分類数編集のスピンドル操作時の処理
/// </summary>
/// <param name="pNMHDR">LPNMUPDOWNオブジェクト</param>
/// <param name="pResult">結果</param>
void CPropTabPageParameter::OnDeltaposSpinNumofclass(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	*pResult = 0;

	UpdateData(true);
	int Min = 1;
	int Max = 100;
	int ival = (int)m_NumberOfClass;
	ival -= pNMUpDown->iDelta;
	if (ival < Min) {
		ival = Min;
	} else if (ival > Max) {
		ival = Max;
	}
	m_NumberOfClass = (UINT)ival;
	UpdateData(false);
	UpdateCmbJoinratioTargetLabel();
	// 更新ボタンの更新
	CWnd *pWnd = GetParent()->GetParent();
	pWnd->SendMessage(WM_UPDATEREQUEST_PROPPAGE,(WPARAM)true,(LPARAM)0);
}


/// <summary>
/// 分類数編集ボックスがフォーカスを失った時の処理
/// </summary>
void CPropTabPageParameter::OnEnKillfocusEdtNumofclass()
{
	UINT org = m_NumberOfClass;
	UpdateData(true);
	if (m_NumberOfClass != org) {
		UpdateData(false);
		// 更新ボタンの更新
		UpdateCmbJoinratioTargetLabel();
		CWnd *pWnd = GetParent()->GetParent();
		pWnd->SendMessage(WM_UPDATEREQUEST_PROPPAGE,(WPARAM)true,(LPARAM)0);
	}
}

/// <summary>
/// パラメータをファイルから読み込む
/// </summary>
/// <param name="id">更新対インデックス</param>
void CPropTabPageParameter::LoadParamater(int id)
{
	CFormView *pWnd = (CFormView *)GetParent()->GetParent();
	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)pWnd->GetDocument();
	UpdateData(true);

	switch(id) {
	case	CWeldEvaluationDoc::eResinSurface	:	// 樹脂
		{
			m_NumberOfClass = pDoc->ResinGetNumberOfClass();
		}
		break;
	case	CWeldEvaluationDoc::eMetalSurface	:	// 金属
		{
			m_NumberOfClass = pDoc->MetalGetNumberOfClass();
		}
		break;
	case	CWeldEvaluationDoc::eJoiningResult	:	// 接合結果
		{
			m_NumberOfClass = pDoc->ResultGetNumberOfClass();
		}
		break;
	}
	int dataid  = m_cmbJoinratioTarget.GetCurSel();
	if (dataid < 0) {
		ViewJointRatio(dataid, -1);
	} else {
		ViewJointRatio(id, (int)m_cmbJoinratioTarget.GetItemData(dataid));
	}
	UpdateData(false);
}

/// <summary>
/// 接合割合の設定
/// </summary>
/// <param name="id">更新対インデックス</param>
/// <param name="ViewJointRatioNo">接合面番号</param>
void CPropTabPageParameter::ViewJointRatio(int id, int ViewJointRatioNo)
{
	if (ViewJointRatioNo < 0) {
		m_JointRatio.Format(_T("%.1lf"),0.0);
	} else {
		CFormView *pWnd = (CFormView *)GetParent()->GetParent();
		CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)pWnd->GetDocument();

		switch(id) {
		case	CWeldEvaluationDoc::eResinSurface	:	// 樹脂
			{
				m_JointRatio.Format(_T("%.1lf"),pDoc->ResinGetJointRetio(ViewJointRatioNo));
				UpdateIDColor(pDoc->ResinGetJointColor(ViewJointRatioNo));
			}
			break;
		case	CWeldEvaluationDoc::eMetalSurface	:	// 金属
			{
				m_JointRatio.Format(_T("%.1lf"),pDoc->MetalGetJointRetio(ViewJointRatioNo));
				UpdateIDColor(pDoc->MetalGetJointColor(ViewJointRatioNo));
		}
			break;
		case	CWeldEvaluationDoc::eJoiningResult	:	// 接合結果
			{
				m_JointRatio.Format(_T("%.1lf"),pDoc->ResultGetJointRetio(ViewJointRatioNo));
				UpdateIDColor(pDoc->ResultGetJointColor(ViewJointRatioNo));
		}
			break;
		}
	}
	UpdateData(false);
}

/// <summary>
/// 分類色を反映
/// </summary>
/// <param name="color">変更色</param>
void CPropTabPageParameter::UpdateIDColor(COLORREF color)
{
	CWnd *pWnd;
	pWnd = GetDlgItem(IDC_STC_IDCOLOR);
	if (pWnd) {
		CDC *dc = pWnd->GetDC();
		if (dc == NULL)
		{
			return;
		}
		CRect rect;
		pWnd->GetClientRect(rect);
		CBrush blackBrush(color);
		CBrush *orgBrush;
		orgBrush = dc->SelectObject(&blackBrush);
		dc->FillRect(&rect, &blackBrush);
		dc->SelectObject(orgBrush);
		ReleaseDC(dc);
	}
}


/// <summary>
/// 分類数を反映
/// </summary>
/// <param name="index">更新対インデックス</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CPropTabPageParameter::Update(int index)
{
	bool bResult = false;
	UpdateData(true);
	CFormView *pWnd = (CFormView *)GetParent()->GetParent();
	CWeldEvaluationDoc *pDoc = (CWeldEvaluationDoc *)pWnd->GetDocument();

	switch(index) {
	case	CWeldEvaluationDoc::eResinSurface	:	// 樹脂
		{
			bResult = pDoc->ResinSetNumberOfClass(m_NumberOfClass);
		}
		break;
	case	CWeldEvaluationDoc::eMetalSurface	:	// 金属
		{
			bResult = pDoc->MetalSetNumberOfClass(m_NumberOfClass);
		}
		break;
	case	CWeldEvaluationDoc::eJoiningResult	:	// 接合結果
		{
			bResult = pDoc->ResultSetNumberOfClass(m_NumberOfClass);
		}
		break;
	}
	UpdateData(false);
	return false;
}


/// <summary>
/// 対象コンボボック項目変更時処理
/// </summary>
void CPropTabPageParameter::OnCbnSelchangeCmbJoinratioTargetLabel()
{
	UpdateData(true);
	int dataid = m_cmbJoinratioTarget.GetCurSel();
	if (dataid == CB_ERR) {
		return;
	}
	CString str;
	m_cmbJoinratioTarget.GetLBText(dataid, str);
	m_strJoinratioTarget = str;
	UpdateData(false);
	CPropTabPageParameter::ViewJointRatio(m_PageID, (int)m_cmbJoinratioTarget.GetItemData(dataid));
}

/// <summary>
/// 対象コンボボックスラベル変更時処理
/// </summary>
void CPropTabPageParameter::UpdateCmbJoinratioTargetLabel()
{
	UpdateData(true);
	int itemCount = m_cmbJoinratioTarget.GetCount();
	if (m_cmbJoinratioTarget.GetCount() != (int)m_NumberOfClass) {
		CString strJoinratioTarget = m_strJoinratioTarget;
		if (itemCount > (int)m_NumberOfClass) {
			for (int i = itemCount-1; i >= (int)m_NumberOfClass; i--) {
				m_cmbJoinratioTarget.DeleteString(i);
			}
			if (m_cmbJoinratioTarget.FindString(0,strJoinratioTarget) == -1) {
				TCHAR sval[255];
				if (_itot_s(m_NumberOfClass,sval,255,10) == 0) {
					m_strJoinratioTarget = sval;
					UpdateData(false);
					int id = m_cmbJoinratioTarget.SelectString(0,m_strJoinratioTarget);
					if (id != CB_ERR ) {
						ViewJointRatio(m_PageID, id);
					}
				}
			}
		} else if (itemCount < (int)m_NumberOfClass) {
			for (int i =itemCount ; i < (int)m_NumberOfClass; i++) {
				TCHAR sval[255];
				if (_itot_s(i+1,sval,255,10) == 0) {
					int id = m_cmbJoinratioTarget.AddString(sval);
					m_cmbJoinratioTarget.SetItemData(id,i);
				}
			}
		}
	}
	UpdateData(false);
}

/// <summary>
/// ウインドアクティブ時処理
/// </summary>
/// <param name="nState">アクティブフラグ</param>
/// <param name="pWndOther">対象ウインドへのポインタ</param>
/// <param name="bMinimized">最小化フラグ</param>
void CPropTabPageParameter::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);

	UpdateCmbJoinratioTargetLabel();
}


/// <summary>
/// 対象コンボボックスのエディットボックスフォーカス消失時処理
/// </summary>
void CPropTabPageParameter::OnCbnKillfocusCmbJoinratioTargetLabel()
{
	CString str = m_strJoinratioTarget;
	UpdateData(true);
	int dataid = m_cmbJoinratioTarget.SelectString(0,m_strJoinratioTarget);
	if (dataid != CB_ERR) {
		CPropTabPageParameter::ViewJointRatio(m_PageID, (int)m_cmbJoinratioTarget.GetItemData(dataid));
	}
	else {
		m_strJoinratioTarget = str;
	}
	UpdateData(false);
}
