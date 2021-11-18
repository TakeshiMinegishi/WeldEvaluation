#pragma once
#include "hsi_api_common.h"
#include <vector>
#include "CLog.h"

/// <summary>
/// スキャンデータIOクラス
/// </summary>
class CScanDataIO
{
	///
	typedef struct {
		CubeFloat*	m_o_p_cube;		///< スキャンデータ
		int			m_band;			///< バンドインデックス
		int			m_row;			///< 高さ
		int			m_col;			///< 幅
		float		m_normalizer;	///< バンドにおける正規化パラメータ
		float		m_offset;		///< バンドにおけるオフセット値
	} Normalizer;

private:
	CString		m_pathName;			///< スキャンデータファイルパス
	CubeFloat*	m_o_p_cube;			///< CubeFloatオブジェクトへのポインタ
	std::vector<std::vector<std::vector<float>>>	m_data;
	bool		m_localData;


public:
	CScanDataIO(void);
	~CScanDataIO(void);

private:
	UINT __cdecl getNomalizer(LPVOID p);
	static void executeNomalizerLauncher(void* args);
	bool getRGBBandSpectrum(int &RBand, int &GBand, int &BBand);

public:
	bool open(CString pathName, bool bReload =false);
	void close();
	void release();
	bool LoadImage(int &height, int &width, int &bands, CImage &img);
	bool GetSpectrumData(CPoint &pos, std::vector<double> &data);
	bool getBandSpectrum(std::vector<double> &BandSpectrum);
	bool joinInit();
	bool join(CString pathName, int nOverlap);
	bool joinend(CString outpathName);


	void aff_trans(float **src, float **dst, int srcWidth, int srcHeight, int dstWidth, int dstHeight, double coef[6]);
	void calc_aff_coef(double HScale, double VScale, double Angle, int offsetX, int offsetY, int OutWidth, int OutHeight, double coef[6]);
	bool bicubic(float *** srcdata, int width, int height, int band, float orignX, float orignY, float *p);
	bool scale(int width , int height, CString outpathPath, CString outName);
//	bool affine(int width, int height, double deg, int ofsetX, int offsetY, CString outpathPath, CString outName);

	static void writeLog(CLog::LOGLEVEL level, CString filePath, long lineNo, CString msg);
	static void errorLog(CString filePath, long lineNo, CString i_caller_name, HSI_RETURN i_return_val);

	double ** MatrixInit();
	void MatrixRelease(double ** &mat);
	void MatrixMultiplication(double **src, double **dst);
	void MatrixMove(double **mat, double x, double y);
	void MatrixRotete(double **mat, double angle);
	void MatrixScale(double **mat, double x, double y);
	void MatrixInvers(double **mat);
	bool affine(int srcWidth, int srcHeight, float ***src, int dstWidth, int distHeight, float ***dst, int band, double **mat, bool bBicubic = true);
	bool ScanDataConvert(int srcWidth, int srcHeight, int band, float ***src, double scale, int &dstWidth, int &distHight, float ***& dist, bool bBicubic = true);
	void FreeConvertData(int dstHeight, int band, float ***& dst);

};

