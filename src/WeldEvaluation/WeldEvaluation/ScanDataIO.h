#pragma once
#include "hsi_api_common.h"
#include <vector>

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
};

