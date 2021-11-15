
// WeldEvaluationDoc.cpp : CWeldEvaluationDoc クラスの実装
//

#include "stdafx.h"
// SHARED_HANDLERS は、プレビュー、サムネイル、および検索フィルター ハンドラーを実装している ATL プロジェクトで定義でき、
// そのプロジェクトとのドキュメント コードの共有を可能にします。
#ifndef SHARED_HANDLERS
#include "WeldEvaluation.h"
#endif

#include "WeldEvaluationDoc.h"

#include <propkey.h>
#include <map>
#include <iomanip>
#include <atlimage.h>
#include "message.h"
#include "FileUtil.h"
#include "ConfigrationIO.h"
#include "CLog.h"

#pragma warning(disable:4800)
#pragma warning(disable:4100)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CWeldEvaluationDoc

IMPLEMENT_DYNCREATE(CWeldEvaluationDoc, CDocument)

BEGIN_MESSAGE_MAP(CWeldEvaluationDoc, CDocument)
END_MESSAGE_MAP()


// CWeldEvaluationDoc コンストラクション/デストラクション

/// <summary>
/// コンストラクタ
/// </summary>
CWeldEvaluationDoc::CWeldEvaluationDoc()
{
	m_ModulePath = CFileUtil::GetModulePath();
	m_SystemFilePathName		= m_ModulePath + CString("system.cnf");
	m_ActiveRegisttedTestName	= _T("");
	m_ParamaterFilePaht			= _T("");
	m_ProjectFilePaht			= _T("");
	m_OpenType					= 0;
}

/// <summary>
/// デストラクタ
/// </summary>
CWeldEvaluationDoc::~CWeldEvaluationDoc()
{
	m_ResinScanData.release();
	m_MetalScanData.release();
	m_ResultScanData.release();
}

/// <summary>
/// 新規プロジェクト作成
/// </summary>
/// <returns>成功した場合はTRUE、失敗した場合はFALSE以外を返す</returns>
BOOL CWeldEvaluationDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	if (!NewProject()) {
		return FALSE;
	}

#if false
	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	/// dummy
	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	CScanDataIO dmy;
	CString path1 = _T("C:\\Users\\Project\\WeldEvaluation\\Data\\検査20210801-1\\normalized_1.hdr");
	CString path2 = _T("C:\\Users\\Project\\WeldEvaluation\\Data\\検査20210801-1\\normalized_2.hdr");
	if (dmy.open(path1)) {
		dmy.joinInit();
		dmy.join(path2, 10);
		dmy.joinend(_T("C:\\Users\\Project\\WeldEvaluation\\Data\\検査20210801-1\\joindata.hdr"));
	}

	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
#endif

	return TRUE;
}

// CWeldEvaluationDoc シリアル化

/// <summary>
/// シリアライズ
/// </summary>
/// <param name="ar">アーカイブオブジェクト</param>
void CWeldEvaluationDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 格納するコードをここに追加してください。
	}
	else
	{
		// TODO: 読み込むコードをここに追加してください。
	}
}

#ifdef SHARED_HANDLERS

// サムネイルのサポート
void CWeldEvaluationDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// このコードを変更してドキュメントのデータを描画します
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 検索ハンドラーのサポート
void CWeldEvaluationDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ドキュメントのデータから検索コンテンツを設定します。 
	// コンテンツの各部分は ";" で区切る必要があります

	// 例:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CWeldEvaluationDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CWeldEvaluationDoc 診断

#ifdef _DEBUG
/// <summary>
/// オブジェクトの妥当性検査の実施
/// </summary>
void CWeldEvaluationDoc::AssertValid() const
{
	CDocument::AssertValid();
}

/// <summary>
/// CDumpContextオブジェクトの内容をダンプ
/// </summary>
/// <param name="dc">afxDump診断ダンプ コンテキスト</param>
void CWeldEvaluationDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CWeldEvaluationDoc コマンド


////////////////////////////////////////////////////////////////////////
// プロパティＩ／Ｏ

/// <summary>
/// プロジェクト名の取得
/// </summary>
/// <returns>プロジェクト名を返す</returns>
CString CWeldEvaluationDoc::GetProjectName()
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		return _T("");
	} else {
		return m_PropatyIO.GetProjectName();
	}
}

/// <summary>
/// プロジェクト名の設定
/// </summary>
/// <param name="projectname">プロジェクト名</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CWeldEvaluationDoc::SetProjectName(CString projectname)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		return false;
	} else {
		return m_PropatyIO.SetProjectName(projectname);
	}
}

/// <summary>
/// 登録済み試験格納フォルダの取得
/// </summary>
/// <returns>登録済み試験格納フォルダを返す</returns>
CString CWeldEvaluationDoc::GetRegistedFolder(void)
{
	CConfigrationIO sys(m_SystemFilePathName);
	CString folder = sys.getString(_T("Common"),_T("RegistedFolder"));
	if (folder.IsEmpty()) {
		folder = m_ModulePath + _T("Registed");
	}
	if (!CFileUtil::fileExists(folder)) {
		if (!CreateDirectory(folder,NULL)) {
			folder.Empty();
		}
	} else {
		if (!sys.setString(_T("Common"),_T("RegistedFolder"),folder)) {
			folder.Empty();
		}
	}
	return folder;
}

/// <summary>
/// 登録済み試験格納フォルダの設定
/// </summary>
/// <param name="RegistedFolder">登録済み試験格納フォルダ</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CWeldEvaluationDoc::SetRegistedFolder(CString RegistedFolder)
{
	CConfigrationIO sys(m_SystemFilePathName);
	return sys.setString(_T("Common"),_T("RegistedFolder"),RegistedFolder);
}

/// <summary>
/// ホワイトバランスファイル名の取得
/// </summary>
/// <returns>ホワイトバランスファイル名を返す</returns>
CString CWeldEvaluationDoc::GetWBFileName(void)
{
	CConfigrationIO sys(m_SystemFilePathName);
	CString file = sys.getString(_T("Common"),_T("WBFileName"));
	if (file.IsEmpty()) {
		file = _T("WhiteBalance");
	}
	return file;
}

/// <summary>
/// ホワイトバランスファイル名の設定
/// </summary>
/// <param name="WBFileName">ホワイトバランスファイル名</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CWeldEvaluationDoc::SetWBFileName(CString WBFileName)
{
	CConfigrationIO sys(m_SystemFilePathName);
	return sys.setString(_T("Common"),_T("WBFileName"),WBFileName);
}

/// <summary>
/// 名称の取得
/// </summary>
/// <returns>名称を返す</returns>
CString CWeldEvaluationDoc::GetTestName()
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		CConfigrationIO sys(m_SystemFilePathName);
		return sys.getString(_T("ParamDefault"),_T("Test_name"));
	} else {
		return m_PropatyIO.GetTestName();
	}
}

/// <summary>
/// 名称の設定
/// </summary>
/// <param name="name">名称</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CWeldEvaluationDoc::SetTestName(CString name)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		if (m_OpenType != 2) {
			return false;
		}
	}
	return m_PropatyIO.SetTestName(name);
}

/// <summary>
/// 重なりピクセル数の取得
/// </summary>
/// <returns>重なりピクセル数を返す</returns>
UINT CWeldEvaluationDoc::NumberOfOverridePixel(void)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		CConfigrationIO sys(m_SystemFilePathName);
		return sys.getInt(_T("ParamDefault"),_T("Number_of_overlapping_pixels"));
	} else {
		UINT val = m_PropatyIO.NumberOfOverridePixel();
		if (val == 0) {
			CConfigrationIO sys(m_SystemFilePathName);
			val = sys.getInt(_T("ParamDefault"),_T("Number_of_overlapping_pixels"));
		}
		return val;
	}
}

/// <summary>
/// 重なりピクセル数の設定
/// </summary>
/// <param name="num">重なりピクセル数</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CWeldEvaluationDoc::SetOverridePixelNumber(UINT num)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		return false;
	} else {
		return m_PropatyIO.SetOverridePixelNumber(num);
	}
}

/// <summary>
/// Integration_time_msの取得
/// </summary>
/// <returns>Integration_time_msを返す</returns>
double CWeldEvaluationDoc::GetIntegrationTimeMs(void)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		CConfigrationIO sys(m_SystemFilePathName);
		return sys.getInt(_T("ParamDefault"),_T("Integration_time_ms"));
	} else {
		double val = m_PropatyIO.GetIntegrationTimeMs();
		if (val == 0) {
			CConfigrationIO sys(m_SystemFilePathName);
			val = sys.getInt(_T("ParamDefault"),_T("Integration_time_ms"));
		}
		return val;
	}
}

/// <summary>
/// Integration_time_msの設定
/// </summary>
/// <param name="time">Integration_time_ms</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CWeldEvaluationDoc::SetIntegrationTimeMs(double time)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		return false;
	} else {
		return m_PropatyIO.SetIntegrationTimeMs(time);
	}
}

/// <summary>
/// 縦方向の解像度の取得
/// </summary>
/// <returns>縦方向の解像度を返す</returns>
UINT CWeldEvaluationDoc::GetVerticalResolution(void)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		CConfigrationIO sys(m_SystemFilePathName);
		return sys.getInt(_T("ParamDefault"),_T("Vertical_resolution"));
	} else {
		UINT val = m_PropatyIO.GetVerticalResolution();
		if (val == 0) {
			CConfigrationIO sys(m_SystemFilePathName);
			val = sys.getInt(_T("ParamDefault"),_T("Vertical_resolution"));
		}
		return val;
	}
}

/// <summary>
/// 縦方向の解像度の設定
/// </summary>
/// <param name="resolution">縦方向の解像度</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CWeldEvaluationDoc::SetVerticalResolution(UINT resolution)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		return false;
	} else {
		return m_PropatyIO.SetVerticalResolution(resolution);
	}
}

/// <summary>
/// 横方向の解像度の取得
/// </summary>
/// <returns>横方向の解像度を返す</returns>
UINT CWeldEvaluationDoc::GetHorizontalResolution(void)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		CConfigrationIO sys(m_SystemFilePathName);
		return sys.getInt(_T("ParamDefault"),_T("Horizontal_resolution"));
	} else {
		UINT val = m_PropatyIO.GetHorizontalResolution();
		if (val == 0) {
			CConfigrationIO sys(m_SystemFilePathName);
			val = sys.getInt(_T("ParamDefault"),_T("Horizontal_resolution"));
		}
		return val;
	}
}

/// <summary>
/// 横方向の解像度の設定
/// </summary>
/// <param name="resolution">横方向の解像度</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CWeldEvaluationDoc::SetHorizontalResolution(UINT resolution)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		return false;
	} else {
		return m_PropatyIO.SetHorizontalResolution(resolution);
	}
}

