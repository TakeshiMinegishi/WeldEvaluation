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
#if 1
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
						dval = _ttof(val);
						m_o_p_cube->format.band_names[cnt] = dval;
					}
					tfd.ReadString(buf);
				}
			}
			m_o_p_cube->format.size_bytes = m_o_p_cube->format.width * m_o_p_cube->format.height * m_o_p_cube->format.nr_bands * sizeof(float);
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
#else
		errorLog(CString(__FILE__), __LINE__, _T("commonLoadCube()"), result);
		close();
		return false;
#endif
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
			int widht = m_o_p_cube->format.width;
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
		return false;
	}

	height = m_o_p_cube->format.height;
	width = m_o_p_cube->format.width;
	bands = m_o_p_cube->format.nr_bands;

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

	unsigned char * p24Img = new unsigned char[bmInfohdr.biSizeImage*8];
	unsigned char * ptr = p24Img;

	unsigned char r, g, b;
	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {
			if (col >= m_o_p_cube->format.width) {
				r = g = b = 0;
			}
			else {
				// �I�[�o�[�t���[�΍�
				r = (BYTE)((m_o_p_cube->ppp_data[band[0]][row][col] > 1.0 ? 1.0 : (m_o_p_cube->ppp_data[band[0]][row][col] < 0.0 ? 0.0 : m_o_p_cube->ppp_data[band[0]][row][col])) * dnormalizer);
				g = (BYTE)((m_o_p_cube->ppp_data[band[1]][row][col] > 1.0 ? 1.0 : (m_o_p_cube->ppp_data[band[1]][row][col] < 0.0 ? 0.0 : m_o_p_cube->ppp_data[band[1]][row][col])) * dnormalizer);
				b = (BYTE)((m_o_p_cube->ppp_data[band[2]][row][col] > 1.0 ? 1.0 : (m_o_p_cube->ppp_data[band[0]][row][col] < 0.0 ? 0.0 : m_o_p_cube->ppp_data[band[2]][row][col])) * dnormalizer);
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
		ptr += 0;
	}

	if (img.Create(width, height, 8 * Bpp, NULL)) {
		HDC dc = img.GetDC();
		int ret = SetDIBitsToDevice(dc, 0, 0, width, height, 0, 0, 0, height, p24Img, &bmInfo, DIB_RGB_COLORS);
		if(ret == 0) {
			DWORD err = GetLastError();
			LPVOID lpMsgBuf;
			FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER  //      �e�L�X�g�̃��������蓖�Ă�v������
				| FORMAT_MESSAGE_FROM_SYSTEM    //      �G���[���b�Z�[�W��Windows���p�ӂ��Ă�����̂��g�p
				| FORMAT_MESSAGE_IGNORE_INSERTS,//      ���̈����𖳎����ăG���[�R�[�h�ɑ΂���G���[���b�Z�[�W���쐬����
				NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),//   ������w��
				(LPTSTR)&lpMsgBuf,                          //      ���b�Z�[�W�e�L�X�g���ۑ������o�b�t�@�ւ̃|�C���^
				0,
				NULL);
//			AfxMessageBox((LPCTSTR)lpMsgBuf, MB_OK | MB_ICONINFORMATION);
			LocalFree(lpMsgBuf);
			bResult = false;
		}
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

#if 0	// �폜����R�[�h
typedef struct {
	int		m_sx;
	int		m_sy;
	int		m_tx;
	int		m_ty;
	double	m_diff;
} sDifPos;

