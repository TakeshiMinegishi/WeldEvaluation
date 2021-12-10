#include "stdafx.h"
#include "ScanDataIO.h"
#include "FileUtil.h"
#include <iostream>
#include <vector>
#include "message.h"

#ifndef _DEBUG
#pragma warning(disable:6001)
#endif

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
	m_localData = false;

	m_RSpectrum = dRSpectrum;		// 赤のスペクトル値
	m_GSpectrum = dGSpectrum;		// 緑のスペクトル値
	m_BSpectrum = dBSpectrum;		// 青のスペクトル値
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
		CString msg;
		msg.Format(_T("CScanDataIO::open():ファイルが存在しません(%s)。"), static_cast<LPCWSTR>(pathName));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
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
		m_localData = true;
		CStdioFile tfd;
		// headerデータ出力
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
/// CubeFloatのクローズ
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
/// CubeFloatの解放
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
/// カラー波長の設定
/// </summary>
/// <param name="r">赤の波長</param>
/// <param name="g">緑の波長</param>
/// <param name="b">青の波長</param>
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
		CString msg;
		msg.Format(_T("CScanDataIO::LoadImage():データがOpenされていません。m_o_p_cube=null"));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}

	height = m_o_p_cube->format.height;
	width = m_o_p_cube->format.width;
	bands = m_o_p_cube->format.nr_bands;

	int band[3];
	getRGBBandSpectrum(band[0], band[1], band[2]);

	// イメージの横サイズバウンダリ調整
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
		msg.Format(_T("CScanDataIO::LoadImage():イメージ領域の確保に失敗しました。"));
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
				// オーバーフロー対策
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
				FORMAT_MESSAGE_ALLOCATE_BUFFER							// テキストのメモリ割り当てを要求する
				| FORMAT_MESSAGE_FROM_SYSTEM							// エラーメッセージはWindowsが用意しているものを使用
				| FORMAT_MESSAGE_IGNORE_INSERTS,						// 次の引数を無視してエラーコードに対するエラーメッセージを作成する
				NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),	// 言語を指定
				(LPTSTR)&lpMsgBuf,										// メッセージテキストが保存されるバッファへのポインタ
				0,
				NULL);
			//				AfxMessageBox((LPCTSTR)lpMsgBuf, MB_OK | MB_ICONINFORMATION);
			LocalFree(lpMsgBuf);

			CString msg,fmt;
			fmt = _T("画像の作成に失敗しました。:%s");
			msg.Format(fmt, static_cast<LPCWSTR>(lpMsgBuf));
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
			bResult = false;
		}
		img.ReleaseDC();
	}
	else {
		CString msg;
		msg.Format(_T("CScanDataIO::LoadImage():ビットマップの作成に失敗しました。"));
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
/// 指定位置スペクトル群の取得
/// </summary>
/// <param name="pos">スペクトル取得位置</param>
/// <param name="data">スペクトル群</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CScanDataIO::GetSpectrumData(CPoint &pos, std::vector<double> &data)
{
	bool bResult = true;
	if (m_o_p_cube == nullptr) {
		CString msg;
		msg.Format(_T("CScanDataIO::GetSpectrumData():データがOpenされていません。m_o_p_cube=null"));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}
	if (((pos.x < 0) || (pos.x >= m_o_p_cube->format.width)) || ((pos.y < 0) || (pos.y >= m_o_p_cube->format.height))) {
		CString msg;
		msg.Format(_T("CScanDataIO::GetSpectrumData():スペクトル取得位置が不正です。pos=(%d,%d)"),pos.x,pos.y);
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
/// RGBスペクトルのバンドインデクス取得
/// </summary>
/// <param name="RBand">赤(dRSpectrum近傍)のバンドID</param>
/// <param name="GBand">緑(dGSpectrum近傍)のバンドID</param>
/// <param name="BBand">青(dBSpectrum近傍)のバンドID</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CScanDataIO::getRGBBandSpectrum(int &RBand, int &GBand, int &BBand)
{
	if (m_o_p_cube == nullptr) {
		CString msg;
		msg.Format(_T("CScanDataIO::getRGBBandSpectrum():データがOpenされていません。m_o_p_cube=null"));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}

	double rDiff, gDiff, bDiff, diff;
	double bdat;

	int bands = m_o_p_cube->format.nr_bands;
	if (bands <= 0) {
		CString msg;
		msg.Format(_T("CScanDataIO::getRGBBandSpectrum():バンド値が正しくありません。m_o_p_cube->format.nr_bands=%d"), bands);
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
/// 各バンドのスペクトル値取得
/// </summary>
/// <param name="BandSpectrum">スペクトル値</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CScanDataIO::getBandSpectrum(std::vector<double> &BandSpectrum)
{
	if (m_o_p_cube == nullptr) {
		CString msg;
		msg.Format(_T("CScanDataIO::getBandSpectrum():データがOpenされていません。m_o_p_cube=null"));
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
/// バンド数の取得
/// </summary>
/// <returns>バンド数を返す</returns>
int  CScanDataIO::GetNumberOfBand()
{
	if (m_o_p_cube == nullptr) {
		CString msg;
		msg.Format(_T("CScanDataIO::GetNumberOfBand():データがOpenされていません。m_o_p_cube=null"));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return 0;
	}
	int bands = m_o_p_cube->format.nr_bands;
	return bands;
}

/// <summary>
/// バンドのスペクトル値取得
/// </summary>
/// <param name="index">バンドインデックス</param>
/// <returns>スペクトル値を返す</returns>
double CScanDataIO::getWaveLength(int index)
{
	if (m_o_p_cube == nullptr) {
		CString msg;
		msg.Format(_T("CScanDataIO::getWaveLength():データがOpenされていません。m_o_p_cube=null"));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return 0.0;
	}
	if ((index < 0) || (index >= m_o_p_cube->format.nr_bands)) {
		return 0.0;
	}
	return m_o_p_cube->format.band_names[index];
}

/// <summary>
/// スキャンデータの反転
/// </summary>
/// <returns>成功の場合はtrue、失敗の場合はfalseを返す</returns>
bool CScanDataIO::InversData(CWnd *pWnd/*=nullptr*/)
{
	if (m_o_p_cube == nullptr) {
		CString msg;
		msg.Format(_T("CScanDataIO::InversData():データがOpenされていません。m_o_p_cube=null"));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}

	bool bResult = true;
	int height = m_o_p_cube->format.height;
	int width = m_o_p_cube->format.width;
	int band = m_o_p_cube->format.nr_bands;
#if 0	// 上下反転
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
#else	// 180度回転
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
/// RAWデータの保存
/// </summary>
/// <param name="pathName">パス名</param>
/// <returns>成功の場合はtrue、失敗の場合はfalseを返す</returns>
bool CScanDataIO::saveRawData(CString pathName)
{
	if (m_o_p_cube == nullptr) {
		CString msg;
		msg.Format(_T("CScanDataIO::saveRawData():データがOpenされていません。m_o_p_cube=null"));
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
		msg.Format(_T("CScanDataIO::saveRawData():RAWデータの保存に失敗しました。[%s]"), static_cast<LPCWSTR>(pathName));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
	}
	return bResult;
}

/// <summary>
/// RAWデータの読み込み
/// </summary>
/// <param name="pathName">パス名</param>
/// <returns>成功の場合はtrue、失敗の場合はfalseを返す</returns>
bool CScanDataIO::readRawData(CString pathName)
{
	if (m_o_p_cube == nullptr) {
		CString msg;
		msg.Format(_T("CScanDataIO::readRawData():データがOpenされていません。m_o_p_cube=null"));
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
/// 線形補間
/// </summary>
/// <param name="srcdata">データ</param>
/// <param name="width">データ幅</param>
/// <param name="height">データ高さ</param>
/// <param name="band">バンド数</param>
/// <param name="orignX">補間対象データX値</param>
/// <param name="orignY">補間対象データY値</param>
/// <param name="p">補間値</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
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
/// ログ出力
/// </summary>
/// <param name="level">ログレベル</param>
/// <param name="filePath">エラーファイル</param>
/// <param name="lineNo">エラー行</param>
/// <param name="msg">ログメッセージ</param>
void CScanDataIO::writeLog(CLog::LOGLEVEL level, CString filePath, long lineNo, CString msg)
{
	CLog log;
	CString ErrMsg;
	ErrMsg.Format(_T(" File:%s Line:%ld:%s"), static_cast<LPCWSTR>(filePath), lineNo, static_cast<LPCWSTR>(msg));
	log.logWrite(level, ErrMsg);
}

/// <summary>
/// エラーログ出力
/// </summary>
/// <param name="filePath">エラーファイル</param>
/// <param name="lineNo">エラー行</param>
/// <param name="i_caller_name">エラーコード</param>
/// <param name="i_return_val">呼び出し名</param>
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
/// マトリックス初期化
/// </summary>
/// <returns>マトリックス領域を返す</returns>
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
/// マトリックス開放
/// </summary>
/// <param name="mat">マトリックス</param>
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
/// マトリックスかけ算
/// </summary>
/// <param name="src">マトリックス1</param>
/// <param name="dst">マトリックス2</param>
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
/// マトリックス移動
/// </summary>
/// <param name="mat">マトリックス</param>
/// <param name="x">X移動量</param>
/// <param name="y">Y移動量</param>
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
/// マトリックス回転
/// </summary>
/// <param name="mat">マトリックス</param>
/// <param name="angle">回転角（度）</param>
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
/// マトリックス拡縮
/// </summary>
/// <param name="mat">マトリックス</param>
/// <param name="x">X縮尺率</param>
/// <param name="y">Y縮尺率</param>
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
/// マトリックス逆行列
/// </summary>
/// <param name="mat">マトリックス</param>
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
/// アファイン変換
/// </summary>
/// <param name="srcWidth">変換元幅</param>
/// <param name="srcHeight">変換元高さ</param>
/// <param name="src">変換元データ</param>
/// <param name="dstWidth">変換先幅</param>
/// <param name="distHeight">変換先高さ</param>
/// <param name="dst">変換先データ</param>
/// <param name="band">バンド数</param>
/// <param name="mat">マトリックス</param>
/// <param name="bBicubic">線形補間有無フラグ</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
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
/// スキャンデータ変換
/// </summary>
/// <param name="srcWidth">変換元幅</param>
/// <param name="srcHeight">変換元高さ</param>
/// <param name="band">バンド数</param>
/// <param name="src">変換元データ</param>
/// <param name="hscale">X軸拡縮率</param>
/// <param name="vscale">Y軸拡縮率</param>
/// <param name="dstWidth">変換先幅</param>
/// <param name="dstHeight">変換先高さ</param>
/// <param name="dst">変換先データ</param>
/// <param name="bBicubic">線形補間有無フラグ</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
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
/// 変換データの開放
/// </summary>
/// <param name="dstHeight">変換データ高さ</param>
/// <param name="band">バンド数</param>
/// <param name="dst">変換データ</param>
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
/// ヘッダファイルから幅、高さを取得
/// </summary>
/// <param name="pathName">ヘッダファイルへのパス</param>
/// <param name="width">幅</param>
/// <param name="height">高さ</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CScanDataIO::GetHeaderFilePrm(CString pathName, int &width, int &height)
{
	bool bResult = true;
	if (!CFileUtil::fileExists(pathName)) {
		CString msg;
		msg.Format(_T("CScanDataIO::GetHeaderFilePrm():ファイルが存在しません。(%s)"), static_cast<LPCWSTR>(pathName));
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
		msg.Format(_T("CScanDataIO::GetHeaderFilePrm():ファイルが壊れている可能性があります。(%s)"), static_cast<LPCWSTR>(pathName));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		bResult = false;
	}
	return bResult;
}

/// <summary>
/// 逆行列の算出
/// </summary>
/// <param name="nprm">次数</param>
/// <param name="ppMat">元マトリックス</param>
/// <param name="ppInvMat">逆行列マトリックス</param>
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
/// ホモグラフィーマトリックス取得
/// </summary>
/// <param name="srcPt">移動元頂点</param>
/// <param name="dstPt">移動先頂点</param>
/// <param name="prm">ホモグラフィーマトリックスパラメータ</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
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

		// 逆行列
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
		msg.Format(_T("CScanDataIO::GetHomographyMatrix():ホモグラフィーマトリックスの算出に失敗しました。"));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
	}

	return bResult;
}

/// <summary>
/// 射影先点の取得
/// </summary>
/// <param name="SrcX">移動元X値</param>
/// <param name="SrcY">移動元Y値</param>
/// <param name="prm">ホモグラフィパラメータ</param>
/// <param name="dTranX">移動先X値</param>
/// <param name="dTranY">移動先Y値</param>
/// @remark aのマトリックスを逆行列をaに設定
void CScanDataIO::Projection(int SrcX, int SrcY, double prm[], double &dTranX, double &dTranY)
{
	dTranX = ((double)SrcX*prm[0] + (double)SrcY*prm[1] + prm[2]) /	((double)SrcX*prm[6] + (double)SrcY*prm[7] + 1);
	dTranY = ((double)SrcX*prm[3] + (double)SrcY*prm[4] + prm[5]) /	((double)SrcX*prm[6] + (double)SrcY*prm[7] + 1);
}

/// <summary>
/// マトリックス逆行列
/// </summary>
/// <param name="n">次数</param>
/// <param name="a">マトリックス</param>
/// @remark aのマトリックスを逆行列をaに設定
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
/// 射影変換
/// </summary>
/// <param name="vOrig">移動元データ</param>
/// <param name="vTrans">移動先データ</param>
/// <param name="aParam">ホモグラフィパラメータ</param>
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
/// 射影変換の移動元取得
/// </summary>
/// <param name="DstX">移動先X値</param>
/// <param name="DstY">移動先Y値</param>
/// <param name="prm">ホモグラフィパラメータ</param>
/// <param name="SrcX">移動元X値</param>
/// <param name="SrcY">移動元Y値</param>
void CScanDataIO::ProjectionInvPos(int DstX, int DstY, double prm[], double &SrcX, double &SrcY)
{
	SrcX = ((prm[5] - DstY) / (prm[7] * DstY - prm[4]) - (prm[2] - DstX) / (prm[7] * DstX - prm[1]))
			/ ((prm[0] - prm[6] * DstX) / (prm[7] * DstX - prm[1]) - (prm[3] - prm[6] * DstY) / (prm[7] * DstY - prm[4]));
	SrcY = ((prm[5] - DstY) / (prm[6] * DstY - prm[3]) - (prm[2] - DstX) / (prm[6] * DstX - prm[0]))
			/ ((prm[1] - prm[7] * DstX) / (prm[6] * DstX - prm[0]) - (prm[4] - prm[7] * DstY) / (prm[6] * DstY - prm[3]));
}