/// <summary>
/// バンド数の取得
/// </summary>
/// <returns>バンド数を返す</returns>
UINT CWeldEvaluationDoc::NumberOfBand(void)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		CConfigrationIO sys(m_SystemFilePathName);
		return sys.getInt(_T("ParamDefault"), _T("Number_of_band"));
	}
	else {
		UINT val = m_PropatyIO.GetNumberOfBand();
		if (val == 0) {
			CConfigrationIO sys(m_SystemFilePathName);
			val = sys.getInt(_T("ParamDefault"), _T("Number_of_band"));
		}
		return val;
	}
}

/// <summary>
/// バンド数の設定
/// </summary>
/// <param name="resolution">バンド数</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CWeldEvaluationDoc::SetNumberOfBand(UINT band)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		return false;
	}
	else {
		return m_PropatyIO.SetHorizontalResolution(band);
	}
}


/// <summary>
/// 樹脂面の分類数の取得
/// </summary>
/// <returns>樹脂面の分類数を返す</returns>
UINT CWeldEvaluationDoc::ResinGetNumberOfClass(void)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		CConfigrationIO sys(m_SystemFilePathName);
		return sys.getInt(_T("ParamDefault"),_T("ResinSurface_Number_of_classifications"));
	} else {
		UINT val = m_PropatyIO.ResinGetNumberOfClass();
		if (val == 0) {
			CConfigrationIO sys(m_SystemFilePathName);
			val = sys.getInt(_T("ParamDefault"),_T("ResinSurface_Number_of_classifications"));
		}
		return val;
	}
}

/// <summary>
/// 樹脂面の分類数の設定
/// </summary>
/// <param name="nClass">樹脂面の分類数</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CWeldEvaluationDoc::ResinSetNumberOfClass(UINT nClass)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		return false;
	} else {
		return m_PropatyIO.ResinSetNumberOfClass(nClass);
	}
}

/// <summary>
/// 樹脂面の接合割合の取得
/// </summary>
/// <param name="ViewJointRatioNo">接合面番号</param>
/// <returns>樹脂面の接合割合を返す</returns>
double CWeldEvaluationDoc::ResinGetJointRetio(int ViewJointRatioNo)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		CConfigrationIO sys(m_SystemFilePathName);
		return sys.getDouble(_T("ParamDefault"),_T("ResinSurface_Joining_ratio"));
	} else {
		double val = m_PropatyIO.ResinGetJointRetio(ViewJointRatioNo);
		if (val == 0) {
			CConfigrationIO sys(m_SystemFilePathName);
			val = sys.getDouble(_T("ParamDefault"),_T("ResinSurface_Joining_ratio"));
		}
		return val;
	}
}

/// <summary>
/// 樹脂面の接合割合の設定
/// </summary>
/// <param name="ViewJointRatioNo">接合面番号</param>
/// <param name="retio">樹脂面の接合割合</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CWeldEvaluationDoc::ResinSetJointRetio(int ViewJointRatioNo, double retio)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		return false;
	} else {
		return m_PropatyIO.ResinSetJointRetio(ViewJointRatioNo, retio);
	}
}

/// <summary>
/// 樹脂面の接合色の取得
/// </summary>
/// <param name="ViewJointRatioNo">接合面番号</param>
/// <returns>樹脂面の接合色を返す</returns>
COLORREF CWeldEvaluationDoc::ResinGetJointColor(int ViewJointRatioNo)
{
	COLORREF color = RGB(0, 0, 0);
	if (!m_ActiveRegisttedTestName.IsEmpty()) {
		color = m_PropatyIO.ResinGetJointColor(ViewJointRatioNo);
	}
	return color;
}

/// <summary>
/// 樹脂面の接合色の設定
/// </summary>
/// <param name="ViewJointRatioNo">接合面番号</param>
/// <param name="color">設定色</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CWeldEvaluationDoc::ResinSetJointColor(int ViewJointRatioNo, COLORREF color)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		return false;
	}
	else {
		return m_PropatyIO.ResinSetJointColor(ViewJointRatioNo, color);
	}
}

/// <summary>
/// 樹脂面の解析方法の取得
/// </summary>
/// <returns>樹脂面の解析方法を返す</returns>
int CWeldEvaluationDoc::ResinGetAnalysisMethod()
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		CConfigrationIO sys(m_SystemFilePathName);
		return sys.getInt(_T("ParamDefault"),_T("ResinSurface_Analysis_method"));
	} else {
		int val = m_PropatyIO.ResinGetAnalysisMethod();
		return val;
	}
}

/// <summary>
/// 樹脂面の解析方法の設定
/// </summary>
/// <param name="method">樹脂面の解析方法</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CWeldEvaluationDoc::ResinSetAnalysisMethod(int method)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		return false;
	} else {
		return m_PropatyIO.ResinSetAnalysisMethod(method);
	}
}

/// <summary>
/// 金属面の分類数の取得
/// </summary>
/// <returns>金属面の分類数を返す</returns>
UINT CWeldEvaluationDoc::MetalGetNumberOfClass(void)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		CConfigrationIO sys(m_SystemFilePathName);
		return sys.getInt(_T("ParamDefault"),_T("MetalSurface_Number_of_classifications"));
	} else {
		UINT val = m_PropatyIO.MetalGetNumberOfClass();
		if (val == 0) {
			CConfigrationIO sys(m_SystemFilePathName);
			val = sys.getInt(_T("ParamDefault"),_T("MetalSurface_Number_of_classifications"));
		}
		return val;
	}
}

/// <summary>
/// 金属面の分類数の設定
/// </summary>
/// <param name="nClass">金属面の分類数</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CWeldEvaluationDoc::MetalSetNumberOfClass(UINT nClass)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		return false;
	} else {
		return m_PropatyIO.MetalSetNumberOfClass(nClass);
	}
}

/// <summary>
/// 金属面の接合割合の取得
/// </summary>
/// <param name="ViewJointRatioNo">接合面番号</param>
/// <returns>金属面の接合割合を返す</returns>
double CWeldEvaluationDoc::MetalGetJointRetio(int ViewJointRatioNo)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		CConfigrationIO sys(m_SystemFilePathName);
		return sys.getDouble(_T("ParamDefault"),_T("MetalSurface_Joining_ratio"));
	} else {
		double val = m_PropatyIO.MetalGetJointRetio(ViewJointRatioNo);
		if (val == 0) {
			CConfigrationIO sys(m_SystemFilePathName);
			val = sys.getDouble(_T("ParamDefault"),_T("MetalSurface_Joining_ratio"));
		}
		return val;
	}
}

/// <summary>
/// 金属面の接合割合の設定
/// </summary>
/// <param name="ViewJointRatioNo">接合面番号</param>
/// <param name="retio">金属面の接合割合</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CWeldEvaluationDoc::MetalSetJointRetio(int ViewJointRatioNo,double retio)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		return false;
	} else {
		return m_PropatyIO.MetalSetJointRetio(ViewJointRatioNo,retio);
	}
}

/// <summary>
/// 金属面の接合色の取得
/// </summary>
/// <param name="ViewJointRatioNo">接合面番号</param>
/// <returns>金属面の接合色を返す</returns>
COLORREF CWeldEvaluationDoc::MetalGetJointColor(int ViewJointRatioNo)
{
	COLORREF color = RGB(0, 0, 0);
	if (!m_ActiveRegisttedTestName.IsEmpty()) {
		color = m_PropatyIO.MetalGetJointColor(ViewJointRatioNo);
	}
	return color;
}

/// <summary>
/// 金属面の接合色の設定
/// </summary>
/// <param name="ViewJointRatioNo">接合面番号</param>
/// <param name="color">設定色</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CWeldEvaluationDoc::MetalSetJointColor(int ViewJointRatioNo, COLORREF color)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		return false;
	}
	else {
		return m_PropatyIO.MetalSetJointColor(ViewJointRatioNo, color);
	}
}

/// <summary>
/// 金属面の解析方法の取得
/// </summary>
/// <returns>金属面の解析方法を返す</returns>
int CWeldEvaluationDoc::MetalGetAnalysisMethod()
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		CConfigrationIO sys(m_SystemFilePathName);
		return sys.getInt(_T("ParamDefault"),_T("MetalSurface_Analysis_method"));
	} else {
		int val = m_PropatyIO.MetalGetAnalysisMethod();
		return val;
	}
}

/// <summary>
/// 金属面の解析方法の設定
/// </summary>
/// <param name="method">金属面の解析方法</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CWeldEvaluationDoc::MetalSetAnalysisMethod(int method)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		return false;
	} else {
		return m_PropatyIO.MetalSetAnalysisMethod(method);
	}
}

/// <summary>
/// 接合結果の分類数の取得
/// </summary>
/// <returns>接合結果の分類数を返す</returns>
UINT CWeldEvaluationDoc::ResultGetNumberOfClass(void)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		CConfigrationIO sys(m_SystemFilePathName);
		return sys.getInt(_T("ParamDefault"),_T("JoiningResult_Number_of_classifications"));
	} else {
		UINT val = m_PropatyIO.ResultGetNumberOfClass();
		if (val == 0) {
			CConfigrationIO sys(m_SystemFilePathName);
			val = sys.getInt(_T("ParamDefault"),_T("JoiningResult_Number_of_classifications"));
		}
		return val;
	}
}

/// <summary>
/// 接合結果の分類数の設定
/// </summary>
/// <param name="nClass">接合結果の分類数</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CWeldEvaluationDoc::ResultSetNumberOfClass(UINT nClass)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		return false;
	} else {
		return m_PropatyIO.ResultSetNumberOfClass(nClass);
	}
}

/// <summary>
/// 接合結果の接合割合の取得
/// </summary>
/// <param name="ViewJointRatioNo">接合面番号</param>
/// <returns>接合結果の接合割合を返す</returns>
double CWeldEvaluationDoc::ResultGetJointRetio(int ViewJointRatioNo)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		CConfigrationIO sys(m_SystemFilePathName);
		return sys.getDouble(_T("ParamDefault"),_T("JoiningResult_Joining_ratio"));
	} else {
		double val = m_PropatyIO.ResultGetJointRetio(ViewJointRatioNo);
		if (val == 0) {
			CConfigrationIO sys(m_SystemFilePathName);
			val = sys.getDouble(_T("ParamDefault"),_T("JoiningResult_Joining_ratio"));
		}
		return val;
	}
}

/// <summary>
/// 接合結果の接合割合の設定
/// </summary>
/// <param name="ViewJointRatioNo">接合面番号</param>
/// <param name="retio">接合結果の接合割合</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CWeldEvaluationDoc::ResultSetJointRetio(int ViewJointRatioNo, double retio)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		return false;
	} else {
		return m_PropatyIO.ResultSetJointRetio(ViewJointRatioNo, retio);
	}
}

