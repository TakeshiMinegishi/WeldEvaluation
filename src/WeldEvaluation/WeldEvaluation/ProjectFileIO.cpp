#include "stdafx.h"
#include "ProjectFileIO.h"
#include "ConfigrationIO.h"
#include "FileUtil.h"


/// <summary>
/// コンストラクタ
/// </summary>
CProjectFileIO::CProjectFileIO(void)
{
	m_ProjectFilePath = _T("");///< プロジェクトファイルへのパス
	Initialze();
}

/// <summary>
/// デストラクタ
/// </summary>
CProjectFileIO::~CProjectFileIO(void)
{
}

/// <summary>
/// プロジェクトファイルパスの取得
/// </summary>
/// <returns>プロジェクトファイルパスを返す</returns>
CString CProjectFileIO::GetProjectFilePath()
{
	return m_ProjectFilePath;
}

/// <summary>
/// プロジェクトファイルパスの設定
/// </summary>
/// <param name="path">プロジェクトファイルパス</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CProjectFileIO::SetProjectFilePath(CString path)
{
	m_ProjectFilePath = path;
	return true;
}

/*
/// <summary>
/// パラメータファイル名の作成
/// </summary>
/// <param name="TestName">名称</param>
/// <param name="date">日付</param>
/// <param name="number">サブ番号</param>
/// <returns>パラメータファイル名を返す</returns>
CString CProjectFileIO::MakeProjectName(CString TestName, COleDateTime date, int number)
{
	CString name;
	name.Format(_T("%s%4d%02d%02d-%d"),TestName,date.GetYear(),date.GetMonth(),date.GetDay(),number);
	return name;
}

/// <summary>
/// プロジェクト名の取得
/// </summary>
/// <returns>プロジェクト名を返す</returns>
CString CProjectFileIO::GetProjectName()
{
	if (m_ProjectFilePath.IsEmpty()) {
		return _T("");
	}
	CConfigrationIO sys(m_ProjectFilePath);
	return sys.getString(_T("Common"),_T("project_name"));
}

/// <summary>
/// プロジェクト名の設定
/// </summary>
/// <param name="projectname">プロジェクト名</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CProjectFileIO::SetProjectName(CString projectname)
{
	if (m_ProjectFilePath.IsEmpty()) {
		return false;
	}
	CConfigrationIO sys(m_ProjectFilePath);
	return sys.setString(_T("Common"),_T("project_name"),projectname);
}
*/

/// <summary>
/// バージョンの取得の取得
/// </summary>
/// <returns>バージョンを返す</returns>
CString CProjectFileIO::GetVersion()
{
	return m_Version;
}

/// <summary>
/// バージョンの設定
/// </summary>
/// <param name="version">バージョン</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CProjectFileIO::SetVersion(CString version)
{
	m_Version = version;
	return true;
}

/// <summary>
/// プロジェクトの初期化
/// </summary>
void CProjectFileIO::Initialze()
{
//	m_ProjectFilePath = _T("");///< プロジェクトファイルへのパス

	m_CreeateDay = COleDateTime(1960,11,18,0,0,0);				///< 作成日
	m_UpdateDay = COleDateTime(1960,11,18,0,0,0);				///< 更新簿
	m_Version = _T("");
	m_ImageDataRootPath								= _T("");	///< イメージデータへのルートパス

	m_ResinScanImageFile							= _T("");	///< 樹脂面スキャン画像ファイル名
	m_ResinDendrogramClassFile						= _T("");	///< 樹脂面階層クラスタリング分類結果ファイル名
	m_ResinKmeansClassFile							= _T("");	///< 樹脂面k-means分類結果ファイル名

	m_MetalScanImageFile							= _T("");	///< 金属面スキャン画像ファイル名
	m_MetalDendrogramClassFile						= _T("");	///< 金属面階層クラスタリング分類結果ファイル名
	m_MetalKmeansClassFile							= _T("");	///< 金属面k-means分類結果ファイル名

	m_ResultScanImageFile							= _T("");	///< 接合結果画像スキャン画像ファイル名
	m_ResultDendrogramClassFile						= _T("");	///< 接合結果階層クラスタリング分類結果ファイル名
	m_ResultKmeansClassFile							= _T("");	///< 接合結果k-means分類結果ファイル名

	m_ResinActiveAnalizeMethod						= 0;		///< 樹脂面の表示対象解析方法
	m_MetalActiveAnalizeMethod						= 0;		///< 金属面の表示対象解析方法
	m_ResultActiveAnalizeMethod						= 0;		///< 結果の表示対象解析方法

	m_nResinDendrogramClassificationClass			= 0;		///< 樹脂面分類結果階層クラスタリング分類数
	m_nResinKMeansClassificationClass				= 0;		///< 樹脂面分類結果k-means分類数
	m_nMetalDendrogramClassificationClass			= 0;		///< 金属面分類結果階層クラスタリング分類数
	m_nMetalKMeansClassificationClass				= 0;		///< 金属面分類結果k-means分類数
	m_nResultDendrogramClassificationClass			= 0;		///< 結果階層クラスタリング分類数
	m_nResultMeansClassificationClass				= 0;		///< 結果階層クラスタリング分類数

	m_SpectralGraphPointTarget						= 0;		///< スペクトルグラフ（点指定）の対象
	m_SpectralGraphPointHolizontalPosition			= 0;		///< スペクトルグラフ（点指定）の水平位置
	m_SpectralGraphPointVerticalPosition			= 0;		///< スペクトルグラフ（点指定）の垂直位置

	m_SpectralGraphSectionTarget					= 0;		///< スペクトルグラフ（区間指定）の対象
	m_SpectralGraphSectionHolizontalStartPosition	= 0;		///< スペクトルグラフ（点指定）の水平開始位置
	m_SpectralGraphSectionVerticalStartPosition		= 0;		///< スペクトルグラフ（点指定）の垂直開始位置
	m_SpectralGraphSectionHolizontalEndPosition		= 0;		///< スペクトルグラフ（点指定）の水平終了位置
	m_SpectralGraphSectionVerticalEndPosition		= 0;		///< スペクトルグラフ（点指定）の垂直終了位置

	m_ScanDataHolizontalSize						= 0;
	m_ScanDataVerticalSize							= 0;

}