bool CScanDataIO::joinInit()
{
	try {
		for (int b = 0; b > m_data.size(); b++) {
			for (int r = 0; r > m_data[b].size(); r++) {
				m_data[b][r].clear();
			}
			m_data[b].clear();
		}
		m_data.clear();

		int bands = m_o_p_cube->format.nr_bands;
		int height = m_o_p_cube->format.height;
		int width = m_o_p_cube->format.width;

		m_data.resize(bands);
		for (int b = 0; b < bands; b++) {
			m_data[b].resize(height);
			for (int row = 0; row < height; row++) {
				m_data[b][row].resize(width);
			}
		}

		for (int b = 0; b < bands; b++) {
			for (int row = 0; row < height; row++) {
				for (int col = 0; col < width; col++) {
					m_data[b][row][col] = m_o_p_cube->ppp_data[b][row][col];
				}
			}
		}
	}
	catch (...) {
		return false;
	}
	return true;
}

bool CScanDataIO::join(CString pathName, int nOverlap)
{
	CScanDataIO target;
	if (!target.open(pathName, true)) {
		return false;
	}
	int bands = target.m_o_p_cube->format.nr_bands;

	int xrange = m_o_p_cube->format.width / 10;
	int ssx = m_o_p_cube->format.width / 2 - xrange / 2;
	int sex = m_o_p_cube->format.width / 2 + xrange / 2;

	float min_diff = -1.0;
	vector<sDifPos> difPos;
	float sdata, diff,sums;
	int band = 0;
	vector<float> diffs;
	vector<vector<float>> list;
	int svx, svy;

	int sy = ((m_o_p_cube->format.height - 1) - nOverlap) - nOverlap/2;

	for (int x = 0; x < xrange; x++) {
		for (int y = 0; y < nOverlap; y++) {
			sums = 0.0;
			int dx = x - xrange / 2;
			for (int sx = ssx+dx; sx < sex+dx; sx++) {
				sdata = 0.0;
				for (int row = 0; row < nOverlap; row++)
				{
					diff = fabs(m_o_p_cube->ppp_data[band][sy + y + row][sx] - target.m_o_p_cube->ppp_data[band][row][sx]);
					sdata += diff;
				}
				sums += sdata / nOverlap;
			}
			sums /= xrange;
			diffs.push_back(sums);
			if ((min_diff < 0.0) || (min_diff > sums)) {
				min_diff = sums;
				svx = x;
				svy = y;
			}
		}
		list.push_back(diffs);
		diffs.clear();
	}

	int offset_col = -3;
	int offset_row = -3;
	int resizewidth = m_o_p_cube->format.width + target.m_o_p_cube->format.width + offset_col;
	int jrow = offset_row;
	for (int b = 0; b < bands; b++) {
		for (int row = 0; row < jrow; row++, jrow++) {
			m_data[b][row].resize(resizewidth);
			for (int col = offset_col; col < target.m_o_p_cube->format.width; col++) {
				if (col < 0) {
					continue;
				}
				m_data[b][row][col] = 0.0;
			}
		}
		for (int row = 0; row < target.m_o_p_cube->format.height; row++, jrow++) {
			if (jrow < 0) {
				continue;
			}
			m_data[b][row].resize(resizewidth);
			for (int col = offset_col; col < target.m_o_p_cube->format.width; col++) {
				if (col < 0) {
					continue;
				}
				m_data[b][row][col] = target.m_o_p_cube->ppp_data[b][row][col];
			}
		}
		for (int row = jrow; row < m_o_p_cube->format.height; row++, jrow++) {
			m_data[b][row].resize(resizewidth);
			for (int col = offset_col; col < target.m_o_p_cube->format.width; col++) {
				if (col < 0) {
					continue;
				}
				m_data[b][row][col] = 0.0;
			}
		}
	}

	target.close();
	return true;
}