/// <summary>
/// 接合結果の接合色の取得
/// </summary>
/// <param name="ViewJointRatioNo">接合面番号</param>
/// <returns>接合結果の接合色を返す</returns>
COLORREF CWeldEvaluationDoc::ResultGetJointColor(int ViewJointRatioNo)
{
	COLORREF color = RGB(0, 0, 0);
	if (!m_ActiveRegisttedTestName.IsEmpty()) {
		color = m_PropatyIO.ResultGetJointColor(ViewJointRatioNo);
	}
	return color;
}

/// <summary>
/// 接合結果の接合色の設定
/// </summary>
/// <param name="ViewJointRatioNo">接合面番号</param>
/// <param name="color">設定色</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CWeldEvaluationDoc::ResultSetJointColor(int ViewJointRatioNo, COLORREF color)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		return false;
	}
	else {
		return m_PropatyIO.ResultSetJointColor(ViewJointRatioNo, color);
	}
}

/// <summary>
/// 表示モードの取得
/// </summary>
/// <param name="targetID">判定素材ID</param>
/// <returns>表示モードを返す</returns>
int CWeldEvaluationDoc::GetDisplayMode(int targetID)
{
	int DisplayMode = DisplayModeScan;
	switch(targetID) {
	case	eResinSurface	:	///< 樹脂
		DisplayMode = m_ProjectIO.GetResinDisplayType();
		break;
	case	eMetalSurface	:	///< 金属
		DisplayMode = m_ProjectIO.GetMetalDisplayType();
		break;
	case	eJoiningResult	:	///< 接合結果
		DisplayMode = m_ProjectIO.GetResultDisplayType();
		break;
	}
	if (DisplayMode == 0) {
		DisplayMode = DisplayModeScan;
	}
	return DisplayMode;
}

/// <summary>
/// 表示モードの設定
/// </summary>
/// <param name="targetID">判定素材ID</param>
/// <param name="DisplayModeID">表示モード</param>
/// <returns>成功の場合はtrue、失敗の場合はfalseを返す</returns>
bool CWeldEvaluationDoc::SetDisplayMode(int targetID, int DisplayModeID)
{
	bool bResult = false;
	switch(targetID) {
	case	eResinSurface	:	///< 樹脂
		bResult = m_ProjectIO.SetResinDisplayType(DisplayModeID);
		break;
	case	eMetalSurface	:	///< 金属
		bResult = m_ProjectIO.SetMetalDisplayType(DisplayModeID);
		break;
	case	eJoiningResult	:	///< 接合結果
		bResult = m_ProjectIO.SetResultDisplayType(DisplayModeID);
		break;
	}
	return bResult;
}


/// <summary>
/// 解析方法の取得
/// </summary>
/// <param name="targetID">判定素材ID</param>
/// <returns>解析モードを返す</returns>
int CWeldEvaluationDoc::GetAnalysisMethod(int targetID)
{
	int AnalizeMode = AnalizeTypeKMeans;
	switch(targetID) {
	case	eResinSurface	:	///< 樹脂
		AnalizeMode = m_PropatyIO.ResinGetAnalysisMethod();
		break;
	case	eMetalSurface	:	///< 金属
		AnalizeMode = m_PropatyIO.MetalGetAnalysisMethod();
		break;
	}
	return AnalizeMode;
}

/// <summary>
/// 解析方法の設定
/// </summary>
/// <param name="targetID">判定素材ID</param>
/// <param name="AnalysisMethodID">解析方法ID</param>
/// <returns>成功の場合はtrue、失敗の場合はfalseを返す</returns>
bool CWeldEvaluationDoc::SetAnalysisMethod(int targetID, int AnalysisMethodID)
{
	bool bResult = false;
	switch(targetID) {
	case	eResinSurface	:	///< 樹脂
		bResult = m_PropatyIO.ResinSetAnalysisMethod(AnalysisMethodID);
		break;
	case	eMetalSurface	:	///< 金属
		bResult = m_PropatyIO.MetalSetAnalysisMethod(AnalysisMethodID);
		break;
	}
	return bResult;
}


//////////////////////////////////////////////////////////////////////////////////
// プロジェクトデータI/O

#if 0
/// <summary>
/// 樹脂面スキャン画像ファイルパスの取得
/// </summary>
/// <returns>樹脂面スキャン画像ファイルパスを返す</returns>
CString CWeldEvaluationDoc::GetResinScanImagePath()
{
	CString root = m_ProjectIO.GetImageDataRootPath();
	CString fname = m_ProjectIO.GetResinScanImageFile();
	fname = fname + _T(".hdr");
	if (root.IsEmpty() || fname.IsEmpty()) {
		return _T("");
	}
	CString path;
	path = CFileUtil::FilePathCombine(root,fname);
	if (!CFileUtil::fileExists(path)) {
		return _T("");
	}
	return path;
}

/// <summary>
/// 樹脂面分類結果ファイルパスの取得
/// </summary>
/// <returns>樹脂面分類結果ファイルパスを返す</returns>
CString CWeldEvaluationDoc::GetResinClassificationResultPath()
{
	CString root = m_ProjectIO.GetImageDataRootPath();
	CString fname = m_ProjectIO.GetResinClassificationResultFile();
	if (root.IsEmpty() || fname.IsEmpty()) {
		return _T("");
	}
	CString path;
	path = CFileUtil::FilePathCombine(root,fname);
	if (!CFileUtil::fileExists(path)) {
		return _T("");
	}
	return path;
}

/// <summary>
/// 金属面スキャン画像ファイルパスの取得
/// </summary>
/// <returns>金属面スキャン画像ファイルパスを返す</returns>
CString CWeldEvaluationDoc::GetMetalScanImagePath()
{
	CString root = m_ProjectIO.GetImageDataRootPath();
	CString fname = m_ProjectIO.GetMetalScanImageFile();
	fname = fname + _T(".hdr");
	if (root.IsEmpty() || fname.IsEmpty()) {
		return _T("");
	}
	CString path;
	path = CFileUtil::FilePathCombine(root,fname);
	if (!CFileUtil::fileExists(path)) {
		return _T("");
	}
	return path;
}

/// <summary>
/// 金属面分類結果ファイルパスの取得
/// </summary>
/// <returns>金属面分類結果ファイルパスを返す</returns>
CString CWeldEvaluationDoc::GetMetalClassificationResultPath()
{
	CString root = m_ProjectIO.GetImageDataRootPath();
	CString fname = m_ProjectIO.GetMetalResultFile();
	if (root.IsEmpty() || fname.IsEmpty()) {
		return _T("");
	}
	CString path;
	path = CFileUtil::FilePathCombine(root,fname);
	if (!CFileUtil::fileExists(path)) {
		return _T("");
	}
	return path;
}

/// <summary>
/// 結果画像スキャン画像ファイルパスの取得
/// </summary>
/// <returns>結果画像スキャン画像ファイルパスを返す</returns>
CString CWeldEvaluationDoc::GetClassScanImagePath()
{
	CString root = m_ProjectIO.GetImageDataRootPath();
	CString fname = m_ProjectIO.GetResultScanImageFile();
	if (root.IsEmpty() || fname.IsEmpty()) {
		return _T("");
	}
	CString path;
	path = CFileUtil::FilePathCombine(root,fname);
	if (!CFileUtil::fileExists(path)) {
		return _T("");
	}
	return path;
}
#endif

/// <summary>
/// スキャンイメージファイルパス取得
/// </summary>
/// <param name="fileID">ファイル種別ID</param>
/// <returns>存在する場合はパスを返す、失敗した場合は空文字を返す</returns>
CString CWeldEvaluationDoc::GetScanImagePath(int fileID)
{
	return getScanImageFilePath(fileID);
}

/// <summary>
/// 分類結果データファイルパス取得
/// </summary>
/// <param name="fileID">ファイル種別ID</param>
/// <param name="type">分類種別ID</param>
/// <returns>存在する場合はパスを返す、失敗した場合は空文字を返す</returns>
CString CWeldEvaluationDoc::GetClassificationResultPath(int fileID, int type)
{
	return getClassificationDataFilePath(fileID, type);
}

/// <summary>
/// スペクトルグラフ（点指定）の対象の取得
/// </summary>
/// <returns>スペクトルグラフ（点指定）の対象を返す</returns>
UINT CWeldEvaluationDoc::GetSpectralGraphPointTarget()
{
	return m_ProjectIO.GetSpectralGraphPointTarget();
}

/// <summary>
/// スペクトルグラフ（点指定）の対象の設定
/// </summary>
/// <param name="targetID">スペクトルグラフ（点指定）の対象</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CWeldEvaluationDoc::SetSpectralGraphPointTarget(UINT targetID)
{
	return m_ProjectIO.SetSpectralGraphPointTarget(targetID);
}

/// <summary>
/// スペクトルグラフ（点指定）の位置の取得
/// </summary>
/// <param name="holizontal">水平値</param>
/// <param name="vertical">垂直値</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CWeldEvaluationDoc::GetSpectralGraphPointPosition(int &holizontal, int &vertical)
{
	return m_ProjectIO.GetSpectralGraphPointPosition(holizontal, vertical);
}

/// <summary>
/// スペクトルグラフ（点指定）の位置の設定
/// </summary>
/// <param name="holizontal">水平値</param>
/// <param name="vertical">垂直値</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CWeldEvaluationDoc::SetSpectralGraphPointPosition(int holizontal, int vertical)
{
	return m_ProjectIO.SetSpectralGraphPointPosition(holizontal, vertical);
}


/// <summary>
/// スペクトルグラフ（区間指定）の対象の取得
/// </summary>
/// <returns>スペクトルグラフ（区間指定）の対象を返す</returns>
UINT CWeldEvaluationDoc::GetSpectralGraphSectionTarget()
{
	return m_ProjectIO.GetSpectralGraphSectionTarget();
}

/// <summary>
/// スペクトルグラフ（区間指定）の対象の設定
/// </summary>
/// <param name="targetID">スペクトルグラフ（区間指定）の対象</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CWeldEvaluationDoc::SetSpectralGraphSectionTarget(UINT targetID)
{
	return m_ProjectIO.SetSpectralGraphSectionTarget(targetID);
}

/// <summary>
/// スペクトルグラフ（区間指定）の開始位置の取得
/// </summary>
/// <param name="holizontal">水平値</param>
/// <param name="vertical">垂直値</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CWeldEvaluationDoc::GetSpectralGraphSectionStartPosition(int &holizontal, int &vertical)
{
	return m_ProjectIO.GetSpectralGraphSectionStartPosition(holizontal, vertical);
}

/// <summary>
/// スペクトルグラフ（区間指定）の開始位置の設定
/// </summary>
/// <param name="holizontal">水平値</param>
/// <param name="vertical">垂直値</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CWeldEvaluationDoc::SetSpectralGraphSectionStartPosition(int holizontal, int vertical)
{
	return m_ProjectIO.SetSpectralGraphSectionStartPosition(holizontal, vertical);
}