/// <summary>
/// 文字列から日付への変換
/// </summary>
/// <param name="str">日付を表す文字列</param>
/// <param name="date">日付</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CProjectFileIO::Str2Date(CString str,COleDateTime &date)
{
	int y,m,d;
	int pos = str.FindOneOf(_T("/"));
	if (pos > 0) {
		CString s = str.Left(pos);
		y = _ttoi(s);
		str = str.Mid(pos+1);
		pos = str.FindOneOf(_T("/"));
		if (pos > 0) {
			s = str.Left(pos);
			m = _ttoi(s);
			str = str.Mid(pos+1);
			d = _ttoi(str);
		} else {
			return false;
		}
	} else {
		return false;
	}

	date.SetDate(y,m,d);
	return true;
}

/// <summary>
/// 日付から文字列への変換
/// </summary>
/// <param name="date">日付</param>
/// <returns>日付を表す文字列を返す</returns>
CString CProjectFileIO::Date2Str(COleDateTime date)
{
	CString str;
	str.Format(_T("%4d/%02d/%02d"),date.GetYear(),date.GetMonth(),date.GetDay());
	return str;
}

/// <summary>
/// プロジェクトの読み込み
/// </summary>
/// <param name="path">プロジェクトファイルパス名</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CProjectFileIO::read(CString path)
{
	CConfigrationIO sys(path);
	CString str;

	m_Version = sys.getString(_T("Common"), _T("Version"));

	// 作成日
	str = sys.getString(_T("Common"),_T("create_date"));
	if (!str.IsEmpty()) {
		if (!Str2Date(str,m_CreeateDay)) {
			CFileUtil::GetFileCreateDay(m_ProjectFilePath,m_CreeateDay);
		}
	} else {
		CFileUtil::GetFileCreateDay(m_ProjectFilePath,m_CreeateDay);
	}
	// 更新簿
	str = sys.getString(_T("Common"),_T("update_date"));
	if (!str.IsEmpty()) {
		if (!Str2Date(str,m_UpdateDay)) {
			CFileUtil::GetFileCreateDay(m_ProjectFilePath,m_UpdateDay);
		}
	} else {
		CFileUtil::GetFileCreateDay(m_ProjectFilePath,m_UpdateDay);
	}

	m_ImageDataRootPath = sys.getString(_T("Common"),_T("image_data_root_path"));							// イメージデータへのルートパス

	m_ResinScanImageFile = sys.getString(_T("ResinSurface"),_T("scan_image_file"));							// 樹脂面スキャン画像ファイル名
	m_ResinDendrogramClassFile = sys.getString(_T("ResinSurface"),_T("dendrogram_classification"));			// 樹脂面分類結果階層クラスタリングファイル名
	m_ResinKmeansClassFile = sys.getString(_T("ResinSurface"), _T("kmeans_classification"));				// 樹脂面分類結果k-meansファイル名

	m_MetalScanImageFile = sys.getString(_T("MetalSurface"),_T("scan_image_file"));							// 金属面スキャン画像ファイル名
	m_MetalDendrogramClassFile = sys.getString(_T("MetalSurface"),_T("dendrogram_classification"));			// 金属面分類結果階層クラスタリングファイル名
	m_MetalKmeansClassFile = sys.getString(_T("MetalSurface"), _T("kmeans_classification"));				// 金属面分類結果k-meansファイル名

	m_ResultScanImageFile = sys.getString(_T("JoiningResult"),_T("scan_image_file"));						// 結果画像スキャン画像ファイル名
	m_ResultDendrogramClassFile = sys.getString(_T("JoiningResult"), _T("dendrogram_classification"));		// 結果階層クラスタリングファイル名
	m_ResultKmeansClassFile = sys.getString(_T("JoiningResult"), _T("kmeans_classification"));				// 結果k-meansファイル名

	m_ResinActiveAnalizeMethod				= sys.getInt(_T("ResinSurface"), _T("analize_method"));			// 樹脂面の表示対象解析方法
	m_MetalActiveAnalizeMethod				= sys.getInt(_T("MetalSurface"), _T("analize_method"));			// 金属面の表示対象解析方法
	m_ResultActiveAnalizeMethod				= sys.getInt(_T("JoiningResult"), _T("analize_method"));			// 結果の表示対象解析方法

	m_nResinDendrogramClassificationClass	= sys.getInt(_T("ResinSurface"), _T("dendrogram_classification_nclass"));		// 樹脂面分類結果階層クラスタリング分類数
	m_nResinKMeansClassificationClass		= sys.getInt(_T("ResinSurface"), _T("kmeans_classification_nclass"));			// 樹脂面分類結果k-means分類数
	m_nMetalDendrogramClassificationClass	= sys.getInt(_T("MetalSurface"), _T("dendrogram_classification_nclass"));		// 金属面分類結果階層クラスタリング分類数
	m_nMetalKMeansClassificationClass		= sys.getInt(_T("MetalSurface"), _T("kmeans_classification_nclass"));			// 金属面分類結果k-means分類数
	m_nResultDendrogramClassificationClass	= sys.getInt(_T("JoiningResult"), _T("dendrogram_classification_nclass"));;		// 結果階層クラスタリング分類数
	m_nResultMeansClassificationClass		= sys.getInt(_T("JoiningResult"), _T("kmeans_classification_nclass"));;			// 結果k-means分類数

	m_SpectralGraphPointTarget = sys.getInt(_T("SpectralGraphPoint"),_T("target"));							// スペクトルグラフ（点指定）の対象
	// スペクトルグラフ（点指定）の位置
	str = sys.getString(_T("SpectralGraphPoint"),_T("position"));
	if (!Str2Pos(str,m_SpectralGraphPointHolizontalPosition,m_SpectralGraphPointVerticalPosition)) {
		m_SpectralGraphPointHolizontalPosition = 0;
		m_SpectralGraphPointVerticalPosition = 0;
	}

	m_SpectralGraphSectionTarget = sys.getInt(_T("SpectralGraphSection"),_T("target"));						// スペクトルグラフ（区間指定）の対象
	// スペクトルグラフ（点指定）の開始位置
	str = sys.getString(_T("SpectralGraphSection"),_T("start_position"));
	if (!Str2Pos(str,m_SpectralGraphSectionHolizontalStartPosition,m_SpectralGraphSectionVerticalStartPosition)) {
		m_SpectralGraphSectionHolizontalStartPosition = 0;
		m_SpectralGraphSectionVerticalStartPosition = 0;
	}
	///< スペクトルグラフ（点指定）の終了位置
	str = sys.getString(_T("SpectralGraphSection"),_T("end_position"));
	if (!Str2Pos(str,m_SpectralGraphSectionHolizontalEndPosition,m_SpectralGraphSectionVerticalEndPosition)) {
		m_SpectralGraphSectionHolizontalEndPosition = 0;
		m_SpectralGraphSectionVerticalEndPosition = 0;
	}

	m_ScanDataHolizontalSize = sys.getInt(_T("Common"), _T("scan_data_holizontal_size"));
	m_ScanDataVerticalSize = sys.getInt(_T("Common"), _T("scan_data_vertical_size"));

	return true;
}

