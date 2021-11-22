#include "stdafx.h"
#include "PropatyIO.h"
#include "ConfigrationIO.h"
#include "FileUtil.h"

/// <summary>
/// コンストラクタ
/// </summary>
CPropatyIO::CPropatyIO(void)
{
	m_ParamaterFilePath	= _T("");
	m_ProjectName	=_T("");		///< プロジェクト名
	m_TestName	=_T("");			///< 名称
	m_ResinAnalysisMethod	= 0;	///< 樹脂面 解析方法
	m_MetalAnalysisMethod	= 0;	///< 金属面 解析方法
	m_ResultAnalysisMethod	= 0;	///< 接合結果 解析方法
}

/// <summary>
/// デストラクタ
/// </summary>
CPropatyIO::~CPropatyIO(void)
{
}

/// <summary>
/// パラメータファイルパスの取得
/// </summary>
/// <returns>パラメータファイルパスを返す</returns>
CString CPropatyIO::GetParamaterFilePath()
{
	return m_ParamaterFilePath;
}

/// <summary>
/// パラメータファイルパスの設定
/// </summary>
/// <param name="path">パラメータファイルパス</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CPropatyIO::SetParamaterFilePath(CString path)
{
//	if (!CFileUtil::fileExists(path)) {
//		m_ParamaterFilePath	= _T("");
//		return false;
//	}
	m_ParamaterFilePath = path;
	return true;
}


/// <summary>
/// パラメータファイル名の作成
/// </summary>
/// <param name="TestName">名称</param>
/// <param name="date">日付</param>
/// <param name="number">サブ番号</param>
/// <returns>パラメータファイル名を返す</returns>
CString CPropatyIO::MakeProjectName(CString TestName, COleDateTime date, int number)
{
	CString name;
	name.Format(_T("%s%4d%02d%02d-%d"), (LPCWSTR)TestName,date.GetYear(),date.GetMonth(),date.GetDay(),number);
	return name;
}

/// <summary>
/// パラメータファイルの読み込み
/// </summary>
/// <param name="path">パラメータファイルへのパス</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CPropatyIO::read(CString path)
{
	CConfigrationIO sys(path);
	m_ProjectName			= sys.getString(_T("Common"),_T("Projent_name"));;					// プロジェクト名
	m_TestName				= sys.getString(_T("Common"),_T("Test_name"));						// 名称
	m_ResinAnalysisMethod	= sys.getInt(_T("ResinSurface"),_T("Analysis_method"));				// 樹脂面 解析方法
	m_MetalAnalysisMethod	= sys.getInt(_T("MetalSurface"),_T("Analysis_method"));				// 金属面 解析方法
	m_ResultAnalysisMethod	= sys.getInt(_T("JoiningResult"), _T("Analysis_method"));			// 接合結果 解析方法

	return true;
}

