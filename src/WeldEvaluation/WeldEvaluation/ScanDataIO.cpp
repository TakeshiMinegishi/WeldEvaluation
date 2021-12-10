#include "stdafx.h"
#include "ScanDataIO.h"
#include "FileUtil.h"
#include <iostream>
#include <vector>
#include "message.h"

#ifndef _DEBUG
#pragma warning(disable:6001)
#endif

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
	m_localData = false;

	m_RSpectrum = dRSpectrum;		// �Ԃ̃X�y�N�g���l
	m_GSpectrum = dGSpectrum;		// �΂̃X�y�N�g���l
	m_BSpectrum = dBSpectrum;		// �̃X�y�N�g���l
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
/// <param name="bReload">�ēǍ��t���O</param>
/// <returns>���������ꍇ��true,���s�����ꍇ��false��Ԃ�</returns>
/// @remark bReload��false�̏ꍇ�́A���f�[�^���J���Ă���ꍇ�͉��������ɏI������
bool CScanDataIO::open(CString pathName, bool bReload/*=false*/ )
{
	if (!CFileUtil::fileExists(pathName)) {
		CString msg;
		msg.Format(_T("CScanDataIO::open():�t�@�C�������݂��܂���(%s)�B"), static_cast<LPCWSTR>(pathName));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}

	// �ēǍ��L���̃`�F�b�N
	if (!bReload && (m_pathName.Compare(pathName) == 0)) {
		if (m_o_p_cube) {
			return true;
		}
	}

	// ���ɊJ���Ă�f�[�^�����
	release();

	m_o_p_cube = new CubeFloat();
	HSI_RETURN result = commonLoadCube(m_o_p_cube, pathName);
	if (result != HSI_OK) {
		m_localData = true;
		CStdioFile tfd;
		// header�f�[�^�o��
		if (tfd.Open(pathName, CFile::modeRead | CFile::typeText)) {
			CString buf;
			CString key, val;
			int byteoder,datatype;
			while (tfd.ReadString(buf)) {
				int id = buf.FindOneOf(_T("="));
				if (id == -1) {
					continue;
				}
				key = buf.Mid(0, id);
				key.Trim();
				val = buf.Mid(id+1);
				val.Trim();
				if (key == _T("bands")) {
					m_o_p_cube->format.nr_bands = _ttoi(val);
				}
				else if (key == _T("byte order")) {
					byteoder = _ttoi(val);
				}
				else if (key == _T("data type")) {
					datatype = _ttoi(val);
				}
				else if (key == _T("file type")) {
				}
				else if (key == _T("header offset")) {
				}
				else if (key == _T("interleave")) {
				}
				else if (key == _T("lines")) {
					m_o_p_cube->format.height = _ttoi(val);
				}
				else if (key == _T("samples")) {
					m_o_p_cube->format.width = _ttoi(val);
				}
				else if (key == _T("wavelength")) {
					tfd.ReadString(buf);
					m_o_p_cube->format.band_names = new double[m_o_p_cube->format.nr_bands];

					double dval;
					int cnt = 0;
					while (cnt < m_o_p_cube->format.nr_bands) {
						id = buf.FindOneOf(_T(","));
						if (id == -1) {
							break;
						}
						val = buf.Mid(0, id);
						val.Trim();
						buf = buf.Mid(id + 1);
						buf.Trim();

						dval = _ttof(val);
						if (!val.IsEmpty()) {
							m_o_p_cube->format.band_names[cnt] = dval;
							cnt++;
						}
					}
					buf.Trim();
					if (!buf.IsEmpty()) {
						if (cnt < m_o_p_cube->format.nr_bands) {
							dval = _ttof(val);
							m_o_p_cube->format.band_names[cnt] = dval;
						}
					}
					tfd.ReadString(buf);
				}
			}
			m_o_p_cube->format.size_bytes = (int)((__int64)m_o_p_cube->format.width *	(__int64)m_o_p_cube->format.height * (__int64)m_o_p_cube->format.nr_bands * 	sizeof(float));
			tfd.Close();

			float ***ppp_data = new float **[m_o_p_cube->format.nr_bands];
			for (int b = 0; b < m_o_p_cube->format.nr_bands; b++) {
				ppp_data[b] = new float*[m_o_p_cube->format.height];
				for (int h = 0; h < m_o_p_cube->format.height; h++) {
					ppp_data[b][h] = new float[m_o_p_cube->format.width];
				}
			}

			CString drive, dir, name, ext;
			CFileUtil::splitPath(pathName, drive, dir, name, ext);
			CString rawpath = drive + dir + name + _T(".raw");
			CFile fd;
			if (fd.Open(rawpath, CFile::modeRead | CFile::typeBinary)) {
				for (int b = 0; b < m_o_p_cube->format.nr_bands; b++) {
					for (int h = 0; h < m_o_p_cube->format.height; h++) {
						fd.Read(ppp_data[b][h], sizeof(float)*m_o_p_cube->format.width);
					}
				}
			}
			m_o_p_cube->ppp_data = ppp_data;
		}
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
		if (m_localData) {
			int band = m_o_p_cube->format.nr_bands;
//			int widht = m_o_p_cube->format.width;
			int height = m_o_p_cube->format.height;
			for (int b = 0; b < band; b++) {
				for (int h = 0; h < height; h++) {
					if (m_o_p_cube->ppp_data[b][h]) {
						delete [] m_o_p_cube->ppp_data[b][h];
					}
				}
				if (m_o_p_cube->ppp_data[b]) {
					delete [] m_o_p_cube->ppp_data[b];
				}
			}
			delete [] m_o_p_cube->ppp_data;
			m_o_p_cube->ppp_data = nullptr;
			if (m_o_p_cube->format.band_names) {
				delete[] m_o_p_cube->format.band_names;
				m_o_p_cube->format.band_names = nullptr;
			}
		}
		else {
			commonDeallocateCube(m_o_p_cube);
		}
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
	m_localData = false;
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
/// �J���[�g���̐ݒ�
/// </summary>
/// <param name="r">�Ԃ̔g��</param>
/// <param name="g">�΂̔g��</param>
/// <param name="b">�̔g��</param>
void CScanDataIO::SetRGBWavelength(double r, double g, double b)
{
	if (r > 0.0 )
		m_RSpectrum = r;
	if (g > 0.0)
		m_GSpectrum = g;
	if (b > 0.0)
		m_BSpectrum = b;
}

/// <summary>
/// �摜�f�[�^�擾
/// </summary>
/// <param name="height">�X�L�����f�[�^�̃��C����</param>
/// <param name="width">�X�L�����f�[�^�̃T���v�����O��</param>
/// <param name="bands">�X�L�����f�[�^�̃o���h��</param>
/// <param name="img">CImage�I�u�W�F�N�g</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
/// @remark �X�L�����f�[�^����摜�f�[�^�ɕϊ�����
bool CScanDataIO::LoadImage(int &height, int &width, int &bands, CImage &img)
{
	if (m_o_p_cube == nullptr) {
		CString msg;
		msg.Format(_T("CScanDataIO::LoadImage():�f�[�^��Open����Ă��܂���Bm_o_p_cube=null"));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}

	height = m_o_p_cube->format.height;
	width = m_o_p_cube->format.width;
	bands = m_o_p_cube->format.nr_bands;

	int band[3];
	getRGBBandSpectrum(band[0], band[1], band[2]);

	// �C���[�W�̉��T�C�Y�o�E���_������
	int imageWidth = width;
	if ((imageWidth % 8) != 0) {
		imageWidth = (int)(imageWidth / 8) * 8 + 8;
	}

	bool bResult = true;
	BITMAPINFOHEADER    bmInfohdr;
	int Bpp = 3;
	// Create the header info
	bmInfohdr.biSize = sizeof(BITMAPINFOHEADER);
	bmInfohdr.biWidth = imageWidth;
	bmInfohdr.biHeight = -height;
	bmInfohdr.biPlanes = 1;
	bmInfohdr.biBitCount = (WORD)(Bpp * 8);
	bmInfohdr.biCompression = BI_RGB;
	bmInfohdr.biSizeImage = imageWidth * height * Bpp;
	bmInfohdr.biXPelsPerMeter = 0;
	bmInfohdr.biYPelsPerMeter = 0;
	bmInfohdr.biClrUsed = 0;
	bmInfohdr.biClrImportant = 0;

	BITMAPINFO bmInfo;
	bmInfo.bmiHeader = bmInfohdr;
	bmInfo.bmiColors[0].rgbBlue = 255;

	unsigned char * p24Img = nullptr;
	p24Img = new unsigned char[(__int64)bmInfohdr.biSizeImage*8];
	if (p24Img == nullptr) {
		CString msg;
		msg.Format(_T("CScanDataIO::LoadImage():�C���[�W�̈�̊m�ۂɎ��s���܂����B"));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}
	ZeroMemory(p24Img, (__int64)bmInfohdr.biSizeImage * 8);
	unsigned char * ptr = p24Img;

	float dnormalizer = (float)255.0;
	unsigned char r, g, b;
	for (int row = 0; row < height; row++) {
		ptr = p24Img + (__int64)((__int64)row * imageWidth) * Bpp;
		for (int col = 0; col < width; col++) {
			if (col >= m_o_p_cube->format.width) {
				r = g = b = 0;
			}
			else {
				// �I�[�o�[�t���[�΍�
				r = (BYTE)((m_o_p_cube->ppp_data[band[0]][row][col] > 1.0 ? 1.0 : (m_o_p_cube->ppp_data[band[0]][row][col] < 0.0 ? 0.0 : m_o_p_cube->ppp_data[band[0]][row][col])) * dnormalizer);
				g = (BYTE)((m_o_p_cube->ppp_data[band[1]][row][col] > 1.0 ? 1.0 : (m_o_p_cube->ppp_data[band[1]][row][col] < 0.0 ? 0.0 : m_o_p_cube->ppp_data[band[1]][row][col])) * dnormalizer);
				b = (BYTE)((m_o_p_cube->ppp_data[band[2]][row][col] > 1.0 ? 1.0 : (m_o_p_cube->ppp_data[band[2]][row][col] < 0.0 ? 0.0 : m_o_p_cube->ppp_data[band[2]][row][col])) * dnormalizer);
				if (r > 255) r = 255;
				else if (r < 0) r = 0;
				if (g > 255) g = 255;
				else if (g < 0) g = 0;
				if (b > 255) b = 255;
				else if (b < 0) b = 0;
			}
			*(ptr++) = b;
			*(ptr++) = g;
			*(ptr++) = r;
		}
		for (int col = width; col < imageWidth; col++) {
			*(ptr++) = 0;
			*(ptr++) = 0;
			*(ptr++) = 0;
		}
	}

	if (img.Create(imageWidth, height, 8 * Bpp, NULL)) {
		HDC dc = img.GetDC();
		int ret = SetDIBitsToDevice(dc, 0, 0, imageWidth, height, 0, 0, 0, height, p24Img, &bmInfo, DIB_RGB_COLORS);
		if(ret == 0) {
			DWORD err = GetLastError();
			LPVOID lpMsgBuf;
			FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER							// �e�L�X�g�̃��������蓖�Ă�v������
				| FORMAT_MESSAGE_FROM_SYSTEM							// �G���[���b�Z�[�W��Windows���p�ӂ��Ă�����̂��g�p
				| FORMAT_MESSAGE_IGNORE_INSERTS,						// ���̈����𖳎����ăG���[�R�[�h�ɑ΂���G���[���b�Z�[�W���쐬����
				NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),	// ������w��
				(LPTSTR)&lpMsgBuf,										// ���b�Z�[�W�e�L�X�g���ۑ������o�b�t�@�ւ̃|�C���^
				0,
				NULL);
			//				AfxMessageBox((LPCTSTR)lpMsgBuf, MB_OK | MB_ICONINFORMATION);
			LocalFree(lpMsgBuf);

			CString msg,fmt;
			fmt = _T("�摜�̍쐬�Ɏ��s���܂����B:%s");
			msg.Format(fmt, static_cast<LPCWSTR>(lpMsgBuf));
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
			bResult = false;
		}
		img.ReleaseDC();
	}
	else {
		CString msg;
		msg.Format(_T("CScanDataIO::LoadImage():�r�b�g�}�b�v�̍쐬�Ɏ��s���܂����B"));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
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
		CString msg;
		msg.Format(_T("CScanDataIO::GetSpectrumData():�f�[�^��Open����Ă��܂���Bm_o_p_cube=null"));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}
	if (((pos.x < 0) || (pos.x >= m_o_p_cube->format.width)) || ((pos.y < 0) || (pos.y >= m_o_p_cube->format.height))) {
		CString msg;
		msg.Format(_T("CScanDataIO::GetSpectrumData():�X�y�N�g���擾�ʒu���s���ł��Bpos=(%d,%d)"),pos.x,pos.y);
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
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
		CString msg;
		msg.Format(_T("CScanDataIO::getRGBBandSpectrum():�f�[�^��Open����Ă��܂���Bm_o_p_cube=null"));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}

	double rDiff, gDiff, bDiff, diff;
	double bdat;

	int bands = m_o_p_cube->format.nr_bands;
	if (bands <= 0) {
		CString msg;
		msg.Format(_T("CScanDataIO::getRGBBandSpectrum():�o���h�l������������܂���Bm_o_p_cube->format.nr_bands=%d"), bands);
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}

	bdat = m_o_p_cube->format.band_names[bands-1];
	rDiff = gDiff = bDiff = bdat;
	RBand = GBand = BBand = 0;

	for (int i = 0; i < bands; i++) {
		bdat = m_o_p_cube->format.band_names[i];
		diff = fabs(m_RSpectrum - bdat);
		if (rDiff > diff) {
			rDiff = diff;
			RBand = i;
		}
		diff = fabs(m_GSpectrum - bdat);
		if (gDiff > diff) {
			gDiff = diff;
			GBand = i;
		}
		diff = fabs(m_BSpectrum - bdat);
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
		CString msg;
		msg.Format(_T("CScanDataIO::getBandSpectrum():�f�[�^��Open����Ă��܂���Bm_o_p_cube=null"));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
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

/// <summary>
/// �o���h���̎擾
/// </summary>
/// <returns>�o���h����Ԃ�</returns>
int  CScanDataIO::GetNumberOfBand()
{
	if (m_o_p_cube == nullptr) {
		CString msg;
		msg.Format(_T("CScanDataIO::GetNumberOfBand():�f�[�^��Open����Ă��܂���Bm_o_p_cube=null"));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return 0;
	}
	int bands = m_o_p_cube->format.nr_bands;
	return bands;
}

/// <summary>
/// �o���h�̃X�y�N�g���l�擾
/// </summary>
/// <param name="index">�o���h�C���f�b�N�X</param>
/// <returns>�X�y�N�g���l��Ԃ�</returns>
double CScanDataIO::getWaveLength(int index)
{
	if (m_o_p_cube == nullptr) {
		CString msg;
		msg.Format(_T("CScanDataIO::getWaveLength():�f�[�^��Open����Ă��܂���Bm_o_p_cube=null"));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return 0.0;
	}
	if ((index < 0) || (index >= m_o_p_cube->format.nr_bands)) {
		return 0.0;
	}
	return m_o_p_cube->format.band_names[index];
}

/// <summary>
/// �X�L�����f�[�^�̔��]
/// </summary>
/// <returns>�����̏ꍇ��true�A���s�̏ꍇ��false��Ԃ�</returns>
bool CScanDataIO::InversData(CWnd *pWnd/*=nullptr*/)
{
	if (m_o_p_cube == nullptr) {
		CString msg;
		msg.Format(_T("CScanDataIO::InversData():�f�[�^��Open����Ă��܂���Bm_o_p_cube=null"));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}

	bool bResult = true;
	int height = m_o_p_cube->format.height;
	int width = m_o_p_cube->format.width;
	int band = m_o_p_cube->format.nr_bands;
#if 0	// �㉺���]
	int hheight = height / 2;
	float  *buff = new float[width];

	for (int h = 0; h < hheight; h++) {
		for (int b = 0; b < band; b++) {
			memcpy(buff, m_o_p_cube->ppp_data[b][h], sizeof(float)*width);
			memcpy(m_o_p_cube->ppp_data[b][h], m_o_p_cube->ppp_data[b][height-1-h], sizeof(float)*width);
			memcpy(m_o_p_cube->ppp_data[b][height - 1 - h], buff, sizeof(float)*width);
		}
	}
	if (buff) {
		delete[] buff;
	}
#else	// 180�x��]
	float ***dst = (float***)nullptr;
	double **mat = MatrixInit();
	int pos = 0, retio = 0;
	double base = 2 * band * height;
	try {
		dst = new float**[band]();
		for (int b = 0; b < band; b++) {
			dst[b] = new float *[height]();
			for (int h = 0; h < height; h++) {
				dst[b][h] = new float[width]();
				ZeroMemory(dst[b][h], sizeof(float)*width);
			}
		}

		MatrixMove(mat, width / 2, height / 2);
		MatrixRotete(mat, 180);
		MatrixMove(mat, -width / 2, -height / 2);
		MatrixInvers(mat);

		if (affine(width, height, (float ***)m_o_p_cube->ppp_data, width, height, dst, band, mat, false)) {
			for (int b = 0; b < band; b++) {
				for (int h = 0; h < height; h++) {
					memcpy(m_o_p_cube->ppp_data[b][h], dst[b][h], sizeof(float)*width);
					if (pWnd) {
						if (retio != (int)(((double)pos / base) * 100.0)) {
							retio = (int)(((double)pos / base) * 100.0);
							pWnd->SendMessage(WM_INVERS_STATUS, PROGRESS_UPDATE, retio);
						}
						pos++;
					}
				}
			}
		}
		else {
			bResult = false;
		}
	}
	catch (...) {
		bResult = false;
	}

	if (dst!=nullptr) {
		for (int b = 0; b < band; b++) {
			if (dst[b]) {
				for (int h = 0; h < height; h++) {
					if (dst[b][h]) {
						delete [] dst[b][h];
						dst[b][h] = nullptr;
					}
					if (pWnd) {
						if (retio != (int)(((double)pos / base) * 100.0)) {
							retio = (int)(((double)pos / base) * 100.0);
							pWnd->SendMessage(WM_INVERS_STATUS, PROGRESS_UPDATE, retio);
						}
						pos++;
					}
				}
				delete [] dst[b];
				dst[b] = nullptr;
			}
		}
		delete [] dst;
		dst = nullptr;
	}

	MatrixRelease(mat);

#endif
	return bResult;
}

/// <summary>
/// RAW�f�[�^�̕ۑ�
/// </summary>
/// <param name="pathName">�p�X��</param>
/// <returns>�����̏ꍇ��true�A���s�̏ꍇ��false��Ԃ�</returns>
bool CScanDataIO::saveRawData(CString pathName)
{
	if (m_o_p_cube == nullptr) {
		CString msg;
		msg.Format(_T("CScanDataIO::saveRawData():�f�[�^��Open����Ă��܂���Bm_o_p_cube=null"));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}

	bool bResult = false;
	CFile fd;
	if (fd.Open(pathName, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary)) {
		for (int b = 0; b < m_o_p_cube->format.nr_bands; b++) {
			for (int h = 0; h < m_o_p_cube->format.height; h++) {
				fd.Write(m_o_p_cube->ppp_data[b][h], sizeof(float)*m_o_p_cube->format.width);
			}
		}
		fd.Close();
		bResult = true;
	}
	else {
		CString msg;
		msg.Format(_T("CScanDataIO::saveRawData():RAW�f�[�^�̕ۑ��Ɏ��s���܂����B[%s]"), static_cast<LPCWSTR>(pathName));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
	}
	return bResult;
}

/// <summary>
/// RAW�f�[�^�̓ǂݍ���
/// </summary>
/// <param name="pathName">�p�X��</param>
/// <returns>�����̏ꍇ��true�A���s�̏ꍇ��false��Ԃ�</returns>
bool CScanDataIO::readRawData(CString pathName)
{
	if (m_o_p_cube == nullptr) {
		CString msg;
		msg.Format(_T("CScanDataIO::readRawData():�f�[�^��Open����Ă��܂���Bm_o_p_cube=null"));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}

	bool bResult = false;
	CFile fd;
	if (fd.Open(pathName, CFile::modeRead | CFile::typeBinary)) {
		for (int b = 0; b < m_o_p_cube->format.nr_bands; b++) {
			for (int h = 0; h < m_o_p_cube->format.height; h++) {
				fd.Read(m_o_p_cube->ppp_data[b][h], sizeof(float)*m_o_p_cube->format.width);
			}
		}
		fd.Close();
		bResult = true;
	}
	return bResult;
}

/// <summary>
/// ���`���
/// </summary>
/// <param name="srcdata">�f�[�^</param>
/// <param name="width">�f�[�^��</param>
/// <param name="height">�f�[�^����</param>
/// <param name="band">�o���h��</param>
/// <param name="orignX">��ԑΏۃf�[�^X�l</param>
/// <param name="orignY">��ԑΏۃf�[�^Y�l</param>
/// <param name="p">��Ԓl</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CScanDataIO::bicubic(float *** srcdata, int width, int height, int band, float orignX, float orignY, float *p)
{
	float integral[2], fractional[2];
	fractional[0] = modff(orignX, &integral[0]);
	fractional[1] = modff(orignY, &integral[1]);

	int x0 = (int)integral[0];
	int x1 = x0 + 1;
	if (x1 >= width) {
		x1 = x0;
	}
	int y0 = (int)integral[1];
	int y1 = y0 + 1;
	if (y1 >= height) {
		y1 = y0;
	}
	float dx = orignX - (float)x0;
	float dy = orignY - (float)y0;
	float a = (float)((1.0 - dx) * (1.0 - dy));
	float b = (float)((1.0 - dx) * dy);
	float c = (float)(dx * (1.0 - dy));
	float d = dx * dy;

//	{
//		CString dbg;
//		dbg.Format(_T(" LT:(%4d,%4d) x a=%.4f LB:(%4d,%4d) x a=%.4f RT:(%4d,%4d) x a=%.4f RB:(%4d,%4d) x a=%.4f\n"), x0,y0,a, x0, y1, b, x1, y0, c, x1, y1, d);
//		OutputDebugString(dbg);
//	}

	for (int bd = 0; bd < band; bd++) {
		p[bd] = srcdata[bd][y0][x0] * a + srcdata[bd][y1][x0] * b + srcdata[bd][y0][x1] * c + srcdata[bd][y1][x0] * d;
	}
	return true;
}

/// <summary>
/// ���O�o��
/// </summary>
/// <param name="level">���O���x��</param>
/// <param name="filePath">�G���[�t�@�C��</param>
/// <param name="lineNo">�G���[�s</param>
/// <param name="msg">���O���b�Z�[�W</param>
void CScanDataIO::writeLog(CLog::LOGLEVEL level, CString filePath, long lineNo, CString msg)
{
	CLog log;
	CString ErrMsg;
	ErrMsg.Format(_T(" File:%s Line:%ld:%s"), static_cast<LPCWSTR>(filePath), lineNo, static_cast<LPCWSTR>(msg));
	log.logWrite(level, ErrMsg);
}

/// <summary>
/// �G���[���O�o��
/// </summary>
/// <param name="filePath">�G���[�t�@�C��</param>
/// <param name="lineNo">�G���[�s</param>
/// <param name="i_caller_name">�G���[�R�[�h</param>
/// <param name="i_return_val">�Ăяo����</param>
void CScanDataIO::errorLog(CString filePath, long lineNo, CString i_caller_name, HSI_RETURN i_return_val)
{
	if (i_return_val == HSI_OK)
	{
		return;
	}

	CString errs;
	switch (i_return_val)
	{
	case HSI_HANDLE_INVALID:
		errs = _T("Invalid device handle specified.");
		break;
	case HSI_ARGUMENT_INVALID:
		errs = _T("Invalid argument provided in function call.");
		break;
	case HSI_CALL_ILLEGAL:
		errs = _T("Function call illegal given the current snapscan state.");
		break;
	case HSI_FILE_NOT_FOUND:
		errs = _T("A file could not be found.");
		break;
	case HSI_CALIBRATION_FILE_NOT_FOUND:
		errs = _T("Sensor calibration file could not be found.");
		break;
	case HSI_CONNECTION_FAILED:
		errs = _T("Snapscan system could not be connected.");
		break;
	case HSI_ALLOCATION_ERROR:
		errs = _T("Allocation of resources failed.");
		break;
	case HSI_ACQUISITION_FAILED:
		errs = _T("Unable to fulfill acquisition.");
		break;
	case HSI_DATA_NOT_ALLOCATED:
		errs = _T("Provided data structure is not allocated.");
		break;
	case HSI_DATA_NOT_VALID:
		errs = _T("Data with valid flag false provided as input for operation.");
		break;
	case HSI_DATA_NOT_COMPATIBLE:
		errs = _T("Data provided is not compatible.");
		break;
	case HSI_FILE_SYSTEM_ERROR:
		errs = _T("Specified directory doesn't exist and could not be created.");
		break;
	case HSI_FILE_IO_ERROR:
		errs = _T("Could not read or write data from the filesystem.");
		break;
	case HSI_INTERNAL_ERROR:
		errs = _T("An undexpected internal error occurred.");
		break;
	default:
		errs = _T("Unknown error.");
		break;
	}

	CLog log;
	CString ErrMsg;
	ErrMsg.Format(_T(" File:%s Line:%ld:%s:%s"), static_cast<LPCWSTR>(filePath), lineNo, static_cast<LPCWSTR>(i_caller_name), static_cast<LPCWSTR>(errs));
	log.logWrite(CLog::LOGLEVEL::Error, ErrMsg);
	return;
}

/// <summary>
/// �}�g���b�N�X������
/// </summary>
/// <returns>�}�g���b�N�X�̈��Ԃ�</returns>
double ** CScanDataIO::MatrixInit()
{
	double init[3][3] = { {1,0,0},{0,1,0},{0,0,1} };
	double **mat = new double *[3];
	for (int i = 0; i < 3; i++) {
		mat[i] = new double[3];
		for (int j = 0; j < 3; j++) {
			mat[i][j] = init[i][j];
		}
	}
	return mat;
}

/// <summary>
/// �}�g���b�N�X�J��
/// </summary>
/// <param name="mat">�}�g���b�N�X</param>
void CScanDataIO::MatrixRelease(double ** &mat)
{
	if (mat != nullptr) {
		for (int i = 0; i < 3; i++) {
			if (mat[i] != nullptr) {
				delete[] mat[i];
				mat[i] = nullptr;
			}
		}
		delete  mat;
		mat = nullptr;
	}
}

/// <summary>
/// �}�g���b�N�X�����Z
/// </summary>
/// <param name="src">�}�g���b�N�X1</param>
/// <param name="dst">�}�g���b�N�X2</param>
void CScanDataIO::MatrixMultiplication(double **src, double **dst)
{
	double d[3];
	d[0] = src[0][0] * dst[0][0] + src[0][1] * dst[1][0] + src[0][2] * dst[2][0];
	d[1] = src[0][0] * dst[0][1] + src[0][1] * dst[1][1] + src[0][2] * dst[2][1];
	d[2] = src[0][0] * dst[0][2] + src[0][1] * dst[1][2] + src[0][2] * dst[2][2];
	src[0][0] = d[0];
	src[0][1] = d[1];
	src[0][2] = d[2];

	d[0] = src[1][0] * dst[0][0] + src[1][1] * dst[1][0] + src[1][2] * dst[2][0];
	d[1] = src[1][0] * dst[0][1] + src[1][1] * dst[1][1] + src[1][2] * dst[2][1];
	d[2] = src[1][0] * dst[0][2] + src[1][1] * dst[1][2] + src[1][2] * dst[2][2];
	src[1][0] = d[0];
	src[1][1] = d[1];
	src[1][2] = d[2];

	d[0] = src[2][0] * dst[0][0] + src[2][1] * dst[1][0] + src[2][2] * dst[2][0];
	d[1] = src[2][0] * dst[0][1] + src[2][1] * dst[1][1] + src[2][2] * dst[2][1];
	d[2] = src[2][0] * dst[0][2] + src[2][1] * dst[1][2] + src[2][2] * dst[2][2];
	src[2][0] = d[0];
	src[2][1] = d[1];
	src[2][2] = d[2];
}

/// <summary>
/// �}�g���b�N�X�ړ�
/// </summary>
/// <param name="mat">�}�g���b�N�X</param>
/// <param name="x">X�ړ���</param>
/// <param name="y">Y�ړ���</param>
void CScanDataIO::MatrixMove(double **mat, double x, double y)
{
	double prm[3][3] = { {1,0,x},{0,1,y},{0,0,1} };
	double **dst = MatrixInit();
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			dst[i][j] = prm[i][j];
		}
	}
	MatrixMultiplication(mat, (double **)dst);
	MatrixRelease(dst);
}

