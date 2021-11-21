#pragma once
#include "ConfigrationIO.h"

/// <summary>
/// �v���p�e�BIO�N���X
/// </summary>
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

	static bool WriteProjectName(CString PropatyFilePath, CString ProjectName);
	static bool WriteTestName(CString PropatyFilePath, CString TestName);

#if 0
	double GetIntegrationTimeMs(void);
	bool SetIntegrationTimeMs(double time);
	UINT GetNumberOfBand(void);
	bool SetNumberOfBand(UINT band);
#endif
	UINT NumberOfOverridePixel(void);
	bool SetOverridePixelNumber(UINT num);
	UINT GetVerticalResolution(void);
	bool SetVerticalResolution(UINT resolution);
	UINT GetHorizontalResolution(void);
	bool SetHorizontalResolution(UINT resolution);

	UINT ResinGetNumberOfClass(void);
	bool ResinSetNumberOfClass(UINT nClass);
	double ResinGetJointRetio(int ViewJointRatioNo);
	bool ResinSetJointRetio(int ViewJointRatioNo, double retio);
	COLORREF ResinGetJointColor(int ViewJointRatioNo);
	bool ResinSetJointColor(int ViewJointRatioNo, COLORREF color);
	int ResinGetAnalysisMethod();
	bool ResinSetAnalysisMethod(int method);

	UINT MetalGetNumberOfClass(void);
	bool MetalSetNumberOfClass(UINT nClass);
	double MetalGetJointRetio(int ViewJointRatioNo);
	bool MetalSetJointRetio(int ViewJointRatioNo, double retio);
	COLORREF MetalGetJointColor(int ViewJointRatioNo);
	bool MetalSetJointColor(int ViewJointRatioNo, COLORREF color);
	int MetalGetAnalysisMethod();
	bool MetalSetAnalysisMethod(int method);

	UINT ResultGetNumberOfClass(void);
	bool ResultSetNumberOfClass(UINT nClass);
	COLORREF ResultGetJointColor(int ViewJointRatioNo);
	bool ResultSetJointColor(int ViewJointRatioNo, COLORREF color);
	double ResultGetJointRetio(int ViewJointRatioNo);
	bool ResultSetJointRetio(int ViewJointRatioNo, double retio);

	bool Read(CString path = _T(""));
	bool Save(bool Update = true, CString path = _T(""));
};