/// <summary>
/// パラメータファイルの書き込み
/// </summary>
/// <param name="name">パラメータファイルへのパス</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CPropatyIO::save(CString path)
{
	bool bResult = true;
	CConfigrationIO sys(path);

	// プロジェクト名
	if (!sys.setString(_T("Common"),_T("Projent_name"),m_ProjectName)) {
		bResult = false;
	}
	// 名称
	if (!sys.setString(_T("Common"),_T("Test_name"),m_TestName)) {
		bResult = false;
	}

	// Settingのダミー出力
	double dval;
	UINT uval;
	uval = sys.getInt(_T("Setting"),_T("Number_of_overlapping_pixels"));
	if (!sys.setInt(_T("Setting"),_T("Number_of_overlapping_pixels"),uval)) {
		bResult = false;
	}
	uval = sys.getInt(_T("Setting"),_T("Integration_time_ms"));
	if (!sys.setInt(_T("Setting"),_T("Integration_time_ms"),uval)) {
		bResult = false;
	}
	uval = sys.getInt(_T("Setting"),_T("Vertical_resolution"));
	if (!sys.setInt(_T("Setting"),_T("Vertical_resolution"),uval)) {
		bResult = false;
	}
	uval = sys.getInt(_T("Setting"),_T("Horizontal_resolution"));
	if (!sys.setInt(_T("Setting"),_T("Horizontal_resolution"),uval)) {
		bResult = false;
	}

	// Settingのダミー出力
#if 0
	uval = sys.getInt(_T("ResinSurface"),_T("Number_of_overlapping_pixels"));
	if (!sys.setInt(_T("ResinSurface"),_T("Number_of_overlapping_pixels"),uval)) {
		bResult = false;
	}
	dval = sys.getDouble(_T("ResinSurface"),_T("Joining_ratio"));
	if (!sys.setDouble(_T("ResinSurface"),_T("Joining_ratio"),uval)) {
		bResult = false;
	}
	uval = sys.getInt(_T("MetalSurface "),_T("Number_of_overlapping_pixels"));
	if (!sys.setInt(_T("MetalSurface "),_T("Number_of_overlapping_pixels"),uval)) {
		bResult = false;
	}
	dval = sys.getDouble(_T("MetalSurface "),_T("Joining_ratio"));
	if (!sys.setDouble(_T("MetalSurface "),_T("Joining_ratio"),uval)) {
		bResult = false;
	}
	uval = sys.getInt(_T("JoiningResult "),_T("Number_of_overlapping_pixels"));
	if (!sys.setInt(_T("JoiningResult "),_T("Number_of_overlapping_pixels"),uval)) {
		bResult = false;
	}
	dval = sys.getDouble(_T("JoiningResult "),_T("Joining_ratio"));
	if (!sys.setDouble(_T("JoiningResult "),_T("Joining_ratio"),uval)) {
		bResult = false;
	}
#endif

	// 樹脂面 解析方法
	if (!sys.setInt(_T("ResinSurface"),_T("Analysis_method"),m_ResinAnalysisMethod)) {
		bResult = false;
	}
	// 金属面 解析方法
	if (!sys.setInt(_T("MetalSurface"),_T("Analysis_method"),m_MetalAnalysisMethod)) {
		bResult = false;
	}
	// 結果面 解析方法
	if (!sys.setInt(_T("JoiningResult"), _T("Analysis_method"), m_ResultAnalysisMethod)) {
		bResult = false;
	}

	return bResult;
}

/// <summary>
/// プロジェクト名の取得
/// </summary>
/// <returns>プロジェクト名を返す</returns>
CString CPropatyIO::GetProjectName()
{
	return m_ProjectName;
}

/// <summary>
/// プロジェクト名の設定
/// </summary>
/// <param name="projectname">プロジェクト名</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CPropatyIO::SetProjectName(CString projectname)
{
	m_ProjectName = projectname;
	return true;
}

/// <summary>
/// 名称の取得
/// </summary>
/// <returns>名称を返す</returns>
CString CPropatyIO::GetTestName()
{
	return m_TestName;
}

/// <summary>
/// 名称の設定
/// </summary>
/// <param name="name">名称</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CPropatyIO::SetTestName(CString name)
{
	m_TestName = name;
	return true;
}

bool CPropatyIO::WriteProjectName(CString PropatyFilePath, CString ProjectName)
{
	if (!CFileUtil::fileExists(PropatyFilePath)) {
		return false;
	}

	bool bResult = true;
	CConfigrationIO sys(PropatyFilePath);
	// プロジェクト名
	if (!sys.setString(_T("Common"), _T("Projent_name"), ProjectName)) {
		bResult = false;
	}
	return bResult;
}

bool CPropatyIO::WriteTestName(CString PropatyFilePath, CString TestName)
{
	if (!CFileUtil::fileExists(PropatyFilePath)) {
		return false;
	}

	bool bResult = true;
	CConfigrationIO sys(PropatyFilePath);
	// 名称
	if (!sys.setString(_T("Common"), _T("Test_name"), TestName)) {
		bResult = false;
	}
	return bResult;
}


#if 0
/// <summary>
/// Integration_time_msの取得
/// </summary>
/// <returns>Integration_time_msを返す</returns>
double CPropatyIO::GetIntegrationTimeMs(void)
{
	if (!CFileUtil::fileExists(m_ParamaterFilePath)) {
		return 0;
	}
	CConfigrationIO sys(m_ParamaterFilePath);
	return sys.getDouble(_T("Setting"),_T("Integration_time_ms"));
}

/// <summary>
/// Integration_time_msの設定
/// </summary>
/// <param name="time">Integration_time_ms</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CPropatyIO::SetIntegrationTimeMs(double time)
{
	if (!CFileUtil::fileExists(m_ParamaterFilePath)) {
		return false;
	}
	CConfigrationIO sys(m_ParamaterFilePath);
	if (!sys.setDouble(_T("Setting"),_T("Integration_time_ms"),time)) {
		return false;
	}
	return true;
}

