
// WeldEvaluationView.h : CWeldEvaluationView クラスのインターフェイス
//

#pragma once

#include "resource.h"
#include "message.h"
#include "afxcmn.h"
#include <vector>

#include "OprtTabPageInitialize.h"
#include "OprtTabPageSetting.h"
#include "OprtTabPageScan.h"
#include "OprtTabPageAnalize.h"

#include "PropTabPageParameter.h"
#include "PropTabPageSetting.h"
#include "GraphWind.h"
#include "ImageWind.h"
#include "ProgressDlg.h"
#include "SpectralDlg.h"

#include "afxwin.h"

/// <summary>
/// 表示フォームクラス
/// </summary>
class CWeldEvaluationView : public CFormView
{
protected: // シリアル化からのみ作成します。
	CWeldEvaluationView();
	DECLARE_DYNCREATE(CWeldEvaluationView)

public:
	enum{ IDD = IDD_WELDEVALUATION_FORM };

// 属性
public:
	CWeldEvaluationDoc* GetDocument() const;

// 操作
public:

// オーバーライド
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual void OnInitialUpdate(); // 構築後に初めて呼び出されます。

// 実装
public:
	virtual ~CWeldEvaluationView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

private:
	COprtTabPageInitialize					m_OprtInitialize;				///< 操作タブページ（初期化用）
	COprtTabPageSetting						m_OprtSetting;					///< 操作タブページ（プロジェクト設定用）
	COprtTabPageScan						m_OprtScan;						///< 操作タブページ（スキャン用）
	COprtTabPageAnalize						m_OprtAnalize;					///< 操作タブページ（解析用）
	CTypedPtrArray< CObArray,CDialogEx *>	m_OprtTab;						///< 操作タブページリスト
	bool									m_bUpdateOperation;				///< 操作更新フラグ
	bool									m_bReadMode;					///< 読み込みモードフラグ
	int										m_SelOprtPageId;				///< 選択中操作ページID

	CPropTabPageParameter					m_PropResinPage;				///< プロパティタブページ（樹脂面用）
	CPropTabPageParameter					m_PropMetalPage;				///< プロパティタブページ（金属面用）
	CPropTabPageParameter					m_PropResultPage;				///< プロパティタブページ（結果用）
	CPropTabPageSetting						m_PropSettingPage;				///< プロパティタブページ（設定用）
	CTypedPtrArray< CObArray,CDialogEx *>	m_PropTab;						///< プロパティタブページリスト

	bool									m_bUpdatePropaty;				///< プロパティ更新フラグ
	int										m_SelPropPageId;				///< 選択中プロパティページID

	CGraphWind								*m_pGraphWnd;					///< グラフウインドへのポインタ
	CImageWind								*m_pReginWnd;					///< 樹脂面表示ウインドへのポインタ
	CImageWind								*m_pMetalWnd;					///< 金属面表示ウインドへのポインタ
	CImageWind								*m_pResultWnd;					///< 結果表示ウインドへのポインタ

	CSpectralDlg							*m_pSpectralDlg;				///< スペクトル表示ダイアログ
	CRect									m_SpectralDlgRect;				///< スペクトル表示ダイアログの位置

	CProgressDlg							*m_pProgress;					///< プログレスダイアログオブジェクト

public:
	CListCtrl								m_lstRegTest;					///< 登録済みリスト用Listオブジェクト
	CTabCtrl								m_tabPropaty;					///< プロパティ用タブオブジェクト
	CTabCtrl								m_tabOperation;					///< 操作用タブオブジェクト
	CStatic									m_stcPropTabPageClient;			///< プロパティページ表示エリア用
	CStatic									m_stcOperationTabPageClient;	///< 処理ページ表示エリア用
	CButton									m_btnPropTabCancel;				///< プロパティタブ・Cancelボタンオブジェクト
	CButton									m_btnPropTabOK;					///< プロパティタブ・OKボタンオブジェクト

// 生成された、メッセージ割り当て関数
protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnTcnSelchangeTabOperation(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnNMDblclkLstRegistTest(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnTcnSelchangeTabPropaty(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnPropUpdate();
	afx_msg void OnBnClickedBtnPropCancel();
	afx_msg void OnBnClickedBtnNewprj();
	afx_msg void OnDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnClose();
	afx_msg void OnNMRClickLstRegistTest(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnProjectOpen();
	afx_msg void OnProjectDelete();
	afx_msg void OnFileSave();
	afx_msg void OnProjectNew();

private:
	void CWeldEvaluationView::logOut(CString filePath, long lineNo, CString msg);
	void MoveItem(CWnd *pWnd, int movex, int movey);

public:
	LRESULT OnUpdateRequestPrpoTab(WPARAM wparam, LPARAM lparam);
	void LoadPropaty();
	void EnablePropaty(bool bActive);
	void UpdatePropaty(int propatyID);
	bool ConfirmChange(int propatyID);
	void ReloadPropaty(int propatyID);

	void LoadSetting();
	void EnableSetting(bool bActive);
	void ReloadSetting(int settingID);

	void EnableOperation(bool bActive);

	LRESULT OnScanRequest(WPARAM wparam, LPARAM lparam);

	LRESULT OnWBScanExistCheck(WPARAM wparam, LPARAM lparam);
	LRESULT OnWBScanRequest(WPARAM wparam, LPARAM lparam);
	bool ScanImage(CStatusDlgThread* pStatus, int ScanID);
	LRESULT OnInversRequest(WPARAM wparam, LPARAM lparam);
	LRESULT OnInversStatus(WPARAM wparam, LPARAM lparam);

	bool ViewChangeRequest(int ScanID, int DisplayMode, bool renew=false);
	LRESULT OnViewChangeRequest(WPARAM wparam, LPARAM lparam);
	void UpdateRegistFolderList();
	LRESULT OnChangeResistFolder(WPARAM wparam, LPARAM lparam);	
	void UnSellectProject();
	LRESULT OnAnalyzeRequest(WPARAM wparam, LPARAM lparam);	
	LRESULT OnProjectResistRequest(WPARAM wparam, LPARAM lparam);	
	LRESULT OnImageOutputRequest(WPARAM wparam, LPARAM lparam);	
	LRESULT OnImageScaling(WPARAM wparam, LPARAM lparam);
	LRESULT OnImageMoveing(WPARAM wparam, LPARAM lparam);
	bool ImageScaling(int targetID, CRect rect);
	LRESULT OnReadResultFileStatus(WPARAM wparam, LPARAM lparam);
	LRESULT OnSpectrumGraphRequest(WPARAM wparam, LPARAM lparam);
	LRESULT OnAreaSpectrumGraphRequest(WPARAM wparam, LPARAM lparam);
	LRESULT OnSpectrumeCloseRequest(WPARAM wparam, LPARAM lparam);
	LRESULT OnAreaSpectrumeGraphSet(WPARAM wparam, LPARAM lparam);
	LRESULT OnImageErace(WPARAM wparam, LPARAM lparam);

	void FitItem();
};

#ifndef _DEBUG  // WeldEvaluationView.cpp のデバッグ バージョン
/// <summary>
/// ドキュメントの取得
/// </summary>
/// <returns>CWeldEvaluationDocへのポインタを返す</returns>
inline CWeldEvaluationDoc* CWeldEvaluationView::GetDocument() const
   { return reinterpret_cast<CWeldEvaluationDoc*>(m_pDocument); }
#endif

