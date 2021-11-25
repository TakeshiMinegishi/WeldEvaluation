// GraphWind.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "WeldEvaluation.h"
#include "GraphWind.h"
#include "afxdialogex.h"


// CGraphWind �_�C�A���O

IMPLEMENT_DYNAMIC(CGraphWind, CDialog)

#define BG_INIT_COLOR		RGB(0, 0, 0)
#define BG_COLOR			RGB(255, 255, 255)
#define GRAPH_LINE_COLOR	RGB(0, 0, 0)

/// <summary>
/// �R���X�g���N�^
/// </summary>
CGraphWind::CGraphWind(CWnd* pParent /*=NULL*/)
	: CDialog(CGraphWind::IDD, pParent)
{
	m_bActive		= false;
	m_bAutoRange	= false;
	m_offset		= 0.0;
	m_holMin		= 0.0;			///< ���������ŏ��l
	m_holMax		= 0.0;			///< ���������ő�l
	m_verMin		= 0.0;			///< ���������ŏ��l
	m_verMax		= 0.0;			///< ���������ŗ���
	m_pBmp			= nullptr;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
CGraphWind::~CGraphWind()
{
	if (m_pBmp) {
		delete m_pBmp;
		m_pBmp = nullptr;
	}
}

/// <summary>
/// �f�[�^�G�N�X�`�F���W
/// </summary>
/// <param name="pDX"CDataExchange�I�u�W�F�N�g�ւ̃|�C���^�[</param>
void CGraphWind::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGraphWind, CDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CGraphWind ���b�Z�[�W �n���h���[

/// <summary>
/// ����������
/// </summary>
BOOL CGraphWind::OnInitDialog() 
{
	m_bActive	= false;

	return TRUE;
}

/// <summary>
/// �`�揈��
/// </summary>
void CGraphWind::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����܂��B
	// �`�惁�b�Z�[�W�� CDialog::OnPaint() ���Ăяo���Ȃ��ł��������B
	//	CDialog::OnPaint();


	CRect rect;
	GetClientRect(&rect);

#if true
	m_DrawArea.top = rect.top + 10;
	m_DrawArea.left = rect.left + 10;
	m_DrawArea.bottom = rect.bottom - 10;
	m_DrawArea.right = rect.right - 10;

	CDC		*pDC = GetDC();
	CreateBackground(rect);
	DrawFrame(pDC, m_DrawArea);
	DrawGraph(pDC, m_DrawArea);
	ReleaseDC(pDC);
#else
//	CreateBackground(rect);
	if (m_pBmp == nullptr) {
		return;
	}

	m_DrawArea.top = rect.top + 10;
	m_DrawArea.left = rect.left + 10;
	m_DrawArea.bottom = rect.bottom - 10;
	m_DrawArea.right = rect.right - 10;

	CBitmap *bmp;
	CDC		*pDC = GetDC();
	CDC		cDC;				// �r�b�g�}�b�v�\���pDC
	BITMAP	bm;					// �r�b�g�}�b�v�I�u�W�F�N�g
	cDC.CreateCompatibleDC(pDC);
	if (m_pBmp) {
		CBitmap *oldBMP = cDC.SelectObject(m_pBmp);
		m_pBmp->GetBitmap(&bm);
		pDC->SetStretchBltMode(HALFTONE);
		pDC->SetStretchBltMode(COLORONCOLOR);
		pDC->StretchBlt(m_DrawArea.left, m_DrawArea.top, m_DrawArea.Width(), m_DrawArea.Height(), &cDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

		cDC.SelectObject(oldBMP);
	}
#endif
}

/// <summary>
/// �w�i�`��
/// </summary>
/// <param name="bgArea">�w�i�̈�</param>
void CGraphWind::CreateBackground(CRect &bgArea)
{
	CDC *dc = GetDC();
	if (dc == NULL)
	{
		return;
	}

	COLORREF BGCol;
	if (m_bActive) {
		BGCol = BG_COLOR;
	} else {
		BGCol = BG_INIT_COLOR;
	}
	CBrush blackBrush(BGCol);
	CBrush *orgBrush;
	orgBrush = dc->SelectObject(&blackBrush);
	dc->FillRect(&bgArea, &blackBrush);
	dc->SelectObject(orgBrush);
	ReleaseDC(dc);		
}

/// <summary>
/// �F������RGB�ւ̕ϊ�
/// </summary>
/// <param name="h">�F��</param>
/// <param name="col">RGB�l</param>
/// @remark �ʓx�A���x��100%�ŕϊ�
void CGraphWind::H2RGB(int h, COLORREF &col)
{
	BYTE r=0, g=0, b=0;
	double min = 0;
	double max = 255;
	if (h < 0) {
		h = h % 360;
		h = 360 + h;
	}
	if (h > 360) {
		h = h % 360;
	}
	double dh = (double)h;

	if (dh <= 60.) {
		r = (BYTE)max;
		g = (BYTE)((dh / 60.) * (max - min) + min);
		b = (BYTE)min;
	}
	else if (dh <= 120.) {
		r = (BYTE)(((120. - dh) / 60.) * (max - min) + min);
		g = (BYTE)max;
		b = (BYTE)min;
	}
	else if (dh <= 180.) {
		r = (BYTE)min;
		g = (BYTE)max;
		b = (BYTE)(((dh - 120.) / 60.) * (max - min) + min);
	}
	else if (dh <= 240.) {
		r = (BYTE)min;
		g = (BYTE)(((240. - dh) / 60.) * (max - min) + min);
		b = (BYTE)max;
	}
	else if (dh <= 300.) {
		r = (BYTE)(((dh - 240.) / 60.) * (max - min) + min);
		g = (BYTE)min;
		b = (BYTE)max;
	}
	else if (dh <= 360.) {
		r = (BYTE)max;
		g = (BYTE)min;
		b = (BYTE)(((360. - dh) / 60.) * (max - min) + min);
	}
	col = RGB(r, g, b);
}

/// <summary>
/// �O���t�`��
/// </summary>
void CGraphWind::DrawGraph()
{
	if (!m_bActive) {
		return;
	}

	CDC *dc = GetDC();
	if (dc == NULL)
	{
		return;
	}
	if (m_data.size() <= 0) {
		Erase();
		return;
	}

	CRect rect;
	int W = m_DrawArea.Width();
	int H = m_DrawArea.Height();

	double band = (double)m_data.size() * m_offset;
	double hight = m_verMax - m_verMin;
	double resolution = hight / (double)(H-band);
	double width = (double)m_data[0].size();
	double h_resoution = (double)W/ width;

	CPen* m_graphPen = new CPen;
	m_graphPen->CreatePen(PS_SOLID, 1, GRAPH_LINE_COLOR);

	int y,x;
	CPen* orgPen = dc->SelectObject(m_graphPen);
	int nGraph = (int)m_data.size();

	double lh = 100.0;
	double hh = 240.0;
	double hs = (hh - lh) / (double)nGraph;

	COLORREF dcol;
	double v = 0.0;
	int zeroLine = (int)(m_offset + 0.5)*(nGraph-1);
	for (int i = nGraph-1; i >= 0; i--) {
		H2RGB((int)(hh - (i*hs)+.5), dcol);
		CPen *graphPen = new CPen;
		graphPen->CreatePen(PS_SOLID, 1, dcol);
		dc->SelectObject(graphPen);

		int nData = (int)m_data[i].size();
		if (nData <= 0) {
			continue;
		}
		x = m_DrawArea.left + (int)(0*h_resoution+.5);
		v = m_data[i][0];
		if (v < m_verMin) v = m_verMin;
		else if (v > m_verMax) v = m_verMax;
		y = m_DrawArea.bottom -(int)((v+fabs(m_verMin))/resolution +.5);
		y += zeroLine;
		dc->MoveTo(x,y);
		for (int j = 1; j < nData; j++) {
			x = m_DrawArea.left + (int)(j*h_resoution+.5);
			v = m_data[i][j];
			if (v < m_verMin) v = m_verMin;
			else if (v > m_verMax) v = m_verMax;
			y = m_DrawArea.bottom -(int)((v+fabs(m_verMin))/resolution +.5);
			y -= zeroLine;
			if (y < m_DrawArea.top) {
				dc->MoveTo(x, y);
			} else {
				dc->LineTo(x, y);
			}
		}
		delete graphPen;
		graphPen = nullptr;
		zeroLine -= (int)(m_offset + 0.5);

	}
	dc->SelectObject(orgPen);
	delete m_graphPen;
	ReleaseDC(dc);
}

/// <summary>
/// �t���[���`��
/// </summary>
void CGraphWind::DrawFrame()
{
	if (!m_bActive) {
		return;
	}

	CDC *dc = GetDC();
	if (dc == NULL)
	{
		return;
	}
	CPen* framePen = new CPen;
	framePen->CreatePen(PS_SOLID, 2, GRAPH_LINE_COLOR);
	CPen* orgPen = dc->SelectObject(framePen);

	dc->MoveTo(m_DrawArea.left, m_DrawArea.top);
	dc->LineTo(m_DrawArea.left, m_DrawArea.bottom);
	dc->LineTo(m_DrawArea.right, m_DrawArea.bottom);

	dc->SelectObject(orgPen);
	delete framePen;
	ReleaseDC(dc);
}

/// <summary>
/// ���������̈�ݒ�i�����l�j
/// </summary>
/// <param name="min">�ŏ��l</param>
/// <param name="max">�ő�l</param>
void CGraphWind::SetHolizontalRange(double min, double max)
{
	m_holMin = min;
	m_holMax = max;
}

/// <summary>
/// ���������̈�擾�i�����l�j
/// </summary>
/// <param name="min">�ŏ��l</param>
/// <param name="max">�ő�l</param>
void CGraphWind::GetHolizontalRange(double &min, double& max)
{
	min = m_holMin;
	max = m_holMax;
}

/// <summary>
/// ���������̈�ݒ�i�����l�j
/// </summary>
/// <param name="min">�ŏ��l</param>
/// <param name="max">�ő�l</param>
void CGraphWind::SetVirticalRange(double min, double max)
{
	m_verMin = min;
	m_verMax = max;
}

/// <summary>
/// ���������̈�擾�i�����l�j
/// </summary>
/// <param name="min">�ŏ��l</param>
/// <param name="max">�ő�l</param>
void CGraphWind::GetVirticalRange(double &min, double& max)
{
	min = m_verMin;
	max = m_verMax;
}

/// <summary>
/// ���������̈攻��i�����l�j
/// </summary>
/// <param name="data">�f�[�^</param>
/// <param name="min">�ŏ��l</param>
/// <param name="max">�ő�l</param>
void CGraphWind::GetVerticalRange(std::vector<std::vector<double>> &data, double &min, double &max)
{
	min = max = 0.0;
	int nGraph = (int)data.size();
	if ((nGraph <= 0) || (data[0].size() <= 0)) {
		return;
	}

	min = max = data[0][0];
	int nData;
	for (int i = 0; i < nGraph; i++) {
		nData = (int)data[i].size();
		for (int j = 0; j < nData; j++) {
			if (min > data[i][j]) {
				min = data[i][j];
			}
			if (max < data[i][j]) {
				max = data[i][j];
			}
		}
	}

	double h = max - min;
	double b = h * 0.2;
	if (min < 0) {
		min -= b / 2;
		max += b / 2;
	} else {
		max += b;
	}
}

/// <summary>
/// �t���[���`��
/// </summary>
void CGraphWind::DrawFrame(CDC *pDC, CRect area)
{
	if (!m_bActive) {
		return;
	}

	if (pDC == NULL)
	{
		return;
	}
	CPen* framePen = new CPen;
	framePen->CreatePen(PS_SOLID, 2, GRAPH_LINE_COLOR);
	CPen* orgPen = pDC->SelectObject(framePen);

	pDC->MoveTo(area.left, area.top);
	pDC->LineTo(area.left, area.bottom);
	pDC->LineTo(area.right, area.bottom);

	pDC->SelectObject(orgPen);
	delete framePen;
}

/// <summary>
/// �O���t�`��
/// </summary>
void CGraphWind::DrawGraph(CDC *pDC, CRect area)
{
	if (!m_bActive) {
		return;
	}

	if (pDC == NULL)
	{
		return;
	}
	if (m_data.size() <= 0) {
		Erase();
		return;
	}

	CRect rect;
	int W = area.Width();
	int H = area.Height();

	double band = (double)m_data.size() * m_offset;
	double hight = m_verMax - m_verMin;
	double resolution = hight / (double)(H - band);
	double width = (double)m_data[0].size();
	double h_resoution = (double)W / width;

	CPen* m_graphPen = new CPen;
	m_graphPen->CreatePen(PS_SOLID, 1, GRAPH_LINE_COLOR);

	int y, x;
	CPen* orgPen = pDC->SelectObject(m_graphPen);
	int nGraph = (int)m_data.size();

	double lh = 0.0;
	double hh = 360.0;
	double hs = (hh - lh) / (double)nGraph;
	if (hs < 30.0) {	// �F����30�x������������A�F���J��Ԃ�
		hs = 30.0;
	}

	COLORREF dcol;
	double v = 0.0;
	int zeroLine = (int)(m_offset + 0.5)*(nGraph - 1);
	for (int i = nGraph - 1; i >= 0; i--) {
		H2RGB((int)(lh+hs*(nGraph-i-1)), dcol);
		CPen *graphPen = new CPen;
		graphPen->CreatePen(PS_SOLID, 1, dcol);
		pDC->SelectObject(graphPen);

		int nData = (int)m_data[i].size();
		if (nData <= 0) {
			continue;
		}
		x = area.left + (int)(0 * h_resoution + .5);
		v = m_data[i][0];
		if (v < m_verMin) v = m_verMin;
		else if (v > m_verMax) v = m_verMax;
		y = area.bottom - (int)((v + fabs(m_verMin)) / resolution + .5);
		y += zeroLine;
		if (y > area.bottom) {
			y = area.bottom;
		}
		else if (y > area.top) {
			y = area.top;
		}
		pDC->MoveTo(x, y);
		for (int j = 1; j < nData; j++) {
			x = area.left + (int)(j*h_resoution + .5);
			v = m_data[i][j];
			if (v < m_verMin) v = m_verMin;
			else if (v > m_verMax) v = m_verMax;
			y = area.bottom - (int)((v + fabs(m_verMin)) / resolution + .5);
			y -= zeroLine;
			if (y < area.top) {
				pDC->MoveTo(x, y);
			}
			else {
				pDC->LineTo(x, y);
			}
		}
		delete graphPen;
		graphPen = nullptr;
		zeroLine -= (int)(m_offset + 0.5);

	}
	pDC->SelectObject(orgPen);
	delete m_graphPen;
}

void CGraphWind::makeImage(CRect &rect)
{
	CDC     *pDC;
	CRect   Rect;
	CDC     DC;             // �ۑ��p�f�o�C�X�R���e�L�X�g

	if (m_pBmp) {
		delete m_pBmp;
	}
	m_pBmp = new CBitmap;
	pDC = GetDC();
	DC.CreateCompatibleDC(pDC);
	m_pBmp->CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	DC.SelectObject(*m_pBmp);

	DrawFrame(pDC, rect);
	DrawGraph(pDC, rect);

	DeleteObject(DC);
	ReleaseDC(pDC);
}

/// <summary>
/// �O���t�`��
/// </summary>
/// <param name="data">�f�[�^</param>
/// <param name="offset">�[�����ړ���[�s�N�Z���P��]</param>
/// <param name="AutoRange">���������W</param>
///@remark AutoRange��true�̏ꍇ���������̃����W�̓f�[�^�́i�ő�l�|�ŏ��l�j*1.2�Ƃ��Ď����ɋ��߂�
///@remark offset�͐��������̃O���t�ԃ[���������炷�ׂ̂���
void CGraphWind::Draw(std::vector<std::vector<double>> &data, double offset/*=0.0*/, bool AutoRange/*=false*/)
{
	m_bActive = true;
	m_bAutoRange = AutoRange;
	if (m_bAutoRange) {
		GetVerticalRange(data,m_verMin,m_verMax);
	}
	m_offset = offset;
	m_data = data;

	CRect rect;
	GetClientRect(&rect);
	m_DrawArea.top = rect.top + 10;
	m_DrawArea.left = rect.left + 10;
	m_DrawArea.bottom = rect.bottom - 10;
	m_DrawArea.right = rect.right - 10;
//	makeImage(m_DrawArea);

	OnPaint();
}

/// <summary>
/// �O���t�폜
/// </summary>
void CGraphWind::Erase()
{
	m_bActive = false;
	OnPaint();
}