/// <summary>
/// バンド数の取得
/// </summary>
/// <returns>バンド数を返す</returns>
UINT CPropatyIO::GetNumberOfBand(void)
{
	if (!CFileUtil::fileExists(m_ParamaterFilePath)) {
		return 0;
	}
	CConfigrationIO sys(m_ParamaterFilePath);
	return sys.getInt(_T("Setting"), _T("Number_of_band"));
}

/// <summary>
/// バンド数の設定
/// </summary>
/// <param name="band">バンド数</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CPropatyIO::SetNumberOfBand(UINT band)
{
	if (!CFileUtil::fileExists(m_ParamaterFilePath)) {
		return false;
	}
	CConfigrationIO sys(m_ParamaterFilePath);
	if (!sys.setInt(_T("Setting"), _T("Number_of_band"), band)) {
		return false;
	}
	return true;
}
#endif

/// <summary>
/// 重なりピクセル数の取得
/// </summary>
/// <returns>重なりピクセル数を返す</returns>
UINT CPropatyIO::NumberOfOverridePixel(void)
{
	if (!CFileUtil::fileExists(m_ParamaterFilePath)) {
		return 0;
	}
	CConfigrationIO sys(m_ParamaterFilePath);
	return sys.getInt(_T("Setting"), _T("Number_of_overlapping_pixels"));
}

/// <summary>
/// 重なりピクセル数の設定
/// </summary>
/// <param name="num">重なりピクセル数</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CPropatyIO::SetOverridePixelNumber(UINT num)
{
	if (!CFileUtil::fileExists(m_ParamaterFilePath)) {
		return false;
	}
	CConfigrationIO sys(m_ParamaterFilePath);
	if (!sys.setInt(_T("Setting"), _T("Number_of_overlapping_pixels"), num)) {
		return false;
	}
	return true;
}

/// <summary>
/// 縦方向の解像度の取得
/// </summary>
/// <returns>縦方向の解像度を返す</returns>
UINT CPropatyIO::GetVerticalResolution(void)
{
	if (!CFileUtil::fileExists(m_ParamaterFilePath)) {
		return 0;
	}
	CConfigrationIO sys(m_ParamaterFilePath);
	return sys.getInt(_T("Setting"), _T("Vertical_resolution"));
}

/// <summary>
/// 縦方向の解像度の設定
/// </summary>
/// <param name="resolution">縦方向の解像度</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CPropatyIO::SetVerticalResolution(UINT resolution)
{
	if (!CFileUtil::fileExists(m_ParamaterFilePath)) {
		return false;
	}
	CConfigrationIO sys(m_ParamaterFilePath);
	if (!sys.setInt(_T("Setting"), _T("Vertical_resolution"), resolution)) {
		return false;
	}
	return true;
}

/// <summary>
/// 横方向の解像度の取得
/// </summary>
/// <returns>横方向の解像度を返す</returns>
UINT CPropatyIO::GetHorizontalResolution(void)
{
	if (!CFileUtil::fileExists(m_ParamaterFilePath)) {
		return 0;
	}
	CConfigrationIO sys(m_ParamaterFilePath);
	return sys.getInt(_T("Setting"), _T("Horizontal_resolution"));
}

/// <summary>
/// 横方向の解像度の設定
/// </summary>
/// <param name="resolution">横方向の解像度</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CPropatyIO::SetHorizontalResolution(UINT resolution)
{
	if (!CFileUtil::fileExists(m_ParamaterFilePath)) {
		return false;
	}
	CConfigrationIO sys(m_ParamaterFilePath);
	if (!sys.setInt(_T("Setting"), _T("Horizontal_resolution"), resolution)) {
		return false;
	}
	return true;
}


/// <summary>
/// 樹脂面の分類数の取得
/// </summary>
/// <returns>樹脂面の分類数を返す</returns>
UINT CPropatyIO::ResinGetNumberOfClass(void)
{
	if (!CFileUtil::fileExists(m_ParamaterFilePath)) {
		return 0;
	}
	CConfigrationIO sys(m_ParamaterFilePath);
	// 樹脂面 分類数
	return sys.getInt(_T("ResinSurface"),_T("Number_of_classifications"));
}

