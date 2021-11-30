#pragma once
#include "../include/snapscan_api.h"
#include "CLog.h"


#define		WavelengthStart_nm  470;
#define		WavelengthEnd_nm	900;

/// <summary>
/// �J����IO�N���X
/// </summary>
class CCameraIO
{
private:
	HANDLE				m_handle;					///< �n���h��
	int					m_width;					///< �B�e��
	int					m_height;					///< �B�e����
	int					m_band;						///< �o���h��
	double				m_integrationTime;			///< �C���e�O���[�V�����^�C��[ms]
	CubeDataFormat		m_cube_format;				///< �L���[�u�t�H�[�}�b�g
	CubeFloat			m_cube;						///< �L���[�u�I�u�W�F�N�g
	FrameFloat			m_dark_reference;			///< �_�[�N���t�@�����X
	CorrectionMatrix	m_correction_matrix;		///< �R���N�V�����}�g���b�N�X
	bool				m_bDemoMode;				///< �f�����[�h

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

