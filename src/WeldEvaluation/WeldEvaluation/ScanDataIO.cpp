#include "stdafx.h"
#include "ScanDataIO.h"
#include "FileUtil.h"
#include <iostream>
#include <vector>


#define dRSpectrum	700.0		///< �Ԃ̃X�y�N�g���l
#define dGSpectrum	546.1		///< �΂̃X�y�N�g���l
#define dBSpectrum	435.8		///< �̃X�y�N�g���l

/// <summary>
/// �R���X�g���N�^
/// </summary>
CScanDataIO::CScanDataIO(void)
{
	m_pathName = _T("");
	m_o_p_cube = nullptr;
}


/// <summary>
/// �f�X�g���N�^
/// </summary>
CScanDataIO::~CScanDataIO(void)
{
	release();
}

/// <summary>
/// �I�[�v��
/// </summary>
/// <param name="pathName">�X�L�����f�[�^�̃t���p�X��</param>
/// <param name="reload">�ēǍ��t���O</param>
/// <returns>���������ꍇ��true,���s�����ꍇ��false��Ԃ�</returns>
/// @remark bReload��false�̏ꍇ�́A���f�[�^���J���Ă���ꍇ�͉��������ɏI������
bool CScanDataIO::open(CString pathName, bool bReload/*=false*/ )
{
	if (!CFileUtil::fileExists(pathName)) {
		return false;
	}

	// �ēǍ��L���̃`�F�b�N
	if (!bReload && (m_pathName.Compare(pathName) == 0)) {
		if (m_o_p_cube) {
			return true;
		}
	}

	// ���ɊJ���Ă�f�[�^�����
	close();

	m_o_p_cube = new CubeFloat();
	HSI_RETURN result = commonLoadCube(m_o_p_cube, pathName);
	if (result != HSI_OK) {
		if (result == HSI_ARGUMENT_INVALID) {
		}
		else if (result == HSI_FILE_IO_ERROR) {
		}
		close();
		return false;
	}
	m_pathName = pathName;
	return true;
}

/// <summary>
/// CubeFloat�̃N���[�Y
/// </summary>
void CScanDataIO::close()
{
	if (m_o_p_cube) {
		m_pathName = _T("");
		commonDeallocateCube(m_o_p_cube);
	}
}

/// <summary>
/// CubeFloat�̉��
/// </summary>
void CScanDataIO::release()
{
	if (m_o_p_cube) {
		close();
		delete m_o_p_cube;
		m_o_p_cube = nullptr;
	}
}

/// <summary>
/// ���K���l�擾�X���b�h�֐�
/// </summary>
/// <param name="width">Normalizer�\���̂ւ̃|�C���^</param>
/// <returns>���������ꍇ��0��Ԃ�</returns>
UINT __cdecl CScanDataIO::getNomalizer(LPVOID p)
{
	Normalizer* pnrm = (Normalizer*)p;
	float min, max;

	min = pnrm->m_o_p_cube->ppp_data[pnrm->m_band][0][0];
	max = pnrm->m_o_p_cube->ppp_data[pnrm->m_band][0][0];
	for (int row = 0; row < pnrm->m_row; row++) {
		for (int col = 0; col < pnrm->m_col; col++) {
			if (min > pnrm->m_o_p_cube->ppp_data[pnrm->m_band][row][col]) {
				min = pnrm->m_o_p_cube->ppp_data[pnrm->m_band][row][col];
			}
			if (max < pnrm->m_o_p_cube->ppp_data[pnrm->m_band][row][col]) {
				max = pnrm->m_o_p_cube->ppp_data[pnrm->m_band][row][col];
			}
		}
	}
	pnrm->m_normalizer = (float)255.0 / (float)(max - min);
	pnrm->m_offset = min;
	return 0;
}

/// <summary>
/// ���K���l�擾�X���b�h�����`���[
/// </summary>
/// <param name="width">Normalizer�\���̂ւ̃|�C���^</param>
void CScanDataIO::executeNomalizerLauncher(void* args)
{
	reinterpret_cast<CScanDataIO*>(args)->getNomalizer(args);
}