/// <summary>
/// 樹脂面の分類数の設定
/// </summary>
/// <param name="nClass">樹脂面の分類数</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CPropatyIO::ResinSetNumberOfClass(UINT nClass)
{
	if (!CFileUtil::fileExists(m_ParamaterFilePath)) {
		return false;
	}
	CConfigrationIO sys(m_ParamaterFilePath);
	// 樹脂面 分類数
	if (!sys.setInt(_T("ResinSurface"),_T("Number_of_classifications"),nClass)) {
		return false;
	}
	return true;
}

/// <summary>
/// 樹脂面の接合割合の取得
/// </summary>
/// <param name="ViewJointRatioNo">接合面番号</param>
/// <returns>樹脂面の接合割合を返す</returns>
double CPropatyIO::ResinGetJointRetio(int ViewJointRatioNo)
{
	if (!CFileUtil::fileExists(m_ParamaterFilePath)) {
		return 0.0;
	}
	if (ViewJointRatioNo < 0) {
		return 0.0;
	}

	CString label;
	label.Format(_T("Joining_ratio%03d"),ViewJointRatioNo);
	CConfigrationIO sys(m_ParamaterFilePath);
	// 樹脂面 接合割合
	return sys.getDouble(_T("ResinSurface"),label);
}

/// <summary>
/// 樹脂面の接合割合の設定
/// </summary>
/// <param name="ViewJointRatioNo">接合面番号</param>
/// <param name="retio">樹脂面の接合割合</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CPropatyIO::ResinSetJointRetio(int ViewJointRatioNo, double retio)
{
	if (ViewJointRatioNo < 0) {
		return false;
	}
	CString label;
	label.Format(_T("Joining_ratio%03d"),ViewJointRatioNo);
	CConfigrationIO sys(m_ParamaterFilePath);
	// 樹脂面 接合割合
	if (!sys.setDouble(_T("ResinSurface"),label,retio)) {
		return false;
	}
	return true;
}

/// <summary>
/// 樹脂面の接合色の取得
/// </summary>
/// <param name="ViewJointRatioNo">接合面番号</param>
/// <returns>樹脂面の接合色を返す</returns>
COLORREF CPropatyIO::ResinGetJointColor(int ViewJointRatioNo)
{
	if (!CFileUtil::fileExists(m_ParamaterFilePath)) {
		return RGB(0, 0, 0);
	}
	if (ViewJointRatioNo < 0) {
		return RGB(0, 0, 0);
	}

	CString label;
	label.Format(_T("Joining_Color%03d"), ViewJointRatioNo);
	CConfigrationIO sys(m_ParamaterFilePath);
	// 樹脂面 接合色
	return (COLORREF)(sys.getInt(_T("ResinSurface"), label));
}

/// <summary>
/// 樹脂面の接合色の設定
/// </summary>
/// <param name="ViewJointRatioNo">接合面番号</param>
/// <param name="color">樹脂面の接合色</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CPropatyIO::ResinSetJointColor(int ViewJointRatioNo, COLORREF color)
{
	if (ViewJointRatioNo < 0) {
		return false;
	}
	CString label;
	label.Format(_T("Joining_color%03d"), ViewJointRatioNo);
	CConfigrationIO sys(m_ParamaterFilePath);
	// 樹脂面 接合色
	if (!sys.setInt(_T("ResinSurface"), label, (DWORD)color)) {
		return false;
	}
	return true;
}

/// <summary>
/// 樹脂面の解析方法の取得
/// </summary>
/// <returns>樹脂面の解析方法を返す</returns>
int CPropatyIO::ResinGetAnalysisMethod()
{
	return m_ResinAnalysisMethod;
}

/// <summary>
/// 樹脂面の解析方法の設定
/// </summary>
/// <param name="method">樹脂面の解析方法</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CPropatyIO::ResinSetAnalysisMethod(int method)
{
	m_ResinAnalysisMethod = method;
	return true;
}

/// <summary>
/// 金属面の分類数の取得
/// </summary>
/// <returns>金属面の分類数を返す</returns>
UINT CPropatyIO::MetalGetNumberOfClass(void)
{
	if (!CFileUtil::fileExists(m_ParamaterFilePath)) {
		return false;
	}
	CConfigrationIO sys(m_ParamaterFilePath);
	return sys.getInt(_T("MetalSurface"),_T("Number_of_classifications"));
}

