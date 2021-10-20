#pragma once
#include	"resource.h"
#include	"GraphWind.h"
#include	<vector>


// CSpectralDlg ダイアログ

class CSpectralDlg : public CDialog
{
	DECLARE_DYNAMIC(CSpectralDlg)
private:
	CWnd				*m_pParemt;						///< 親ウインド
	CGraphWind			*m_pGraphWnd;					///< グラフウインドへのポインタ
	CRect				m_orgClientSize;
	CRect				m_bfClientSize;
	std::vector<std::vector<double>>	m_data;

public:
	CSpectralDlg(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CSpectralDlg();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SPECTRAL_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

public:
	void draw(std::vector<std::vector<double>> data);
};