/// <summary>
/// �}�g���b�N�X��]
/// </summary>
/// <param name="mat">�}�g���b�N�X</param>
/// <param name="angle">��]�p�i�x�j</param>
void CScanDataIO::MatrixRotete(double **mat, double angle)
{
	double PI = 3.141592648777698869248;
	double r = angle * PI / 180.0;
	double s = sin(r);
	if (fabs(s) < 1.0E-6) {
		s = 0.0;
	}
	double c = cos(r);
	if (fabs(c) < 1.0E-6) {
		c = 0.0;
	}

	double prm[3][3] = { {c,-s,0,},{s,c,0},{0,0,1} };
	double **dst = MatrixInit();
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			dst[i][j] = prm[i][j];
		}
	}

	MatrixMultiplication(mat, (double **)dst);
	MatrixRelease(dst);
}

/// <summary>
/// �}�g���b�N�X�g�k
/// </summary>
/// <param name="mat">�}�g���b�N�X</param>
/// <param name="x">X�k�ڗ�</param>
/// <param name="y">Y�k�ڗ�</param>
void CScanDataIO::MatrixScale(double **mat, double x, double y)
{
	double prm[3][3] = { {x,0,0,},{0,y,0},{0,0,1} };
	double **dst = MatrixInit();
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			dst[i][j] = prm[i][j];
		}
	}

	MatrixMultiplication(mat, (double **)dst);
	MatrixRelease(dst);
}

