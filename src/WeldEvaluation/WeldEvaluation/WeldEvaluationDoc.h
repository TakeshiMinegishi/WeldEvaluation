
// WeldEvaluationDoc.h : CWeldEvaluationDoc �N���X�̃C���^�[�t�F�C�X
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
/// �h�L�������g�N���X
/// </summary>
class CWeldEvaluationDoc : public CDocument
{
public:
	enum {
		eResinSurface	= 0,	///< ����
		eMetalSurface	= 1,	///< ����
		eJoiningResult	= 2,	///< �ڍ�����
	} ;

	enum {
		DisplayModeScan			= 0,
		DisplayModeResult		= 1
	};

	enum {
		AnalizeTypeKMeans		= 0,	///< k-means
		AnalizeTypeHiClustering	= 1		///< �K�w�N���X�^�����O
	};

private:
	CString			m_ModulePath;					///< ���W���[���p�X
	CString			m_SystemFilePathName;			///< �V�X�e���ݒ�t�@�C��
	CString			m_ActiveRegisttedTestName;		///< �A�N�e�B�u�ȓo�^�ςݎ�����
	CString			m_ActiveRegisttedTestFolder;	///< �A�N�e�B�u�ȓo�^�ςݎ����ւ̃p�X
	CString			m_ParamaterFilePaht;			///< �A�N�e�B�u�ȃp�����[�^�[�t�@�C���ւ̃p�X
	CString			m_ProjectFilePaht;				///< �A�N�e�B�u�ȃv���W�F�N�g�t�@�C���ւ̃p�X

	CPropatyIO		m_PropatyIO;					///< �v���p�e�B�I�u�W�F�N�g
	CProjectFileIO	m_ProjectIO;					///< �v���W�F�N�g�I�u�W�F�N�g

	int				m_OpenType;						///< �I�[�v���̏�� 0:Close 1:Open 2:New
	bool			m_CalcResultStopRequest;		///< ��͏������~���N�G�X�g�t���O
	CScanDataIO		m_ResinScanData;				///< �����ʃX�L�����f�[�^IO�I�u�W�F�N�g
	CScanDataIO		m_MetalScanData;				///< �����ʃX�L�����f�[�^IO�I�u�W�F�N�g
	CScanDataIO		m_ResultScanData;				///< ���ʃX�L�����f�[�^IO�I�u�W�F�N�g

	bool			m_bVisible;						///< ����ߕs�t���O

	bool			m_bInitialized;					///< �������ς݃t���O
	CSize			m_minRect;						///< �E�C���h�ŏ��T�C�Y

protected: // �V���A��������̂ݍ쐬���܂��B
	CWeldEvaluationDoc();
	DECLARE_DYNCREATE(CWeldEvaluationDoc)

// ����
public:

// ����
public:

// �I�[�o�[���C�h
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// ����
public:
	virtual ~CWeldEvaluationDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// �����n���h���[�̌����R���e���c��ݒ肷��w���p�[�֐�
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS

private:
	CString SetRegistedTestFolder(void);
	bool split(std::string& input, char delimiter, int &dataID, std::vector<int> &data);
	bool getResultFile(CString path, std::vector<int>& data);
	CString getScanImageFilePath(int ScanID);
	CString getClassificationDataFilePath(int ScanID, int type);
	CString getClassificationImageFilePath(int ScanID, int type);
	bool CalcJoJointRetio(std::vector<int>data, int nClass, std::vector<double> &retio);
	std::vector<int> CWeldEvaluationDoc::split(std::string& input, char delimiter, int size= 0);
	void H2RGB(int h, BYTE &r, BYTE &g, BYTE &b);
	void writeLog(CLog::LOGLEVEL level, CString filePath, long lineNo, CString msg);
	bool getAnalizeArea(CPoint &tlPos, CSize &size);


public:
	void SetVisible(bool bVisible);

	//////////////////////////////////////////////////////////////////////////////////
	// System�p�����[�^
	int		GetShootingWidth();
	int		GetShootingHeight();

	double	GetIntegrationTimeMs();
	bool	SetIntegrationTimeMs(double IntegrationTime_ms);
	UINT	NumberOfBand(void);
	bool	SetNumberOfBand(UINT band);
	double	GetVerticalScale();
	double	GetHorizontalScale();
	CString GetWBFileName(void);
	bool	SetWBFileName(CString WBFileName);
	bool	GetMinWndSize(CSize &size);
	bool	SetMinWndSize(CSize size);