/// <summary>
/// スペクトルグラフ（区間指定）の終了位置の取得
/// </summary>
/// <param name="holizontal">水平値</param>
/// <param name="vertical">垂直値</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CWeldEvaluationDoc::GetSpectralGraphSectionEndPosition(int &holizontal, int &vertical)
{
	return m_ProjectIO.GetSpectralGraphSectionEndPosition(holizontal, vertical);
}

/// <summary>
/// スペクトルグラフ（区間指定）の終了位置の設定
/// </summary>
/// <param name="holizontal">水平値</param>
/// <param name="vertical">垂直値</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CWeldEvaluationDoc::SetSpectralGraphSectionEndPosition(int holizontal, int vertical)
{
	return m_ProjectIO.SetSpectralGraphSectionEndPosition(holizontal, vertical);
}

//////////////////////////////////////////////////////////////////////////////////
// 

//############################################################################
//## Private function

/// <summary>
/// ラインスプリッタ
/// </summary>
/// <param name="input">文字列</param>
/// <param name="delimiter">デリミタ</param>
/// <param name="size">最大データ数</param>
/// <returns>デリミタで分解したデータ</returns>
std::vector<int> CWeldEvaluationDoc::split(std::string& input, char delimiter, int size/*= 0*/)
{
	std::istringstream stream(input);
	std::string field;
	std::vector<int> result;
	result.resize(size);
	int i = 0;
	while (getline(stream, field, delimiter)) {
		result[i++] = stoi(field);
	}
	return result;
}

bool CWeldEvaluationDoc::split(std::string& input, char delimiter, int &dataID, std::vector<int> &data)
{
	std::istringstream stream(input);
	std::string field;
	while (getline(stream, field, delimiter)) {
		data[dataID++] = stoi(field);
	}
	return true;
}

CString CWeldEvaluationDoc::SetRegistedTestFolder(void)
{
	CString folder = GetRegistedFolder();
	m_ActiveRegisttedTestFolder = CFileUtil::FilePathCombine(folder,m_ActiveRegisttedTestName);

	CString ParmaterFileName;
	ParmaterFileName.LoadString(IDS_PROPATYFILENAME);
	m_ParamaterFilePaht = CFileUtil::FilePathCombine(m_ActiveRegisttedTestFolder,ParmaterFileName);
	m_ProjectFilePaht = CFileUtil::FilePathCombine(m_ActiveRegisttedTestFolder,_T("WeldEvalution.prj"));

	return m_ActiveRegisttedTestFolder;
}

/// <summary>
/// スキャンファイルの設定有無判定
/// </summary>
/// <param name="fileID">ファイル種別ID</param>
/// <returns>存在する場合はtrue、失敗の場合はfalseを返す</returns>
bool CWeldEvaluationDoc::ExistScanFile(int fileID)
{
	bool bResult = false;
	CString name;

	switch (fileID) {
	case	eResinSurface:	///< 樹脂
		name = m_ProjectIO.GetResinScanImageFile();
		break;
	case	eMetalSurface:	///< 金属
		name = m_ProjectIO.GetMetalScanImageFile();
		break;
	case	eJoiningResult:	///< 接合結果
		name = m_ProjectIO.GetResultScanImageFile();
		break;
	default:
		return false;
	}
	if (name.IsEmpty()) {
		bResult = false;
	}
	else {
		bResult = true;
	}
	return bResult;
}

/// <summary>
/// スキャンファイルパス取得（拡張子なし）
/// </summary>
/// <param name="fileID">ファイル種別ID</param>
/// <returns>存在する場合はパスを返す、失敗した場合は空文字を返す</returns>
CString CWeldEvaluationDoc::getScanDataPath(int ScanID)
{
	CString root, name;

	root = m_ProjectIO.GetImageDataRootPath();
	switch (ScanID) {
	case	eResinSurface:	///< 樹脂
		name = m_ProjectIO.GetResinScanImageFile();
		break;
	case	eMetalSurface:	///< 金属
		name = m_ProjectIO.GetMetalScanImageFile();
		break;
	case	eJoiningResult:	///< 接合結果
		name = m_ProjectIO.GetResultScanImageFile();
		break;
	default:
		return _T("");
	}
	if (root.IsEmpty() || name.IsEmpty()) {
		return _T("");
	}
	return CFileUtil::FilePathCombine(root, name);
}

/// <summary>
/// スキャンデータファイルパス取得
/// </summary>
/// <param name="fileID">ファイル種別ID</param>
/// <returns>存在する場合はパスを返す、失敗した場合は空文字を返す</returns>
CString CWeldEvaluationDoc::getScanDataFilePath(int ScanID)
{
	CString pathName = getScanDataPath(ScanID);
	CString fPathName = pathName + _T(".hdr");
	return fPathName;
}

/// <summary>
/// スキャンイメージファイルパス取得
/// </summary>
/// <param name="fileID">ファイル種別ID</param>
/// <returns>存在する場合はパスを返す、失敗した場合は空文字を返す</returns>
CString CWeldEvaluationDoc::getScanImageFilePath(int ScanID)
{
	CString pathName = getScanDataPath(ScanID);
	CString fPathName = pathName + _T(".bmp");
	return fPathName;
}

/// <summary>
/// 分類結果データファイルパス取得
/// </summary>
/// <param name="ScanID">ファイル種別ID</param>
/// <param name="type">分類種別ID</param>
/// <returns>存在する場合はパスを返す、失敗した場合は空文字を返す</returns>
CString CWeldEvaluationDoc::getClassificationDataFilePath(int ScanID, int type)
{
	CString root, name;
	CString classfile, imagefile;

	root = m_ProjectIO.GetImageDataRootPath();
	switch (ScanID) {
	case	eResinSurface:	///< 樹脂
		if (type == AnalizeTypeKMeans) {
			name = m_ProjectIO.GetResinDendrogramClassFile();
		}
		else {
			name = m_ProjectIO.GetResinKmeansClassFile();
		}
		break;
	case	eMetalSurface:	///< 金属
		if (type == AnalizeTypeKMeans) {
			name = m_ProjectIO.GetMetalDendrogramClassFile();
		}
		else {
			name = m_ProjectIO.GetMetalKmeansClassFile();
		}
		break;
	case	eJoiningResult:	///< 接合結果
		if (type == AnalizeTypeKMeans) {
			name = m_ProjectIO.GetResultDendrogramClassFile();
		}
		else {
			name = m_ProjectIO.GetResultKmeansClassFile();
		}
		break;
	default:
		return false;
	}
	if (root.IsEmpty() || name.IsEmpty()) {
		return _T("");
	}

	classfile = CFileUtil::FilePathCombine(root, name);
	return classfile;
}

/// <summary>
/// 分類結果イメージファイルパス取得
/// </summary>
/// <param name="ScanID">ファイル種別ID</param>
/// <param name="type">分類種別ID</param>
/// <returns>存在する場合はパスを返す、失敗した場合は空文字を返す</returns>
CString CWeldEvaluationDoc::getClassificationImageFilePath(int ScanID, int type)
{
	CString classfile = getClassificationDataFilePath(ScanID,type);
	if (classfile.IsEmpty()) {
		return _T("");
	}
	CString imagePath = classfile + _T(".bmp");
	return imagePath;
}

/// <summary>
/// メインウインドの位置とサイズの取得
/// </summary>
/// <param name="rect">ウインドの位置とサイズ</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CWeldEvaluationDoc::GetWindRect(CRect &rect)
{
	CConfigrationIO sys(m_SystemFilePathName);
	int top,left,width,hight;

	top = sys.getInt(_T("System"),_T("wind_top"));
	left = sys.getInt(_T("System"),_T("wind_left"));
	width = sys.getInt(_T("System"),_T("form_width"));
	hight = sys.getInt(_T("System"),_T("form_height"));
	rect.top = top;
	rect.left = left;
	rect.bottom = top + hight;
	rect.right = left + width;
	if ((width <= 0) || (hight <= 0) || (top < 0) || (left < 0)) {
		return false;
	} else {
		return true;
	}
}

/// <summary>
/// メインウインドの位置とサイズの設定
/// </summary>
/// <param name="rect">ウインドの位置とサイズ</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CWeldEvaluationDoc::SetWindRect(CRect &rect)
{
	CConfigrationIO sys(m_SystemFilePathName);
	sys.setInt(_T("System"),_T("wind_top"),rect.top);
	sys.setInt(_T("System"),_T("wind_left"),rect.left);
	sys.setInt(_T("System"),_T("form_width"),rect.Width());
	sys.setInt(_T("System"),_T("form_height"),rect.Height());
	return true;
}

/// <summary>
/// 区間スペクトルダイアログの位置とサイズの取得
/// </summary>
/// <param name="rect">ウインドの位置とサイズ</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CWeldEvaluationDoc::GetSpectralDlgRect(CRect &rect)
{
	CConfigrationIO sys(m_SystemFilePathName);
	int top, left, width, hight;

	top = sys.getInt(_T("System"), _T("SpectralDlg_top"));
	left = sys.getInt(_T("System"), _T("SpectralDlg_left"));
	width = sys.getInt(_T("System"), _T("SpectralDlg_width"));
	hight = sys.getInt(_T("System"), _T("SpectralDlg_height"));
	rect.top = top;
	rect.left = left;
	rect.bottom = top + hight;
	rect.right = left + width;
	if ((width <= 0) || (hight <= 0) || (top < 0) || (left < 0)) {
		return false;
	}
	else {
		return true;
	}
}

/// <summary>
/// 区間スペクトルダイアログの位置とサイズの取得
/// </summary>
/// <param name="rect">ウインドの位置とサイズ</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CWeldEvaluationDoc::SetSpectralDlgRect(CRect &rect)
{
	CConfigrationIO sys(m_SystemFilePathName);
	sys.setInt(_T("System"), _T("SpectralDlg_top"), rect.top);
	sys.setInt(_T("System"), _T("SpectralDlg_left"), rect.left);
	sys.setInt(_T("System"), _T("SpectralDlg_width"), rect.Width());
	sys.setInt(_T("System"), _T("SpectralDlg_height"), rect.Height());
	return true;
}

CString CWeldEvaluationDoc::GetDeviceName()
{
	CConfigrationIO sys(m_SystemFilePathName);
	CString name;
	name = sys.getString(_T("System"), _T("DeviceName"), _T(""));
	return name;
}

bool CWeldEvaluationDoc::IsCameraDummyApi()
{
	CConfigrationIO sys(m_SystemFilePathName);
	int use = sys.getInt(_T("System"), _T("UseCameraDummyApi"));
	if (use == 0) {
		return false;
	}
	else {
		return true;
	}
}

/// <summary>
/// プロジェクトのオープン判定
/// </summary>
/// <returns>オープンしている場合はtrue、そうでない場合はfalseを返す</returns>
bool CWeldEvaluationDoc::IsOpen(void)
{
	return m_OpenType!=0?true:false;
}