/// <summary>
/// �摜�f�[�^�擾
/// </summary>
/// <param name="pathName">LPNMITEMACTIVATE�I�u�W�F�N�g�ւ̃|�C���^</param>
/// <param name="height">�X�L�����f�[�^�̃��C����</param>
/// <param name="width">�X�L�����f�[�^�̃T���v�����O��</param>
/// <param name="bands">�X�L�����f�[�^�̃o���h��</param>
/// <param name="img">CImage�I�u�W�F�N�g</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
/// @remark �X�L�����f�[�^����摜�f�[�^�ɕϊ�����
bool CScanDataIO::LoadImage(int &height, int &width, int &bands, CImage &img)
{
	if (m_o_p_cube == nullptr) {
		return false;
	}

	height	= m_o_p_cube->format.height;
	width	= m_o_p_cube->format.width;
	bands	= m_o_p_cube->format.nr_bands;

	int band[3];
	getRGBBandSpectrum(band[0], band[1], band[2]);

	float dnormalizer = (float)255.0;

#if false
	HANDLE *hThreads = new HANDLE[3];
	Normalizer norm[3]{
		{m_o_p_cube,band[0],height,width,0.0,0.0},
		{m_o_p_cube,band[1],height,width,0.0,0.0},
		{m_o_p_cube,band[2],height,width,0.0,0.0}
	};

	for (int i = 0; i < 3; i++)
	{
		hThreads[i] = (HANDLE)_beginthread(
			&CScanDataIO::executeNomalizerLauncher,	// �����`�����N��
			0,
			&norm[i]);
	}

	WaitForMultipleObjects(
		3, //�X���b�h��
		hThreads, //�X���b�h�n���h���̔z��
		TRUE, //�ҋ@�I�v�V����
		INFINITE);//�^�C���A�E�g����

	if (hThreads) {
		delete hThreads;
		hThreads = nullptr;
	}
	for (int i = 0; i < 3; i++) {
		noemal[i] = norm[i].m_normalizer;
		offset[i] = norm[i].m_offset;
	}
#endif

	bool bResult = true;
	BITMAPINFOHEADER    bmInfohdr;
	int Bpp = 3;
	// Create the header info
	bmInfohdr.biSize = sizeof(BITMAPINFOHEADER);
	bmInfohdr.biWidth = width;
	bmInfohdr.biHeight = -height;
	bmInfohdr.biPlanes = 1;
	bmInfohdr.biBitCount = (WORD)(Bpp * 8);
	bmInfohdr.biCompression = BI_RGB;
	bmInfohdr.biSizeImage = width * height * Bpp;
	bmInfohdr.biXPelsPerMeter = 0;
	bmInfohdr.biYPelsPerMeter = 0;
	bmInfohdr.biClrUsed = 0;
	bmInfohdr.biClrImportant = 0;

	BITMAPINFO bmInfo;
	bmInfo.bmiHeader = bmInfohdr;
	bmInfo.bmiColors[0].rgbBlue = 255;

	unsigned char * p24Img = new unsigned char[bmInfohdr.biSizeImage];
	unsigned char * ptr = p24Img;

	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {
			unsigned char r = (BYTE)(m_o_p_cube->ppp_data[band[0]][row][col] * dnormalizer);
			unsigned char g = (BYTE)(m_o_p_cube->ppp_data[band[1]][row][col] * dnormalizer);
			unsigned char b = (BYTE)(m_o_p_cube->ppp_data[band[2]][row][col] * dnormalizer);
			if (r > 255) r = 255;
			else if (r < 0) r = 0;
			if (g > 255) g = 255;
			else if (g < 0) g = 0;
			if (b > 255) b = 255;
			else if (b < 0) b = 0;
			*(ptr++) = b;
			*(ptr++) = g;
			*(ptr++) = r;
		}
	}

	if (img.Create(width, height, 8 * Bpp, NULL)) {
		HDC dc = img.GetDC();
		SetDIBitsToDevice(dc, 0, 0, width, height, 0, 0, 0, height, p24Img, &bmInfo, DIB_RGB_COLORS);
		img.ReleaseDC();
	}
	else {
		bResult = false;
	}

	if (p24Img) {
		delete[] p24Img;
		p24Img = nullptr;
	}
	return bResult;
}

/// <summary>
/// �w��ʒu�X�y�N�g���Q�̎擾
/// </summary>
/// <param name="pos">�X�y�N�g���擾�ʒu</param>
/// <param name="data">�X�y�N�g���Q</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CScanDataIO::GetSpectrumData(CPoint &pos, std::vector<double> &data)
{
	bool bResult = true;
	if (m_o_p_cube == nullptr) {
		return false;
	}
	if (((pos.x < 0) || (pos.x >= m_o_p_cube->format.width)) || ((pos.y < 0) || (pos.y >= m_o_p_cube->format.height))) {
		return false;
	}
	int bands = m_o_p_cube->format.nr_bands;

	data.clear();
	data.resize(bands);
	for (int i = 0; i < bands; i++) {
		data[i] = m_o_p_cube->ppp_data[i][pos.y][pos.x];
	}
	return bResult;
}

/// <summary>
/// RGB�X�y�N�g���̃o���h�C���f�N�X�擾
/// </summary>
/// <param name="RBand">��(dRSpectrum�ߖT)�̃o���hID</param>
/// <param name="GBand">��(dGSpectrum�ߖT)�̃o���hID</param>
/// <param name="BBand">��(dBSpectrum�ߖT)�̃o���hID</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CScanDataIO::getRGBBandSpectrum(int &RBand, int &GBand, int &BBand)
{
	if (m_o_p_cube == nullptr) {
		return false;
	}

	double rDiff, gDiff, bDiff, diff;
	double bdat;

	int bands = m_o_p_cube->format.nr_bands;
	if (bands <= 0) {
		return false;
	}

	bdat = m_o_p_cube->format.band_names[bands-1];
	rDiff = gDiff = bDiff = bdat;
	RBand = GBand = BBand = 0;

	for (int i = 0; i < bands; i++) {
		bdat = m_o_p_cube->format.band_names[i];
		diff = fabs(dRSpectrum - bdat);
		if (rDiff > diff) {
			rDiff = diff;
			RBand = i;
		}
		diff = fabs(dGSpectrum - bdat);
		if (gDiff > diff) {
			gDiff = diff;
			GBand = i;
		}
		diff = fabs(dBSpectrum - bdat);
		if (bDiff > diff) {
			bDiff = diff;
			BBand = i;
		}
	}
	return true;
}

/// <summary>
/// �e�o���h�̃X�y�N�g���l�擾
/// </summary>
/// <param name="BandSpectrum">�X�y�N�g���l</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CScanDataIO::getBandSpectrum(std::vector<double> &BandSpectrum)
{
	if (m_o_p_cube == nullptr) {
		return false;
	}
	int bands = m_o_p_cube->format.nr_bands;
	BandSpectrum.clear();
	BandSpectrum.resize(bands);
	for (int i = 0; i < bands; i++) {
		BandSpectrum[i] = m_o_p_cube->format.band_names[i];
	}
	return true;
}