	//////////////////////////////////////////////////////////////////////////////////
	// �v���p�e�B�f�[�^IO
	CString GetProjectName();
	bool	SetProjectName(CString projectname);
	CString GetTestName();
	bool	SetTestName(CString name);
	CString GetRegistedFolder(void);
	bool	SetRegistedFolder(CString RegistedFolder);
	UINT	GetVerticalResolution(void);
	bool	SetVerticalResolution(UINT resolution);
	UINT	GetHorizontalResolution();
	bool	SetHorizontalResolution(UINT HorizontalResolution);
	UINT	NumberOfOverridePixel(void);
	bool	SetOverridePixelNumber(UINT num);
	//	UINT	NumberOfBand(void);
	//	bool	SetNumberOfBand(UINT band);
	UINT	ResinGetNumberOfClass(int method);
	bool	ResinSetNumberOfClass(int method, UINT nClass);
	double	ResinGetJointRetio(int method, int ViewJointRatioNo);
	bool	ResinSetJointRetio(int method, int ViewJointRatioNo, double retio);
	COLORREF ResinGetJointColor(int method, int ViewJointRatioNo);
	bool	ResinSetJointColor(int method, int ViewJointRatioNo, COLORREF color);
	int		ResinGetAnalysisMethod();
	bool	ResinSetAnalysisMethod(int method);
	UINT	MetalGetNumberOfClass(int method);
	bool	MetalSetNumberOfClass(int method, UINT nClass);
	double	MetalGetJointRetio(int method, int ViewJointRatioNo);
	bool	MetalSetJointRetio(int method, int ViewJointRatioNo, double retio);
	COLORREF MetalGetJointColor(int method, int ViewJointRatioNo);
	bool	MetalSetJointColor(int method, int ViewJointRatioNo, COLORREF color);
	int		MetalGetAnalysisMethod();
	bool	MetalSetAnalysisMethod(int method);
	UINT	ResultGetNumberOfClass(int method);
	bool	ResultSetNumberOfClass(int method, UINT nClass);
	double	ResultGetJointRetio(int method, int ViewJointRatioNo);
	bool	ResultSetJointRetio(int method, int ViewJointRatioNo, double retio);
	COLORREF ResultGetJointColor(int method, int ViewJointRatioNo);
	bool	ResultSetJointColor(int method, int ViewJointRatioNo, COLORREF color);

	int		GetAnalysisMethod(int targetID);
	bool	SetAnalysisMethod(int targetID, int AnalysisMethodID);

	//////////////////////////////////////////////////////////////////////////////////
	// �v���W�F�N�g�f�[�^IO
	CString getScanDataPath(int ScanID);
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

	bool	GetScanDataSize(int &holizontal, int &vertical);
	bool	SetScanDataSize(int holizontal, int vertical);

	//////////////////////////////////////////////////////////////////////////////////
	// 
	bool	GetWindRect(CRect &rect);
	bool	SetWindRect(CRect &rect);
	bool	GetSpectralDlgRect(CRect &rect);
	bool	SetSpectralDlgRect(CRect &rect);
	bool	GetRGBWavelength(double &r, double &g, double &b);
	bool	GetHomographyPoint(vector<CPoint> &vOrignPt, vector<CPoint> &vTransPt);

	bool	NewProject();
	bool	OpenProject(CString RegistedTestName);
	bool	ProjectUpdate();
	bool	CloseProject(bool bSave=false);
	bool	SaveProject();
	CString GetActiveRegistedTest();
	bool	GetRegistTestList(CStringArray &list);

	bool	IsOpen(void);
	bool	IsNew(void);
	bool	ExistScanFile(int fileID);
	CString GetScanDataName(int ScanID, CString Prefix);
	bool	SaveScanImage(int ScanID);
	bool	IsExistClassificationResultFile(int ScanID, int type);
	bool	IsUpdateNumberOfClassifications(int ScanID, int method);
	bool	SaveClassificationResultFile(int ScanID, int type);
	CString GetDeviceName();
	bool	IsCameraDummyApi();
	CString getSnapscanFile();
	int		GetDivisionNumber();

	//////////////////////////////////////////////////////////////////////////////////
	//
	bool	Analize(int ScanID, int AnalysisMethodID);
	void	CalcResultStopRequest();

	bool	LoadScanImage(int targetID, CImage &img, bool renew = false);
	COLORREF GetClassColor(int id, int nClass);
	bool	LoadClassificationResultImage(int targetID, int type, CImage &img, bool renew = false);
	
	bool	GetSpectrumData(int ScanID, CPoint &pos, std::vector<double> &data);
	bool	GetSpectrumRange(int ScanID, double &min, double &max);
	bool	WriteImage(CString writePath);
	CString getScanDataFilePath(int ScanID);
	CString GetTmpFolderName();
	CString GetTmpFolderPath();
	bool	DeleteScanImageFilePath(int ScanID);
	bool	InversScanData(int ScanID);
	bool	InversAnalizeData(int ScanID);
	bool	IsInversAnalizeData(int ScanID);

	CString GetWorkProjectFolderName();
	CString GetWorkProjectFolderPath();
	CString GetWorkProjectStatus(CString WKPrjPath, CString key);
	bool	SetWorkProjectStatus(CString WKPrjPath, CString key, CString value);
	CString GetWorkProjectPath();
	bool IsWorkProjectUpdated();
	bool SetWorkProjectUpdteStatus(bool bUpdate);
	bool PushProject(CString ResistPath, CString ProjentName);
	bool PopProject(CString ResistPath, CString ProjentName);
	void ClrWorkProject();

	void DeleteAnalizeData(int ScanID);
	void DeleteContents(int ScanID);
	void DeleteWBFile(CString title=_T(""));
	bool DeleteProject(CString ProjentName);

};
