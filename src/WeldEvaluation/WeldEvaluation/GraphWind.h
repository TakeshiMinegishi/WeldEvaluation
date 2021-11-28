#pragma once

#include <vector>

/// <summary>
/// CGraphWind �_�C�A���O
/// </summary>
class CGraphWind : public CDialog
{
	DECLARE_DYNAMIC(CGraphWind)

public:
	CGraphWind(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CGraphWind();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_GRAPH_WIND };

private:
	bool		m_bActive;						///< �O���t�\���L��
	bool		m_bAutoRange;					///< ���������̎��������W�i�ő�l��1.2�����ő�l�j
	double		m_offset;						///< �[�����I�t�Z�b�g�l
	CRect		m_DrawArea;						///< �O���t�\���G���A�i�E�C���h�g�ł͖����O���t�̕\���̈�j
	double		m_holMin;						///< ���������ŏ��l
	double		m_holMax;						///< ���������ő�l
	double		m_verMin;						///< ���������ŏ��l
	double		m_verMax;						///< ���������ŗ���
	CBitmap		*m_pBmp;
	std::vector<std::vector<double>> m_data;	///< �\���f�[�^

	CString		YMinLabel;						///< Y���ŏ��l���x��
	CString		YMaxLabel;						///< Y���ő�l���x��
	CString		XMinLabel;						///< X���ŏ��l���x��
	CString		XMaxLabel;						///< X���ő�l���x��

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

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
