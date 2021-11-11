// ImageWind.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "WeldEvaluation.h"
#include "ImageWind.h"
#include "afxdialogex.h"
#include "message.h"
#include <vector>


// CImageWind �_�C�A���O

IMPLEMENT_DYNAMIC(CImageWind, CDialog)

#define BG_INIT_COLOR		RGB(0, 0, 0)		///< �����J���[�l
#define BG_COLOR			RGB(0, 0, 0)		///< �o�b�N�O���E���h�J���[

/// <summary>
/// �R���X�g���N�^
/// </summary>
CImageWind::CImageWind(CWnd* pParent /*=NULL*/)
	: CDialog(CImageWind::IDD, pParent)
{
	m_bActive		= false;
	m_Type			= eResinSurface;
	m_mode			= 0;
	m_bButtonDown	= false;
	m_bAriaSelect	= false;
	m_pImg			= nullptr;
	m_zoomRetio		= 1.0;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
CImageWind::~CImageWind()
{
	if (m_pImg) {
		delete m_pImg;
		m_pImg = nullptr;
	}
}

/// <summary>
/// �f�[�^�G�N�X�`�F���W
/// </summary>
/// <param name="pDX"CDataExchange�I�u�W�F�N�g�ւ̃|�C���^�[</param>
void CImageWind::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CImageWind, CDialog)
	ON_WM_PAINT()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEACTIVATE()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEWHEEL()
	ON_WM_NCDESTROY()
END_MESSAGE_MAP()


// CImageWind ���b�Z�[�W �n���h���[

/// <summary>
/// ����������
/// </summary>
BOOL CImageWind::OnInitDialog()
{
	m_bActive	= false;
	m_pImg		= new CImage();
	return TRUE;
}

/// <summary>
/// �E�C���h�j���O����
/// </summary>
void CImageWind::OnNcDestroy()
{
	CDialog::OnNcDestroy();

	if (m_pImg) {
		m_pImg->Detach();
		m_pImg->Destroy();
		delete m_pImg;
		m_pImg = nullptr;
	}
}

/// <summary>
/// �`�揈��
/// </summary>
void CImageWind::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rect;
	GetClientRect(&rect);
	CreateBackground(rect);

	if (m_bActive) {
		if (m_pImg == nullptr) {
			return;
		}

		CBitmap *bmp;
		CDC		*pDC = GetDC();
		CDC		cDC;				// �r�b�g�}�b�v�\���pDC
		BITMAP	bm;					// �r�b�g�}�b�v�I�u�W�F�N�g
		cDC.CreateCompatibleDC(pDC);
		bmp = CBitmap::FromHandle(*m_pImg);
		if (bmp) {
			CBitmap *oldBMP = cDC.SelectObject(bmp);
			bmp->GetBitmap(&bm);
			pDC->SetStretchBltMode(HALFTONE);
			pDC->SetStretchBltMode(COLORONCOLOR);
			pDC->StretchBlt(m_imageX, m_imageY, m_imageWidth, m_imageHeight, &cDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
			TRACE(_T("OnPaint(%d) X=%d Y=%d, W=%d H=%d\n"), m_Type, m_imageX, m_imageY, m_imageWidth, m_imageHeight);

			cDC.SelectObject(oldBMP);
		}
	}
}


/// <summary>
/// �}�E�X���{�^���_�u���N���b�N������
/// </summary>
/// <param name="nFlags">���z�L�[�t���O</param>
/// <param name="point">�J�[�\�����W</param>
/// <returns>�����̏ꍇ��TRUE�A���s�̏ꍇ��FALSE��Ԃ�</returns>
/// @remark ���z�L�[�t���OnFlags��MK_CONTROL CTRL �L�[�AMK_LBUTTON �}�E�X�̍��{�^���AMK_MBUTTON �}�E�X�̒����{�^���AMK_RBUTTON �}�E�X�̉E�{�^���AMK_SHIFT SHIFT �L�[
void CImageWind::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	m_bButtonDown = false;
	m_bAriaSelect = false;

	CDialog::OnLButtonDblClk(nFlags, point);
}


