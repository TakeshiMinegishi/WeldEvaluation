#pragma once
#include "ConfigrationIO.h"

class CPropatyIO
{
public:
	/// <summary>
	/// ��͕��@��\���񋓌^
	/// </summary>
	enum {
		AnalizeTypeKMeans		= 0,	///< k-means
		AnalizeTypeHiClustering	= 1		///< �K�w�N���X�^�����O
	};

private:
	CString		m_ParamaterFilePath;		///< �p�����[�^�t�@�C���ւ̃p�X

	CString		m_ProjectName;				///< �v���W�F�N�g��
	CString		m_TestName;					///< ����
	int			m_ResinAnalysisMethod;		///< ������ ��͕��@
	int			m_MetalAnalysisMethod;		///< ������ ��͕��@

private:
	bool read(CString path);
	bool save(CString path);

public:
	CPropatyIO(void);
	~CPropatyIO(void);

	CString GetParamaterFilePath();
	bool SetParamaterFilePath(CString path);


	CString MakeProjectName(CString TestName, COleDateTime date, int nunber);
	CString GetProjectName();
	bool SetProjectName(CString projectname);

	CString GetTestName();
	bool SetTestName(CString name);

	UINT NumberOfOverridePixel(void);
	bool SetOverridePixelNumber(UINT num);
	UINT GetIntegrationTimeMs(void);
	bool SetIntegrationTimeMs(UINT time);
	UINT GetVerticalResolution(void);
	bool SetVerticalResolution(UINT resolution);
	UINT GetHorizontalResolution(void);
	bool SetHorizontalResolution(UINT resolution);

	UINT ResinGetNumberOfClass(void);
	bool ResinSetNumberOfClass(UINT nClass);
	double ResinGetJointRetio(int ViewJointRatioNo);
	bool ResinSetJointRetio(int ViewJointRatioNo, double retio);
	int ResinGetAnalysisMethod();
	bool ResinSetAnalysisMethod(int method);

	UINT MetalGetNumberOfClass(void);
	bool MetalSetNumberOfClass(UINT nClass);
	double MetalGetJointRetio(int ViewJointRatioNo);
	bool MetalSetJointRetio(int ViewJointRatioNo, double retio);
	int MetalGetAnalysisMethod();
	bool MetalSetAnalysisMethod(int method);

	UINT ResultGetNumberOfClass(void);
	bool ResultSetNumberOfClass(UINT nClass);
	double ResultGetJointRetio(int ViewJointRatioNo);
	bool ResultSetJointRetio(int ViewJointRatioNo, double retio);

	bool Read(CString path = _T(""));
	bool Save(bool Update = true, CString path = _T(""));
};