/// <summary>
/// �}�g���b�N�X�t�s��
/// </summary>
/// <param name="mat">�}�g���b�N�X</param>
void  CScanDataIO::MatrixInvers(double **mat)
{
	double a[3][3];
	double D = 1.0 / (mat[0][0] * mat[1][1] * mat[2][2] + mat[0][1] * mat[1][2] * mat[2][0] + mat[0][2] * mat[1][0] * mat[2][1] - mat[0][2] * mat[1][1] * mat[2][0] - mat[0][1] * mat[1][0] * mat[2][2] - mat[0][0] * mat[1][2] * mat[2][1]);

	a[0][0] =  (mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1]) * D;
	a[0][1] = -(mat[0][1] * mat[2][2] - mat[0][2] * mat[2][1]) * D;
	a[0][2] =  (mat[0][1] * mat[1][2] - mat[0][2] * mat[1][1]) * D;

	a[1][0] = -(mat[1][0] * mat[2][2] - mat[1][2] * mat[2][0]) * D;
	a[1][1] =  (mat[0][0] * mat[2][2] - mat[0][2] * mat[2][0]) * D;
	a[1][2] = -(mat[0][0] * mat[1][2] - mat[0][2] * mat[1][0]) * D;

	a[2][0] =  (mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0]) * D;
	a[2][1] = -(mat[0][0] * mat[2][1] - mat[0][1] * mat[2][0]) * D;
	a[2][2] =  (mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0]) * D;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			mat[i][j] = a[i][j];
		}
	}
}