/// <summary>
/// プロジェクトの保存
/// </summary>
/// <param name="path">プロジェクトファイルパス名</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CProjectFileIO::save(CString path)
{
	bool bResult = true;
	CConfigrationIO sys(path);
	CString str;

	if (!sys.setString(_T("Common"), _T("Version"), m_Version)) {
		bResult = false;
	}

	// 作成日
	str = Date2Str(m_CreeateDay);
	if (str.IsEmpty() || (!sys.setString(_T("Common"),_T("create_date"),str))) {
		bResult = false;
	}
	// 更新簿
	str = Date2Str(m_UpdateDay);
	if (str.IsEmpty() || (!sys.setString(_T("Common"),_T("update_date"),str))) {
		bResult = false;
	}

	if (!sys.setInt(_T("Common"), _T("scan_data_holizontal_size"), m_ScanDataHolizontalSize)) {
		bResult = false;
	}

	if (!sys.setInt(_T("Common"), _T("scan_data_vertical_size"), m_ScanDataVerticalSize)) {
		bResult = false;
	}

/*
	// イメージデータへのルートパス
	if (!sys.setString(_T("Common"),_T("image_data_root_path"),m_ImageDataRootPath)) {
		bResult = false;
	}
*/
	// 樹脂面スキャン画像ファイル名
	if (!sys.setString(_T("ResinSurface"),_T("scan_image_file"),m_ResinScanImageFile)) {
		bResult = false;
	}
	// 樹脂面階層クラスタリング分類結果ファイル名
	if (!sys.setString(_T("ResinSurface"),_T("dendrogram_classification"), m_ResinDendrogramClassFile)) {
		bResult = false;
	}
	// 樹脂面k-means分類結果ファイル名
	if (!sys.setString(_T("ResinSurface"), _T("kmeans_classification"), m_ResinKmeansClassFile)) {
		bResult = false;
	}

	// 金属面スキャン画像ファイル名
	if (!sys.setString(_T("MetalSurface"),_T("scan_image_file"),m_MetalScanImageFile)) {
		bResult = false;
	}
	// 金属面階層クラスタリング分類結果ファイル名
	if (!sys.setString(_T("MetalSurface"),_T("dendrogram_classification"), m_MetalDendrogramClassFile)) {
		bResult = false;
	}
	// 金属面k-means分類結果ファイル名
	if (!sys.setString(_T("MetalSurface"), _T("kmeans_classification"), m_MetalKmeansClassFile)) {
		bResult = false;
	}

	// 結果画像スキャン画像ファイル名
	if (!sys.setString(_T("JoiningResult"),_T("scan_image_file"),m_ResultScanImageFile)) {
		bResult = false;
	}
	// 接合結果階層クラスタリング分類結果ファイル名
	if (!sys.setString(_T("JoiningResult"), _T("dendrogram_classification"), m_ResultDendrogramClassFile)) {
		bResult = false;
	}
	// 接合結果k-means分類結果ファイル名
	if (!sys.setString(_T("JoiningResult"), _T("kmeans_classification"), m_ResultKmeansClassFile)) {
		bResult = false;
	}

	// 樹脂面の表示対象解析方法
	if (!sys.setInt(_T("ResinSurface"), _T("analize_method"), m_ResinActiveAnalizeMethod)) {
		bResult = false;
	}
	// 金属面の表示対象解析方法
	if (!sys.setInt(_T("MetalSurface"), _T("analize_method"), m_MetalActiveAnalizeMethod)) {
		bResult = false;
	}
	// 結果の表示対象解析方法
	if (!sys.setInt(_T("JoiningResult"), _T("analize_method"), m_ResultActiveAnalizeMethod)) {
		bResult = false;
	}
	// 樹脂面分類結果階層クラスタリング分類数
	if (!sys.setInt(_T("ResinSurface"), _T("dendrogram_classification_nclass"), m_nResinDendrogramClassificationClass)) {
		bResult = false;
	}
	// 樹脂面分類結果k-means分類数
	if (!sys.setInt(_T("ResinSurface"), _T("kmeans_classification_nclass"), m_nResinKMeansClassificationClass)) {
		bResult = false;
	}
	// 金属面分類結果階層クラスタリング分類数
	if (!sys.setInt(_T("MetalSurface"), _T("dendrogram_classification_nclass"), m_nMetalDendrogramClassificationClass)) {
		bResult = false;
	}
	// 金属面分類結果k-means分類数
	if (!sys.setInt(_T("MetalSurface"), _T("kmeans_classification_nclass"), m_nMetalKMeansClassificationClass)) {
		bResult = false;
	}
	// 結果階層クラスタリング分類数
	if (!sys.setInt(_T("JoiningResult"), _T("dendrogram_classification_nclass"), m_nResultDendrogramClassificationClass)) {
		bResult = false;
	}
	// 結果k-means分類数
	if (!sys.setInt(_T("JoiningResult"), _T("kmeans_classification_nclass"), m_nResultMeansClassificationClass)) {
		bResult = false;
	}

	// スペクトルグラフ（点指定）の対象
	if (!sys.setInt(_T("SpectralGraphPoint"),_T("target"),m_SpectralGraphPointTarget)) {
		bResult = false;
	}
	// スペクトルグラフ（点指定）の位置
	str.Format(_T("%d,%d"),m_SpectralGraphPointHolizontalPosition,m_SpectralGraphPointVerticalPosition);
	if (!sys.setString(_T("SpectralGraphPoint"),_T("position"),str)) {
		bResult = false;
	}

	// スペクトルグラフ（区間指定）の対象
	if (!sys.setInt(_T("SpectralGraphSection"),_T("target"),m_SpectralGraphSectionTarget)) {
		bResult = false;
	}
	// スペクトルグラフ（点指定）の開始位置
	str.Format(_T("%d,%d"),m_SpectralGraphSectionHolizontalStartPosition,m_SpectralGraphSectionVerticalStartPosition);
	if (!sys.setString(_T("SpectralGraphSection"),_T("start_position"),str)) {
		bResult = false;
	}
	///< スペクトルグラフ（点指定）の終了位置
	str.Format(_T("%d,%d"),m_SpectralGraphSectionHolizontalEndPosition,m_SpectralGraphSectionVerticalEndPosition);
	if (!sys.setString(_T("SpectralGraphSection"),_T("end_position"),str)) {
		bResult = false;
	}
	return bResult;
}

