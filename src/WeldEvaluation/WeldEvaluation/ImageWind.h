#pragma once


/// <summary>
/// 描画ウインドグラス
/// </summary>
class CImageWind : public CDialog
{
	DECLARE_DYNAMIC(CImageWind)

public:
	/// <summary>
	/// 表示タイプを表す列挙型
	/// </summary>
	enum {
		eResinSurface = 0,	///< 樹脂
		eMetalSurface = 1,	///< 金属
		eJoiningResult = 2	///< 接合結果
	};

public:
	CImageWind(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CImageWind();

// ダイアログ データ
	enum { IDD = IDD_IMAGE_WIND };
private:
	bool		m_bActive;						///< 画像表示有無
	CImage		*m_pImg;						///< 表示データ
	CImage		m_Img;							///< 表示データ
//	CRect		m_DrawArea;						///< 画像表示エリア
	int			m_Type;							///< 対象タイプ
	CPoint		m_pLButtonDownosPos;			///< 左ボタン押下位置
	bool		m_bButtonDown;					///< 左ボタン押下フラグ
	bool		m_bAriaSelect;					///< 範囲選択フラグ（区間スペクトル用）

	int			m_orgImageWidth;				///< 表示画像幅
	int			m_orgImageHeight;				///< 表示画像高さ
	int			m_orgImageX;					///< 表示画像X位置
	int			m_orgImageY;					///< 表示画像Y位置

	int			m_imageWidth;					///< 表示画像幅
	int			m_imageHeight;					///< 表示画像高さ
	int			m_imageX;						///< 表示画像X位置
	int			m_imageY;						///< 表示画像Y位置

	CPoint		m_deltaPos;						///< 移動点
	double		m_zoomRetio;					///< ズーム比
	int			m_mode;							///< モード

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnNcDestroy();

private:
	void CreateBackground(CRect &bgArea);

public:
	void setType(int type);
	void setMode(int mode);
	void Erase(bool bClrPrm=true);
	void Reset();
	void Draw();
	bool clientToScan(CPoint &pos);
	bool ScanToclient(CPoint &pos);
	void SetImagePos(CRect &rect);
	void Scaling(double ScalingRetio, CPoint pt);
	bool GetScalingInfo(double &ScalingRetio, CPoint &pt);
	bool MoveImage(int x, int y, int width, int height, double scaingRetio);
	CPoint ConvertImagePos(CPoint pos);

	CImage *GetImage(bool bDetach = true);
};
