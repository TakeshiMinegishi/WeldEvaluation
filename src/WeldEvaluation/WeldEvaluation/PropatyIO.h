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
	int			m_ResultAnalysisMethod;		///< ���ʖ� ��͕��@
	CString		m_joinRetioFormat;			///< �ڍ���t�H�[�}�b�g
	CString		m_joinColorFormat;			///< �ڍ��F�t�H�[�}�b�g
	CString		m_nClassFormat;				///< ���ސ��t�H�[�}�b�g  


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

	UINT ResinGetNumberOfClass(int method);
	bool ResinSetNumberOfClass(int method,UINT nClass);
	double ResinGetJointRetio(int method, int ViewJointRatioNo);
	bool ResinSetJointRetio(int method, int ViewJointRatioNo, double retio);
	COLORREF ResinGetJointColor(int method, int ViewJointRatioNo);
	bool ResinSetJointColor(int method, int ViewJointRatioNo, COLORREF color);
	int ResinGetAnalysisMethod();
	bool ResinSetAnalysisMethod(int method);

	UINT MetalGetNumberOfClass(int method);
	bool MetalSetNumberOfClass(int method,UINT nClass);
	double MetalGetJointRetio(int method, int ViewJointRatioNo);
	bool MetalSetJointRetio(int method, int ViewJointRatioNo, double retio);
	COLORREF MetalGetJointColor(int method, int ViewJointRatioNo);
	bool MetalSetJointColor(int method, int ViewJointRatioNo, COLORREF color);
	int MetalGetAnalysisMethod();
	bool MetalSetAnalysisMethod(int method);

	UINT ResultGetNumberOfClass(int method);
	bool ResultSetNumberOfClass(int method, UINT nClass);
	double ResultGetJointRetio(int method, int ViewJointRatioNo);
	bool ResultSetJointRetio(int method, int ViewJointRatioNo, double retio);
	COLORREF ResultGetJointColor(int method, int ViewJointRatioNo);
	bool ResultSetJointColor(int method, int ViewJointRatioNo, COLORREF color);
	int ResultGetAnalysisMethod();
	bool ResultSetAnalysisMethod(int method);

	bool Read(CString path = _T(""));
	bool Save(bool Update = true, CString path = _T(""));
};