/// <summary>
/// 作成日の取得
/// </summary>
/// <returns>作成日を返す</returns>
COleDateTime CProjectFileIO::GetCreeateDay()
{
	return m_CreeateDay;
}

/// <summary>
/// 作成日の設定
/// </summary>
/// <param name="date">日付</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CProjectFileIO::SetCreateDay(COleDateTime date)
{
	m_CreeateDay = date;
	return true;
}

/// <summary>
/// 更新日の取得
/// </summary>
/// <returns>更新日を返す</returns>
COleDateTime CProjectFileIO::GetUpdateDay()
{
	return m_UpdateDay;
}

/// <summary>
/// 更新日の設定
/// </summary>
/// <param name="date">日付</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CProjectFileIO::SetUpdateDay(COleDateTime date)
{
	m_UpdateDay = date;
	return true;
}

/// <summary>
/// イメージデータへのルートパスの取得
/// </summary>
/// <returns>イメージデータへのルートパスを返す</returns>
CString CProjectFileIO::GetImageDataRootPath()
{
	return m_ImageDataRootPath;
}

/// <summary>
/// イメージデータへのルートパスの設定
/// </summary>
/// <param name="rootPath">ルートパス</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CProjectFileIO::SetImageDataRootPath(CString rootPath)
{
	m_ImageDataRootPath = rootPath;
	return true;
}