/// <summary>
/// �}�E�X���{�^�����グ������
/// </summary>
/// <param name="nFlags">���z�L�[�t���O</param>
/// <param name="point">�J�[�\�����W</param>
/// <returns>�����̏ꍇ��TRUE�A���s�̏ꍇ��FALSE��Ԃ�</returns>
/// @remark ���z�L�[�t���OnFlags��MK_CONTROL CTRL �L�[�AMK_LBUTTON �}�E�X�̍��{�^���AMK_MBUTTON �}�E�X�̒����{�^���AMK_RBUTTON �}�E�X�̉E�{�^���AMK_SHIFT SHIFT �L�[
void CImageWind::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_bButtonDown && m_bActive) {
		m_bButtonDown = false;
		double dist = sqrt((point.x - m_pLButtonDownosPos.x)*(point.x - m_pLButtonDownosPos.x) + (point.y - m_pLButtonDownosPos.y)*(point.y - m_pLButtonDownosPos.y));
		if (dist < 2.0) {
			CRect rect;
			GetWindowRect(rect);

			double wRetio = (double)m_imageWidth / (double)rect.Width();
			double hRetio = (double)m_imageHeight / (double)rect.Height();

			CPoint pos;
			pos.x = (int)((double)point.x * wRetio);
			pos.y = (int)((double)point.y * hRetio);
		}
		else {
			if (m_mode == 1) {
				m_bAriaSelect = false;
			}

			if (nFlags == MK_CONTROL) {
//				TRACE(_T("Zoom (%d,%d)-(%d,%d)\n"), m_pLButtonDownosPos.x, m_pLButtonDownosPos.y, point.x, point.y);
			} else {
				AfxGetMainWnd()->GetActiveWindow()->SendMessage(WM_AREA_SPECTRUM_GRAPH_REQUEST, (WPARAM)&m_pLButtonDownosPos, (LPARAM)&point);
//				TRACE(_T("Line (%d,%d)-(%d,%d)\n"), m_pLButtonDownosPos.x, m_pLButtonDownosPos.y, point.x, point.y);
			}
		}
	}
//	TRACE(_T("LButton UP\n"));
	CDialog::OnLButtonUp(nFlags, point);
}

/// <summary>
/// �}�E�X���{�^������������
/// </summary>
/// <param name="nFlags">���z�L�[�t���O</param>
/// <param name="point">�J�[�\�����W</param>
/// <returns>�����̏ꍇ��TRUE�A���s�̏ꍇ��FALSE��Ԃ�</returns>
/// @remark ���z�L�[�t���OnFlags��MK_CONTROL CTRL �L�[�AMK_LBUTTON �}�E�X�̍��{�^���AMK_MBUTTON �}�E�X�̒����{�^���AMK_RBUTTON �}�E�X�̉E�{�^���AMK_SHIFT SHIFT �L�[
void CImageWind::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_bActive) {
		if (m_mode == 1) {
			if (m_bAriaSelect) {
				m_bAriaSelect = false;
				std::vector<CPoint> pos;
				CPoint cpt = m_pLButtonDownosPos;
				clientToScan(cpt);
				pos.push_back(cpt);
				cpt = point;
				clientToScan(cpt);
				pos.push_back(cpt);
				GetParent()->SendMessage(WM_AREA_SPECTRUM_GRAPH_SET, (WPARAM)m_Type, (LPARAM)&pos);
				pos.clear();
			}
			else {
				m_bButtonDown = true;
				m_bAriaSelect = true;
				m_pLButtonDownosPos = point;
				m_deltaPos = point;
			}
		}
		else {
			m_bButtonDown = true;
			m_pLButtonDownosPos = point;
			m_deltaPos = point;
		}
	}
	CDialog::OnLButtonDown(nFlags, point);
}