/// <summary>
/// 金属面の分類数の設定
/// </summary>
/// <param name="nClass">金属面の分類数</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CPropatyIO::MetalSetNumberOfClass(UINT nClass)
{
//	if (!CFileUtil::fileExists(m_ParamaterFilePath)) {
//		return false;
//	}
	CConfigrationIO sys(m_ParamaterFilePath);
	if (!sys.setInt(_T("MetalSurface"),_T("Number_of_classifications"),nClass)) {
		return false;
	}
	return true;
}

/// <summary>
/// 金属面の接合割合の取得
/// </summary>
/// <param name="ViewJointRatioNo">接合面番号</param>
/// <returns>金属面の接合割合を返す</returns>
double CPropatyIO::MetalGetJointRetio(int ViewJointRatioNo)
{
	if (!CFileUtil::fileExists(m_ParamaterFilePath)) {
		return false;
	}
	CString label;
	label.Format(_T("Joining_ratio%03d"),ViewJointRatioNo);
	CConfigrationIO sys(m_ParamaterFilePath);
	return sys.getDouble(_T("MetalSurface"),label);
}

/// <summary>
/// 金属面の接合割合の設定
/// </summary>
/// <param name="ViewJointRatioNo">接合面番号</param>
/// <param name="retio">金属面の接合割合</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CPropatyIO::MetalSetJointRetio(int ViewJointRatioNo, double retio)
{
	if (ViewJointRatioNo < 0) {
		return false;
	}
	CString label;
	label.Format(_T("Joining_ratio%03d"),ViewJointRatioNo);
	CConfigrationIO sys(m_ParamaterFilePath);
	if (!sys.setDouble(_T("MetalSurface"),label,retio)) {
		return false;
	}
	return true;
}

/// <summary>
/// 金属面の接合色の取得
/// </summary>
/// <param name="ViewJointRatioNo">接合面番号</param>
/// <returns>金属面の接合色を返す</returns>
COLORREF CPropatyIO::MetalGetJointColor(int ViewJointRatioNo)
{
	if (!CFileUtil::fileExists(m_ParamaterFilePath)) {
		return RGB(0, 0, 0);
	}
	if (ViewJointRatioNo < 0) {
		return RGB(0, 0, 0);
	}

	CString label;
	label.Format(_T("Joining_Color%03d"), ViewJointRatioNo);
	CConfigrationIO sys(m_ParamaterFilePath);
	// 金属面 接合色
	return (COLORREF)(sys.getInt(_T("MetalSurface"), label));
}

/// <summary>
/// 金属面の接合色の設定
/// </summary>
/// <param name="ViewJointRatioNo">接合面番号</param>
/// <param name="color">金属面の接合色</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CPropatyIO::MetalSetJointColor(int ViewJointRatioNo, COLORREF color)
{
	if (ViewJointRatioNo < 0) {
		return false;
	}
	CString label;
	label.Format(_T("Joining_color%03d"), ViewJointRatioNo);
	CConfigrationIO sys(m_ParamaterFilePath);
	// 金属面 接合色
	if (!sys.setInt(_T("MetalSurface"), label, (DWORD)color)) {
		return false;
	}
	return true;
}

/// <summary>
/// 金属面の解析方法の取得
/// </summary>
/// <returns>金属面の解析方法を返す</returns>
int CPropatyIO::MetalGetAnalysisMethod()
{
	return m_MetalAnalysisMethod;
}

/// <summary>
/// 金属面の解析方法の設定
/// </summary>
/// <param name="method">金属面の解析方法</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CPropatyIO::MetalSetAnalysisMethod(int method)
{
	m_MetalAnalysisMethod = method;
	return true;
}

/// <summary>
/// 接合結果の分類数の取得
/// </summary>
/// <returns>接合結果の分類数を返す</returns>
UINT CPropatyIO::ResultGetNumberOfClass(void)
{
	if (!CFileUtil::fileExists(m_ParamaterFilePath)) {
		return false;
	}
	CConfigrationIO sys(m_ParamaterFilePath);
	// 接合結果 分類数
	return sys.getInt(_T("JoiningResult"),_T("Number_of_classifications"));
}