/// <summary>
/// �A�t�@�C���ϊ�
/// </summary>
/// <param name="srcWidth">�ϊ�����</param>
/// <param name="srcHeight">�ϊ�������</param>
/// <param name="src">�ϊ����f�[�^</param>
/// <param name="dstWidth">�ϊ��敝</param>
/// <param name="distHeight">�ϊ��捂��</param>
/// <param name="dst">�ϊ���f�[�^</param>
/// <param name="band">�o���h��</param>
/// <param name="mat">�}�g���b�N�X</param>
/// <param name="bBicubic">���`��ԗL���t���O</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CScanDataIO::affine(int srcWidth, int srcHeight, float ***src, int dstWidth, int distHeight, float ***dst, int band, double **mat, bool bBicubic/*=true*/)
{
	double sx, sy;
	int x, y;
	float *p = new float[band];
	for (int dy = 0; dy < distHeight; dy++) {
		for (int dx = 0; dx < dstWidth; dx++) {
			sx = mat[0][0] * (double)dx + mat[0][1] * (double)dy + mat[0][2];
			sy = mat[1][0] * (double)dx + mat[1][1] * (double)dy + mat[1][2];
			x = (int)floor(sx + .5);
			y = (int)floor(sy + .5);
			if (x < 0) x = 0;
			else if (x >= srcWidth) x = srcWidth - 1;
			if (y < 0) y = 0;
			else if (y >= srcHeight) y = srcHeight - 1;

			if ((x < 0) || (x >= srcWidth) || (y < 0) || (y >= srcHeight)) {
				for (int b = 0; b < band; b++) {
					dst[b][dx][dy] = 0.0;
				}
			}
			else {
				if (bBicubic) {
					bicubic(src, srcWidth, srcHeight, band, (float)x, (float)y, p);
					for (int b = 0; b < band; b++) {
						dst[b][dy][dx] = p[b];
					}
				} else {
					for (int b = 0; b < band; b++) {
						dst[b][dy][dx] = src[b][y][x];
					}
				}
			}
		}
	}
	if (p) {
		delete[] p;
		p = nullptr;
	}
	return true;
}

