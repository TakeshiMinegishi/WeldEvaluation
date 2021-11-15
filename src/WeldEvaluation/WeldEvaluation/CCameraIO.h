#pragma once
#include "../include/snapscan_api.h"
#include "CLog.h"


#define		WavelengthStart_nm  470;
#define		WavelengthEnd_nm	900;

class CCameraIO
{
private:
	HANDLE				m_handle;
	int					m_width;
	int					m_height;
	int					m_band;
	double				m_integrationTime;
	CubeDataFormat		m_cube_format;
	CubeFloat			m_cube;
	CorrectionMatrix	m_correction_matrix;


private:
	void CameraPrmInit();
	void FleeCameraPrm();
	void writeLog(CLog::LOGLEVEL level, CString filePath, long lineNo, CString msg);
	void errorLog(CString filePath, long lineNo, CString i_caller_name, HSI_RETURN i_return_val);
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
	bool AcquireReference(CString refarenceFilePath, CString refarenceFileName);
	bool LoadReference(CubeFloat &reference_corrected, CString refarenceFilePath, CString refarenceFileName);
	bool AcquireSpectralCube(CString spectralFilePath, CString spectralFileName, CubeFloat &reference_corrected);
};