bool CScanDataIO::joinend(CString outpathName)
{
	if (m_data.size() <= 0) {
		return false;
	}
	if (m_data[0].size() <= 0) {
		return false;
	}
	if (m_data[0][0].size() <= 0) {
		return false;
	}

	int band = (int)m_data.size();
	int width = (int)m_data[0][0].size();
	int height = (int)m_data[0].size();


	float *** ppp_data;
	ppp_data = new float **[m_data.size()];
	for (int b = 0; b < band; b++) {
		ppp_data[b] = new float*[m_data[b].size()];
		for (int r = 0; r < height; r++) {
			ppp_data[b][r] = new float[m_data[b][r].size()];
			for (int c = 0; c < width; c++) {
				ppp_data[b][r][c] = m_data[b][r][c];
			}
		}
	}

	int m_o_p_cube_height = m_o_p_cube->format.height;
	int m_o_p_cube_bands = m_o_p_cube->format.nr_bands;

	for (int b = 0; b > m_o_p_cube_bands; b++) {
		for (int r = 0; r > m_o_p_cube_height; r++) {
			if (m_o_p_cube->ppp_data[b][r]) {
				delete[] m_o_p_cube->ppp_data[b][r];
			}
		}
		delete[] m_o_p_cube->ppp_data[b];
	}
	delete[] m_o_p_cube->ppp_data;

	m_o_p_cube->ppp_data = ppp_data;
	m_o_p_cube->format.width = width;
	m_o_p_cube->format.height = height;

	return true;
}

bool CScanDataIO::scale(int width, int height, CString outpathPath, CString outName)
{
	int X_SIZE = m_o_p_cube->format.width;
	int Y_SIZE = m_o_p_cube->format.height;
	double zx	= (double)width / (double)m_o_p_cube->format.width;
	double zy	= (double)height / (double)m_o_p_cube->format.height;
	int band	= m_o_p_cube->format.nr_bands;

	float *** ppp_data;
	ppp_data = new float **[band];
	for (int b = 0; b < band; b++) {
		ppp_data[b] = new float*[Y_SIZE];
		for (int h = 0; h < Y_SIZE; h++) {
			ppp_data[b][h] = new float[X_SIZE];
			ZeroMemory(ppp_data[b][h], sizeof(float)*X_SIZE);
		}
	}

	int		i, j, m, n, d;
	float	x, y, p, q;
	int		xs = m_o_p_cube->format.width / 2;
	int		ys = m_o_p_cube->format.height / 2;

	for (int b = 0; b < band; b++) {
		for (i = -ys; i < ys; i++) {
			for (j = -xs; j < xs; j++) {
				y = (float)(i / zy);
				x = (float)(j / zx);
				if (y > 0) m = (int)y;
				else m = (int)(y - 1);
				if (x > 0) n = (int)x;
				else n = (int)(x - 1);
				q = y - m;
				p = x - n;
				if (q == 1) { q = 0; m = m + 1; }
				if (p == 1) { p = 0; n = n + 1; }
				if ((m >= -ys) && (m < ys) && (n >= -xs) && (n < xs)) {
					d = (int)((1.0 - q)*((1.0 - p) * m_o_p_cube->ppp_data[b][m + ys][n + xs]
						+ p * m_o_p_cube->ppp_data[b][m + ys][n + 1 + xs])
						+ q * ((1.0 - p) * m_o_p_cube->ppp_data[b][m + 1 + ys][n + xs]
							+ p * m_o_p_cube->ppp_data[b][m + 1 + ys][n + 1 + xs]));
				}
				else {
					d = 0;
				}

//				ppp_data[b][(int)((i + ys)*zy)][(int)((j + xs)*zx)] = d;
				ppp_data[b][i + ys][j + xs] = (float)d;
			}
		}
	}

	CubeFloat*	o_p_cube = new CubeFloat();
	o_p_cube->format = m_o_p_cube->format;
	o_p_cube->format.width	= width;
	o_p_cube->format.height = height;
	o_p_cube->format.size_bytes = width * height * band * sizeof(float);

	o_p_cube->info = m_o_p_cube->info;
	o_p_cube->p_reserved = m_o_p_cube->p_reserved;
	o_p_cube->ppp_data = ppp_data;

	HSI_RETURN return_val = commonSaveCube(*o_p_cube, outpathPath, outName, FF_ENVI);
	if (HSI_OK != return_val)
	{
		errorLog(CString(__FILE__), __LINE__, _T("SaveCube (cube)"), return_val);
		return false;
	}
	return true;
}