/// <summary>
/// 樹脂面スキャン画像ファイル名の取得
/// </summary>
/// <returns>樹脂面スキャン画像ファイル名を返す</returns>
CString CProjectFileIO::GetResinScanImageFile()
{
	return m_ResinScanImageFile;
}

/// <summary>
/// 樹脂面スキャン画像ファイル名の設定
/// </summary>
/// <param name="fileName">樹脂面スキャン画像ファイル名</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CProjectFileIO::SetResinScanImageFile(CString fileName)
{
	m_ResinScanImageFile = fileName;
	return true;
}

/// <summary>
/// 樹脂面階層クラスタリング分類結果ファイル名の取得
/// </summary>
/// <returns>樹脂面クラスタリング分類結果ファイル名を返す</returns>
CString CProjectFileIO::GetResinDendrogramClassFile()
{
	return m_ResinDendrogramClassFile;
}

/// <summary>
/// 樹脂面階層クラスタリング分類結果ファイル名の設定
/// </summary>
/// <param name="fileName">樹脂面階層クラスタリング分類結果ファイル名</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CProjectFileIO::SetResinDendrogramClassFile(CString fileName)
{
	m_ResinDendrogramClassFile = fileName;
	return true;
}

/// <summary>
/// 樹脂面k-means分類結果ファイル名の取得
/// </summary>
/// <returns>樹脂面k-means分類結果ファイル名を返す</returns>
CString CProjectFileIO::GetResinKmeansClassFile()
{
	return m_ResinKmeansClassFile;
}

/// <summary>
/// 樹脂面k-means分類結果ファイル名の設定
/// </summary>
/// <param name="fileName">樹脂面k-means分類結果ファイル名</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CProjectFileIO::SetResinKmeansClassFile(CString fileName)
{
	m_ResinKmeansClassFile = fileName;
	return true;
}

/// <summary>
/// 樹脂画像表示タイプの取得
/// </summary>
/// <returns>樹脂画像表示タイプを返す</returns>
int CProjectFileIO::GetResinDisplayType()
{
	if (!CFileUtil::fileExists(m_ProjectFilePath)) {
		return 0;
	}
	CConfigrationIO sys(m_ProjectFilePath);
	return sys.getInt(_T("ResinSurface"),_T("display_type"));
}

/// <summary>
/// 樹脂画像表示タイプの設定
/// </summary>
/// <param name="displayType">樹脂画像表示タイプ</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CProjectFileIO::SetResinDisplayType(int displayType)
{
	if (!CFileUtil::fileExists(m_ProjectFilePath)) {
		return false;
	}
	CConfigrationIO sys(m_ProjectFilePath);
	return sys.setInt(_T("ResinSurface"),_T("display_type"),displayType);
}

/// <summary>
/// 樹脂面解析方法の取得
/// </summary>
/// <returns>樹脂面解析方法を返す</returns>
int CProjectFileIO::GetResinAnalizeMethod()
{
	return m_ResinActiveAnalizeMethod;
}

/// <summary>
/// 樹脂面解析方法の設定
/// </summary>
/// <param name="method">解析方法</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CProjectFileIO::SetResinAnalizeMethod(int method)
{
	m_ResinActiveAnalizeMethod = method;
	return true;
}

/// <summary>
/// 作成された樹脂面階層クラスタリング解析のクラス数の取得
/// </summary>
/// <returns>クラス数を返す</returns>
int CProjectFileIO::GetNumbetOfResinDendrogramClassificationClass()
{
	return m_nResinDendrogramClassificationClass;
}

/// <summary>
/// 作成された樹脂面階層クラスタリング解析のクラス数の取得
/// </summary>
/// <param name="nClass">クラス数</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CProjectFileIO::SetNumbetOfResinDendrogramClassificationClass(int nClass)
{
	m_nResinDendrogramClassificationClass = nClass;
	return true;
}

/// <summary>
/// 作成された樹脂面K-Means解析のクラス数の取得
/// </summary>
/// <returns>クラス数を返す</returns>
int CProjectFileIO::GetNumbetOfResinKMeansClassificationClass()
{
	return m_nResinKMeansClassificationClass;
}

/// <summary>
/// 作成された樹脂面K-Means解析のクラス数の取得
/// </summary>
/// <param name="nClass">クラス数</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CProjectFileIO::SetNumbetOfResinKMeansClassificationClass(int nClass)
{
	m_nResinKMeansClassificationClass = nClass;
	return true;
}