/// <summary>
/// プロジェクトの新規判定
/// </summary>
/// <returns>新規の場合はtrue、そうでない場合はfalseを返す</returns>
bool CWeldEvaluationDoc::IsNew(void)
{
	return m_OpenType==2?true:false;
}

/// <summary>
/// 新規プロジェクトの構築
/// </summary>
/// <returns>成功した場合はtrue、そうでない場合はfalseを返す</returns>
bool CWeldEvaluationDoc::NewProject()
{
	m_ResinScanData.close();
	m_MetalScanData.close();
	m_ResultScanData.close();

	CString tmpFolder = GetRegistedFolder();
	if (tmpFolder.IsEmpty()) {
		tmpFolder	= m_ModulePath;
	}
	m_ActiveRegisttedTestName = tmpFolder;
	m_ActiveRegisttedTestFolder = CFileUtil::FilePathCombine(m_ActiveRegisttedTestName,_T(""));
	if (!CFileUtil::fileExists(m_ActiveRegisttedTestFolder)) {
		if (!CFileUtil::MakeDir(m_ActiveRegisttedTestFolder)) {
			return false;
		}
	}

	m_ParamaterFilePaht = CFileUtil::FilePathCombine(m_ActiveRegisttedTestFolder,_T("paramater.tmp"));
	m_ProjectFilePaht	= CFileUtil::FilePathCombine(m_ActiveRegisttedTestFolder,_T("project.tmp"));

	m_PropatyIO.SetParamaterFilePath(m_ParamaterFilePaht);
	m_ProjectIO.SetProjectFilePath(m_ProjectFilePaht);

	UINT uval;
	CString str;
	double dval;
	// Default
	CConfigrationIO sys(m_SystemFilePathName);
	str = sys.getString(_T("ParamDefault"),_T("Test_name"));
	m_PropatyIO.SetTestName(str);
	uval = sys.getInt(_T("ParamDefault"),_T("Number_of_overlapping_pixels"));
	m_PropatyIO.SetOverridePixelNumber(uval);
	uval = sys.getInt(_T("ParamDefault"),_T("Integration_time_ms"));
	m_PropatyIO.SetIntegrationTimeMs(uval);
	uval = sys.getInt(_T("ParamDefault"),_T("Vertical_resolution"));
	m_PropatyIO.SetVerticalResolution(uval);
	uval = sys.getInt(_T("ParamDefault"),_T("Horizontal_resolution"));
	m_PropatyIO.SetHorizontalResolution(uval);

	uval = sys.getInt(_T("ParamDefault"),_T("ResinSurface_Number_of_classifications"));
	m_PropatyIO.ResinSetNumberOfClass(uval);
	for (int i = 0; i < (int)uval;i++) {
		dval = sys.getDouble(_T("ParamDefault"),_T("ResinSurface_Joining_ratio"));
		m_PropatyIO.ResinSetJointRetio(i,dval);
	}
	uval = sys.getInt(_T("ParamDefault"),_T("ResinSurface_Analysis_method"));
	m_PropatyIO.ResinSetAnalysisMethod(uval);

	uval = sys.getInt(_T("ParamDefault"),_T("MetalSurface_Number_of_classifications"));
	m_PropatyIO.MetalSetNumberOfClass(uval);
	for (int i = 0; i < (int)uval;i++) {
		dval = sys.getDouble(_T("ParamDefault"),_T("MetalSurface_Joining_ratio"));
		m_PropatyIO.MetalSetJointRetio(i,dval);
	}
	uval = sys.getInt(_T("ParamDefault"),_T("MetalSurface_Analysis_method"));
	m_PropatyIO.MetalSetAnalysisMethod(uval);

	uval = sys.getInt(_T("ParamDefault"),_T("JoiningResult_Number_of_classifications"));
	m_PropatyIO.ResultSetNumberOfClass(uval);
	for (int i = 0; i < (int)uval;i++) {
		uval = sys.getInt(_T("ParamDefault"),_T("JoiningResult_Joining_ratio"));
		m_PropatyIO.ResultSetJointRetio(i,uval);
	}

	m_ProjectIO.Initialze();

	m_OpenType = 2;

	return true;
}

/// <summary>
/// 既存プロジェクトのオープン
/// </summary>
/// <param name="RegistedTestName">検査名</param>
/// <returns>成功した場合はtrue、そうでない場合はfalseを返す</returns>
bool CWeldEvaluationDoc::OpenProject(CString RegistedTestName)
{
	CString PopatyFileName,ProjecName, ProjectFileName;
	PopatyFileName.LoadStringW(IDS_PROPATYFILENAME);
	ProjecName.LoadStringW(IDS_PROJECTFILENAME);

	// プロジェクトフォルダ取得
	CString folder = GetRegistedFolder();
	CString RegisttedTestFolder = CFileUtil::FilePathCombine(folder,RegistedTestName);
	if (!CFileUtil::fileExists(RegisttedTestFolder)) {
		return false;
	}

	// パラメータ取得
	CString ParamaterFilePaht;
	ParamaterFilePaht = CFileUtil::FilePathCombine(RegisttedTestFolder,PopatyFileName);
	if (!CFileUtil::fileExists(ParamaterFilePaht)) {
		return false;
	}

	if (!m_PropatyIO.SetParamaterFilePath(ParamaterFilePaht)) {
		m_PropatyIO.SetParamaterFilePath(m_ParamaterFilePaht);
		return false;
	}
	if (!m_PropatyIO.Read()) {
		return false;
	}

	// プロパティ取得
	CString str = m_PropatyIO.GetProjectName();
	if (!str.IsEmpty()) {
		ProjecName = str;
	}
	CString ProjectFilePaht;
	ProjectFileName.Format(_T("%s.prj"), (LPCWSTR)ProjecName);
	ProjectFilePaht = CFileUtil::FilePathCombine(RegisttedTestFolder,ProjectFileName);
	if (!CFileUtil::fileExists(ProjectFilePaht)) {
		m_ProjectIO.SetProjectFilePath(m_ProjectFilePaht);
		return false;
	}
	if (!m_ProjectIO.SetProjectFilePath(ProjectFilePaht)) {
		m_ProjectIO.SetProjectFilePath(m_ProjectFilePaht);
		return false;
	}
	if (!m_ProjectIO.Read()) {
		return false;
	}

	m_ActiveRegisttedTestName = RegistedTestName;
	m_ActiveRegisttedTestFolder = RegisttedTestFolder;
	m_ParamaterFilePaht = ParamaterFilePaht;
	m_ProjectFilePaht = ProjectFilePaht;

	m_ResinScanData.close();
	m_MetalScanData.close();
	m_ResultScanData.close();

	CString path;
	path = getScanDataFilePath(eResinSurface);
	if (CFileUtil::fileExists(path)) {
		m_ResinScanData.open(path);
	}
	path = getScanDataFilePath(eMetalSurface);
	if (CFileUtil::fileExists(path)) {
		m_MetalScanData.open(path);
	}
	path = getScanDataFilePath(eJoiningResult);
	if (CFileUtil::fileExists(path)) {
		m_ResultScanData.open(path);
	}

	m_OpenType = 1;
	return true;
}

/// <summary>
/// プロジェクトの終了
/// </summary>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CWeldEvaluationDoc::CloseProject(bool bSave)
{
	if (bSave) {
		if (!SaveProject()) {
			return false;
		}
	}

	m_ActiveRegisttedTestName = _T("");
	m_ActiveRegisttedTestFolder = _T("");
	m_ParamaterFilePaht = _T("");
	m_ProjectFilePaht = _T("");

	m_OpenType = 0;
	return true;
}