/// <summary>
/// �}�E�X�ړ�������
/// </summary>
/// <param name="nFlags">���z�L�[�t���O</param>
/// <param name="point">�J�[�\�����W</param>
/// <returns>�����̏ꍇ��TRUE�A���s�̏ꍇ��FALSE��Ԃ�</returns>
/// @remark ���z�L�[�t���OnFlags��MK_CONTROL CTRL �L�[�AMK_LBUTTON �}�E�X�̍��{�^���AMK_MBUTTON �}�E�X�̒����{�^���AMK_RBUTTON �}�E�X�̉E�{�^���AMK_SHIFT SHIFT �L�[
void CImageWind::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bActive) {
		if (m_bButtonDown) {
			CPoint delta;
			delta.x = m_deltaPos.x - point.x;
			delta.y = m_deltaPos.y - point.y;

			int x = m_imageX;
			int y = m_imageY;
			CRect rect;
			GetClientRect(rect);
			if (m_imageWidth > rect.Width()) {
				m_imageX -= delta.x;
				if (m_imageX > 0) {
					m_imageX = 0;
				}
				else if ((rect.Width() - m_imageWidth) > m_imageX) {
					m_imageX = (rect.Width() - m_imageWidth);
				}
			}

			if (m_imageHeight > 0) {
				m_imageY -= delta.y;
				if (m_imageY > 0) {
					m_imageY = 0;
				}
				else if ((rect.Height() - m_imageHeight)  > m_imageY) {
					m_imageY = (rect.Height() - m_imageHeight);
				}
//				TRACE(_T("imageY:%d Height:%d-%d\n"), m_imageY, m_imageHeight, rect.Height());
			}
			if ((m_deltaPos.x != point.x) || (m_deltaPos.y != point.y)) {
				m_deltaPos = point;
				if ((x != m_imageX) || (y != m_imageY)) {
					OnPaint();
					CRect ImageRect(m_imageX, m_imageY, m_imageX + m_imageWidth, m_imageY + m_imageHeight);
					GetParent()->SendMessage(WM_IMAGE_MOVEING, (WPARAM)m_Type, (LPARAM)&ImageRect);
				}
			}
		}
		else {
			CPoint pos = point;
			if (clientToScan(pos)) {
//				TRACE(_T("Pos(%d,%d)->(%d,%d)\n"), point.x, point.y, pos.x, pos.y);
				GetParent()->SendMessage(WM_SPECTRUM_GRAPH_REQUEST, (WPARAM)m_Type, (LPARAM)&pos);
			}
		}
	}

#if 0
	if (m_bButtonDown && m_bActive) {
		if (nFlags == MK_CONTROL) {
//			TRACE(_T("OnMouseMove(%d,%d)\n"), point.x, point.y);
		}
	}
#endif
	CDialog::OnMouseMove(nFlags, point);
}

/// <summary>
/// �}�E�X�z�C�[������]������
/// </summary>
/// <param name="nFlags">���z�L�[�t���O</param>
/// <param name="zDelta">��]����</param>
/// <param name="pt">�J�[�\�����W</param>
/// <returns>�����̏ꍇ��TRUE�A���s�̏ꍇ��FALSE��Ԃ�</returns>
/// @remark ���z�L�[�t���OnFlags��MK_CONTROL CTRL �L�[�AMK_LBUTTON �}�E�X�̍��{�^���AMK_MBUTTON �}�E�X�̒����{�^���AMK_RBUTTON �}�E�X�̉E�{�^���AMK_SHIFT SHIFT �L�[
/// @remark ��]����zDelta �l��120�̔{��
BOOL CImageWind::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
#if 0
	if (m_bActive) {
		CPoint pos = pt;
		ScreenToClient(&pos);

		CRect rect;
		GetClientRect(&rect);
		CPoint delta;
		delta.x = pos.x - rect.Width() / 2;
		delta.y = pos.y - rect.Height() / 2;
		m_imageX -= delta.x;
		m_imageY -= delta.y;

		m_zoomRetio += ((double)zDelta / 120.0) * 0.1;
		m_imageWidth = (int)(m_orgImageWidth * m_zoomRetio);
		m_imageHeight = (int)(m_orgImageHeight * m_zoomRetio);;

		if (m_imageWidth > m_imageHeight) {
			if (m_imageHeight < rect.Height()) {
				m_zoomRetio = 1.0;
				m_imageWidth = m_orgImageWidth;
				m_imageHeight = m_orgImageHeight;
				m_imageY = (int)((rect.Height() - m_imageHeight) / 2.0);
			}
			if (m_imageWidth < rect.Width()) {
				m_imageX = (int)((rect.Width() - m_imageWidth) / 2.0);
			}
			else {

			}
		}
		else {
			if (m_imageHeight < rect.Height()) {
				m_imageY = (int)((rect.Height() - m_imageHeight) / 2.0);
			}
			if (m_imageWidth < rect.Width()) {
				m_zoomRetio = 1.0;
				m_imageWidth = m_orgImageWidth;
				m_imageHeight = m_orgImageHeight;
				m_imageX = (int)((rect.Width() - m_imageWidth) / 2.0);
			}
		}

		if (m_imageWidth > rect.Width()) {
			if (m_imageX > 0) {
				m_imageX = 0;
			}
			else if ((rect.Width() - m_imageWidth) > m_imageX) {
				m_imageX = (rect.Width() - m_imageWidth);
			}
		}

		if (m_imageHeight > 0) {
			if (m_imageY > 0) {
				m_imageY = 0;
			}
			else if ((rect.Height() - m_imageHeight) > m_imageY) {
				m_imageY = (rect.Height() - m_imageHeight);
			}
		}

//		TRACE(_T("pos(%d,%d) Width= %d height = %d (%lf)\n"), pos.x, pos.y, m_imageWidth, m_imageHeight, m_zoomRetio);
		OnPaint();
	}