/// <summary>
/// �X�L�����f�[�^�ϊ�
/// </summary>
/// <param name="srcWidth">�ϊ�����</param>
/// <param name="srcHeight">�ϊ�������</param>
/// <param name="band">�o���h��</param>
/// <param name="src">�ϊ����f�[�^</param>
/// <param name="hscale">X���g�k��</param>
/// <param name="vscale">Y���g�k��</param>
/// <param name="dstWidth">�ϊ��敝</param>
/// <param name="dstHeight">�ϊ��捂��</param>
/// <param name="dst">�ϊ���f�[�^</param>
/// <param name="bBicubic">���`��ԗL���t���O</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CScanDataIO::ScanDataConvert(int srcWidth, int srcHeight, int band, float ***src, double hscale, double vscale, int &dstWidth, int &dstHeight, float ***& dst, bool bBicubic/* = true*/)
{
	double **mat = MatrixInit();
	dstHeight = (int)(srcWidth * hscale);
	dstWidth  = (int)(srcHeight * vscale);

	MatrixMove(mat, dstWidth / 2, dstHeight / 2);
	MatrixScale(mat, hscale, vscale);
	MatrixRotete(mat, 90.0);
	MatrixMove(mat, -srcWidth / 2, -srcHeight / 2);
	MatrixInvers(mat);

	dst = new float**[band];
	for (int b = 0; b < band; b++) {
		dst[b] = new float*[dstHeight];
		for (int y = 0; y < dstHeight; y++) {
			dst[b][y] = new float[dstWidth];
			ZeroMemory(dst[b][y],sizeof(float)*dstWidth);
		}
	}
	affine(srcWidth, srcHeight, src, dstWidth, dstHeight, dst, band, mat, bBicubic);
	MatrixRelease(mat);
	return true;
}

/// <summary>
/// �ϊ��f�[�^�̊J��
/// </summary>
/// <param name="dstHeight">�ϊ��f�[�^����</param>
/// <param name="band">�o���h��</param>
/// <param name="dst">�ϊ��f�[�^</param>
void CScanDataIO::FreeConvertData(int dstHeight, int band, float ***& dst)
{
	if (dst) {
		for (int b = 0; b < band; b++) {
			if (dst[b]) {
				for (int y = 0; y < dstHeight; y++) {
					if (dst[b][y]) {
						delete[] dst[b][y];
						dst[b][y] = nullptr;
					}
				}
				delete[] dst[b];
				dst[b] = nullptr;
			}
		}
		delete[] dst;
		dst = nullptr;
	}
}

