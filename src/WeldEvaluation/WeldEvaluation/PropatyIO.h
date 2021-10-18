#pragma once
#include "ConfigrationIO.h"

class CPropatyIO
{
public:
	/// <summary>
	/// 解析方法を表す列挙型
	/// </summary>
	enum {
		AnalizeTypeKMeans		= 0,	///< k-means
		AnalizeTypeHiClustering	= 1		///< 階層クラスタリング
	};

private:
	CString		m_ParamaterFilePath;		///< パラメータファイルへのパス

	CString		m_ProjectName;				///< プロジェクト名
	CString		m_TestName;					///< 名称
	int			m_ResinAnalysisMethod;		///< 樹脂面 解析方法
	int			m_MetalAnalysisMethod;		///< 金属面 解析方法

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