/// <summary>
/// プロジェクトの保存
/// </summary>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CWeldEvaluationDoc::SaveProject()
{
	CString TestName = m_PropatyIO.GetTestName();
	if (TestName.IsEmpty()) {
		return false;
	}
	CString tmpParamFilePath = _T("");
	if (m_OpenType != 2) {
		if (m_ActiveRegisttedTestFolder.IsEmpty() || !CFileUtil::fileExists(m_ActiveRegisttedTestFolder)) {
			return false;
		}
	} else {
		tmpParamFilePath = m_ParamaterFilePaht;
	}

	COleDateTime date = COleDateTime::GetCurrentTime();
	CString prjName = m_PropatyIO.GetProjectName();
	CString ProjectFileName;
	CString folder = GetRegistedFolder();
	if (prjName.IsEmpty()) {
		int i=1;
		while(true) {
			prjName = m_PropatyIO.MakeProjectName(TestName,date,i);
			CString PathName = CFileUtil::FilePathCombine(folder,prjName);
			if (!CFileUtil::fileExists(PathName)) {
				if (!CreateDirectory(PathName,NULL)) {
					return false;
				}
				m_ActiveRegisttedTestName=prjName;
				m_ActiveRegisttedTestFolder = PathName;
				m_ProjectIO.SetCreateDay(date);
				break;
			}
			i++;
		}
		ProjectFileName.Format(_T("%s.prj"), (LPCWSTR)prjName);
		m_PropatyIO.SetProjectName(prjName);
	} else {
		ProjectFileName.Format(_T("%s.prj"), (LPCWSTR)prjName);
	}

	CString ParmaterFileName;
	ParmaterFileName.LoadString(IDS_PROPATYFILENAME);
	m_ParamaterFilePaht = CFileUtil::FilePathCombine(m_ActiveRegisttedTestFolder,ParmaterFileName);
	m_PropatyIO.SetParamaterFilePath(m_ParamaterFilePaht);
	if (!m_PropatyIO.Save(false)) {
		CFileUtil::fileDelete(m_ActiveRegisttedTestFolder);
		m_ActiveRegisttedTestName=_T("");
		m_ActiveRegisttedTestFolder=_T("");
		m_PropatyIO.SetProjectName(_T(""));
		CString tmpFolder	= m_ModulePath;
		m_ParamaterFilePaht = CFileUtil::FilePathCombine(tmpFolder,_T("paramater.tmp"));
		m_ProjectFilePaht	= CFileUtil::FilePathCombine(tmpFolder,_T("project.tmp"));
		return false;
	}
	if (m_OpenType == 2) {
		UINT uval;
		CPropatyIO pf;
		pf.SetParamaterFilePath(tmpParamFilePath);
		uval = pf.NumberOfOverridePixel();
		bool bResult = true;
		if (!m_PropatyIO.SetOverridePixelNumber(uval)) {
			bResult = false;
		}
		double dval;
		dval = pf.GetIntegrationTimeMs();
		if (!m_PropatyIO.SetIntegrationTimeMs(dval)) {
			bResult = false;
		}
		uval = pf.GetHorizontalResolution();
		if (!m_PropatyIO.SetHorizontalResolution(uval)) {
			bResult = false;
		}
		uval = pf.GetVerticalResolution();
		if (!m_PropatyIO.SetVerticalResolution(uval)) {
			bResult = false;
		}

		uval = pf.ResinGetNumberOfClass();
		if (!m_PropatyIO.ResinSetNumberOfClass(uval)) {
			bResult = false;
		}
		for (int i = 0; i < (int)uval; i++) {
			dval = pf.ResinGetJointRetio(i);
			if (!m_PropatyIO.ResinSetJointRetio(i,dval)) {
				bResult = false;
			}
		}
		uval = pf.MetalGetNumberOfClass();
		if (!m_PropatyIO.MetalSetNumberOfClass(uval)) {
			bResult = false;
		}
		for (int i = 0; i < (int)uval; i++) {
			dval = pf.MetalGetJointRetio(i);
			if (!m_PropatyIO.MetalSetJointRetio(i,dval)) {
				bResult = false;
			}
		}
		uval = pf.ResultGetNumberOfClass();
		if (!m_PropatyIO.ResultSetNumberOfClass(uval)) {
			bResult = false;
		}
		for (int i = 0; i < (int)uval; i++) {
			dval = pf.ResultGetJointRetio(i);
			if (!m_PropatyIO.ResultSetJointRetio(i,dval)) {
				bResult = false;
			}
		}
	}

	CString path;
	CString ImageRootPath = m_ProjectIO.GetImageDataRootPath();
	if (ImageRootPath == _T("")) {
		if (!m_ProjectIO.SetImageDataRootPath(m_ActiveRegisttedTestFolder)) {
		}
		ImageRootPath = m_ProjectIO.GetImageDataRootPath();
	}

	m_ProjectIO.SetUpdateDay(date);
	CString filename;
	// 樹脂面スキャンイ画像がある場合
	{
		if (!SaveScanImage(eResinSurface)) {
			return false;
		}
	}

	// 金属面スキャンイ画像がある場合
	{
		if (!SaveScanImage(eMetalSurface)) {
			return false;
		}
	}

	// 接合結果スキャンイ画像がある場合
	{
		if (!SaveScanImage(eJoiningResult)) {
			return false;
		}
	}

	// 樹脂面k-means分類結果ファイルがある場合
	{
		if (!SaveClassificationResultFile(eResinSurface, AnalizeTypeKMeans)) {
			return false;
		}
	}
	// 樹脂面階層クラスタリング分類結果ファイルがある場合
	{
		if (!SaveClassificationResultFile(eResinSurface, AnalizeTypeHiClustering)) {
			return false;
		}
	}

	// 金属面k-means分類結果ファイルがある場合
	{
		if (!SaveClassificationResultFile(eMetalSurface, AnalizeTypeKMeans)) {
			return false;
		}
	}
	// 金属面階層クラスタリング分類結果ファイルがある場合
	{
		if (!SaveClassificationResultFile(eMetalSurface, AnalizeTypeHiClustering)) {
			return false;
		}
	}

	
	// 接合結果k-means分類結果ファイルがある場合
	{
		if (!SaveClassificationResultFile(eJoiningResult, AnalizeTypeKMeans)) {
			return false;
		}
	}
	// 接合結果階層クラスタリング分類結果ファイルがある場合
	{
		if (!SaveClassificationResultFile(eJoiningResult, AnalizeTypeHiClustering)) {
			return false;
		}
	}


	m_ProjectFilePaht = CFileUtil::FilePathCombine(m_ActiveRegisttedTestFolder,ProjectFileName);
	m_ProjectIO.SetProjectFilePath(m_ProjectFilePaht);
	if (!m_ProjectIO.Save(false)) {
		CFileUtil::fileDelete(m_ParamaterFilePaht);
		CFileUtil::fileDelete(m_ActiveRegisttedTestFolder);
		m_ActiveRegisttedTestName=_T("");
		m_ActiveRegisttedTestFolder=_T("");
		m_PropatyIO.SetProjectName(_T(""));
		CString tmpFolder	= m_ModulePath;
		m_ParamaterFilePaht = CFileUtil::FilePathCombine(tmpFolder,_T("paramater.tmp"));
		m_ProjectFilePaht	= CFileUtil::FilePathCombine(tmpFolder,_T("project.tmp"));
		return false;
	}
	if (m_OpenType == 2) {
		m_OpenType = 1;
	}
	return true;
}

/// <summary>
/// アクティブな検査名の取得
/// </summary>
/// <returns>アクティブな検査名を返す</returns>
CString CWeldEvaluationDoc::GetActiveRegistedTest()
{
	return m_ActiveRegisttedTestName;
}

/// <summary>
/// スナップスキャンファイルパスの取得
/// </summary>
/// <returns>スナップスキャンファイルへのパスを返す</returns>
CString CWeldEvaluationDoc::getSnapscanFile()
{
	CConfigrationIO sys(m_SystemFilePathName);
	CString Snapscan_file = sys.getString(_T("Common"), _T("Snapscan_file"));
	return Snapscan_file;
}

/// <summary>
/// スキャンの分割数
/// </summary>
/// <returns>スキャンの分割数を返す</returns>
int	CWeldEvaluationDoc::GetDivisionNumber()
{
	CConfigrationIO sys(m_SystemFilePathName);
	int DivisionNumber = sys.getInt(_T("System"), _T("DivisionNumber"));
	return DivisionNumber;
}

/// <summary>
/// 解析結果ファイルの読み込み
/// </summary>
/// <param name="path">解析ファイルパス</param>
/// <param name="data">解析データ</param>
/// <returns>成功の場合はtrue、失敗の場合はfalseを返す</returns>
bool CWeldEvaluationDoc::getResultFile(CString path, vector<int>& data)
{
	m_CalcResultStopRequest = false;
	std::wstring spath((LPCTSTR)path);
	std::ifstream ifs(spath);

	UINT fileSize = 0;
	CView *pView = nullptr;
	POSITION p = GetFirstViewPosition();
	if (p != NULL) {
		pView = GetNextView(p);
		{ // ファイルのサイズを求める
			ifs.seekg(0, fstream::end);			//ファイル末尾を探す
			UINT eofPos = (UINT)ifs.tellg();	//ファイル末尾インデクスを取得
			ifs.clear();						//先頭にもどるために一度clear()をかける。これをしないと次のseekg()でコケるときがある。
			ifs.seekg(0, fstream::beg);			//ファイル先頭に戻る
			UINT begPos = (UINT)ifs.tellg();	//ファイル先頭インデクスを取得
			fileSize = eofPos - begPos;			//末尾-先頭でファイルサイズを計算
		}
		pView->SendMessage(WM_READ_RESULT_STATUS,READ_RESULT_INIT,fileSize);
	}

	if (!ifs.is_open())
	{
		if (pView) {
			pView->SendMessage(WM_READ_RESULT_STATUS,READ_RESULT_END,NULL);
		}
		return false;
	}
	
	data.clear();
	int h = m_PropatyIO.GetHorizontalResolution();
	int v = m_PropatyIO.GetVerticalResolution();
	data.resize(h*v);

	string line;
	int id = 0;
	while (getline(ifs, line)) {
#if 0
		vector<int> strvec = split(line, ',',h);
		for (int i=0; i<strvec.size();i++){
			data[id++] = strvec[i];
			if (pView) {
				UINT readPos = (UINT)ifs.tellg();
				pView->SendMessage(WM_READ_RESULT_STATUS,READ_RESULT_READ,readPos);
			}
			if (m_CalcResultStopRequest) {	// 処理停止リクエスト
				if (pView) {
					pView->SendMessage(WM_READ_RESULT_STATUS,READ_RESULT_END,NULL);
				}
				return false;
			}
		}
#else
		if (split(line, ',', id, data)) {
			if (pView) {
				UINT readPos = (UINT)ifs.tellg();
				pView->SendMessage(WM_READ_RESULT_STATUS, READ_RESULT_READ, readPos);
			}

			if (m_CalcResultStopRequest) {	// 処理停止リクエスト
				if (pView) {
					pView->SendMessage(WM_READ_RESULT_STATUS, READ_RESULT_END, NULL);
				}
				return false;
			}
		}
		else {
			return false;
		}
#endif
	}
	if (pView) {
		pView->SendMessage(WM_READ_RESULT_STATUS,READ_RESULT_END,NULL);
	}
	return true;
}

/// <summary>
/// スキャンデータの保存
/// </summary>
/// <param name="ScanID">フスキャンID</param>
/// <returns>成功した場合はtrue、失敗の場合はfalseを返す</returns>
bool CWeldEvaluationDoc::SaveScanImage(int ScanID)
{
	CString ProjectName = m_PropatyIO.GetProjectName();
	CString fileName;
	switch(ScanID) {
	case	eResinSurface	:	///< 樹脂
		{
			fileName.Format(_T("%s_ResinScanImage"), (LPCWSTR)ProjectName);
			////////////////////////////////////////////////////////////////////
			//
			// 保存処理を行う(.hdr,.raw,.bmp)
			// 画像の保存に成功したらプロジェクトへ登録
			//
			////////////////////////////////////////////////////////////////////
			if (!m_ProjectIO.SetResinScanImageFile(fileName)) {
				return false;
			}
		}
		break;
	case	eMetalSurface	:	///< 金属
		{
			fileName.Format(_T("%s_MetalScanImage"), (LPCWSTR)ProjectName);
			////////////////////////////////////////////////////////////////////
			//
			// 保存処理を行う(.hdr,.raw,.bmp)
			// 画像の保存に成功したらプロジェクトへ登録
			//
			////////////////////////////////////////////////////////////////////
			if (!m_ProjectIO.SetMetalScanImageFile(fileName)) {
				return false;
			}
		}
		break;
	case	eJoiningResult	:	///< 接合結果
		{
			fileName.Format(_T("%s_ResultScanImage"), (LPCWSTR)ProjectName);
			////////////////////////////////////////////////////////////////////
			//
			// 保存処理を行う(.hdr,.raw,.bmp)
			// 画像の保存に成功したらプロジェクトへ登録
			//
			////////////////////////////////////////////////////////////////////
			if (!m_ProjectIO.SetResultScanImageFile(fileName)) {
				return false;
			}
		}
		break;
	default :
		return false;
	}

	return true;
}

/// <summary>
/// 分類ファイルの有無判定
/// </summary>
/// <param name="fileID">ファイル種別ID</param>
/// <param name="type">解析タイプ</param>
/// <returns>存在する場合はtrue、失敗の場合はfalseを返す</returns>
bool CWeldEvaluationDoc::ExistClassificationResultFile(int fileID, int type)
{
	bool bResult = false;
	CString name;

	name = getClassificationDataFilePath(fileID, type);
	if (name.IsEmpty()) {
		bResult = false;
	}
	else {
		bResult = true;
	}
	return bResult;
}