/// <summary>
/// 金属面スキャン画像ファイル名の取得
/// </summary>
/// <returns>金属面スキャン画像ファイル名を返す</returns>
CString CProjectFileIO::GetMetalScanImageFile()
{
	return m_MetalScanImageFile;
}

/// <summary>
/// 金属面スキャン画像ファイル名の設定
/// </summary>
/// <param name="fileName">金属面スキャン画像ファイル名</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CProjectFileIO::SetMetalScanImageFile(CString fileName)
{
	m_MetalScanImageFile = fileName;
	return true;
}

/// <summary>
/// 金属面階層クラスタリング分類結果ファイル名の取得
/// </summary>
/// <returns>金属面階層クラスタリング分類結果ファイル名を返す</returns>
CString CProjectFileIO::GetMetalDendrogramClassFile()
{
	return m_MetalDendrogramClassFile;
}

/// <summary>
/// 金属面階層クラスタリング分類結果ファイル名の設定
/// </summary>
/// <param name="fileName">金属面階層クラスタリング分類結果ファイル名</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CProjectFileIO::SetMetalDendrogramClassFile(CString fileName)
{
	m_MetalDendrogramClassFile = fileName;
	return true;
}

/// <summary>
/// 金属面k-means分類結果ファイル名の取得
/// </summary>
/// <returns>金属面分類結果k-meansファイル名を返す</returns>
CString CProjectFileIO::GetMetalKmeansClassFile()
{
	return m_MetalKmeansClassFile;
}

/// <summary>
/// 金属面k-means分類結果ファイル名の設定
/// </summary>
/// <param name="fileName">金属面分類結果k-meansファイル名</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CProjectFileIO::SetMetalKmeansClassFile(CString fileName)
{
	m_MetalKmeansClassFile = fileName;
	return true;
}

/// <summary>
/// 金属画像表示タイプの取得
/// </summary>
/// <returns>金属画像表示タイプを返す</returns>
int CProjectFileIO::GetMetalDisplayType()
{
	if (!CFileUtil::fileExists(m_ProjectFilePath)) {
		return 0;
	}
	CConfigrationIO sys(m_ProjectFilePath);
	return sys.getInt(_T("MetalSurface"),_T("display_type"));
}

/// <summary>
/// 金属画像表示タイプの設定
/// </summary>
/// <param name="displayType">金属画像表示タイプ</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CProjectFileIO::SetMetalDisplayType(int displayType)
{
	if (!CFileUtil::fileExists(m_ProjectFilePath)) {
		return false;
	}
	CConfigrationIO sys(m_ProjectFilePath);
	return sys.setInt(_T("MetalSurface"),_T("display_type"),displayType);
}

/// <summary>
/// 金属面解析方法の取得
/// </summary>
/// <returns>樹脂面解析方法を返す</returns>
int CProjectFileIO::GetMetalAnalizeMethod()
{
	return m_MetalActiveAnalizeMethod;
}

/// <summary>
/// 金属面解析方法の設定
/// </summary>
/// <param name="method">解析方法</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CProjectFileIO::SetMetalAnalizeMethod(int method)
{
	m_MetalActiveAnalizeMethod = method;
	return true;
}

/// <summary>
/// 作成された金属面階層クラスタリング解析のクラス数の取得
/// </summary>
/// <returns>クラス数を返す</returns>
int CProjectFileIO::GetNumbetOfMetalDendrogramClassificationClass()
{
	return m_nMetalDendrogramClassificationClass;
}

/// <summary>
/// 作成された金属面階層クラスタリング解析のクラス数の取得
/// </summary>
/// <param name="nClass">クラス数</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CProjectFileIO::SetNumbetOfMetalDendrogramClassificationClass(int nClass)
{
	m_nMetalDendrogramClassificationClass = nClass;
	return true;
}

/// <summary>
/// 作成された金属面K-Means解析のクラス数の取得
/// </summary>
/// <returns>クラス数を返す</returns>
int CProjectFileIO::GetNumbetOfMetalKMeansClassificationClass()
{
	if (!CFileUtil::fileExists(m_ProjectFilePath)) {
		return false;
	}
	CConfigrationIO sys(m_ProjectFilePath);
	return sys.getInt(_T("MetalSurface"), _T("kmeans_classification_nclass"));
}

/// <summary>
/// 作成された金属面K-Means解析のクラス数の取得
/// </summary>
/// <param name="nClass">クラス数</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CProjectFileIO::SetNumbetOfMetalKMeansClassificationClass(int nClass)
{
	if (!CFileUtil::fileExists(m_ProjectFilePath)) {
		return false;
	}
	CConfigrationIO sys(m_ProjectFilePath);
	return sys.setInt(_T("MetalSurface"), _T("kmeans_classification_nclass"), nClass);
}

/// <summary>
/// 接合結果画像スキャン画像ファイル名の取得
/// </summary>
/// <returns>接合結果画像スキャン画像ファイル名を返す</returns>
CString CProjectFileIO::GetResultScanImageFile()
{
	return m_ResultScanImageFile;
}

