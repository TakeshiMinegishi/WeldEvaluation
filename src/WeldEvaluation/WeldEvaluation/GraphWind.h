#pragma once

#include <vector>

/// <summary>
/// CGraphWind ダイアログ
/// </summary>
class CGraphWind : public CDialog
{
	DECLARE_DYNAMIC(CGraphWind)

public:
	CGraphWind(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CGraphWind();

// ダイアログ データ
	enum { IDD = IDD_GRAPH_WIND };

private:
	bool		m_bActive;						///< グラフ表示有無
	bool		m_bAutoRange;					///< 垂直方向の自動レンジ（最大値の1.2割が最大値）
	double		m_offset;						///< ゼロ線オフセット値
	CRect		m_DrawArea;						///< グラフ表示エリア（ウインド枠では無くグラフの表示領域）
	double		m_holMin;						///< 水平方向最小値
	double		m_holMax;						///< 水平方向最大値
	double		m_verMin;						///< 垂直方向最小値
	double		m_verMax;						///< 垂直方向最立ち
	CBitmap		*m_pBmp;
	std::vector<std::vector<double>> m_data;	///< 表示データ

	CString		YMinLabel;						///< Y軸最小値ラベル
	CString		YMaxLabel;						///< Y軸最大値ラベル
	CString		XMinLabel;						///< X軸最小値ラベル
	CString		XMaxLabel;						///< X軸最大値ラベル

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();

private:
	void CreateBackground(CRect &bgArea);
	void H2RGB(int h, COLORREF &col);
	void DrawGraph();
	void DrawFrame();
	void GetVerticalRange(std::vector<std::vector<double>> &data, double &min, double &max);
	void makeImage(CRect &rect);
	void DrawFrame(CDC *pDC, CRect area);
	void DrawLabel(CDC *pDC, CRect area);
	void DrawGraph(CDC *pDC, CRect area);

public:
	void SetHolizontalRange(double min, double max);
	void GetHolizontalRange(double &min, double& max);
	void SetVirticalRange(double min, double max);
	void GetVirticalRange(double &min, double& max);
	void Draw(std::vector<std::vector<double>> &data, double offset = 0.0, bool AutoRange=false);
	void Erase();
	void SetXLabel(CString min, CString max);
	void SetYLabel(CString min, CString max);
};
