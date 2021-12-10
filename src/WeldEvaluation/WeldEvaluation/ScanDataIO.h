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
//	std::vector<std::vector<std::vector<float>>>	m_data;
	bool		m_localData;

	double		m_RSpectrum;		///< 赤のスペクトル値
	double		m_GSpectrum;		///< 緑のスペクトル値
	double		m_BSpectrum;		///< 青のスペクトル値

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
	void SetRGBWavelength(double r, double g, double b);
	bool LoadImage(int &height, int &width, int &bands, CImage &img);
	bool GetSpectrumData(CPoint &pos, std::vector<double> &data);
	bool getBandSpectrum(std::vector<double> &BandSpectrum);
	int  GetNumberOfBand();
	double getWaveLength(int index);

	bool bicubic(float *** srcdata, int width, int height, int band, float orignX, float orignY, float *p);

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
	bool ScanDataConvert(int srcWidth, int srcHeight, int band, float ***src, double hscale, double vscale, int &dstWidth, int &distHight, float ***& dst, bool bBicubic = true);
	void FreeConvertData(int dstHeight, int band, float ***& dst);

	static bool GetHeaderFilePrm(CString pathName, int &width, int &height);

	void MatrixInvers(int nprm, double **ppMat, double **ppInvMat);
	bool GetHomographyMatrix(CPoint srcPt[4], CPoint dstPt[4], double prm[]);
	void Projection(int SrcX, int SrcY, double prm[], double &dTranX, double &dTranY);

	void matinv(int n, double **a);
	void Calc_ProjectionParam(vector<CPoint> &vOrig, vector<CPoint> &vTrans, double aParam[8]);
	void ProjectionInvPos(int DstX, int DstY, double prm[], double &SrcX, double &SrcY);

	bool InversData(CWnd *pWnd=nullptr);
	bool saveRawData(CString pathName);
	bool readRawData(CString pathName);
};

