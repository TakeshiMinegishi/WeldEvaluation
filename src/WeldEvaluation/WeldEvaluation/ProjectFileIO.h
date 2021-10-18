#pragma once
class CProjectFileIO
{
public:
	enum {
		eResinSurface	= 0,	///< 樹脂
		eMetalSurface	= 1,	///< 金属
		eJoiningResult	= 2,	///< 接合結果
	} ;

private:
	CString			m_ProjectFilePath;								///< プロジェクトファイルへのパス

	COleDateTime	m_CreeateDay;									///< 作成日
	COleDateTime	m_UpdateDay;									///< 更新簿
	CString			m_ImageDataRootPath;							///< イメージデータへのルートパス

	CString			m_ResinScanImageFile;							///< 樹脂面スキャン画像ファイル名
	CString			m_ResinClassificationResultFile;				///< 樹脂面分類結果ファイル名

	CString			m_MetalScanImageFile;							///< 金属面スキャン画像ファイル名
	CString			m_MetalClassificationResultFile;				///< 金属面分類結果ファイル名

	CString			m_ResultScanImageFile;							///< 結果画像スキャン画像ファイル名

/*
	int				m_ResinDisplayType;								///< 樹脂画像表示タイプ
	int				m_MetalDisplayType;								///< 金属画像表示タイプ
	int				m_ResultDisplayType;							///< 結果画像表示タイプ
*/

	UINT			m_SpectralGraphPointTarget;						///< スペクトルグラフ（点指定）の対象
	int				m_SpectralGraphPointHolizontalPosition;			///< スペクトルグラフ（点指定）の水平位置
	int				m_SpectralGraphPointVerticalPosition;			///< スペクトルグラフ（点指定）の垂直位置

	UINT			m_SpectralGraphSectionTarget;					///< スペクトルグラフ（区間指定）の対象
	int				m_SpectralGraphSectionHolizontalStartPosition;	///< スペクトルグラフ（点指定）の水平開始位置
	int				m_SpectralGraphSectionVerticalStartPosition;	///< スペクトルグラフ（点指定）の垂直開始位置
	int				m_SpectralGraphSectionHolizontalEndPosition;	///< スペクトルグラフ（点指定）の水平終了位置
	int				m_SpectralGraphSectionVerticalEndPosition;		///< スペクトルグラフ（点指定）の垂直終了位置

private:
	bool read(CString path);
	bool save(CString path);

public:
	CProjectFileIO(void);
	~CProjectFileIO(void);

	void Initialze();

	CString GetProjectFilePath();
	bool SetProjectFilePath(CString path);

/*
	CString MakeProjectName(CString TestName, COleDateTime date, int nunber);
	CString GetProjectName();
	bool SetProjectName(CString projectname);
*/

	bool Str2Date(CString str,COleDateTime &date);
	CString Date2Str(COleDateTime date);

	COleDateTime GetCreeateDay();
	bool SetCreateDay(COleDateTime date);
	COleDateTime GetUpdateDay();
	bool SetUpdateDay(COleDateTime date);
	CString GetImageDataRootPath();
	bool SetImageDataRootPath(CString rootPath);

	CString GetResinScanImageFile();
	bool SetResinScanImageFile(CString fileName);
	CString GetResinClassificationResultFile();
	bool SetResinClassificationResultFile(CString fileName);
	int GetResinDisplayType();
	bool SetResinDisplayType(int displayType);

	CString GetMetalScanImageFile();
	bool SetMetalScanImageFile(CString fileName);
	CString GetMetalClassificationResultFile();
	bool SetMetalClassificationResultFile(CString fileName);
	int GetMetalDisplayType();
	bool SetMetalDisplayType(int displayType);

	CString GetResultScanImageFile();
	bool SetResultScanImageFile(CString fileName);
	int GetResultDisplayType();
	bool SetResultDisplayType(int displayType);

	bool Str2Pos(CString str,int &holizontal, int &bvertical);

	UINT GetSpectralGraphPointTarget();
	bool SetSpectralGraphPointTarget(UINT targetID);
	bool GetSpectralGraphPointPosition(int &holizontal, int &bvertical);
	bool SetSpectralGraphPointPosition(int holizontal, int bvertical);

	UINT GetSpectralGraphSectionTarget();
	bool SetSpectralGraphSectionTarget(UINT targetID);
	bool GetSpectralGraphSectionStartPosition(int &holizontal, int &bvertical);
	bool SetSpectralGraphSectionStartPosition(int holizontal, int bvertical);
	bool GetSpectralGraphSectionEndPosition(int &holizontal, int &bvertical);
	bool SetSpectralGraphSectionEndPosition(int holizontal, int bvertical);

	bool Read(CString path = _T(""));
	bool Save(bool bUpdate=true, CString path = _T(""));
};