#else
	if (m_bActive) {
		CPoint pos = pt;
		ScreenToClient(&pos);

		CRect rect;
		GetClientRect(&rect);
		CPoint delta;
		delta.x = pos.x - rect.Width() / 2;
		delta.y = pos.y - rect.Height() / 2;
		pos.x = m_imageX - delta.x;
		pos.y = m_imageY - delta.y;
		double scalingRetio = m_zoomRetio + ((double)zDelta / 120.0) * 0.1;

		Scaling(scalingRetio, pos);
		CRect ImageRect(m_imageX, m_imageY, m_imageX + m_imageWidth, m_imageY + m_imageHeight);
		GetParent()->SendMessage(WM_IMAGE_SCALING, (WPARAM)m_Type, (LPARAM)&ImageRect);
	}
#endif
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

/// <summary>
/// �}�E�X�|�C���^���N���C�A���g�̈悩��o���ʒm
/// </summary>
void CImageWind::OnMouseLeave()
{
	if (m_bButtonDown) {
		m_bAriaSelect = false;
		m_bButtonDown = false;
	}
	CDialog::OnMouseLeave();
}


////////////////////////////////////////////////////////////////////////////////////////
//
//

/// <summary>
/// CImage�I�u�W�F�N�g�̎擾
/// </summary>
/// <param name="bDetach">Detach�̎��s�L��</param>
/// <returns>CImage�I�u�W�F�N�g�ւ̃|�C���^��Ԃ�</returns>
CImage *CImageWind::GetImage(bool bDetach/* = true*/)
{
	if (m_pImg && bDetach) {
		if (!m_pImg->IsNull()) {
			m_pImg->Detach();
		}
	}
	return m_pImg;
}

