#include "stdafx.h"
#include "ScanDataIO.h"
#include "FileUtil.h"
#include <iostream>
#include <vector>


#define dRSpectrum	700.0		///< 赤のスペクトル値
#define dGSpectrum	546.1		///< 緑のスペクトル値
#define dBSpectrum	435.8		///< 青のスペクトル値

/// <summary>
/// コンストラクタ
/// </summary>
CScanDataIO::CScanDataIO(void)
{
	m_pathName = _T("");
	m_o_p_cube = nullptr;
}


/// <summary>
/// デストラクタ
/// </summary>
CScanDataIO::~CScanDataIO(void)
{
	release();
}

/// <summary>
/// オープン
/// </summary>
/// <param name="pathName">スキャンデータのフルパス名</param>
/// <param name="bReload">再読込フラグ</param>
/// <returns>成功した場合はtrue,失敗した場合はfalseを返す</returns>
/// @remark bReloadがfalseの場合は、同データが開いている場合は何もせずに終了する
bool CScanDataIO::open(CString pathName, bool bReload/*=false*/ )
{
	if (!CFileUtil::fileExists(pathName)) {
		return false;
	}

	// 再読込有無のチェック
	if (!bReload && (m_pathName.Compare(pathName) == 0)) {
		if (m_o_p_cube) {
			return true;
		}
	}

	// 既に開いてるデータを閉じる
	release();

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
/// CubeFloatのクローズ
/// </summary>
void CScanDataIO::close()
{
	if (m_o_p_cube) {
		m_pathName = _T("");
		commonDeallocateCube(m_o_p_cube);
	}
}

/// <summary>
/// CubeFloatの解放
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
/// 正規化値取得スレッド関数
/// </summary>
/// <param name="width">Normalizer構造体へのポインタ</param>
/// <returns>成功した場合は0を返す</returns>
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
/// 正規化値取得スレッドランチャー
/// </summary>
/// <param name="width">Normalizer構造体へのポインタ</param>
void CScanDataIO::executeNomalizerLauncher(void* args)
{
	reinterpret_cast<CScanDataIO*>(args)->getNomalizer(args);
}


/// <summary>
/// 画像データ取得
/// </summary>
/// <param name="height">スキャンデータのライン数</param>
/// <param name="width">スキャンデータのサンプリング数</param>
/// <param name="bands">スキャンデータのバンド数</param>
/// <param name="img">CImageオブジェクト</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
/// @remark スキャンデータから画像データに変換する
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
			&CScanDataIO::executeNomalizerLauncher,	// ランチャを起動
			0,
			&norm[i]);
	}

	WaitForMultipleObjects(
		3, //スレッド数
		hThreads, //スレッドハンドルの配列
		TRUE, //待機オプション
		INFINITE);//タイムアウト時間

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
/// 指定位置スペクトル群の取得
/// </summary>
/// <param name="pos">スペクトル取得位置</param>
/// <param name="data">スペクトル群</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
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
/// RGBスペクトルのバンドインデクス取得
/// </summary>
/// <param name="RBand">赤(dRSpectrum近傍)のバンドID</param>
/// <param name="GBand">緑(dGSpectrum近傍)のバンドID</param>
/// <param name="BBand">青(dBSpectrum近傍)のバンドID</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
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
/// 各バンドのスペクトル値取得
/// </summary>
/// <param name="BandSpectrum">スペクトル値</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
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