/// <summary>
/// 分類結果の保存
/// </summary>
/// <param name="ScanID">対象スキャンID</param>
/// <param name="type">解析タイプ</param>
/// <returns>成功した場合はtrue、失敗の場合はfalseを返す</returns>
bool CWeldEvaluationDoc::SaveClassificationResultFile(int ScanID, int type)
{
	CString ProjectName = m_PropatyIO.GetProjectName();
	CString fileName;
	switch(ScanID) {
	case	eResinSurface	:	// 樹脂
		if (type == AnalizeTypeKMeans) 
		{
			fileName = _T("ResinDendrogramClassification.csv");
			////////////////////////////////////////////////////////////////////
			//
			// 保存処理を行う
			// 保存に成功したらプロジェクトへ登録
			//
			////////////////////////////////////////////////////////////////////
			if (!m_ProjectIO.SetResinDendrogramClassFile(fileName)) {
				return false;
			}
		}
		else if (type == AnalizeTypeHiClustering) 
		{
			fileName = _T("ResinkmeansClassification.csv");
			////////////////////////////////////////////////////////////////////
			//
			// 保存処理を行う
			// 保存に成功したらプロジェクトへ登録
			//
			////////////////////////////////////////////////////////////////////
			if (!m_ProjectIO.SetResinKmeansClassFile(fileName)) {
				return false;
			}
		}
		break;
	case	eMetalSurface	:	// 金属
		if (type == AnalizeTypeKMeans)
		{
			fileName = _T("ResinDendrogramClassification.csv");
			////////////////////////////////////////////////////////////////////
			//
			// 保存処理を行う
			// 保存に成功したらプロジェクトへ登録
			//
			////////////////////////////////////////////////////////////////////
			if (!m_ProjectIO.SetMetalDendrogramClassFile(fileName)) {
				return false;
			}
		}
		else if (type == AnalizeTypeHiClustering) {
			fileName = _T("ResinkmeansClassification.csv");
			////////////////////////////////////////////////////////////////////
			//
			// 保存処理を行う
			// 保存に成功したらプロジェクトへ登録
			//
			////////////////////////////////////////////////////////////////////
			if (!m_ProjectIO.SetMetalKmeansClassFile(fileName)) {
				return false;
			}
		}
		break;
	case	eJoiningResult:	// 接合結果
		if (type == AnalizeTypeKMeans)
		{
			fileName = _T("ResinDendrogramClassification.csv");
			////////////////////////////////////////////////////////////////////
			//
			// 保存処理を行う
			// 保存に成功したらプロジェクトへ登録
			//
			////////////////////////////////////////////////////////////////////
			if (!m_ProjectIO.SetResultDendrogramClassFile(fileName)) {
				return false;
			}
		}
		else if (type == AnalizeTypeHiClustering) {
			fileName = _T("ResinkmeansClassification.csv");
			////////////////////////////////////////////////////////////////////
			//
			// 保存処理を行う
			// 保存に成功したらプロジェクトへ登録
			//
			////////////////////////////////////////////////////////////////////
			if (!m_ProjectIO.SetResultKmeansClassFile(fileName)) {
				return false;
			}
		}
		break;
	default :
		return false;
	}
	return true;
}


/// <summary>
/// 登録済み検査の取得
/// </summary>
/// <param name="list">登録済み検査のリスト</param>
/// <returns>成功した場合はtrue、失敗の場合はfalseを返す</returns>
bool CWeldEvaluationDoc::GetRegistTestList(CStringArray &list)
{
	list.RemoveAll();
	CString path = GetRegistedFolder();
	if (path.IsEmpty() || (!CFileUtil::fileExists(path))) {
		return false;	
	}

	CFileFind    cFileFind;
	CString      strSearchDir = m_ActiveRegisttedTestFolder = CFileUtil::FilePathCombine(path,_T("*"));
	if (!cFileFind.FindFile(strSearchDir)) {
		return false;	
	}

	bool bResult = false;
	do
	{
		bResult = cFileFind.FindNextFile();
		if (!cFileFind.IsDots()) {
			if (cFileFind.IsDirectory()) {
				CString patnname = cFileFind.GetFileTitle();
				list.Add(patnname);
			}
		}
	} while (bResult);
	return true;
}

/// <summary>
/// 解析処理中止リクエスト
/// </summary>
void CWeldEvaluationDoc::CalcResultStopRequest()
{
	m_CalcResultStopRequest = true;
}

/// <summary>
/// 接合率を求める
/// </summary>
/// <param name="buff">データ</param>
/// <param name="ndata">データ数</param>
/// <param name="nClass">分類数</param>
/// <param name="retio">接合率</param>
/// <returns>成功の場合はtrue、失敗の場合はfalseを返す</returns>
bool CWeldEvaluationDoc::CalcJoJointRetio(vector<int> data, int nClass, vector<double> &retio)
{
	retio.clear();
	retio.resize(nClass);
	retio.reserve(nClass);

	std::map<int,int> sum;
	for (int i = 0; i < nClass; i++) {
		sum[i] = 0;
	}

	for (int i = 0; i < data.size(); i++) {
		if ((data[i] < 0) || (data[i] >= nClass)) {
			continue;
		}
		sum[data[i]] += 1;
	}

	for (int i = 0; i < nClass; i++) {
		retio[i] = (double)sum[i] / (double)data.size();
	}
	return true;
}

/// <summary>
/// 解析の実施
/// </summary>
/// <param name="targetID">判定素材ID</param>
/// <param name="AnalysisMethodID">解析方法ID</param>
/// <returns>成功の場合はtrue、失敗の場合はfalseを返す</returns>
bool CWeldEvaluationDoc::Analize(int targetID, int AnalysisMethodID)
{
	int nClass = 1;
	CString ScanDataFilePath, ClassificationDataFilePath;
	////////////////////////////////////////////////////////////////////
	// 解析を実施
	////////////////////////////////////////////////////////////////////
	ScanDataFilePath = getScanDataFilePath(targetID);
	ClassificationDataFilePath = getClassificationDataFilePath(targetID, AnalysisMethodID);
	switch (targetID) {
	case	eResinSurface	:	// 樹脂
		nClass = ResinGetNumberOfClass();
		break;
	case	eMetalSurface	:	// 金属
		nClass = MetalGetNumberOfClass();
		break;
	case	eJoiningResult	:	// 接合結果
		nClass = ResultGetNumberOfClass();
		break;
	}

	CConfigrationIO sys(m_SystemFilePathName);
	CString modulePath = sys.getString(_T("Common"), _T("AIModuleFolder"));
	if (modulePath.IsEmpty()) {
		return false;
	}
	CString returnfile = CFileUtil::FilePathCombine(modulePath, _T("return.txt"));
	CFileUtil::fileDelete(returnfile);

	CString execpath;


#if true
	PROCESS_INFORMATION p;
	STARTUPINFO s;
	ZeroMemory(&s, sizeof(s));
	s.cb = sizeof(s);
#else
	SHELLEXECUTEINFO ShExecInfo;
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = NULL;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = NULL;
	ShExecInfo.lpParameters = NULL;
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_SHOW;
	ShExecInfo.hInstApp = NULL;
#endif
	CString prm;
	switch (AnalysisMethodID) {
	case	AnalizeTypeKMeans:				// k-means
		execpath = CFileUtil::FilePathCombine(modulePath, _T("kmeans.bat"));
		break;
	case	AnalizeTypeHiClustering:		// 階層クラスタリング
		execpath = CFileUtil::FilePathCombine(modulePath, _T("dendrogram.bat"));
		break;
	default:
		return false;
	}

	if (CFileUtil::fileExists(execpath)) {
		prm.Format(_T("%s %s %d"), (LPCTSTR)ScanDataFilePath, (LPCTSTR)ClassificationDataFilePath, nClass);
		CString cmd;
		cmd.Format(_T("%s %s"), (LPCTSTR)execpath, (LPCTSTR)prm);
		TCHAR pszText[1049], pszMpath[1049];
		ZeroMemory(pszText, 1049);
		ZeroMemory(pszMpath, 1049);
		_tcscpy_s(pszText, 1049, cmd);
		_tcscpy_s(pszMpath, 1049, modulePath);
		int ret = CreateProcess(
			NULL,
			pszText,
			NULL,
			NULL,
			FALSE,
			NULL,
			NULL,
			pszMpath,
			&s,
			&p
		);
		if (ret == 0) {
			CloseHandle(p.hProcess);
			return false;
		}
		else {
			CloseHandle(p.hThread);
			WaitForSingleObject(p.hProcess, INFINITE);
			CloseHandle(p.hProcess);
		}
	}
	else {
		return false;
	}


	bool bResult = false;
	int count = 0;
	int TimeOut = 500;
	while (count < TimeOut) {
		if (CFileUtil::fileExists(returnfile)) {
			bResult = true;
			break;
		}
		Sleep(10);
		count++;
	}
	if (!bResult) {
		// タイムアウト
		return false;
	}

	CStdioFile fd;
	UINT nOpenFlags = CFile::modeRead;
	if (fd.Open(returnfile, nOpenFlags))
	{
		CString read;
		if (!fd.ReadString(read)) {
			fd.Close();
			return false;
		}
		int val = _ttol(read);
		fd.Close();
		if (val != 0) {
			return false;
		}
	}
	else {
		return false;
	}

	////////////////////////////////////////////////////////////////////
	//
	// 解析ファイルをプロジェクトへ登録
	//
	////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////
	//
	// 接合率を求める
	//
	////////////////////////////////////////////////////////////////////
	CString path;
	vector<int> data;
	vector<double> retio;
#if 0
	CString ResultFileName = m_ProjectIO.GetResinClassificationResultFile();

	if (targetID == eResinSurface) {
		ResultFileName = m_ProjectIO.GetResinClassificationResultFile();
		nClass = m_PropatyIO.ResinGetNumberOfClass();
	} else if (targetID == eMetalSurface) {
		ResultFileName = m_ProjectIO.GetMetalClassificationResultFile();
		nClass = m_PropatyIO.MetalGetNumberOfClass();
	}

	path = CFileUtil::FilePathCombine(m_ActiveRegisttedTestFolder,ResultFileName);
#else
//	path = getClassificationDataFilePath(targetID, AnalysisMethodID);
//	if (targetID == eResinSurface) {
//		nClass = m_PropatyIO.ResinGetNumberOfClass();
//	}
//	else if (targetID == eMetalSurface) {
//		nClass = m_PropatyIO.MetalGetNumberOfClass();
//	}
//	else if (targetID == eJoiningResult) {
//		nClass = m_PropatyIO.ResultGetNumberOfClass();
//	}
#endif
	if (!CWeldEvaluationDoc::getResultFile(path, data)) {
		return false;
	}
	else {
		if (CalcJoJointRetio(data, nClass, retio)) {
		}
		else {
			return false;
		}
	}

	if (targetID == eResinSurface) {
		for (int ViewJointRatioNo = 0; ViewJointRatioNo < retio.size(); ViewJointRatioNo++) {
			if (!ResinSetJointRetio(ViewJointRatioNo,retio[ViewJointRatioNo]*100.0)) {
				bResult = false;
			}
		}
	} else if (targetID == eMetalSurface) {
		for (int ViewJointRatioNo = 0; ViewJointRatioNo < retio.size(); ViewJointRatioNo++) {
			if (!MetalSetJointRetio(ViewJointRatioNo,retio[ViewJointRatioNo]*100.0)) {
				bResult = false;
			}
		}
	} else if (targetID == eMetalSurface) {
		for (int ViewJointRatioNo = 0; ViewJointRatioNo < retio.size(); ViewJointRatioNo++) {
			if (!ResultSetJointRetio(ViewJointRatioNo, retio[ViewJointRatioNo] * 100.0)) {
				bResult = false;
			}
		}
	}
	return bResult;
}