void CScanDataIO::calc_aff_coef(double HScale, double VScale, double Angle, int InWidth, int InHeight, int OutWidth, int OutHeight, double coef[6])
{
	double PI = 3.141592648777698869248;
	double rad = (PI / 180.0) * Angle;

	//�@����A�E��A�����A�E���̏��ɔz��
	double dX[4], dY[4];
	double MinX, MinY;

	coef[0] = HScale * cos(rad);
	if (fabs(coef[0]) < 1.0E-6) {
		coef[0] = 0.0;
	}
	coef[1] = VScale * sin(rad);
	if (fabs(coef[1]) < 1.0E-6) {
		coef[1] = 0.0;
	}
	coef[3] = -1 * HScale * sin(rad);
	if (fabs(coef[3]) < 1.0E-6) {
		coef[3] = 0.0;
	}
	coef[4] = VScale * cos(rad);
	if (fabs(coef[4]) < 1.0E-6) {
		coef[4] = 0.0;
	}

	//�ϊ����4���̍��W���v�Z����
	dX[0] = 0.0;
	dX[1] = coef[0] * (double)(InWidth);
	dX[2] = coef[1] * (double)(InHeight);
	dX[3] = (double)OutWidth;
	dY[0] = 0.0;
	dY[1] = coef[3] * (double)(InWidth);
	dY[2] = coef[4] * (double)(InHeight);
	dY[3] = (double)OutHeight;

	//�ϊ���̍��W�l�̍ő�l�ƍŏ��l�����߂�B
	MinX = dX[0];
	MinY = dY[0];

	for (int i = 1; i < 4; i++)
	{
		if (MinX > dX[i]) MinX = dX[i];
		if (MinY > dY[i]) MinY = dY[i];
	}

	//�ϊ���̍��W�����ɂȂ�Ȃ��悤�ɕ��s�ړ��ʂ����肷��B
	coef[2] = -1 * MinX;
	coef[5] = -1 * MinY;
}

void CScanDataIO::aff_trans(float **src, float **dst, int srcWidth, int srcHeight, int dstWidth, int dstHeight, double coef[6])
{
	int r, c;
	int nRow, nCol;
	double dRow, dCol;

	for (r = 0; r < dstHeight; r++) {
		for (c = 0; c < dstWidth; c++) {
			dCol = coef[0] * (double)c + coef[1] * (double)r + coef[2];
			dRow = coef[3] * (double)c + coef[4] * (double)r + coef[5];
			nCol = (int)floor(dCol + 0.5);
			nRow = (int)floor(dRow + 0.5);

			if ((nCol < 0) || (nCol >= srcWidth) || (nRow < 0) || (nRow >= srcHeight)) {
				dst[r][c] = 0.0;
			}
			else {
				dst[r][c] = src[nRow][nCol];
			}
		}
	}

	return;
}
#endif

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
	ErrMsg.Format(_T(" File:%s Line:%ld:%s"), (LPCTSTR)filePath, lineNo, (LPCTSTR)msg);
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
	ErrMsg.Format(_T(" File:%s Line:%ld:%s:%s"), (LPCTSTR)filePath, lineNo, (LPCTSTR)i_caller_name, (LPCTSTR)errs);
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
/// <param name="distHeight">�ϊ��捂��</param>
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
/// <param name="distHeight">�ϊ��f�[�^����</param>
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
		return false;
	}

	int find = 0;
	int bands = 0;
	CStdioFile tfd;
	if (tfd.Open(pathName, CFile::modeRead | CFile::typeText)) {
		CString buf;
		CString key, val;
		int byteoder, datatype;
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
		bResult = false;
	}
	return bResult;
}

bool CScanDataIO::GetHomographyMatrix(CPoint srcPt[4], CPoint dstPt[4], double prm[6])
{
	bool bResult = true;

	return bResult;
}