/// <summary>
/// �w�b�_�t�@�C�����畝�A�������擾
/// </summary>
/// <param name="pathName">�w�b�_�t�@�C���ւ̃p�X</param>
/// <param name="width">��</param>
/// <param name="height">����</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CScanDataIO::GetHeaderFilePrm(CString pathName, int &width, int &height)
{
	bool bResult = true;
	if (!CFileUtil::fileExists(pathName)) {
		CString msg;
		msg.Format(_T("CScanDataIO::GetHeaderFilePrm():�t�@�C�������݂��܂���B(%s)"), static_cast<LPCWSTR>(pathName));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}

	int find = 0;
	int bands = 0;
	CStdioFile tfd;
	if (tfd.Open(pathName, CFile::modeRead | CFile::typeText)) {
		CString buf;
		CString key, val;
		while (tfd.ReadString(buf)) {
			int id = buf.FindOneOf(_T("="));
			if (id == -1) {
				continue;
			}
			key = buf.Mid(0, id);
			key.Trim();
			val = buf.Mid(id + 1);
			val.Trim();
			if (key == _T("bands")) {
				bands = _ttoi(val);
			}
			else if (key == _T("interleave")) {
			}
			else if (key == _T("lines")) {
				height = _ttoi(val);
				find |= 0x01;
				if ((find & 0x03) == 0x03) {
					break;
				}
			}
			else if (key == _T("samples")) {
				width = _ttoi(val);
				find |= 0x02;
				if ((find & 0x03) == 0x03) {
					break;
				}
			}
		}
		tfd.Close();
	}
	if ((find & 0x03) != 0x03) {
		CString msg;
		msg.Format(_T("CScanDataIO::GetHeaderFilePrm():�t�@�C�������Ă���\��������܂��B(%s)"), static_cast<LPCWSTR>(pathName));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		bResult = false;
	}
	return bResult;
}

/// <summary>
/// �t�s��̎Z�o
/// </summary>
/// <param name="nprm">����</param>
/// <param name="ppMat">���}�g���b�N�X</param>
/// <param name="ppInvMat">�t�s��}�g���b�N�X</param>
void  CScanDataIO::MatrixInvers(int nprm, double **ppMat, double **ppInvMat)
{
	int i, j, k;
	double t, u, det;
	det = 1;

	for (i = 0; i < nprm; i++) {
		memcpy(ppInvMat[i], ppMat[i], sizeof(double)*nprm);
	}

	for (k = 0; k < nprm; k++)
	{
		t = ppInvMat[k][k];
		det *= t;

		for (i = 0; i < nprm; i++) {
			if (t != 0.0) {
				ppInvMat[k][i] /= t;
			}
		}

		if (t != 0.0) {
			ppInvMat[k][k] = 1 / t;
		}

		for (j = 0; j < nprm; j++)
		{
			if (j != k)
			{
				u = ppInvMat[j][k];

				for (i = 0; i < nprm; i++)
				{
					if (i != k) {
						ppInvMat[j][i] -= ppInvMat[k][i] * u;
					}
					else {
						if (t != 0.0) {
							ppInvMat[j][i] = -u / t;
						}
					}
				}
			}
		}
	}
}

