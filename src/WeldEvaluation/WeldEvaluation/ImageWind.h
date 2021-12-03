#pragma once


/// <summary>
/// �`��E�C���h�O���X
/// </summary>
class CImageWind : public CDialog
{
	DECLARE_DYNAMIC(CImageWind)

public:
	/// <summary>
	/// �\���^�C�v��\���񋓌^
	/// </summary>
	enum {
		eResinSurface = 0,	///< ����
		eMetalSurface = 1,	///< ����
		eJoiningResult = 2	///< �ڍ�����
	};

public:
	CImageWind(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CImageWind();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_IMAGE_WIND };
private:
	bool		m_bActive;						///< �摜�\���L��
	CImage		*m_pImg;						///< �\���f�[�^
	CImage		m_Img;							///< �\���f�[�^
//	CRect		m_DrawArea;						///< �摜�\���G���A
	int			m_Type;							///< �Ώۃ^�C�v
	CPoint		m_pLButtonDownosPos;			///< ���{�^�������ʒu
	bool		m_bButtonDown;					///< ���{�^�������t���O
	bool		m_bAriaSelect;					///< �͈͑I���t���O�i��ԃX�y�N�g���p�j

	int			m_orgImageWidth;				///< �\���摜��
	int			m_orgImageHeight;				///< �\���摜����
	int			m_orgImageX;					///< �\���摜X�ʒu
	int			m_orgImageY;					///< �\���摜Y�ʒu

	int			m_imageWidth;					///< �\���摜��
	int			m_imageHeight;					///< �\���摜����
	int			m_imageX;						///< �\���摜X�ʒu
	int			m_imageY;						///< �\���摜Y�ʒu

	CPoint		m_deltaPos;						///< �ړ��_
	double		m_zoomRetio;					///< �Y�[����
	int			m_mode;							///< ���[�h

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

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
