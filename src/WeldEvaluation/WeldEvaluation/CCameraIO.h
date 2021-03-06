#pragma once
#include "../include/snapscan_api.h"
#include "CLog.h"


#define		WavelengthStart_nm  470;
#define		WavelengthEnd_nm	900;

/// <summary>
/// カメラIOクラス
/// </summary>
class CCameraIO
{
private:
	HANDLE				m_handle;					///< ハンドル
	int					m_width;					///< 撮影幅
	int					m_height;					///< 撮影高さ
	int					m_band;						///< バンド数
	double				m_integrationTime;			///< インテグレーションタイム[ms]
	CubeDataFormat		m_cube_format;				///< キューブフォーマット
	CubeFloat			m_cube;						///< キューブオブジェクト
	FrameFloat			m_dark_reference;			///< ダークリファレンス
	CorrectionMatrix	m_correction_matrix;		///< コレクションマトリックス
	bool				m_bDemoMode;				///< デモモード

private:
	void CameraPrmInit();
	void FleeCameraPrm();
	bool InitLlogger();

public:
	CCameraIO();
	CCameraIO(int width, int height, int band);
	~CCameraIO();
	bool Open(CString snapscan_file,bool dummyApi=true);
	void Close();

	bool getSystemFormat(int &width_max, int &height_max);
	bool setFormat(int width, int height, int band);
	bool getFormat(int &widht, int &height, int &band);
	bool setIntegrationTime(double integrationTime);
	double getIntegrationTime();
	bool StartScan();
	bool StopScan();
	bool AcquireReference(CString refarenceFilePath, CString refarenceFileName);
	bool LoadReference(CubeFloat &reference_corrected, CString refarenceFilePath, CString refarenceFileName);
	bool AcquireSpectralCube(CString spectralFilePath, CString spectralFileName, CubeFloat &reference_corrected, CubeFloat  &cube_corrected);
};