/// <summary>
/// �z���O���t�B�[�}�g���b�N�X�擾
/// </summary>
/// <param name="srcPt">�ړ������_</param>
/// <param name="dstPt">�ړ��撸�_</param>
/// <param name="prm">�z���O���t�B�[�}�g���b�N�X�p�����[�^</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CScanDataIO::GetHomographyMatrix(CPoint srcPt[4], CPoint dstPt[4], double prm[])
{
	bool bResult = true;
	double **dATA = nullptr, **dATA_I = nullptr;
	int i;

	try {
		dATA = (double **)new double *[8]{ nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr };
		dATA_I	= (double **)new double *[8]{ nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr };

		for (i = 0; i < 8; i++)
		{
			dATA[i]		= new double[8];
			if (dATA[i]) {
				ZeroMemory(dATA[i], sizeof(double) * 8);
			} 
			dATA_I[i]	= new double[8];
			if (dATA_I[i]) {
				ZeroMemory(dATA_I[i], sizeof(double) * 8);
			}
		}

		dATA[0][0] = (double)srcPt[0].x;
		dATA[0][1] = (double)srcPt[0].y;
		dATA[0][2] =  1.0;
		dATA[0][3] =  0.0;
		dATA[0][4] =  0.0;
		dATA[0][5] =  0.0;
		dATA[0][6] = -1.0 * (double)dstPt[0].x * (double)srcPt[0].x;
		dATA[0][7] = -1.0 * (double)dstPt[0].x * (double)srcPt[0].y;
		dATA[1][0] = (double)srcPt[1].x;
		dATA[1][1] = (double)srcPt[1].y;
		dATA[1][2] =  1.0;
		dATA[1][3] =  0.0;
		dATA[1][4] =  0.0;
		dATA[1][5] =  0.0;
		dATA[1][6] = -1.0 * (double)dstPt[1].x * (double)srcPt[1].x;
		dATA[1][7] = -1.0 * (double)dstPt[1].x * (double)srcPt[1].y;
		dATA[2][0] = (double)srcPt[2].x;
		dATA[2][1] = (double)srcPt[2].y;
		dATA[2][2] =  1.0;
		dATA[2][3] =  0.0;
		dATA[2][4] =  0.0;
		dATA[2][5] =  0.0;
		dATA[2][6] = -1.0 * (double)dstPt[2].x * (double)srcPt[2].x;
		dATA[2][7] = -1.0 * (double)dstPt[2].x * (double)srcPt[2].y;
		dATA[3][0] = (double)srcPt[3].x;
		dATA[3][1] = (double)srcPt[3].y;
		dATA[3][2] =  1.0;
		dATA[3][3] =  0.0;
		dATA[3][4] =  0.0;
		dATA[3][5] =  0.0;
		dATA[3][6] = -1.0 * (double)dstPt[3].x * (double)srcPt[3].x;
		dATA[3][7] = -1.0 * (double)dstPt[3].x * (double)srcPt[3].y;
		dATA[4][0] =  0.0;
		dATA[4][1] =  0.0;
		dATA[4][2] =  0.0;
		dATA[4][3] = (double)srcPt[0].x;
		dATA[4][4] = (double)srcPt[0].y;
		dATA[4][5] =  1.0;
		dATA[4][6] = -1.0 * (double)dstPt[0].y * (double)srcPt[0].x;
		dATA[4][7] = -1.0 * (double)dstPt[0].y * (double)srcPt[0].y;
		dATA[5][0] =  0.0;
		dATA[5][1] =  0.0;
		dATA[5][2] =  0.0;
		dATA[5][3] = (double)srcPt[1].x;
		dATA[5][4] = (double)srcPt[1].y;
		dATA[5][5] =  1.0;
		dATA[5][6] = -1.0 * (double)dstPt[1].y * (double)srcPt[1].x;
		dATA[5][7] = -1.0 * (double)dstPt[1].y * (double)srcPt[1].y;
		dATA[6][0] =  0.0;
		dATA[6][1] =  0.0;
		dATA[6][2] =  0.0;
		dATA[6][3] = (double)srcPt[2].x;
		dATA[6][4] = (double)srcPt[2].y;
		dATA[6][5] =  1.0;
		dATA[6][6] = -1.0 * (double)dstPt[2].y * (double)srcPt[2].x;
		dATA[6][7] = -1.0 * (double)dstPt[2].y * (double)srcPt[2].y;
		dATA[7][0] =  0.0;
		dATA[7][1] =  0.0;
		dATA[7][2] =  0.0;
		dATA[7][3] = (double)srcPt[3].x;
		dATA[7][4] = (double)srcPt[3].y;
		dATA[7][5] =  1.0;
		dATA[7][6] = -1.0 * (double)dstPt[3].y * (double)srcPt[3].x;
		dATA[7][7] = -1.0 * (double)dstPt[3].y * (double)srcPt[3].y;

		// �t�s��
	//	MatrixInvers(8, dATA, dATA_I);

		prm[0] =	dATA_I[0][0] * (double)dstPt[0].x + dATA_I[0][1] * (double)dstPt[1].x +
					dATA_I[0][2] * (double)dstPt[2].x + dATA_I[0][3] * (double)dstPt[3].x +
					dATA_I[0][4] * (double)dstPt[0].y + dATA_I[0][5] * (double)dstPt[1].y +
					dATA_I[0][6] * (double)dstPt[2].y + dATA_I[0][7] * (double)dstPt[3].y;
		prm[1] =	dATA_I[1][0] * (double)dstPt[0].x + dATA_I[1][1] * (double)dstPt[1].x +
					dATA_I[1][2] * (double)dstPt[2].x + dATA_I[1][3] * (double)dstPt[3].x +
					dATA_I[1][4] * (double)dstPt[0].y + dATA_I[1][5] * (double)dstPt[1].y +
					dATA_I[1][6] * (double)dstPt[2].y + dATA_I[1][7] * (double)dstPt[3].y;
		prm[2] =	dATA_I[2][0] * (double)dstPt[0].x + dATA_I[2][1] * (double)dstPt[1].x +
					dATA_I[2][2] * (double)dstPt[2].x + dATA_I[2][3] * (double)dstPt[3].x +
					dATA_I[2][4] * (double)dstPt[0].y + dATA_I[2][5] * (double)dstPt[1].y +
					dATA_I[2][6] * (double)dstPt[2].y + dATA_I[2][7] * (double)dstPt[3].y;
		prm[3] =	dATA_I[3][0] * (double)dstPt[0].x + dATA_I[3][1] * (double)dstPt[1].x +
					dATA_I[3][2] * (double)dstPt[2].x + dATA_I[3][3] * (double)dstPt[3].x +
					dATA_I[3][4] * (double)dstPt[0].y + dATA_I[3][5] * (double)dstPt[1].y +
					dATA_I[3][6] * (double)dstPt[2].y + dATA_I[3][7] * (double)dstPt[3].y;
		prm[4] =	dATA_I[4][0] * (double)dstPt[0].x + dATA_I[4][1] * (double)dstPt[1].x +
					dATA_I[4][2] * (double)dstPt[2].x + dATA_I[4][3] * (double)dstPt[3].x +
					dATA_I[4][4] * (double)dstPt[0].y + dATA_I[4][5] * (double)dstPt[1].y +
					dATA_I[4][6] * (double)dstPt[2].y + dATA_I[4][7] * (double)dstPt[3].y;
		prm[5] =	dATA_I[5][0] * (double)dstPt[0].x + dATA_I[5][1] * (double)dstPt[1].x +
					dATA_I[5][2] * (double)dstPt[2].x + dATA_I[5][3] * (double)dstPt[3].x +
					dATA_I[5][4] * (double)dstPt[0].y + dATA_I[5][5] * (double)dstPt[1].y +
					dATA_I[5][6] * (double)dstPt[2].y + dATA_I[5][7] * (double)dstPt[3].y;
		prm[6] =	dATA_I[6][0] * (double)dstPt[0].x + dATA_I[6][1] * (double)dstPt[1].x +
					dATA_I[6][2] * (double)dstPt[2].x + dATA_I[6][3] * (double)dstPt[3].x +
					dATA_I[6][4] * (double)dstPt[0].y + dATA_I[6][5] * (double)dstPt[1].y +
					dATA_I[6][6] * (double)dstPt[2].y + dATA_I[6][7] * (double)dstPt[3].y;
		prm[7] =	dATA_I[7][0] * (double)dstPt[0].x + dATA_I[7][1] * (double)dstPt[1].x +
					dATA_I[7][2] * (double)dstPt[2].x + dATA_I[7][3] * (double)dstPt[3].x +
					dATA_I[7][4] * (double)dstPt[0].y + dATA_I[7][5] * (double)dstPt[1].y +
					dATA_I[7][6] * (double)dstPt[2].y + dATA_I[7][7] * (double)dstPt[3].y;

		for (i = 0; i < 8; i++)
		{
			delete dATA[i];
			delete dATA_I[i];
		}
		delete [] dATA;
		delete [] dATA_I;
	}
	catch (...) {
		if (dATA) {
			for (i = 0; i < 8; i++)
			{
				if (dATA[i]) {
					delete dATA[i];
				}
			}
			delete[] dATA;
		}

		if (dATA_I) {
			for (i = 0; i < 8; i++)
			{
				if (dATA_I[i]) {
					delete dATA_I[i];
				}
			}
			delete[] dATA_I;
		}
		bResult = false;
		CString msg;
		msg.Format(_T("CScanDataIO::GetHomographyMatrix():�z���O���t�B�[�}�g���b�N�X�̎Z�o�Ɏ��s���܂����B"));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
	}

	return bResult;
}

/// <summary>
/// �ˉe��_�̎擾
/// </summary>
/// <param name="SrcX">�ړ���X�l</param>
/// <param name="SrcY">�ړ���Y�l</param>
/// <param name="prm">�z���O���t�B�p�����[�^</param>
/// <param name="dTranX">�ړ���X�l</param>
/// <param name="dTranY">�ړ���Y�l</param>
/// @remark a�̃}�g���b�N�X���t�s���a�ɐݒ�
void CScanDataIO::Projection(int SrcX, int SrcY, double prm[], double &dTranX, double &dTranY)
{
	dTranX = ((double)SrcX*prm[0] + (double)SrcY*prm[1] + prm[2]) /	((double)SrcX*prm[6] + (double)SrcY*prm[7] + 1);
	dTranY = ((double)SrcX*prm[3] + (double)SrcY*prm[4] + prm[5]) /	((double)SrcX*prm[6] + (double)SrcY*prm[7] + 1);
}

/// <summary>
/// �}�g���b�N�X�t�s��
/// </summary>
/// <param name="n">����</param>
/// <param name="a">�}�g���b�N�X</param>
/// @remark a�̃}�g���b�N�X���t�s���a�ɐݒ�
void CScanDataIO::matinv(int n, double **a)
{
	int i, j, k;
	double t, u, det;
	det = 1;

	for (k = 0; k < n; k++)
	{
		t = a[k][k];
		det *= t;

		for (i = 0; i < n; i++) a[k][i] /= t;

		a[k][k] = 1 / t;

		for (j = 0; j < n; j++)
		{
			if (j != k)
			{
				u = a[j][k];

				for (i = 0; i < n; i++)
				{
					if (i != k) a[j][i] -= a[k][i] * u;
					else a[j][i] = -u / t;
				}
			}
		}
	}
}

