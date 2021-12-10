
// WeldEvaluationView.h : CWeldEvaluationView �N���X�̃C���^�[�t�F�C�X
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
/// �\���t�H�[���N���X
/// </summary>
class CWeldEvaluationView : public CFormView
{
protected: // �V���A��������̂ݍ쐬���܂��B
	CWeldEvaluationView();
	DECLARE_DYNCREATE(CWeldEvaluationView)

public:
	enum{ IDD = IDD_WELDEVALUATION_FORM };

// ����
public:
	CWeldEvaluationDoc* GetDocument() const;

// ����
public:

// �I�[�o�[���C�h
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	virtual void OnInitialUpdate(); // �\�z��ɏ��߂ČĂяo����܂��B

// ����
public:
	virtual ~CWeldEvaluationView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

private:
	COprtTabPageInitialize					m_OprtInitialize;				///< ����^�u�y�[�W�i�������p�j
	COprtTabPageSetting						m_OprtSetting;					///< ����^�u�y�[�W�i�v���W�F�N�g�ݒ�p�j
	COprtTabPageScan						m_OprtScan;						///< ����^�u�y�[�W�i�X�L�����p�j
	COprtTabPageAnalize						m_OprtAnalize;					///< ����^�u�y�[�W�i��͗p�j
	CTypedPtrArray< CObArray,CDialogEx *>	m_OprtTab;						///< ����^�u�y�[�W���X�g
	bool									m_bUpdateOperation;				///< ����X�V�t���O
	bool									m_bReadMode;					///< �ǂݍ��݃��[�h�t���O
	int										m_SelOprtPageId;				///< �I�𒆑���y�[�WID

	CPropTabPageParameter					m_PropResinPage;				///< �v���p�e�B�^�u�y�[�W�i�����ʗp�j
	CPropTabPageParameter					m_PropMetalPage;				///< �v���p�e�B�^�u�y�[�W�i�����ʗp�j
	CPropTabPageParameter					m_PropResultPage;				///< �v���p�e�B�^�u�y�[�W�i���ʗp�j
	CPropTabPageSetting						m_PropSettingPage;				///< �v���p�e�B�^�u�y�[�W�i�ݒ�p�j
	CTypedPtrArray< CObArray,CDialogEx *>	m_PropTab;						///< �v���p�e�B�^�u�y�[�W���X�g

	bool									m_bUpdatePropaty;				///< �v���p�e�B�X�V�t���O
	int										m_SelPropPageId;				///< �I�𒆃v���p�e�B�y�[�WID

	CGraphWind								*m_pGraphWnd;					///< �O���t�E�C���h�ւ̃|�C���^
	CImageWind								*m_pReginWnd;					///< �����ʕ\���E�C���h�ւ̃|�C���^
	CImageWind								*m_pMetalWnd;					///< �����ʕ\���E�C���h�ւ̃|�C���^
	CImageWind								*m_pResultWnd;					///< ���ʕ\���E�C���h�ւ̃|�C���^

	CSpectralDlg							*m_pSpectralDlg;				///< �X�y�N�g���\���_�C�A���O
	CRect									m_SpectralDlgRect;				///< �X�y�N�g���\���_�C�A���O�̈ʒu

	CProgressDlg							*m_pProgress;					///< �v���O���X�_�C�A���O�I�u�W�F�N�g

public:
	CListCtrl								m_lstRegTest;					///< �o�^�ς݃��X�g�pList�I�u�W�F�N�g
	CTabCtrl								m_tabPropaty;					///< �v���p�e�B�p�^�u�I�u�W�F�N�g
	CTabCtrl								m_tabOperation;					///< ����p�^�u�I�u�W�F�N�g
	CStatic									m_stcPropTabPageClient;			///< �v���p�e�B�y�[�W�\���G���A�p
	CStatic									m_stcOperationTabPageClient;	///< �����y�[�W�\���G���A�p
	CButton									m_btnPropTabCancel;				///< �v���p�e�B�^�u�ECancel�{�^���I�u�W�F�N�g
	CButton									m_btnPropTabOK;					///< �v���p�e�B�^�u�EOK�{�^���I�u�W�F�N�g

// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
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

#ifndef _DEBUG  // WeldEvaluationView.cpp �̃f�o�b�O �o�[�W����
/// <summary>
/// �h�L�������g�̎擾
/// </summary>
/// <returns>CWeldEvaluationDoc�ւ̃|�C���^��Ԃ�</returns>
inline CWeldEvaluationDoc* CWeldEvaluationView::GetDocument() const
   { return reinterpret_cast<CWeldEvaluationDoc*>(m_pDocument); }
#endif

