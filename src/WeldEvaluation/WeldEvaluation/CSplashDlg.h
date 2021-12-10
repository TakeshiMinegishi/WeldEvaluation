#pragma once


// CSplashDlg ダイアログ

class CSplashDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSplashDlg)

public:
	CSplashDlg(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CSplashDlg();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SPLASH_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