/// <summary>
/// �w�i�`��
/// </summary>
/// <param name="bgArea">�w�i�̈�</param>
void CImageWind::CreateBackground(CRect &bgArea)
{
	CDC *dc = GetDC();
	if (dc == NULL)
	{
		return;
	}

	COLORREF BGCol;
	if (m_bActive) {
		BGCol = BG_COLOR;
	}
	else {
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
/// �\���E�C���h�^�C�v�̐ݒ�
/// </summary>
/// <param name="type">�\���E�C���h�^�C�v</param>
void CImageWind::setType(int type)
{
	m_Type = type;
}

/// <summary>
/// ���[�h�̐ݒ�
/// </summary>
/// <param name="mode">�\���E�C���h�^�C�v</param>
/// @remark 0:�m�[�}�����[�h�A1:��ԃX�y�N�g�����[�h
void CImageWind::setMode(int mode)
{
	m_mode = mode;
}

/// <summary>
/// �摜�̍폜
/// </summary>
void CImageWind::Erase()
{
	m_bActive = false;
	m_zoomRetio = 1.0;
	m_imageY = (int)0.0;
	m_imageX = (int)0.0;
	OnPaint();
}

/// <summary>
/// �摜�̕`��
/// </summary>
void CImageWind::Draw()
{
	CBitmap *virtualBmp;
	CDC *pDC = GetDC();
	CDC virtualDC;			// �r�b�g�}�b�v�\���pDC
	BITMAP bm;				// �r�b�g�}�b�v�I�u�W�F�N�g

	CRect sizeRect;
	GetClientRect(sizeRect);
	virtualBmp = CBitmap::FromHandle(*m_pImg);
	virtualDC.CreateCompatibleDC(pDC);
	CBitmap *oldBMP = virtualDC.SelectObject(virtualBmp);

	virtualBmp->GetBitmap(&bm);

	//--���W���Z
	double flameAspect = (double)sizeRect.Height() / (double)sizeRect.Width();
	double imageAspect = (double)bm.bmHeight / (double)bm.bmWidth;
	m_orgImageWidth = 0;
	m_orgImageHeight = 0;
	m_orgImageX = 0;
	m_orgImageY = 0;

	if (flameAspect > imageAspect) {//--�t���[���A�X�y�N�g�䁄�摜�A�X�y�N�g��
		// ���
		m_orgImageWidth = sizeRect.Width();
		m_orgImageHeight = (int)((double)bm.bmHeight*((double)sizeRect.Width() / (double)bm.bmWidth));
		m_orgImageX = 0;
		m_orgImageY = ((sizeRect.Height() - m_orgImageHeight) / 2);
	}
	else {
		// �c�
		m_orgImageWidth = (int)((double)bm.bmWidth*((double)sizeRect.Height() / (double)bm.bmHeight));
		m_orgImageHeight = sizeRect.Height();
		m_orgImageX = ((sizeRect.Width() - m_orgImageWidth) / 2);
		m_orgImageY = 0;
	}

	m_imageWidth = (int)(m_orgImageWidth * m_zoomRetio);
	m_imageHeight = (int)(m_orgImageHeight * m_zoomRetio);
	m_imageX = m_orgImageX;
	m_imageY = m_orgImageY;

	pDC->SetStretchBltMode(HALFTONE);
	pDC->SetStretchBltMode(COLORONCOLOR);
	pDC->StretchBlt(m_imageX, m_imageY, m_imageWidth, m_imageHeight, &virtualDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

	virtualDC.SelectObject(oldBMP);
	m_bActive = true;
}

/// <summary>
/// �摜�̍Ĕz�u
/// </summary>
/// <param name="x">��X�_</param>
/// <param name="y">��Y�_</param>
/// <param name="width">��</param>
/// <param name="height">����</param>
/// <param name="scaingRetio">�X�P�[����</param>
bool CImageWind::MoveImage(int x, int y, int width, int height, double scaingRetio)
{
	m_imageX		= x;
	m_imageY		= y;
	m_imageWidth	= width;
	m_imageHeight	= height;
	m_zoomRetio		= scaingRetio;

	TRACE(_T("Type:%d pos(%d,%d) IW= %d IH = %d (%lf)\n"),m_Type, x, y, m_imageWidth, m_imageHeight, m_zoomRetio);
	OnPaint();
	return true;
}

/// <summary>
/// �摜�̃X�P�[�����O
/// </summary>
/// <param name="ScalingRetio">�X�P�[����</param>
/// <param name="pt">�摜�ʒu</param>
void CImageWind::Scaling(double ScalingRetio, CPoint pt)
{
	if (!m_bActive) {
		return;
	}

	CRect rect;
	GetClientRect(&rect);
	int imageX = pt.x;
	int imageY = pt.y;
	double zoomRetio = ScalingRetio;

	int imageWidth = (int)(m_orgImageWidth * zoomRetio);
	int imageHeight = (int)(m_orgImageHeight * zoomRetio);

	if (imageWidth > imageHeight) {
		if (imageHeight < rect.Height()) {
			zoomRetio = 1.0;
			imageWidth = m_orgImageWidth;
			imageHeight = m_orgImageHeight;
			imageY = (int)((rect.Height() - imageHeight) / 2.0);
		}
		if (imageWidth < rect.Width()) {
			imageX = (int)((rect.Width() - imageWidth) / 2.0);
		}
		else {

		}
	}
	else {
		if (imageHeight < rect.Height()) {
			imageY = (int)((rect.Height() - imageHeight) / 2.0);
		}
		if (imageWidth < rect.Width()) {
			zoomRetio = 1.0;
			imageWidth = m_orgImageWidth;
			imageHeight = m_orgImageHeight;
			imageX = (int)((rect.Width() - imageWidth) / 2.0);
		}
	}

	if (imageWidth > rect.Width()) {
		if (imageX > 0) {
			imageX = 0;
		}
		else if ((rect.Width() - imageWidth) > m_imageX) {
			imageX = (rect.Width() - imageWidth);
		}
	}

	if (m_imageHeight > 0) {
		if (imageY > 0) {
			imageY = 0;
		}
		else if ((rect.Height() - imageHeight) > imageY) {
			imageY = (rect.Height() - imageHeight);
		}
	}
	MoveImage(imageX, imageY, imageWidth, imageHeight, zoomRetio);
}

/// <summary>
/// �X�P�[�����O���̎擾
/// </summary>
/// <param name="ScalingRetio">�X�P�[����</param>
/// <param name="pt">�摜�ʒu</param>
/// <returns>�����̏ꍇ��true�A���s�̏ꍇ��false��Ԃ�</returns>
bool CImageWind::GetScalingInfo(double &ScalingRetio, CPoint &pt)
{
	if (!m_bActive) {
		return false;
	}
	pt.x = m_imageX;
	pt.y = m_imageY;
	ScalingRetio = m_zoomRetio;

	return true;
}

/// <summary>
/// �N���C�A���g���W����X�L�����摜���W�ւ̕ϊ�
/// </summary>
/// <param name="pos">(IN)�N���C�A���g���W/(OUT)�X�L�����摜���W</param>
/// <returns>�����̏ꍇ��true�A���s�̏ꍇ��false��Ԃ�</returns>
bool CImageWind::clientToScan(CPoint &pos)
{
	CPoint point = pos;
	if (((point.x < m_imageX) || (point.x >= (m_imageX + m_imageWidth))) ||
		((point.y < m_imageY) || (point.y >= (m_imageY + m_imageHeight)))) {
		return false;
	}
	point.x = (point.x - m_imageX) * m_pImg->GetHeight() / m_imageHeight;
	point.y = (point.y - m_imageY) * m_pImg->GetWidth() / m_imageWidth;
	pos = point;
	return true;
}

/// <summary>
/// �X�L�����摜���W����N���C�A���g���W�ւ̕ϊ�
/// </summary>
/// <param name="pos">(IN)�X�L�����摜���W/(OUT)�N���C�A���g���W</param>
/// <returns>�����̏ꍇ��true�A���s�̏ꍇ��false��Ԃ�</returns>
bool CImageWind::ScanToclient(CPoint &pos)
{
	CPoint point = pos;
	pos = point;
	return true;
}

/// <summary>
/// �摜�T�C�Y�̐ݒ�
/// </summary>
/// <param name="rect">�摜�T�C�Y</param>
void CImageWind::SetImagePos(CRect &rect)
{
	m_imageX		= rect.left;		///< �\���摜X�ʒu
	m_imageY		= rect.top;			///< �\���摜Y�ʒu
	m_imageWidth	= rect.Width();		///< �\���摜��
	m_imageHeight	= rect.Height();	///< �\���摜����
}

/// <summary>
/// �_�̈ʒu���㉺���]������
/// </summary>
/// <param name="rect">�_�̈ʒu</param>
/// <returns>���]�������W�_��Ԃ�</returns>
CPoint CImageWind::ConvertImagePos(CPoint pos)
{
	CRect rect;
	CPoint rpos;
	GetWindowRect(rect);

	rpos.x = pos.x;
//	rpos.y = rect.Height() - (m_imageHeight + pos.y);
//	rpos.y = m_imageHeight - rect.Height() - pos.y;
	rpos.y = rect.Height() + pos.y - m_imageHeight - m_imageY;
	TRACE(_T("Type:%d Y:%d->%d VH=%d IH=%d IY=%d\n"),m_Type, pos.y,rpos.y, rect.Height(), m_imageHeight, m_imageY);
	return rpos;
}