/// <summary>
/// 接合結果画像スキャン画像ファイル名の設定
/// </summary>
/// <param name="fileName">接合結果画像スキャン画像ファイル名</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CProjectFileIO::SetResultScanImageFile(CString fileName)
{
	m_ResultScanImageFile = fileName;
	return true;
}

/// <summary>
/// 接合結果階層クラスタリング分類ファイル名の取得
/// </summary>
/// <returns>接合結果階層クラスタリング分類ファイル名を返す</returns>
CString CProjectFileIO::GetResultDendrogramClassFile()
{
	return m_ResultDendrogramClassFile;
}

/// <summary>
/// 接合結果階層クラスタリングフ分類ァイル名の設定
/// </summary>
/// <param name="fileName">接合結果階層クラスタリング分類ファイル名</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CProjectFileIO::SetResultDendrogramClassFile(CString fileName)
{
	m_ResultDendrogramClassFile = fileName;
	return true;
}

/// <summary>
/// 接合結果k-meansファイル名の取得
/// </summary>
/// <returns>接合結果k-meansファイル名を返す</returns>
CString CProjectFileIO::GetResultKmeansClassFile()
{
	return m_ResultKmeansClassFile;
}

/// <summary>
/// 接合結果k-meansファイル名の設定
/// </summary>
/// <param name="fileName">接合結果k-meansファイル名</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CProjectFileIO::SetResultKmeansClassFile(CString fileName)
{
	m_ResultKmeansClassFile = fileName;
	return true;
}
/// <summary>
/// 接合結果画像表示タイプの取得
/// </summary>
/// <returns>接合結果画像表示タイプを返す</returns>
int CProjectFileIO::GetResultDisplayType()
{
	if (!CFileUtil::fileExists(m_ProjectFilePath)) {
		return 0;
	}
	CConfigrationIO sys(m_ProjectFilePath);
	return sys.getInt(_T("JoiningResult"),_T("display_type"));
}

/// <summary>
/// 接合結果画像表示タイプの設定
/// </summary>
/// <param name="displayType">接合結果画像表示タイプ</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CProjectFileIO::SetResultDisplayType(int displayType)
{
	if (!CFileUtil::fileExists(m_ProjectFilePath)) {
		return false;
	}
	CConfigrationIO sys(m_ProjectFilePath);
	return sys.setInt(_T("JoiningResult"),_T("display_type"),displayType);
}


/// <summary>
/// 接合結果解析方法の取得
/// </summary>
/// <returns>接合結果解析方法を返す</returns>
int CProjectFileIO::GetResultAnalizeMethod()
{
	return m_ResultActiveAnalizeMethod;
}

/// <summary>
/// 接合結果解析方法の設定
/// </summary>
/// <param name="method">解析方法</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CProjectFileIO::SetResultAnalizeMethod(int method)
{
	m_ResultActiveAnalizeMethod = method;
	return true;
}

/// <summary>
/// 作成された接合結果階層クラスタリング解析のクラス数の取得
/// </summary>
/// <returns>クラス数を返す</returns>
int CProjectFileIO::GetNumbetOfResultDendrogramClassificationClass()
{
	return m_nResultDendrogramClassificationClass;
}

/// <summary>
/// 作成された接合結果階層クラスタリング解析のクラス数の取得
/// </summary>
/// <param name="nClass">クラス数</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CProjectFileIO::SetNumbetOfResultDendrogramClassificationClass(int nClass)
{
	m_nResultDendrogramClassificationClass = nClass;
	return true;
}

/// <summary>
/// 作成された接合結果K-Means解析のクラス数の取得
/// </summary>
/// <returns>クラス数を返す</returns>
int CProjectFileIO::GetNumbetOfResultKMeansClassificationClass()
{
	return m_nResultMeansClassificationClass;
}

/// <summary>
/// 作成された接合結果K-Means解析のクラス数の取得
/// </summary>
/// <param name="nClass">クラス数</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CProjectFileIO::SetNumbetOfResultKMeansClassificationClass(int nClass)
{
	m_nResultMeansClassificationClass = nClass;
	return true;
}

/// <summary>
/// スペクトルグラフ（点指定）の対象の取得
/// </summary>
/// <returns>スペクトルグラフ（点指定）の対象を返す</returns>
UINT CProjectFileIO::GetSpectralGraphPointTarget()
{
	return m_SpectralGraphPointTarget;
}

/// <summary>
/// スペクトルグラフ（点指定）の対象の設定
/// </summary>
/// <param name="targetID">スペクトルグラフ（点指定）の対象</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CProjectFileIO::SetSpectralGraphPointTarget(UINT targetID)
{
	m_SpectralGraphPointTarget = targetID;
	return true;
}

/// <summary>
/// 文字列からポイントへの変換
/// </summary>
/// <param name="str">文字列</param>
/// <param name="holizontal">水平値</param>
/// <param name="vertical">垂直値</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CProjectFileIO::Str2Pos(CString str,int &holizontal, int &vertical)
{
	holizontal = 0;
	vertical = 0;
	int pos = str.FindOneOf(_T(","));
	if (pos > 0) {
		CString s = str.Left(pos);
		holizontal = _ttoi(s);
		str = str.Mid(pos+1);
		vertical =  _ttoi(str);
		return true;
	} else {
		return false;
	}
}