/// <summary>
/// スキャンデータイメージの取得
/// </summary>
/// <param name="ScanID">対象スキャンID</param>
/// <param name="img">取得イメージ</param>
/// <param name="renew">再読込フラグ</param>
/// <returns>成功の場合はtrue、失敗の場合はfalseを返す</returns>
bool CWeldEvaluationDoc::LoadScanImage(int ScanID, CImage &img, bool renew/* = false*/)
{
	CString root,name;
	CString scanfile,imagefile;

	scanfile = getScanDataFilePath(ScanID);
	if (!CFileUtil::fileExists(scanfile)) {
		return false;
	}

	imagefile = getScanImageFilePath(ScanID);
	if (!CFileUtil::fileExists(imagefile)) {
		CScanDataIO *pSdio = nullptr;

		int width, height, nBand;
		switch (ScanID) {
		case	eResinSurface:	///< 樹脂
			pSdio = &m_ResinScanData;
			break;
		case	eMetalSurface:	///< 金属
			pSdio = &m_MetalScanData;
			break;
		case	eJoiningResult:	///< 接合結果
			pSdio = &m_ResultScanData;
			break;
		default:
			return false;
		}
		if (pSdio->open(scanfile)) {
			if (!pSdio->LoadImage(height, width, nBand, img)) {
				return false;
			}
			img.Save(imagefile);
		}
		else {
			return false;
		}
	}
	else {
		// jpegからCBitmapに変換
		HRESULT ret;
		ret = img.Load(imagefile);
		if (FAILED(ret)) {
			return false;
		}
	}
	return true;
}

void CWeldEvaluationDoc::H2RGB(int h, BYTE &r, BYTE &g, BYTE &b)
{
	double min = 0.;
	double max = 255.;
	if (h < 0) {
		h = h % 360;
		h = 360 + h;
	}
	if (h > 360) {
		h = h % 360;
	}
	double dh = (double)h;

	if (dh <= 60.) {
		r = (BYTE)max;
		g = (BYTE)((dh / 60.) * (max - min) + min);
		b = (BYTE)min;
	}
	else if (dh <= 120.) {
		r = (BYTE)(((120. - dh) / 60.) * (max - min) + min);
		g = (BYTE)max;
		b = (BYTE)min;
	}
	else if (dh <= 180.) {
		r = (BYTE)min;
		g = (BYTE)max;
		b = (BYTE)(((dh -120.) / 60.) * (max - min) + min);
	}
	else if (dh <= 240.) {
		r = (BYTE)min;
		g = (BYTE)(((240. - dh) / 60.) * (max - min) + min);
		b = (BYTE)max;
	}
	else if (dh <= 300.) {
		r = (BYTE)(((dh - 240.) / 60.) * (max - min) + min);
		g = (BYTE)min;
		b = (BYTE)max;
	}
	else if (dh <= 360.) {
		r = (BYTE)max;
		g = (BYTE)min;
		b = (BYTE)(((360. - dh) / 60.) * (max - min) + min);
	}
}

bool CWeldEvaluationDoc::LoadClassificationResultImage(int targetID, int type, CImage &img, bool renew/* = false*/)
{
	bool bResult = true;
	CString root, name;
	CString classfile, imagefile;
	int nClass = 0;

	classfile = getClassificationDataFilePath(targetID,type);
	if (!CFileUtil::fileExists(classfile)) {
		return false;
	}
	imagefile = getClassificationImageFilePath(targetID,type);
	if (!CFileUtil::fileExists(imagefile)) {
		vector<int> data;
		if (!CWeldEvaluationDoc::getResultFile(classfile, data)) {
			return false;
		}
		else {
			switch (targetID) {
			case	eResinSurface:	///< 樹脂
				nClass = m_PropatyIO.ResinGetNumberOfClass();
				break;
			case	eMetalSurface:	///< 金属
				nClass = m_PropatyIO.MetalGetNumberOfClass();
				break;
			case	eJoiningResult:	///< 接合結果
				nClass = m_PropatyIO.ResultGetNumberOfClass();
				break;
			default:
				return false;
			}

			int Bpp = 3;
			int width = m_PropatyIO.GetHorizontalResolution();
			int height = m_PropatyIO.GetVerticalResolution();

			if (data.size() != (width * height)) {
				return false;
			}
			unsigned char * p24Img = new unsigned char[width * height * Bpp];
			BYTE *ptr = p24Img;
			unsigned char r, g, b;
#if true
			COLORREF *col = new COLORREF[nClass];
			for (int id = 0; id < nClass; id++) {
				int h = id * (240 / (nClass - 1));
				H2RGB(h, r, g, b);
				col[id] = RGB(r, g, b);
			}

			switch (targetID) {
			case	eResinSurface:	///< 樹脂
				for (int id = 0; id < nClass; id++) {
					ResinSetJointColor(id, col[id]);
				}
				break;
			case	eMetalSurface:	///< 金属
				for (int id = 0; id < nClass; id++) {
					ResinSetJointColor(id, col[id]);
				}
				break;
			case	eJoiningResult:	///< 接合結果
				for (int id = 0; id < nClass; id++) {
					ResinSetJointColor(id, col[id]);
				}
				break;
			}

			for (int id = 0; id < data.size(); id++) {
				*(ptr++) = GetBValue(col[data[id]]);
				*(ptr++) = GetGValue(col[data[id]]);
				*(ptr++) = GetRValue(col[data[id]]);
			}
#else
			for (int id = 0; id < data.size(); id++) {
				int h = data[id] * (240 / (nClass - 1));
				H2RGB(h, r, g, b);
				*(ptr++) = b;
				*(ptr++) = g;
				*(ptr++) = r;
			}
#endif


			BITMAPINFOHEADER    bmInfohdr;
			// Create the header info
			bmInfohdr.biSize = sizeof(BITMAPINFOHEADER);
			bmInfohdr.biWidth = width;
			bmInfohdr.biHeight = -height;
			bmInfohdr.biPlanes = 1;
			bmInfohdr.biBitCount = (WORD)(Bpp * 8);
			bmInfohdr.biCompression = BI_RGB;
			bmInfohdr.biSizeImage = width * height * Bpp;
			bmInfohdr.biXPelsPerMeter = 0;
			bmInfohdr.biYPelsPerMeter = 0;
			bmInfohdr.biClrUsed = 0;
			bmInfohdr.biClrImportant = 0;

			BITMAPINFO bmInfo;
			bmInfo.bmiHeader = bmInfohdr;
			bmInfo.bmiColors[0].rgbBlue = 255;

			if (img.Create(width, height, 8 * Bpp, NULL)) {
				HDC dc = img.GetDC();
				SetDIBitsToDevice(dc, 0, 0, width, height, 0, 0, 0, height, p24Img, &bmInfo, DIB_RGB_COLORS);
				img.ReleaseDC();
			}
			else {
				bResult = false;
			}

			if (p24Img) {
				delete[] p24Img;
				p24Img = nullptr;
			}
			img.Save(imagefile);
		}
	}
	else {
		HRESULT ret;
		ret = img.Load(imagefile);
		if (FAILED(ret)) {
			return false;
		}
	}

	return bResult;
}

bool CWeldEvaluationDoc::GetSpectrumData(int ScanID, CPoint &pos, std::vector<double> &data)
{
	CString root, name;
	CString scanfile, imagefile;

	root = m_ProjectIO.GetImageDataRootPath();
	switch (ScanID) {
	case	eResinSurface:	///< 樹脂
		if (!m_ResinScanData.GetSpectrumData(pos, data)) {
			return false;
		}
		break;
	case	eMetalSurface:	///< 金属
		if (!m_MetalScanData.GetSpectrumData(pos, data)) {
			return false;
		}
		break;
	case	eJoiningResult:	///< 接合結果
		if (!m_ResultScanData.GetSpectrumData(pos, data)) {
			return false;
		}
		break;
	default:
		return false;
	}
	return true;
}

bool CWeldEvaluationDoc::WriteImage(CString writePath)
{
	int ScanID[3] = { eResinSurface ,eMetalSurface , eJoiningResult};
	int type[2] = { AnalizeTypeKMeans, AnalizeTypeHiClustering };

	bool bResult = true;
	CString drive, dir, fname, ext;
	CString path,classfile, outPath;
	bool bexist = true;
	for (int ID = 0; ID < 3; ID++) {
		if (ExistScanFile(ScanID[ID])) {
			bexist = true;
			path = getScanImageFilePath(ScanID[ID]);
			if (!CFileUtil::fileExists(path)) {
				CImage img;
				if (!LoadScanImage(ScanID[ID], img)) {
					bexist = false;
				}
				else {
					HRESULT iret = img.Save(path);
					if (iret != 0) {
						bexist = false;
					}
				}
			}
			if (bexist) {
				if (CFileUtil::splitPath(path, drive, dir, fname, ext)) {
					fname.Format(_T("%d_%s%s"), ScanID[ID], (LPCTSTR)fname, (LPCTSTR)ext);
					outPath = CFileUtil::FilePathCombine(writePath, fname);
					if (!CopyFile(path, outPath, true)) {
						bResult = false;
					}
				}
				else {
					bResult = false;
				}
			}

			for (int TypeID = 0; TypeID < 2; TypeID++) {
				// 結果画像は解析したもののみ表示
				classfile = getClassificationDataFilePath(ScanID[ID], type[TypeID]);
				if (CFileUtil::fileExists(classfile)) {
					path = getClassificationImageFilePath(ScanID[ID], type[TypeID]);
					bexist = true;
					if (!CFileUtil::fileExists(path)) {
						CImage img;
						if (!LoadClassificationResultImage(ScanID[ID], type[TypeID], img)) {
							bexist = false;
						}
						else {
							HRESULT iret = img.Save(path);
							if (iret != 0) {
								bexist = false;
							}
						}
					}

					if (bexist) {
						if (CFileUtil::splitPath(path, drive, dir, fname, ext)) {
							fname.Format(_T("%d_%d_%s%s"), ScanID[ID], type[TypeID], (LPCTSTR)fname, (LPCTSTR)ext);
							outPath = CFileUtil::FilePathCombine(writePath, fname);
							if (!CopyFile(path, outPath, true)) {
								bResult = false;
							}
						}
						else {
							bResult = false;
						}
					}
				}
			}
		}
	}
	return bResult;
}
