
// WeldEvaluationDoc.h : CWeldEvaluationDoc クラスのインターフェイス
//
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include "PropatyIO.h"
#include "ProjectFileIO.h"
#include "ScanDataIO.h"

#pragma once

/// <summary>
/// ドキュメントクラス
/// </summary>
class CWeldEvaluationDoc : public CDocument
{
public:
	enum {
		eResinSurface	= 0,	///< 樹脂
		eMetalSurface	= 1,	///< 金属
		eJoiningResult	= 2,	///< 接合結果
	} ;

	enum {
		DisplayModeScan			= 0,
		DisplayModeResult		= 1
	};

	enum {
		AnalizeTypeKMeans		= 0,	///< k-means
		AnalizeTypeHiClustering	= 1		///< 階層クラスタリング
	};

private:
	CString			m_ModulePath;					///< モジュールパス
	CString			m_SystemFilePathName;			///< システム設定ファイル
	CString			m_ActiveRegisttedTestName;		///< アクティブな登録済み試験名
	CString			m_ActiveRegisttedTestFolder;	///< アクティブな登録済み試験へのパス
	CString			m_ParamaterFilePaht;			///< アクティブなパラメーターファイルへのパス
	CString			m_ProjectFilePaht;				///< アクティブなプロジェクトファイルへのパス

	CPropatyIO		m_PropatyIO;					///< プロパティオブジェクト
	CProjectFileIO	m_ProjectIO;					///< プロジェクトオブジェクト

	int				m_OpenType;						///< オープンの状態 0:Close 1:Open 2:New
	bool			m_CalcResultStopRequest;		///< 解析処理中止リクエストフラグ
	CScanDataIO		m_ResinScanData;				///< 樹脂面スキャンデータIOオブジェクト
	CScanDataIO		m_MetalScanData;				///< 金属面スキャンデータIOオブジェクト
	CScanDataIO		m_ResultScanData;				///< 結果スキャンデータIOオブジェクト


protected: // シリアル化からのみ作成します。
	CWeldEvaluationDoc();
	DECLARE_DYNCREATE(CWeldEvaluationDoc)

// 属性
public:

// 操作
public:

// オーバーライド
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 実装
public:
	virtual ~CWeldEvaluationDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成された、メッセージ割り当て関数
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 検索ハンドラーの検索コンテンツを設定するヘルパー関数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS

private:
	CString SetRegistedTestFolder(void);
	bool split(std::string& input, char delimiter, int &dataID, std::vector<int> &data);
	bool getResultFile(CString path, std::vector<int>& data);
	CString getScanDataPath(int ScanID);
	CString getScanDataFilePath(int ScanID);
	CString getScanImageFilePath(int ScanID);
	CString getClassificationDataFilePath(int ScanID, int type);
	CString getClassificationImageFilePath(int ScanID, int type);
	bool CalcJoJointRetio(std::vector<int>data, int nClass, std::vector<double> &retio);
	std::vector<int> CWeldEvaluationDoc::split(std::string& input, char delimiter, int size= 0);
	void H2RGB(int h, BYTE &r, BYTE &g, BYTE &b);


public:

	//////////////////////////////////////////////////////////////////////////////////
	// プロパティデータIO
	CString GetProjectName();
	bool	SetProjectName(CString projectname);
	CString GetTestName();
	bool	SetTestName(CString name);
	CString GetRegistedFolder(void);
	bool	SetRegistedFolder(CString RegistedFolder);
	CString GetWBFileName(void);
	bool	SetWBFileName(CString WBFileName);
	UINT	NumberOfOverridePixel(void);
	bool	SetOverridePixelNumber(UINT num);
	UINT	GetIntegrationTimeMs(void);
	bool	SetIntegrationTimeMs(UINT time);
	UINT	GetVerticalResolution(void);
	bool	SetVerticalResolution(UINT resolution);
	UINT	GetHorizontalResolution(void);
	bool	SetHorizontalResolution(UINT resolution);
	UINT	ResinGetNumberOfClass(void);
	bool	ResinSetNumberOfClass(UINT nClass);
	double	ResinGetJointRetio(int ViewJointRatioNo);
	bool	ResinSetJointRetio(int ViewJointRatioNo, double retio);
	COLORREF ResinGetJointColor(int ViewJointRatioNo);
	bool	ResinSetJointColor(int ViewJointRatioNo, COLORREF color);
	int		ResinGetAnalysisMethod();
	bool	ResinSetAnalysisMethod(int method);
	UINT	MetalGetNumberOfClass(void);
	bool	MetalSetNumberOfClass(UINT nClass);
	double	MetalGetJointRetio(int ViewJointRatioNo);
	bool	MetalSetJointRetio(int ViewJointRatioNo, double retio);
	COLORREF MetalGetJointColor(int ViewJointRatioNo);
	bool	MetalSetJointColor(int ViewJointRatioNo, COLORREF color);
	int		MetalGetAnalysisMethod();
	bool	MetalSetAnalysisMethod(int method);
	UINT	ResultGetNumberOfClass(void);
	bool	ResultSetNumberOfClass(UINT nClass);
	double	ResultGetJointRetio(int ViewJointRatioNo);
	bool	ResultSetJointRetio(int ViewJointRatioNo, double retio);
	COLORREF ResultGetJointColor(int ViewJointRatioNo);
	bool	ResultSetJointColor(int ViewJointRatioNo, COLORREF color);

	int		GetAnalysisMethod(int targetID);
	bool	SetAnalysisMethod(int targetID, int AnalysisMethodID);

	//////////////////////////////////////////////////////////////////////////////////
	// プロジェクトデータIO
//	CString GetResinScanImagePath();
//	CString GetResinClassificationResultPath();

//	CString GetMetalScanImagePath();
//	CString GetMetalClassificationResultPath();

//	CString GetClassScanImagePath();
	CString GetScanImagePath(int fileID);
	CString GetClassificationResultPath(int fileID, int type);

	int		GetDisplayMode(int targetID);
	bool	SetDisplayMode(int targetID, int DisplayModeID);

	UINT	GetSpectralGraphPointTarget();
	bool	SetSpectralGraphPointTarget(UINT targetID);
	bool	GetSpectralGraphPointPosition(int &holizontal, int &vertical);
	bool	SetSpectralGraphPointPosition(int holizontal, int vertical);

	UINT	GetSpectralGraphSectionTarget();
	bool	SetSpectralGraphSectionTarget(UINT targetID);
	bool	GetSpectralGraphSectionStartPosition(int &holizontal, int &vertical);
	bool	SetSpectralGraphSectionStartPosition(int holizontal, int vertical);
	bool	GetSpectralGraphSectionEndPosition(int &holizontal, int &vertical);
	bool	SetSpectralGraphSectionEndPosition(int holizontal, int vertical);

	//////////////////////////////////////////////////////////////////////////////////
	// 
	bool	GetWindRect(CRect &rect);
	bool	SetWindRect(CRect &rect);
	bool	GetSpectralDlgRect(CRect &rect);
	bool	SetSpectralDlgRect(CRect &rect);

	bool	NewProject();
	bool	OpenProject(CString RegistedTestName);
	bool	CloseProject(bool bSave=false);
	bool	SaveProject();
	CString GetActiveRegistedTest();
	bool	GetRegistTestList(CStringArray &list);

	bool	IsOpen(void);
	bool	IsNew(void);
	bool	ExistScanFile(int fileID);
	bool	SaveScanImage(int ScanID);
	bool	ExistClassificationResultFile(int fileID, int type);
	bool	SaveClassificationResultFile(int ScanID, int type);

	//////////////////////////////////////////////////////////////////////////////////
	//
	bool	Analize(int targetID, int AnalysisMethodID);
	void	CalcResultStopRequest();

	bool	LoadScanImage(int targetID, CImage &img, bool renew = false);
	bool	LoadClassificationResultImage(int targetID, int type, CImage &img, bool renew = false);
	
	bool	GetSpectrumData(int ScanID, CPoint &pos, std::vector<double> &data);
};