/// <summary>
/// 接合結果の分類数の設定
/// </summary>
/// <param name="nClass">接合結果の分類数</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CPropatyIO::ResultSetNumberOfClass(UINT nClass)
{
//	if (!CFileUtil::fileExists(m_ParamaterFilePath)) {
//		return false;
//	}
	CConfigrationIO sys(m_ParamaterFilePath);
	// 接合結果 分類数
	if (!sys.setInt(_T("JoiningResult"),_T("Number_of_classifications"),nClass)) {
		return false;
	}
	return true;
}

/// <summary>
/// 接合結果の接合割合の取得
/// </summary>
/// <param name="ViewJointRatioNo">接合面番号</param>
/// <returns>接合結果の接合割合を返す</returns>
double CPropatyIO::ResultGetJointRetio(int ViewJointRatioNo)
{
	if (!CFileUtil::fileExists(m_ParamaterFilePath)) {
		return false;
	}
	CString label;
	label.Format(_T("Joining_ratio%03d"),ViewJointRatioNo);
	CConfigrationIO sys(m_ParamaterFilePath);
	return sys.getDouble(_T("JoiningResult"),label);
}

/// <summary>
/// 接合結果の接合割合の設定
/// </summary>
/// <param name="ViewJointRatioNo">接合面番号</param>
/// <param name="retio">接合結果の接合割合</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CPropatyIO::ResultSetJointRetio(int ViewJointRatioNo, double retio)
{
	if (ViewJointRatioNo < 0) {
		return false;
	}
	CString label;
	label.Format(_T("Joining_ratio%03d"),ViewJointRatioNo);
	CConfigrationIO sys(m_ParamaterFilePath);
	if (!sys.setDouble(_T("JoiningResult"),label,retio)) {
		return false;
	}
	return true;
}

/// <summary>
/// 接合結果の接合色の取得
/// </summary>
/// <param name="ViewJointRatioNo">接合面番号</param>
/// <returns>接合結果の接合色を返す</returns>
COLORREF CPropatyIO::ResultGetJointColor(int ViewJointRatioNo)
{
	if (!CFileUtil::fileExists(m_ParamaterFilePath)) {
		return RGB(0, 0, 0);
	}
	if (ViewJointRatioNo < 0) {
		return RGB(0, 0, 0);
	}

	CString label;
	label.Format(_T("Joining_Color%03d"), ViewJointRatioNo);
	CConfigrationIO sys(m_ParamaterFilePath);
	// 接合結果 接合色
	return (COLORREF)(sys.getInt(_T("JoiningResult"), label));
}

/// <summary>
/// 接合結果の接合色の設定
/// </summary>
/// <param name="ViewJointRatioNo">接合面番号</param>
/// <param name="color">接合結果の接合色</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CPropatyIO::ResultSetJointColor(int ViewJointRatioNo, COLORREF color)
{
	if (ViewJointRatioNo < 0) {
		return false;
	}
	CString label;
	label.Format(_T("Joining_color%03d"), ViewJointRatioNo);
	CConfigrationIO sys(m_ParamaterFilePath);
	// 接合結果 接合色
	if (!sys.setInt(_T("JoiningResult"), label, (DWORD)color)) {
		return false;
	}
	return true;
}

/// <summary>
/// 接合結果の解析方法の取得
/// </summary>
/// <returns>接合結果の解析方法を返す</returns>
int CPropatyIO::ResultGetAnalysisMethod()
{
	return m_ResultAnalysisMethod;
}

/// <summary>
/// 接合結果の解析方法の設定
/// </summary>
/// <param name="method">接合結果の解析方法</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CPropatyIO::ResultSetAnalysisMethod(int method)
{
	m_ResultAnalysisMethod = method;
	return true;
}

/// <summary>
/// パラメータファイルの読み込み
/// </summary>
/// <param name="path">パラメータファイルへのパス</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CPropatyIO::Read(CString path/* = _T("")*/)
{
	if (path.IsEmpty()) {
		path = m_ParamaterFilePath;
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
bool CPropatyIO::Save(bool bUpdate/* = true*/, CString path/* = _T("")*/)
{
	if (path.IsEmpty()) {
		path = m_ParamaterFilePath;
	}
	if ((bUpdate) && (!CFileUtil::fileExists(path))) {
		return false;
	}
	return save(path);
}