/// <summary>
/// �ˉe�ϊ�
/// </summary>
/// <param name="vOrig">�ړ����f�[�^</param>
/// <param name="vTrans">�ړ���f�[�^</param>
/// <param name="aParam">�z���O���t�B�p�����[�^</param>
void CScanDataIO::Calc_ProjectionParam(vector<CPoint> &vOrig, vector<CPoint> &vTrans,double aParam[8])
{
	UINT i, j;
	double **dA;
	double dV[8];

	dA = new double*[8];
	for (i = 0; i < 8; i++)
	{
		dA[i] = new double[8];
	}

	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 8; j++)
		{
			dA[i][j] = 0.0;
		}
		dV[i] = 0.0;
		aParam[i] = 0.0;
	}

	for (i = 0; i < vOrig.size(); i++)
	{
		// AT*A
		dA[0][0] += (double)vOrig[i].x * (double)vOrig[i].x;
		dA[3][3] += (double)vOrig[i].x * (double)vOrig[i].x;
		dA[0][1] += (double)vOrig[i].x * (double)vOrig[i].y;
		dA[1][0] += (double)vOrig[i].x * (double)vOrig[i].y;
		dA[3][4] += (double)vOrig[i].x * (double)vOrig[i].y;
		dA[4][3] += (double)vOrig[i].x * (double)vOrig[i].y;
		dA[1][1] += (double)vOrig[i].y * (double)vOrig[i].y;
		dA[4][4] += (double)vOrig[i].y * (double)vOrig[i].y;
		dA[0][2] += (double)vOrig[i].x;
		dA[2][0] += (double)vOrig[i].x;
		dA[3][5] += (double)vOrig[i].x;
		dA[5][3] += (double)vOrig[i].x;
		dA[1][2] += (double)vOrig[i].y;
		dA[2][1] += (double)vOrig[i].y;
		dA[4][5] += (double)vOrig[i].y;
		dA[5][4] += (double)vOrig[i].y;
		dA[2][2] += 1;
		dA[5][5] += 1;
		dA[0][6] += -1 * (double)vTrans[i].x * (double)vOrig[i].x * (double)vOrig[i].x;
		dA[6][0] += -1 * (double)vTrans[i].x * (double)vOrig[i].x * (double)vOrig[i].x;
		dA[0][7] += -1 * (double)vTrans[i].x * (double)vOrig[i].x * (double)vOrig[i].y;
		dA[1][6] += -1 * (double)vTrans[i].x * (double)vOrig[i].x * (double)vOrig[i].y;
		dA[6][1] += -1 * (double)vTrans[i].x * (double)vOrig[i].x * (double)vOrig[i].y;
		dA[7][0] += -1 * (double)vTrans[i].x * (double)vOrig[i].x * (double)vOrig[i].y;
		dA[1][7] += -1 * (double)vTrans[i].x * (double)vOrig[i].y * (double)vOrig[i].y;
		dA[7][1] += -1 * (double)vTrans[i].x * (double)vOrig[i].y * (double)vOrig[i].y;
		dA[2][6] += -1 * (double)vTrans[i].x * (double)vOrig[i].x;
		dA[6][2] += -1 * (double)vTrans[i].x * (double)vOrig[i].x;
		dA[2][7] += -1 * (double)vTrans[i].x * (double)vOrig[i].y;
		dA[7][2] += -1 * (double)vTrans[i].x * (double)vOrig[i].y;
		dA[3][6] += -1 * (double)vTrans[i].y * (double)vOrig[i].x * (double)vOrig[i].x;
		dA[6][3] += -1 * (double)vTrans[i].y * (double)vOrig[i].x * (double)vOrig[i].x;
		dA[3][7] += -1 * (double)vTrans[i].y * (double)vOrig[i].x * (double)vOrig[i].y;
		dA[4][6] += -1 * (double)vTrans[i].y * (double)vOrig[i].x * (double)vOrig[i].y;
		dA[6][4] += -1 * (double)vTrans[i].y * (double)vOrig[i].x * (double)vOrig[i].y;
		dA[7][3] += -1 * (double)vTrans[i].y * (double)vOrig[i].x * (double)vOrig[i].y;
		dA[4][7] += -1 * (double)vTrans[i].y * (double)vOrig[i].y * (double)vOrig[i].y;
		dA[7][4] += -1 * (double)vTrans[i].y * (double)vOrig[i].y * (double)vOrig[i].y;
		dA[5][6] += -1 * (double)vTrans[i].y * (double)vOrig[i].x;
		dA[6][5] += -1 * (double)vTrans[i].y * (double)vOrig[i].x;
		dA[5][7] += -1 * (double)vTrans[i].y * (double)vOrig[i].y;
		dA[7][5] += -1 * (double)vTrans[i].y * (double)vOrig[i].y;
		dA[6][6] += (double)vTrans[i].x * (double)vTrans[i].x * (double)vOrig[i].x * (double)vOrig[i].x +
			(double)vTrans[i].y * (double)vTrans[i].y * (double)vOrig[i].x * (double)vOrig[i].x;
		dA[6][7] += (double)vTrans[i].x * (double)vTrans[i].x * (double)vOrig[i].x * (double)vOrig[i].y +
			(double)vTrans[i].y * (double)vTrans[i].y * (double)vOrig[i].x * (double)vOrig[i].y;
		dA[7][6] += (double)vTrans[i].x * (double)vTrans[i].x * (double)vOrig[i].x * (double)vOrig[i].y +
			(double)vTrans[i].y * (double)vTrans[i].y * (double)vOrig[i].x * (double)vOrig[i].y;
		dA[7][7] += (double)vTrans[i].x * (double)vTrans[i].x * (double)vOrig[i].y * (double)vOrig[i].y +
			(double)vTrans[i].y * (double)vTrans[i].y * (double)vOrig[i].y * (double)vOrig[i].y;

		// AT*V
		dV[0] += (double)vTrans[i].x * (double)vOrig[i].x;
		dV[1] += (double)vTrans[i].x * (double)vOrig[i].y;
		dV[2] += (double)vTrans[i].x;
		dV[3] += (double)vTrans[i].y * (double)vOrig[i].x;
		dV[4] += (double)vTrans[i].y * (double)vOrig[i].y;
		dV[5] += (double)vTrans[i].y;
		dV[6] += -1 * ((double)vTrans[i].x * (double)vTrans[i].x * (double)vOrig[i].x +
			(double)vTrans[i].y * (double)vTrans[i].y * (double)vOrig[i].x);
		dV[7] += -1 * ((double)vTrans[i].x * (double)vTrans[i].x * (double)vOrig[i].y +
			(double)vTrans[i].y * (double)vTrans[i].y * (double)vOrig[i].y);
	}

	// AT*A-1 * AT*V
	matinv(8, dA);

	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 8; j++)
		{
			aParam[i] += dA[i][j] * dV[j];
		}
	}

	for (i = 0; i < 8; i++)
	{
		delete dA[i];
	}
	delete [] dA;

}

/// <summary>
/// �ˉe�ϊ��̈ړ����擾
/// </summary>
/// <param name="DstX">�ړ���X�l</param>
/// <param name="DstY">�ړ���Y�l</param>
/// <param name="prm">�z���O���t�B�p�����[�^</param>
/// <param name="SrcX">�ړ���X�l</param>
/// <param name="SrcY">�ړ���Y�l</param>
void CScanDataIO::ProjectionInvPos(int DstX, int DstY, double prm[], double &SrcX, double &SrcY)
{
	SrcX = ((prm[5] - DstY) / (prm[7] * DstY - prm[4]) - (prm[2] - DstX) / (prm[7] * DstX - prm[1]))
			/ ((prm[0] - prm[6] * DstX) / (prm[7] * DstX - prm[1]) - (prm[3] - prm[6] * DstY) / (prm[7] * DstY - prm[4]));
	SrcY = ((prm[5] - DstY) / (prm[6] * DstY - prm[3]) - (prm[2] - DstX) / (prm[6] * DstX - prm[0]))
			/ ((prm[1] - prm[7] * DstX) / (prm[6] * DstX - prm[0]) - (prm[4] - prm[7] * DstY) / (prm[6] * DstY - prm[3]));
}