/// <summary>
/// スペクトルグラフ（点指定）の位置の取得
/// </summary>
/// <param name="holizontal">水平値</param>
/// <param name="vertical">垂直値</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CProjectFileIO::GetSpectralGraphPointPosition(int &holizontal, int &vertical)
{
	holizontal	= m_SpectralGraphPointHolizontalPosition;
	vertical	= m_SpectralGraphPointVerticalPosition;
	return true;
}

/// <summary>
/// スペクトルグラフ（点指定）の位置の設定
/// </summary>
/// <param name="holizontal">水平値</param>
/// <param name="vertical">垂直値</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CProjectFileIO::SetSpectralGraphPointPosition(int holizontal, int vertical)
{
	m_SpectralGraphPointHolizontalPosition = holizontal;
	m_SpectralGraphPointVerticalPosition = vertical;
	return true;
}

/// <summary>
/// スペクトルグラフ（区間指定）の対象の取得
/// </summary>
/// <returns>スペクトルグラフ（区間指定）の対象を返す</returns>
UINT CProjectFileIO::GetSpectralGraphSectionTarget()
{
	return m_SpectralGraphSectionTarget;
}

/// <summary>
/// スペクトルグラフ（区間指定）の対象の設定
/// </summary>
/// <param name="targetID">スペクトルグラフ（区間指定）の対象</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CProjectFileIO::SetSpectralGraphSectionTarget(UINT targetID)
{
	m_SpectralGraphSectionTarget = targetID;
	return true;
}

/// <summary>
/// スペクトルグラフ（区間指定）の開始位置の取得
/// </summary>
/// <param name="holizontal">水平値</param>
/// <param name="vertical">垂直値</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CProjectFileIO::GetSpectralGraphSectionStartPosition(int &holizontal, int &vertical)
{
	holizontal	= m_SpectralGraphSectionHolizontalStartPosition;
	vertical	= m_SpectralGraphSectionVerticalStartPosition;
	return true;
}

/// <summary>
/// スペクトルグラフ（区間指定）の開始位置の設定
/// </summary>
/// <param name="holizontal">水平値</param>
/// <param name="vertical">垂直値</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CProjectFileIO::SetSpectralGraphSectionStartPosition(int holizontal, int vertical)
{
	m_SpectralGraphSectionHolizontalStartPosition = holizontal;
	m_SpectralGraphSectionVerticalStartPosition = vertical;
	return true;
}

/// <summary>
/// スペクトルグラフ（区間指定）の終了位置の取得
/// </summary>
/// <param name="holizontal">水平値</param>
/// <param name="vertical">垂直値</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CProjectFileIO::GetSpectralGraphSectionEndPosition(int &holizontal, int &vertical)
{
	holizontal	= m_SpectralGraphSectionHolizontalEndPosition;
	vertical	= m_SpectralGraphSectionVerticalEndPosition;
	return true;
}

/// <summary>
/// スペクトルグラフ（区間指定）の終了位置の設定
/// </summary>
/// <param name="holizontal">水平値</param>
/// <param name="vertical">垂直値</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CProjectFileIO::SetSpectralGraphSectionEndPosition(int holizontal, int vertical)
{
	m_SpectralGraphSectionHolizontalEndPosition = holizontal;
	m_SpectralGraphSectionVerticalEndPosition = vertical;
	return true;
}

/// <summary>
/// スキャンデータのサイズ取得
/// </summary>
/// <param name="holizontal">水平値</param>
/// <param name="vertical">垂直値</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CProjectFileIO::GetScanDataSize(int &holizontal, int &vertical)
{
	holizontal	= m_ScanDataHolizontalSize;
	vertical	= m_ScanDataVerticalSize;
	return true;
}

/// <summary>
/// スキャンデータのサイズ設定
/// </summary>
/// <param name="holizontal">水平値</param>
/// <param name="vertical">垂直値</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CProjectFileIO::SetScanDataSize(int holizontal, int vertical)
{
	m_ScanDataHolizontalSize = holizontal;
	m_ScanDataVerticalSize = vertical;
	return true;
}

/// <summary>
/// パラメータファイルの読み込み
/// </summary>
/// <param name="path">パラメータファイルへのパス</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CProjectFileIO::Read(CString path/* = _T("")*/)
{
	if (path.IsEmpty()) {
		path = m_ProjectFilePath;
	}
	if (!CFileUtil::fileExists(path)) {
		return false;
	}
	return read(path);
}

/// <summary>
/// パラメータファイルの書き込み
/// </summary>
/// <param name="bUpdate">更新フラグ</param>
/// <param name="path">パラメータファイルへのパス</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CProjectFileIO::Save(bool bUpdate/*=true*/, CString path/* = _T("")*/)
{
	if (path.IsEmpty()) {
		path = m_ProjectFilePath;
	}
	if (bUpdate && (!CFileUtil::fileExists(path))) {
		return false;
	}
	return save(path);
}
