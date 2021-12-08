
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

#define StatysFileName _T("__Status__.sys")
#define NewProjectFolder _T("NewProject")

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
	m_bVisible					= false;
	m_bInitialized				= false;
	m_minRect					= CSize(0, 0);
	m_CalcResultStopRequest		= false;
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
/// ログ出力
/// </summary>
/// <param name="level">ログレベル</param>
/// <param name="filePath">エラーファイル</param>
/// <param name="lineNo">エラー行</param>
/// <param name="msg">ログメッセージ</param>
void CWeldEvaluationDoc::writeLog(CLog::LOGLEVEL level, CString filePath, long lineNo, CString msg)
{
	CLog log;
	CString ErrMsg;
	ErrMsg.Format(_T(" File:%s Line:%ld:%s"), (LPCTSTR)filePath, lineNo, (LPCTSTR)msg);
	log.logWrite(level, ErrMsg);
}

/// <summary>
/// ビジブルフラグ設定
/// </summary>
/// <param name="bVisible">状態</param>
void CWeldEvaluationDoc::SetVisible(bool bVisible)
{
	m_bVisible = bVisible;
}

/// <summary>
/// 新規プロジェクト作成
/// </summary>
/// <returns>成功した場合はTRUE、失敗した場合はFALSEを返す</returns>
BOOL CWeldEvaluationDoc::OnNewDocument()
{
	if (IsWorkProjectUpdated()) {
		CString msg,fmt;
		if (!m_bVisible) {
			CString rootPath = GetWorkProjectFolderPath();
			CString prjName = GetWorkProjectStatus(rootPath, _T("ProjectName"));
			if (!fmt.LoadString(IDS_EXIST_REPAIRABLEDATA)) {
				fmt = _T("修復可能な登録データ[%s]が存在します。\n修復しますか？");
			}
			msg.Format(fmt, (LPCTSTR)prjName);
			if (AfxMessageBox(msg, MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON1) != IDNO) {
				CString resistFolder = GetRegistedFolder();
				if (!PopProject(resistFolder, prjName)) {
					if (!msg.LoadString(IDM_ERR_FAIL_REGIST)) {
						msg = _T("修復に失敗完了しました。");
					}
					AfxMessageBox(msg, MB_ICONSTOP);
				}
				else {
					if (!msg.LoadString(IDS_REPAIR_COMPLEAT)) {
						msg = _T("修復が完了しました。");
					}
					AfxMessageBox(msg, MB_ICONINFORMATION);
				}
			}
		}
	}

	if (!CDocument::OnNewDocument()) {
		CString msg;
		msg.Format(_T("新規プロジェクトの作成に失敗しました。"));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return FALSE;
	}

	if (!NewProject()) {
		CString msg;
		msg.Format(_T("新規プロジェクトの作成に失敗しました。"));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return FALSE;
	}
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
		CString msg;
		msg.Format(_T("プロジェクト名の設定の設定に失敗しました。"));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
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
/// 縦方向の解像度の取得
/// </summary>
/// <returns>縦方向の解像度を返す</returns>
UINT CWeldEvaluationDoc::GetVerticalResolution(void)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		CConfigrationIO sys(m_SystemFilePathName);
		return sys.getInt(_T("ParamDefault"), _T("Vertical_resolution"));
	}
	else {
		UINT val = m_PropatyIO.GetVerticalResolution();
		if (val == 0) {
			CConfigrationIO sys(m_SystemFilePathName);
			val = sys.getInt(_T("ParamDefault"), _T("Vertical_resolution"));
		}
		return val;
	}
}

/// <summary>
/// 縦方向の解像度の設定
/// </summary>
/// <param name="VerticalResolution">縦方向の解像度</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CWeldEvaluationDoc::SetVerticalResolution(UINT VerticalResolution)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		CString msg;
		msg.Format(_T("縦方向の解像度の設定に失敗しました。"));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}
	else {
		return m_PropatyIO.SetVerticalResolution(VerticalResolution);
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
		return sys.getInt(_T("ParamDefault"), _T("Horizontal_resolution"));
	}
	else {
		UINT val = m_PropatyIO.GetHorizontalResolution();
		if (val == 0) {
			CConfigrationIO sys(m_SystemFilePathName);
			val = sys.getInt(_T("ParamDefault"), _T("Horizontal_resolution"));
		}
		return val;
	}
}

/// <summary>
/// 横方向の解像度の設定
/// </summary>
/// <param name="HorizontalResolution">横方向の解像度</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CWeldEvaluationDoc::SetHorizontalResolution(UINT HorizontalResolution)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		CString msg;
		msg.Format(_T("横方向の解像度の設定に失敗しました。"));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}
	else {
		return m_PropatyIO.SetHorizontalResolution(HorizontalResolution);
	}
}

/// <summary>
/// 重なりピクセル数の取得
/// </summary>
/// <returns>重なりピクセル数を返す</returns>
UINT CWeldEvaluationDoc::NumberOfOverridePixel(void)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		CConfigrationIO sys(m_SystemFilePathName);
		return sys.getInt(_T("ParamDefault"), _T("Number_of_overlapping_pixels"));
	}
	else {
		UINT val = m_PropatyIO.NumberOfOverridePixel();
		if (val == 0) {
			CConfigrationIO sys(m_SystemFilePathName);
			val = sys.getInt(_T("ParamDefault"), _T("Number_of_overlapping_pixels"));
		}
		return val;
	}
	bool SetOverridePixelNumber(UINT num);
}

/// <summary>
/// 重なりピクセル数の設定
/// </summary>
/// <param name="num">重なりピクセル数</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CWeldEvaluationDoc::SetOverridePixelNumber(UINT num)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		CString msg;
		msg.Format(_T("重なりピクセル数の設定に失敗しました。"));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}
	else {
		return m_PropatyIO.SetOverridePixelNumber(num);
	}
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
		CString TestName = m_PropatyIO.GetTestName();
		if (TestName.IsEmpty()) {
			CConfigrationIO sys(m_SystemFilePathName);
			TestName = sys.getString(_T("ParamDefault"), _T("Test_name"));
		}
		return TestName;
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
			CString msg;
			msg.Format(_T("試験名の設定に失敗しました。(name=[%s])"), static_cast<LPCWSTR>(name));
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
			return false;
		}
	}
	return m_PropatyIO.SetTestName(name);
}

/// <summary>
/// 樹脂面の分類数の取得
/// </summary>
/// <param name="method">解析方法</param>
/// <returns>樹脂面の分類数を返す</returns>
UINT CWeldEvaluationDoc::ResinGetNumberOfClass(int method)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		CConfigrationIO sys(m_SystemFilePathName);
		return sys.getInt(_T("ParamDefault"),_T("ResinSurface_Number_of_classifications"));
	} else {
		UINT val = m_PropatyIO.ResinGetNumberOfClass(method);
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
/// <param name="method">解析方法</param>
/// <param name="nClass">樹脂面の分類数</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CWeldEvaluationDoc::ResinSetNumberOfClass(int method, UINT nClass)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		CString msg;
		msg.Format(_T("樹脂面の分類数の設定に失敗しました。(method=%d, nClass=%d)"), method, nClass);
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	} else {
		return m_PropatyIO.ResinSetNumberOfClass(method,nClass);
	}
}

/// <summary>
/// 樹脂面の接合割合の取得
/// </summary>
/// <param name="method">解析方法</param>
/// <param name="ViewJointRatioNo">接合面番号</param>
/// <returns>樹脂面の接合割合を返す</returns>
double CWeldEvaluationDoc::ResinGetJointRetio(int method, int ViewJointRatioNo)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		CConfigrationIO sys(m_SystemFilePathName);
		return sys.getDouble(_T("ParamDefault"),_T("ResinSurface_Joining_ratio"));
	} else {
		double val = m_PropatyIO.ResinGetJointRetio(method,ViewJointRatioNo);
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
/// <param name="method">解析方法</param>
/// <param name="ViewJointRatioNo">接合面番号</param>
/// <param name="retio">樹脂面の接合割合</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CWeldEvaluationDoc::ResinSetJointRetio(int method, int ViewJointRatioNo, double retio)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		CString msg;
		msg.Format(_T("樹脂面の接合割合の設定に失敗しました。(method=%d, no=%d, retio=%lf)"), method, ViewJointRatioNo, retio);
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	} else {
		return m_PropatyIO.ResinSetJointRetio(method,ViewJointRatioNo, retio);
	}
}

/// <summary>
/// 樹脂面の接合色の取得
/// </summary>
/// <param name="method">解析方法</param>
/// <param name="ViewJointRatioNo">接合面番号</param>
/// <returns>樹脂面の接合色を返す</returns>
COLORREF CWeldEvaluationDoc::ResinGetJointColor(int method, int ViewJointRatioNo)
{
	COLORREF color = RGB(0, 0, 0);
	if (!m_ActiveRegisttedTestName.IsEmpty()) {
		color = m_PropatyIO.ResinGetJointColor(method,ViewJointRatioNo);
	}
	return color;
}

/// <summary>
/// 樹脂面の接合色の設定
/// </summary>
/// <param name="method">解析方法</param>
/// <param name="ViewJointRatioNo">接合面番号</param>
/// <param name="color">設定色</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CWeldEvaluationDoc::ResinSetJointColor(int method, int ViewJointRatioNo, COLORREF color)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		CString msg;
		msg.Format(_T("樹脂面の接合色の設定に失敗しました。(method=%d, no=%d, color=%ld)"), method, ViewJointRatioNo, color);
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}
	else {
		return m_PropatyIO.ResinSetJointColor(method, ViewJointRatioNo, color);
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
		CString msg;
		msg.Format(_T("樹脂面の解析方法の設定に失敗しました。(method=%d)"), method);
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	} else {
		return m_PropatyIO.ResinSetAnalysisMethod(method);
	}
}

/// <summary>
/// 金属面の分類数の取得
/// </summary>
/// <param name="method">解析方法</param>
/// <returns>金属面の分類数を返す</returns>
UINT CWeldEvaluationDoc::MetalGetNumberOfClass(int method)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		CConfigrationIO sys(m_SystemFilePathName);
		return sys.getInt(_T("ParamDefault"),_T("MetalSurface_Number_of_classifications"));
	} else {
		UINT val = m_PropatyIO.MetalGetNumberOfClass(method);
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
/// <param name="method">解析方法</param>
/// <param name="nClass">金属面の分類数</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CWeldEvaluationDoc::MetalSetNumberOfClass(int method, UINT nClass)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		CString msg;
		msg.Format(_T("金属面の分類数の設定に失敗しました。(method=%d, nClass=%d)"), method, nClass);
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	} else {
		return m_PropatyIO.MetalSetNumberOfClass(method, nClass);
	}
}

/// <summary>
/// 金属面の接合割合の取得
/// </summary>
/// <param name="method">解析方法</param>
/// <param name="ViewJointRatioNo">接合面番号</param>
/// <returns>金属面の接合割合を返す</returns>
double CWeldEvaluationDoc::MetalGetJointRetio(int method, int ViewJointRatioNo)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		CConfigrationIO sys(m_SystemFilePathName);
		return sys.getDouble(_T("ParamDefault"),_T("MetalSurface_Joining_ratio"));
	} else {
		double val = m_PropatyIO.MetalGetJointRetio(method,ViewJointRatioNo);
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
/// <param name="method">解析方法</param>
/// <param name="ViewJointRatioNo">接合面番号</param>
/// <param name="retio">金属面の接合割合</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CWeldEvaluationDoc::MetalSetJointRetio(int method, int ViewJointRatioNo,double retio)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		CString msg;
		msg.Format(_T("金属面の接合割合の設定に失敗しました。(method=%d, no=%d, retio=%lf)"), method, ViewJointRatioNo, retio);
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	} else {
		return m_PropatyIO.MetalSetJointRetio(method,ViewJointRatioNo,retio);
	}
}

/// <summary>
/// 金属面の接合色の取得
/// </summary>
/// <param name="method">解析方法</param>
/// <param name="ViewJointRatioNo">接合面番号</param>
/// <returns>金属面の接合色を返す</returns>
COLORREF CWeldEvaluationDoc::MetalGetJointColor(int method, int ViewJointRatioNo)
{
	COLORREF color = RGB(0, 0, 0);
	if (!m_ActiveRegisttedTestName.IsEmpty()) {
		color = m_PropatyIO.MetalGetJointColor(method, ViewJointRatioNo);
	}
	return color;
}

/// <summary>
/// 金属面の接合色の設定
/// </summary>
/// <param name="method">解析方法</param>
/// <param name="ViewJointRatioNo">接合面番号</param>
/// <param name="color">設定色</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CWeldEvaluationDoc::MetalSetJointColor(int method, int ViewJointRatioNo, COLORREF color)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		CString msg;
		msg.Format(_T("金属面の接合色の設定に失敗しました。(method=%d, no=%d, color=%ld)"), method, ViewJointRatioNo, color);
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}
	else {
		return m_PropatyIO.MetalSetJointColor(method, ViewJointRatioNo, color);
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
		CString msg;
		msg.Format(_T("金属面の解析方法の設定に失敗しました。(method=%d)"), method);
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	} else {
		return m_PropatyIO.MetalSetAnalysisMethod(method);
	}
}

/// <summary>
/// 接合結果の分類数の取得
/// </summary>
/// <param name="method">解析方法</param>
/// <returns>接合結果の分類数を返す</returns>
UINT CWeldEvaluationDoc::ResultGetNumberOfClass(int method)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		CConfigrationIO sys(m_SystemFilePathName);
		return sys.getInt(_T("ParamDefault"),_T("JoiningResult_Number_of_classifications"));
	} else {
		UINT val = m_PropatyIO.ResultGetNumberOfClass(method);
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
/// <param name="method">解析方法</param>
/// <param name="nClass">接合結果の分類数</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CWeldEvaluationDoc::ResultSetNumberOfClass(int method, UINT nClass)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		CString msg;
		msg.Format(_T("接合結果の分類数の設定に失敗しました。(method=%d, nClass=%d)"), method, nClass);
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	} else {
		return m_PropatyIO.ResultSetNumberOfClass(method,nClass);
	}
}

/// <summary>
/// 接合結果の接合割合の取得
/// </summary>
/// <param name="method">解析方法</param>
/// <param name="ViewJointRatioNo">接合面番号</param>
/// <returns>接合結果の接合割合を返す</returns>
double CWeldEvaluationDoc::ResultGetJointRetio(int method, int ViewJointRatioNo)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		CConfigrationIO sys(m_SystemFilePathName);
		return sys.getDouble(_T("ParamDefault"),_T("JoiningResult_Joining_ratio"));
	} else {
		double val = m_PropatyIO.ResultGetJointRetio(method, ViewJointRatioNo);
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
/// <param name="method">解析方法</param>
/// <param name="ViewJointRatioNo">接合面番号</param>
/// <param name="retio">接合結果の接合割合</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CWeldEvaluationDoc::ResultSetJointRetio(int method, int ViewJointRatioNo, double retio)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		CString msg;
		msg.Format(_T("接合結果の接合割合の設定に失敗しました。(method=%d, no=%d, retio=%lf)"), method, ViewJointRatioNo, retio);
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	} else {
		return m_PropatyIO.ResultSetJointRetio(method, ViewJointRatioNo, retio);
	}
}

/// <summary>
/// 接合結果の接合色の取得
/// </summary>
/// <param name="method">解析方法</param>
/// <param name="ViewJointRatioNo">接合面番号</param>
/// <returns>接合結果の接合色を返す</returns>
COLORREF CWeldEvaluationDoc::ResultGetJointColor(int method, int ViewJointRatioNo)
{
	COLORREF color = RGB(0, 0, 0);
	if (!m_ActiveRegisttedTestName.IsEmpty()) {
		color = m_PropatyIO.ResultGetJointColor(method, ViewJointRatioNo);
	}
	return color;
}

/// <summary>
/// 接合結果の接合色の設定
/// </summary>
/// <param name="method">解析方法</param>
/// <param name="ViewJointRatioNo">接合面番号</param>
/// <param name="color">設定色</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CWeldEvaluationDoc::ResultSetJointColor(int method, int ViewJointRatioNo, COLORREF color)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		CString msg;
		msg.Format(_T("接合結果の接合色の設定に失敗しました。(method=%d, no=%d, color=%ld)"), method, ViewJointRatioNo, color);
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}
	else {
		return m_PropatyIO.ResultSetJointColor(method, ViewJointRatioNo, color);
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
	case	eResinSurface	:	// 樹脂
		AnalizeMode = m_PropatyIO.ResinGetAnalysisMethod();
		break;
	case	eMetalSurface	:	// 金属
		AnalizeMode = m_PropatyIO.MetalGetAnalysisMethod();
		break;
	case	eJoiningResult:		// 接合結果
		AnalizeMode = m_PropatyIO.ResultGetAnalysisMethod();
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
	case	eResinSurface	:	// 樹脂
		bResult = m_PropatyIO.ResinSetAnalysisMethod(AnalysisMethodID);
		break;
	case	eMetalSurface	:	// 金属
		bResult = m_PropatyIO.MetalSetAnalysisMethod(AnalysisMethodID);
		break;
	case	eJoiningResult:		// 接合結果
		bResult = m_PropatyIO.ResultSetAnalysisMethod(AnalysisMethodID);
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
/// プロジェクトからのスキャンイメージファイルパス取得
/// </summary>
/// <param name="fileID">ファイル種別ID</param>
/// <returns>存在する場合はパスを返す、失敗した場合は空文字を返す</returns>
CString CWeldEvaluationDoc::GetScanImagePath(int fileID)
{
	return getScanImageFilePath(fileID);
}

/// <summary>
/// プロジェクトへの分類結果データファイルパス取得
/// </summary>
/// <param name="fileID">ファイル種別ID</param>
/// <param name="method">解析方法</param>
/// <returns>存在する場合はパスを返す、失敗した場合は空文字を返す</returns>
CString CWeldEvaluationDoc::GetClassificationResultPath(int fileID, int method)
{
	return getClassificationDataFilePath(fileID, method);
}

/// <summary>
/// プロジェクトからのスペクトルグラフ（点指定）の対象の取得
/// </summary>
/// <returns>スペクトルグラフ（点指定）の対象を返す</returns>
UINT CWeldEvaluationDoc::GetSpectralGraphPointTarget()
{
	return m_ProjectIO.GetSpectralGraphPointTarget();
}

/// <summary>
/// プロジェクトへのスペクトルグラフ（点指定）の対象の設定
/// </summary>
/// <param name="targetID">スペクトルグラフ（点指定）の対象</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CWeldEvaluationDoc::SetSpectralGraphPointTarget(UINT targetID)
{
	return m_ProjectIO.SetSpectralGraphPointTarget(targetID);
}

/// <summary>
/// プロジェクトからのスペクトルグラフ（点指定）の位置の取得
/// </summary>
/// <param name="holizontal">水平値</param>
/// <param name="vertical">垂直値</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CWeldEvaluationDoc::GetSpectralGraphPointPosition(int &holizontal, int &vertical)
{
	return m_ProjectIO.GetSpectralGraphPointPosition(holizontal, vertical);
}

/// <summary>
/// プロジェクトへのスペクトルグラフ（点指定）の位置の設定
/// </summary>
/// <param name="holizontal">水平値</param>
/// <param name="vertical">垂直値</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CWeldEvaluationDoc::SetSpectralGraphPointPosition(int holizontal, int vertical)
{
	return m_ProjectIO.SetSpectralGraphPointPosition(holizontal, vertical);
}


/// <summary>
/// プロジェクトからのスペクトルグラフ（区間指定）の対象の取得
/// </summary>
/// <returns>スペクトルグラフ（区間指定）の対象を返す</returns>
UINT CWeldEvaluationDoc::GetSpectralGraphSectionTarget()
{
	return m_ProjectIO.GetSpectralGraphSectionTarget();
}

/// <summary>
/// プロジェクトへのスペクトルグラフ（区間指定）の対象の設定
/// </summary>
/// <param name="targetID">スペクトルグラフ（区間指定）の対象</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CWeldEvaluationDoc::SetSpectralGraphSectionTarget(UINT targetID)
{
	return m_ProjectIO.SetSpectralGraphSectionTarget(targetID);
}

/// <summary>
/// プロジェクトからのスペクトルグラフ（区間指定）の開始位置の取得
/// </summary>
/// <param name="holizontal">水平値</param>
/// <param name="vertical">垂直値</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CWeldEvaluationDoc::GetSpectralGraphSectionStartPosition(int &holizontal, int &vertical)
{
	return m_ProjectIO.GetSpectralGraphSectionStartPosition(holizontal, vertical);
}

/// <summary>
/// プロジェクトへのスペクトルグラフ（区間指定）の開始位置の設定
/// </summary>
/// <param name="holizontal">水平値</param>
/// <param name="vertical">垂直値</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CWeldEvaluationDoc::SetSpectralGraphSectionStartPosition(int holizontal, int vertical)
{
	return m_ProjectIO.SetSpectralGraphSectionStartPosition(holizontal, vertical);
}

/// <summary>
/// プロジェクトからのスペクトルグラフ（区間指定）の終了位置の取得
/// </summary>
/// <param name="holizontal">水平値</param>
/// <param name="vertical">垂直値</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CWeldEvaluationDoc::GetSpectralGraphSectionEndPosition(int &holizontal, int &vertical)
{
	return m_ProjectIO.GetSpectralGraphSectionEndPosition(holizontal, vertical);
}

/// <summary>
/// プロジェクトへのスペクトルグラフ（区間指定）の終了位置の設定
/// </summary>
/// <param name="holizontal">水平値</param>
/// <param name="vertical">垂直値</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CWeldEvaluationDoc::SetSpectralGraphSectionEndPosition(int holizontal, int vertical)
{
	return m_ProjectIO.SetSpectralGraphSectionEndPosition(holizontal, vertical);
}

/// <summary>
/// プロジェクトからスキャンデータの縦横サイズの取得
/// </summary>
/// <param name="holizontal">水平値</param>
/// <param name="vertical">垂直値</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CWeldEvaluationDoc::GetScanDataSize(int &holizontal, int &vertical)
{
	return m_ProjectIO.GetScanDataSize(holizontal, vertical);
}

/// <summary>
/// プロジェクトへのスキャンデータの縦横サイズの設定
/// </summary>
/// <param name="holizontal">水平値</param>
/// <param name="vertical">垂直値</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CWeldEvaluationDoc::SetScanDataSize(int holizontal, int vertical)
{
	return m_ProjectIO.SetScanDataSize(holizontal, vertical);
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
	if (!ParmaterFileName.LoadString(IDS_PROPATYFILENAME)) {
		ParmaterFileName = _T("parameter.inf");
	}
	CString projectFileName;
	if (!projectFileName.LoadString(IDS_PROJECTFILENAME)) {
		projectFileName = _T("WeldEvalution.prj");
	}
	m_ParamaterFilePaht = CFileUtil::FilePathCombine(m_ActiveRegisttedTestFolder,ParmaterFileName);
	m_ProjectFilePaht = CFileUtil::FilePathCombine(m_ActiveRegisttedTestFolder, projectFileName);

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
		name = name + _T(".hdr");
		CString root = GetWorkProjectPath();
//		CString root = m_ProjectIO.GetImageDataRootPath();
		CString path = CFileUtil::FilePathCombine(root, name);
		bool bExist = CFileUtil::fileExists(path);

		bResult = bExist;
	}
	return bResult;
}

/// <summary>
/// スキャンファイルパス取得（拡張子なし）
/// </summary>
/// <param name="ScanID">対象スキャンID</param>
/// <returns>存在する場合はパスを返す、失敗した場合は空文字を返す</returns>
CString CWeldEvaluationDoc::getScanDataPath(int ScanID)
{
	CString root, name;

	root = GetWorkProjectPath();
//	root = m_ProjectIO.GetImageDataRootPath();
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
/// <param name="ScanID">対象スキャンID</param>
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
/// <param name="ScanID">ファイル種別ID</param>
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
/// <param name="ScanID">対象スキャンID</param>
/// <param name="method">解析方法</param>
/// <returns>存在する場合はパスを返す、失敗した場合は空文字を返す</returns>
CString CWeldEvaluationDoc::getClassificationDataFilePath(int ScanID, int method)
{
	CString root, name;
	CString classfile, imagefile;

	root = GetWorkProjectPath();
//	root = m_ProjectIO.GetImageDataRootPath();
	switch (ScanID) {
	case	eResinSurface:	///< 樹脂
		if (method == AnalizeTypeKMeans) {
			name = m_ProjectIO.GetResinKmeansClassFile();
		}
		else {
			name = m_ProjectIO.GetResinDendrogramClassFile();
		}
		break;
	case	eMetalSurface:	///< 金属
		if (method == AnalizeTypeKMeans) {
			name = m_ProjectIO.GetMetalKmeansClassFile();
		}
		else {
			name = m_ProjectIO.GetMetalDendrogramClassFile();
		}
		break;
	case	eJoiningResult:	///< 接合結果
		if (method == AnalizeTypeKMeans) {
			name = m_ProjectIO.GetResultKmeansClassFile();
		}
		else {
			name = m_ProjectIO.GetResultDendrogramClassFile();
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
/// <param name="ScanID">対象スキャンID</param>
/// <param name="method">解析方法</param>
/// <returns>存在する場合はパスを返す、失敗した場合は空文字を返す</returns>
CString CWeldEvaluationDoc::getClassificationImageFilePath(int ScanID, int method)
{
	CString classfile = getClassificationDataFilePath(ScanID, method);
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
		CString msg;
		msg.Format(_T("メインウインドの位置とサイズの取得に失敗しました。(pos=(%d,%d) width=%d height=%d"), left, top, width, hight);
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
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
		CString msg;
		msg.Format(_T("区間スペクトルダイアログの位置とサイズの取得に失敗しました。(pos=(%d,%d) width=%d height=%d"), left, top, width, hight);
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
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

/// <summary>
/// RGBに対応する波長の取得
/// </summary>
/// <param name="r">赤の波長</param>
/// <param name="g">緑の波長</param>
/// <param name="b">青の波長</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CWeldEvaluationDoc::GetRGBWavelength(double &r, double &g, double &b)
{
	if (!CFileUtil::fileExists(m_SystemFilePathName)) {
		CString msg;
		msg.Format(_T("RGBに対応する波長の取得に失敗しました"));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}
	CConfigrationIO sys(m_SystemFilePathName);
	r = sys.getDouble(_T("System"), _T("RedWavelength"));
	g = sys.getDouble(_T("System"), _T("GreenWavelength"));
	b = sys.getDouble(_T("System"), _T("BlueWavelength"));
	return true;
}

/// <summary>
/// 射影変換用頂点の取得
/// </summary>
/// <param name="vOrignPt">射影元の頂点</param>
/// <param name="vTransPt">射影先の頂点</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
/// @remark 頂点は左上、左した、右下、右上で格納
bool CWeldEvaluationDoc::GetHomographyPoint(vector<CPoint> &vOrignPt, vector<CPoint> &vTransPt)
{
	if (!CFileUtil::fileExists(m_SystemFilePathName)) {
		CString msg;
		msg.Format(_T("射影変換用頂点の取得に失敗しました"));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}
	vOrignPt.resize(4);
	vTransPt.resize(4);
	CConfigrationIO sys(m_SystemFilePathName);
	vOrignPt[0].x = sys.getInt(_T("System"), _T("HomographyOrignTLPtX"));
	vOrignPt[0].y = sys.getInt(_T("System"), _T("HomographyOrignTLPtY"));
	vOrignPt[1].x = sys.getInt(_T("System"), _T("HomographyOrignBLPtX"));
	vOrignPt[1].y = sys.getInt(_T("System"), _T("HomographyOrignBLPtY"));
	vOrignPt[2].x = sys.getInt(_T("System"), _T("HomographyOrignBRPtX"));
	vOrignPt[2].y = sys.getInt(_T("System"), _T("HomographyOrignBRPtY"));
	vOrignPt[3].x = sys.getInt(_T("System"), _T("HomographyOrignTRPtX"));
	vOrignPt[3].y = sys.getInt(_T("System"), _T("HomographyOrignTRPtY"));

	vTransPt[0].x = sys.getInt(_T("System"), _T("HomographyTransTLPtX"));
	vTransPt[0].y = sys.getInt(_T("System"), _T("HomographyTransTLPtY"));
	vTransPt[1].x = sys.getInt(_T("System"), _T("HomographyTransBLPtX"));
	vTransPt[1].y = sys.getInt(_T("System"), _T("HomographyTransBLPtY"));
	vTransPt[2].x = sys.getInt(_T("System"), _T("HomographyTransBRPtX"));
	vTransPt[2].y = sys.getInt(_T("System"), _T("HomographyTransBRPtY"));
	vTransPt[3].x = sys.getInt(_T("System"), _T("HomographyTransTRPtX"));
	vTransPt[3].y = sys.getInt(_T("System"), _T("HomographyTransTRPtY"));

	return true;
}

/// <summary>
/// デバイス名の取得
/// </summary>
/// <returns>デバイス名を返す</returns>
CString CWeldEvaluationDoc::GetDeviceName()
{
	CConfigrationIO sys(m_SystemFilePathName);
	CString name;
	name = sys.getString(_T("System"), _T("DeviceName"), _T(""));
	return name;
}

/// <summary>
/// カメラダミーモジュール使用有無判定
/// </summary>
/// <returns>ダミーモジュールを使用する場合はtrue、そうでない場合はfalseを返す</returns>
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
/// 撮影幅の取得
/// </summary>
/// <returns>撮影幅を返す</returns>
int CWeldEvaluationDoc::GetShootingWidth()
{
	CConfigrationIO sys(m_SystemFilePathName);
	int width = sys.getInt(_T("System"), _T("ShootingWidth"));
	if (width == 0) {
		CString msg;
		msg.Format(_T("撮影幅の取得の取得に失敗しました。デフォルトの撮影幅[%d]を使用します。"),width);
		writeLog(CLog::Warning, CString(__FILE__), __LINE__, msg);
		return 2048;
	}
	else {
		return width;
	}
}

/// <summary>
/// 撮影高さの取得
/// </summary>
/// <returns>撮影高さを返す</returns>
int CWeldEvaluationDoc::GetShootingHeight()
{
	CConfigrationIO sys(m_SystemFilePathName);
	int height = sys.getInt(_T("System"), _T("ShootingHeight"));
	if (height == 0) {
		CString msg;
		msg.Format(_T("撮影高さの取得の取得に失敗しました。デフォルトの撮影高さ[%d]を使用します。"), height);
		writeLog(CLog::Warning, CString(__FILE__), __LINE__, msg);
		return 1088;
	}
	else {
		return height;
	}
}

/// <summary>
/// バンド数の取得
/// </summary>
/// <returns>バンド数を返す</returns>
UINT CWeldEvaluationDoc::NumberOfBand(void)
{
	if (!CFileUtil::fileExists(m_SystemFilePathName)) {
		CString msg;
		msg.Format(_T("バンド数の取得に失敗しました。"));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return 0;
	}
	CConfigrationIO sys(m_SystemFilePathName);
	return sys.getInt(_T("System"), _T("Number_of_band"));
}

/// <summary>
/// バンド数の設定
/// </summary>
/// <param name="band">バンド数</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CWeldEvaluationDoc::SetNumberOfBand(UINT band)
{
	if (!CFileUtil::fileExists(m_SystemFilePathName)) {
		CString msg;
		msg.Format(_T("バンド数の設定に失敗しました。(バンド数=%d)"),band);
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}
	CConfigrationIO sys(m_SystemFilePathName);
	if (!sys.setInt(_T("Setting"), _T("Number_of_band"), band)) {
		return false;
	}
	return true;
}

/// <summary>
/// Integration_time_msの取得
/// </summary>
/// <returns>Integration_time_msを返す</returns>
double CWeldEvaluationDoc::GetIntegrationTimeMs(void)
{
	if (!CFileUtil::fileExists(m_SystemFilePathName)) {
		CString msg;
		msg.Format(_T("Integration_time_msの取得に失敗しました。"));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return 0;
	}
	CConfigrationIO sys(m_SystemFilePathName);
	return sys.getDouble(_T("System"), _T("Integration_time_ms"));
}

/// <summary>
/// Integration_time_msの設定
/// </summary>
/// <param name="IntegrationTime_ms">Integration_time_ms</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CWeldEvaluationDoc::SetIntegrationTimeMs(double IntegrationTime_ms)
{
	if (!CFileUtil::fileExists(m_SystemFilePathName)) {
		CString msg;
		msg.Format(_T("Integration_time_msの設定に失敗しました。(IntegrationTime_ms=%lf)"), IntegrationTime_ms);
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}
	CConfigrationIO sys(m_SystemFilePathName);
	if (!sys.setDouble(_T("System"), _T("Integration_time_ms"), IntegrationTime_ms)) {
		CString msg;
		msg.Format(_T("Integration_time_msの設定に失敗しました。(IntegrationTime_ms=%lf)"), IntegrationTime_ms);
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}
	return true;
}

/// <summary>
/// 取得データの縦方向拡縮率の取得
/// </summary>
/// <returns>成功場合は拡縮率、失敗場合は0.0を返す</returns>
double CWeldEvaluationDoc::GetVerticalScale()
{
	if (!CFileUtil::fileExists(m_SystemFilePathName)) {
		CString msg;
		msg.Format(_T("取得データの縦方向拡縮率の取得に失敗しました。"));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return 0.0;
	}
	int ShootingHeight = GetShootingHeight();
	int VerticalResolution = GetVerticalResolution();
	double retio;
	if ((ShootingHeight == 0) || (VerticalResolution == 0)) {
		CString msg;
		msg.Format(_T("取得データの縦方向拡縮率の取得に失敗しました。(ShootingHeight=%d, VerticalResolution=%d)"), ShootingHeight, VerticalResolution);
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		retio = 0.0;
	}
	else {
		retio = (double)VerticalResolution / (double)ShootingHeight;
	}
	return retio;
}

/// <summary>
/// 取得データの横方向拡縮率の取得
/// </summary>
/// <returns>成功場合は拡縮率、失敗場合は0.0を返す</returns>
double CWeldEvaluationDoc::GetHorizontalScale()
{
	if (!CFileUtil::fileExists(m_SystemFilePathName)) {
		CString msg;
		msg.Format(_T("取得データの横方向拡縮率の取得に失敗しました。"));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return 0.0;
	}
	int ShootingWidth = GetShootingWidth();
	int HorizontalResolution = GetHorizontalResolution();
	double retio;
	if ((ShootingWidth == 0) || (HorizontalResolution == 0)) {
		CString msg;
		msg.Format(_T("取得データの横方向拡縮率の取得に失敗しました。(ShootingWidth=%d, HorizontalResolution=%d)"), ShootingWidth, HorizontalResolution);
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		retio = 0.0;
	}
	else {
		retio = (double)HorizontalResolution / (double)ShootingWidth;
	}
	return retio;
}

/// <summary>
/// ウインドの最小サイズの取得
/// </summary>
/// <param name="size">最小サイズ</param>
/// <returns>最小サイズが確定している場合はtrue、未定の場合はfalseを返す</returns>
bool CWeldEvaluationDoc::GetMinWndSize(CSize &size)
{
	if (!m_bInitialized) {
		size = CSize(0, 0);
		return false;
	}
	else {
		size = m_minRect;
		return true;
	}
}

/// <summary>
/// ウインドの最小サイズの設定
/// </summary>
/// <param name="size">最小サイズ</param>
/// <returns>成功した場合ははtrue、失敗した場合はfalseを返す</returns>
bool CWeldEvaluationDoc::SetMinWndSize(CSize size)
{
	m_bInitialized = true;
	m_minRect = size;
	return true;
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
#if 0
	////////////////////////////////////////////////////////////////
	// 消してね
	// 射影変換の確認用コーディング
	////////////////////////////////////////////////////////////////
	{
		AtlTrace(_T("##### 射影変換の確認 #####\n"));
		double aParam[8], ppt[2];
		CScanDataIO sc;
		vector<CPoint> vOrign,vTrans;
		GetHomographyPoint(vOrign, vTrans);

		double tolerance = 0.000001;
		bool bProjResult = true;
		sc.Calc_ProjectionParam(vOrign, vTrans, aParam);
		for (int i = 0; i < 4; i++) {
			sc.Projection(vOrign[i].x, vOrign[i].y, aParam, ppt[0], ppt[1]);
			if ((fabs(ppt[0] - (double)vTrans[i].x) > tolerance) || (fabs(ppt[1] - (double)vTrans[i].y) > tolerance) ) {
				AtlTrace(_T("FAIL:Orign->Trans:(%.2lf,%2.lf) -> (%.2lf,%2.lf):(%.2lf,%2.lf)\n"), vOrign[i].x, vOrign[i].y, vTrans[i].x, vTrans[i].y, ppt[0], ppt[1]);
				bProjResult = false;
			}
			sc.ProjectionInvPos(vTrans[i].x, vTrans[i].y, aParam, ppt[0], ppt[1]);
			if ((fabs(ppt[0] - (double)vOrign[i].x) > tolerance) || (fabs(ppt[1] - (double)vOrign[i].y) > tolerance)) {
				AtlTrace(_T("FAIL:Trans->Orign:(%.2lf,%2.lf) -> (%.2lf,%2.lf):(%.2lf,%2.lf)\n"), vTrans[i].x, vTrans[i].y, vOrign[i].x, vOrign[i].y, ppt[0], ppt[1]);
				bProjResult = false;
			}
		}
		if (bProjResult) {
			AtlTrace(_T("##### 射影変換:成功 #####\n"));
		}
		else {
			AtlTrace(_T("##### 射影変換:失敗 #####\n"));
		}
	}
	////////////////////////////////////////////////////////////////
#endif

	ClrWorkProject();

	m_ResinScanData.close();
	m_MetalScanData.close();
	m_ResultScanData.close();

	CString tmpFolder = GetRegistedFolder();
	PushProject(tmpFolder,_T(""));
	CString WrokPrjectPath = GetWorkProjectPath();
	CString version;

	m_ActiveRegisttedTestName = tmpFolder;
	m_ActiveRegisttedTestFolder = WrokPrjectPath;
	if (!CFileUtil::fileExists(m_ActiveRegisttedTestFolder)) {
		if (!CFileUtil::MakeDir(m_ActiveRegisttedTestFolder)) {
			CString msg;
			msg.Format((LPCTSTR)_T("ディレクトリの作成に失敗しました。(folder=%s)"), static_cast<LPCWSTR>(m_ActiveRegisttedTestFolder));
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
			return false;
		}
	}

	CString paramFileName;
	if (!paramFileName.LoadString(IDS_PROPATYFILENAME)) {
		paramFileName = _T("parameter.inf");
	}
	CString projectFileName;
	if (!projectFileName.LoadString(IDS_PROJECTFILENAME)) {
		projectFileName = _T("WeldEvalution.prj");
	}

	m_ParamaterFilePaht = CFileUtil::FilePathCombine(m_ActiveRegisttedTestFolder, paramFileName);
	m_ProjectFilePaht	= CFileUtil::FilePathCombine(m_ActiveRegisttedTestFolder, projectFileName);

	UINT uval;
	CString str;
	double dval;
	//////////////////////////////////////////////////////////
	// パラメータの設定
	m_PropatyIO.SetParamaterFilePath(m_ParamaterFilePaht);

	// Default
	CConfigrationIO sys(m_SystemFilePathName);
	str = sys.getString(_T("ParamDefault"),_T("Test_name"));
	CString prjName;
	{
		COleDateTime date = COleDateTime::GetCurrentTime();
		if (str.IsEmpty()) {
			str = _T("Test");
		}

		CString folder = GetRegistedFolder();
		CString ProjectFileName;
		int i = 1;
		while (true) {
			prjName = m_PropatyIO.MakeProjectName(str, date, i);
			CString PathName = CFileUtil::FilePathCombine(folder, prjName);
			if (!CFileUtil::fileExists(PathName)) {
				break;
			}
			i++;
		}
	}

	if (!version.LoadString(IDS_PARAMETERFILE_VERSION)) {
		version = _T("");
	}
	m_PropatyIO.SetVersion(version);
		
	m_PropatyIO.Save(false);
	m_PropatyIO.SetTestName(prjName);

#if 0
	m_PropatyIO.SetTestName(prjName);
	uval = sys.getInt(_T("ParamDefault"),_T("Number_of_overlapping_pixels"));
	m_PropatyIO.SetOverridePixelNumber(uval);
	uval = sys.getInt(_T("ParamDefault"),_T("Integration_time_ms"));
	m_PropatyIO.SetIntegrationTimeMs(uval);
	uval = sys.getInt(_T("ParamDefault"),_T("Vertical_resolution"));
	m_PropatyIO.SetVerticalResolution(uval);
	uval = sys.getInt(_T("ParamDefault"),_T("Horizontal_resolution"));
	m_PropatyIO.SetHorizontalResolution(uval);
#endif
	uval = sys.getInt(_T("ParamDefault"),_T("ResinSurface_Number_of_classifications"));

	for (int method = 0; method < 2; method++) {
		m_PropatyIO.ResinSetNumberOfClass(method,uval);
		for (int i = 0; i < (int)uval; i++) {
			dval = sys.getDouble(_T("ParamDefault"), _T("ResinSurface_Joining_ratio"));
			m_PropatyIO.ResinSetJointRetio(method, i, dval);
		}
	}
	uval = sys.getInt(_T("ParamDefault"),_T("ResinSurface_Analysis_method"));
	m_PropatyIO.ResinSetAnalysisMethod(uval);

	uval = sys.getInt(_T("ParamDefault"),_T("MetalSurface_Number_of_classifications"));
	for (int method = 0; method < 2; method++) {
		m_PropatyIO.MetalSetNumberOfClass(method,uval);
		for (int i = 0; i < (int)uval; i++) {
			dval = sys.getDouble(_T("ParamDefault"), _T("MetalSurface_Joining_ratio"));
			m_PropatyIO.MetalSetJointRetio(method, i, dval);
		}
	}
	uval = sys.getInt(_T("ParamDefault"),_T("MetalSurface_Analysis_method"));
	m_PropatyIO.MetalSetAnalysisMethod(uval);

	uval = sys.getInt(_T("ParamDefault"),_T("JoiningResult_Number_of_classifications"));
	for (int method = 0; method < 2; method++) {
		m_PropatyIO.ResultSetNumberOfClass(method,uval);
		for (int i = 0; i < (int)uval; i++) {
			dval = sys.getInt(_T("ParamDefault"), _T("JoiningResult_Joining_ratio"));
			m_PropatyIO.ResultSetJointRetio(method,i, dval);
		}
	}

	uval = NumberOfOverridePixel();
	m_PropatyIO.SetOverridePixelNumber(uval);
	uval = GetHorizontalResolution();
	m_PropatyIO.SetHorizontalResolution(uval);
	uval = GetVerticalResolution();
	m_PropatyIO.SetVerticalResolution(uval);

	//////////////////////////////////////////////////////////
	// プロジェクトの設定
	m_ProjectIO.Initialze();
	m_ProjectIO.SetImageDataRootPath(WrokPrjectPath);
	m_ProjectIO.SetProjectFilePath(m_ProjectFilePaht);
	CString name;
	name = GetScanDataName(eResinSurface,_T(""));
	m_ProjectIO.SetResinScanImageFile(name);
	name = GetScanDataName(eMetalSurface, _T(""));
	m_ProjectIO.SetMetalScanImageFile(name);
	name = GetScanDataName(eJoiningResult, _T(""));
	m_ProjectIO.SetResultScanImageFile(name);

	SaveClassificationResultFile(eResinSurface, AnalizeTypeKMeans);			// k-means
	SaveClassificationResultFile(eResinSurface, AnalizeTypeHiClustering);	// 階層クラスタリング

	SaveClassificationResultFile(eMetalSurface, AnalizeTypeKMeans);			// k-means
	SaveClassificationResultFile(eMetalSurface, AnalizeTypeHiClustering);	// 階層クラスタリング

	SaveClassificationResultFile(eJoiningResult, AnalizeTypeKMeans);		// k-means
	SaveClassificationResultFile(eJoiningResult, AnalizeTypeHiClustering);	// 階層クラスタリング

	if (!version.LoadString(IDS_PROJECTFILE_VERSION)) {
		version = _T("");
	}
	m_ProjectIO.SetVersion(version);
	m_ProjectIO.Save(false);

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
	CWaitCursor waitCursol;
	CString PopatyFileName,ProjecName, ProjectFileName;
	if (!PopatyFileName.LoadString(IDS_PROPATYFILENAME)) {
		PopatyFileName = _T("parameter.inf");
	}
	if (!ProjecName.LoadString(IDS_PROJECTFILENAME)) {
		ProjecName = _T("WeldEvalution.prj");
	}

	m_ProjectIO.Initialze();
	m_PropatyIO.Initialze();

	// プロジェクトフォルダ取得
	CString folder = GetRegistedFolder();
	CString RegisttedTestFolder = CFileUtil::FilePathCombine(folder,RegistedTestName);
	if (!CFileUtil::fileExists(RegisttedTestFolder)) {
		CString msg;
		msg.Format((LPCTSTR)_T("プロジェクトフォルダ取得が存在しません。(folder=%s)"), static_cast<LPCWSTR>(RegisttedTestFolder));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}

	if (!PushProject(folder, RegistedTestName)) {
		return false;
	}
	CString WorkProjectPath = GetWorkProjectPath();

	// パラメータ取得
	CString ParamaterFilePaht;
	ParamaterFilePaht = CFileUtil::FilePathCombine(WorkProjectPath,PopatyFileName);
	if (!CFileUtil::fileExists(ParamaterFilePaht)) {
		CString msg;
		msg.Format((LPCTSTR)_T("パラメータファイルが存在しません。(path=%s)"), static_cast<LPCWSTR>(ParamaterFilePaht));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}


	if (!m_PropatyIO.SetParamaterFilePath(ParamaterFilePaht)) {
		m_PropatyIO.SetParamaterFilePath(m_ParamaterFilePaht);
		CString msg;
		msg.Format((LPCTSTR)_T("パラメータファイルの設定に失敗しました。。(path=%s)"), static_cast<LPCWSTR>(ParamaterFilePaht));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}
	if (!m_PropatyIO.Read()) {
		CString msg;
		msg.Format((LPCTSTR)_T("パラメータの読み込みに失敗しました。。(path=%s)"), static_cast<LPCWSTR>(ParamaterFilePaht));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}

	// プロパティ取得
	CString str = m_PropatyIO.GetProjectName();
	if (!str.IsEmpty()) {
		ProjecName = str;
	}
	CString ProjectFilePaht;
//	ProjectFileName.Format(_T("%s.prj"), (LPCWSTR)ProjecName);
	ProjectFilePaht = CFileUtil::FilePathCombine(WorkProjectPath, ProjecName);
	if (!CFileUtil::fileExists(ProjectFilePaht)) {
		m_ProjectIO.SetProjectFilePath(m_ProjectFilePaht);
		CString msg;
		msg.Format((LPCTSTR)_T("プロジェクトファイルが存在しません。(path=%s)"), static_cast<LPCWSTR>(ProjectFilePaht));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}
	if (!m_ProjectIO.SetProjectFilePath(ProjectFilePaht)) {
		m_ProjectIO.SetProjectFilePath(m_ProjectFilePaht);
		CString msg;
		msg.Format((LPCTSTR)_T("プロジェクトファイルの設定に失敗しました。(path=%s)"), static_cast<LPCWSTR>(ProjectFilePaht));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}
	if (!m_ProjectIO.Read()) {
		CString msg;
		msg.Format((LPCTSTR)_T("プロジェクトの読み込みに失敗しました。(path=%s)"), static_cast<LPCWSTR>(ProjectFilePaht));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}

	m_ActiveRegisttedTestName = RegistedTestName;
	m_ActiveRegisttedTestFolder = WorkProjectPath;
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

	ProjectUpdate();

	m_OpenType = 1;
	return true;
}

/// <summary>
/// プロジェクトの更新
/// </summary>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CWeldEvaluationDoc::ProjectUpdate()
{
	CConfigrationIO sys(m_SystemFilePathName);
	CConfigrationIO prm(m_PropatyIO.GetParamaterFilePath());
	CConfigrationIO prj(m_ProjectIO.GetProjectFilePath());

	CString prjVersion, proVersion, fileVersion, key;
	int iVal;
	if (!proVersion.LoadString(IDS_PARAMETERFILE_VERSION)) {
		proVersion = _T("");
	}
	fileVersion = m_PropatyIO.GetVersion();
	if (proVersion.CollateNoCase(fileVersion) != 0) {
		CString AnalizeTypeKMeansKey, AnalizeTypeHiClusteringKey;
		AnalizeTypeKMeansKey.Format(_T("%02d_Number_of_classifications"), AnalizeTypeKMeans);
		AnalizeTypeHiClusteringKey.Format(_T("%02d_Number_of_classifications"), AnalizeTypeHiClustering);

		iVal = prm.getInt(_T("ResinSurface"), _T("Number_of_classifications"));
		if (iVal == 0) {
			iVal = sys.getInt(_T("ParamDefault"), _T("ResinSurface_Number_of_classifications"));
		}
		prm.setInt(_T("ResinSurface"), AnalizeTypeKMeansKey, iVal);
		prm.setInt(_T("ResinSurface"), AnalizeTypeHiClusteringKey, iVal);
		prm.deleteKey(_T("ResinSurface"), _T("Number_of_classifications"));

		iVal = prm.getInt(_T("MetalSurface"), _T("Number_of_classifications"));
		if (iVal == 0) {
			iVal = sys.getInt(_T("ParamDefault"), _T("MetalSurface_Number_of_classifications"));
		}
		prm.setInt(_T("MetalSurface"), AnalizeTypeKMeansKey, iVal);
		prm.setInt(_T("MetalSurface"), AnalizeTypeHiClusteringKey, iVal);
		prm.deleteKey(_T("MetalSurface"), _T("Number_of_classifications"));

		iVal = prm.getInt(_T("JoiningResult"), _T("Number_of_classifications"));
		if (iVal == 0) {
			iVal = sys.getInt(_T("ParamDefault"), _T("JoiningResult_Number_of_classifications"));
		}
		prm.setInt(_T("JoiningResult"), AnalizeTypeKMeansKey, iVal);
		prm.setInt(_T("JoiningResult"), AnalizeTypeHiClusteringKey, iVal);
		prm.deleteKey(_T("JoiningResult"), _T("Number_of_classifications"));
	}

	if (!prjVersion.LoadString(IDS_PROJECTFILE_VERSION)) {
		prjVersion = _T("");
	}
	fileVersion = m_ProjectIO.GetVersion();
	if (prjVersion.CollateNoCase(fileVersion) != 0) {
		CString AnalizeTypeKMeansKey, AnalizeTypeHiClusteringKey, sVal;
		AnalizeTypeKMeansKey.Format(_T("%02d_Number_of_classifications"), AnalizeTypeKMeans);
		AnalizeTypeHiClusteringKey.Format(_T("%02d_Number_of_classifications"), AnalizeTypeHiClustering);

		// 樹脂面
		sVal = sys.getString(_T("ParamDefault"), _T("ResinSurface_Analysis_method"));
		if (sVal.CollateNoCase(_T("k_means")) == 0) {
			prj.setInt(_T("ResinSurface"), _T("analize_method"), AnalizeTypeKMeans);
		}
		else {
			prj.setInt(_T("ResinSurface"), _T("analize_method"), AnalizeTypeHiClustering);
		}
		iVal = prm.getInt(_T("ResinSurface"), AnalizeTypeKMeansKey);
		prj.setInt(_T("ResinSurface"), _T("kmeans_classification_nclass"), iVal);
		iVal = prm.getInt(_T("ResinSurface"), AnalizeTypeHiClusteringKey);
		prj.setInt(_T("ResinSurface"), _T("dendrogram_classification_nclass"), iVal);

		// 金属面
		sVal = sys.getString(_T("ParamDefault"), _T("MetalSurface_Analysis_method"));
		if (sVal.CollateNoCase(_T("k_means")) == 0) {
			prj.setInt(_T("MetalSurface"), _T("analize_method"), AnalizeTypeKMeans);
		}
		else {
			prj.setInt(_T("MetalSurface"), _T("analize_method"), AnalizeTypeHiClustering);
		}
		iVal = prm.getInt(_T("MetalSurface"), AnalizeTypeKMeansKey);
		prj.setInt(_T("MetalSurface"), _T("kmeans_classification_nclass"), iVal);
		iVal = prm.getInt(_T("MetalSurface"), AnalizeTypeHiClusteringKey);
		prj.setInt(_T("MetalSurface"), _T("dendrogram_classification_nclass"), iVal);

		// 結合結果
		sVal = sys.getString(_T("ParamDefault"), _T("MetalSurface_Analysis_method"));
		if (sVal.CollateNoCase(_T("k_means")) == 0) {
			prj.setInt(_T("MetalSurface"), _T("analize_method"), AnalizeTypeKMeans);
		}
		else {
			prj.setInt(_T("MetalSurface"), _T("analize_method"), AnalizeTypeHiClustering);
		}
		iVal = prm.getInt(_T("MetalSurface"), AnalizeTypeKMeansKey);
		prj.setInt(_T("MetalSurface"), _T("kmeans_classification_nclass"), iVal);
		iVal = prm.getInt(_T("MetalSurface"), AnalizeTypeHiClusteringKey);
		prj.setInt(_T("MetalSurface"), _T("dendrogram_classification_nclass"), iVal);
	}
	m_PropatyIO.SetVersion(proVersion);
	m_ProjectIO.SetVersion(prjVersion);

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
			CString msg;
			msg.Format(_T("プロジェクトの保存に失敗しました。"));
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
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
		CString msg;
		msg.Format(_T("テスト名が設定されていません。"));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}

	CString tmpParamFilePath = _T("");
	if (m_OpenType != 2) {
		if (m_ActiveRegisttedTestFolder.IsEmpty() || !CFileUtil::fileExists(m_ActiveRegisttedTestFolder)) {
			CString msg;
			msg.Format(_T("登録フォルダが存在しません。(%s)"), static_cast<LPCWSTR>(m_ActiveRegisttedTestFolder));
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
			return false;
		}
	} else {
		tmpParamFilePath = m_ParamaterFilePaht;
	}

	COleDateTime date = COleDateTime::GetCurrentTime();

	CString rootPath = GetWorkProjectFolderPath();
	CString NewProject = GetWorkProjectStatus(rootPath, _T("NewProject"));
	if (NewProject.CollateNoCase(_T("TRUE")) == 0) {
		m_ProjectIO.SetCreateDay(date);
	}
	m_ActiveRegisttedTestName = TestName;

	CString projectFileName;
	if (!projectFileName.LoadString(IDS_PROJECTFILENAME)) {
		projectFileName = _T("WeldEvalution.prj");
	}
	CString ParmaterFileName;
	if (!ParmaterFileName.LoadString(IDS_PROPATYFILENAME)) {
		ParmaterFileName = _T("parameter.inf");
	}

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

		CString msg;
		msg.Format(_T("プロパティの保存に失敗しました。"));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}
	if (m_OpenType == 2) {
		UINT uval;
		double dval;
		bool bResult = true;
		CPropatyIO pf;
		pf.SetParamaterFilePath(tmpParamFilePath);
		uval = pf.GetHorizontalResolution();
		if (!m_PropatyIO.SetHorizontalResolution(uval)) {
			CString msg;
			msg.Format(_T("横方向の解像度の設定に失敗しました。(%d)"),uval);
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
			bResult = false;
		}
		uval = pf.GetVerticalResolution();
		if (!m_PropatyIO.SetVerticalResolution(uval)) {
			CString msg;
			msg.Format(_T("縦方向の解像度の設定に失敗しました。(%d)"), uval);
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
			bResult = false;
		}
		for (int method = 0; method < 2; method++) {
			uval = pf.ResinGetNumberOfClass(method);
			if (!m_PropatyIO.ResinSetNumberOfClass(method,uval)) {
				CString msg;
				msg.Format(_T("樹脂面の分類数の設定に失敗しました。(%d)"), uval);
				writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
				bResult = false;
			}
			for (int i = 0; i < (int)uval; i++) {
				dval = pf.ResinGetJointRetio(method,i);
				if (!m_PropatyIO.ResinSetJointRetio(method,i, dval)) {
					CString msg;
					msg.Format(_T("樹脂面の接合割合の設定。(%lf)"), dval);
					writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
					bResult = false;
				}
			}
		}
		for (int method = 0; method < 2; method++) {
			uval = pf.MetalGetNumberOfClass(method);
			if (!m_PropatyIO.MetalSetNumberOfClass(method,uval)) {
				CString msg;
				msg.Format(_T("金属面の分類数の設定に失敗しました。(%d)"), uval);
				writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
				bResult = false;
			}
			for (int i = 0; i < (int)uval; i++) {
				dval = pf.MetalGetJointRetio(method,i);
				if (!m_PropatyIO.MetalSetJointRetio(method,i, dval)) {
					CString msg;
					msg.Format(_T("金属面の接合割合の設定に失敗しました。(%d)"), uval);
					writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
					bResult = false;
				}
			}
		}
		for (int method = 0; method < 2; method++) {
			uval = pf.ResultGetNumberOfClass(method);
			if (!m_PropatyIO.ResultSetNumberOfClass(method,uval)) {
				CString msg;
				msg.Format(_T("接合結果の分類数の設定に失敗しました。(%d)"), uval);
				writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
				bResult = false;
			}
			for (int i = 0; i < (int)uval; i++) {
				dval = pf.ResultGetJointRetio(method,i);
				if (!m_PropatyIO.ResultSetJointRetio(method,i, dval)) {
					CString msg;
					msg.Format(_T("接合結果の接合割合の設定に失敗しました。(%d)"), uval);
					writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
					bResult = false;
				}
			}
		}
	}

	CString path;
	CString ImageRootPath = m_ActiveRegisttedTestFolder;

	m_ProjectIO.SetUpdateDay(date);
	CString filename;
	// 樹脂面スキャンイ画像がある場合
	{
		if (!SaveScanImage(eResinSurface)) {
			CString msg;
			msg.Format(_T("樹脂面スキャンイ画像の保存に失敗しました。"));
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
			return false;
		}
	}

	// 金属面スキャンイ画像がある場合
	{
		if (!SaveScanImage(eMetalSurface)) {
			CString msg;
			msg.Format(_T("金属面スキャンイ画像の保存に失敗しました。"));
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
			return false;
		}
	}

	// 接合結果スキャンイ画像がある場合
	{
		if (!SaveScanImage(eJoiningResult)) {
			CString msg;
			msg.Format(_T("接合結果スキャンイ画像の保存に失敗しました。"));
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
			return false;
		}
	}

	// 樹脂面k-means分類結果ファイルがある場合
	{
		if (!SaveClassificationResultFile(eResinSurface, AnalizeTypeKMeans)) {
			CString msg;
			msg.Format(_T("樹脂面k-means分類結果ファイルの保存に失敗しました。"));
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
			return false;
		}
	}
	// 樹脂面階層クラスタリング分類結果ファイルがある場合
	{
		if (!SaveClassificationResultFile(eResinSurface, AnalizeTypeHiClustering)) {
			CString msg;
			msg.Format(_T("樹脂面階層クラスタリング分類結果ファイルの保存に失敗しました。"));
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
			return false;
		}
	}

	// 金属面k-means分類結果ファイルがある場合
	{
		if (!SaveClassificationResultFile(eMetalSurface, AnalizeTypeKMeans)) {
			CString msg;
			msg.Format(_T("金属面k-means分類結果ファイルの保存に失敗しました。"));
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
			return false;
		}
	}
	// 金属面階層クラスタリング分類結果ファイルがある場合
	{
		if (!SaveClassificationResultFile(eMetalSurface, AnalizeTypeHiClustering)) {
			CString msg;
			msg.Format(_T("金属面階層クラスタリング分類結果ファイルの保存に失敗しました。"));
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
			return false;
		}
	}

	
	// 接合結果k-means分類結果ファイルがある場合
	{
		if (!SaveClassificationResultFile(eJoiningResult, AnalizeTypeKMeans)) {
			CString msg;
			msg.Format(_T("接合結果k-means分類結果ファイルの保存に失敗しました。"));
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
			return false;
		}
	}
	// 接合結果階層クラスタリング分類結果ファイルがある場合
	{
		if (!SaveClassificationResultFile(eJoiningResult, AnalizeTypeHiClustering)) {
			CString msg;
			msg.Format(_T("接合結果階層クラスタリング分類結果ファイルの保存に失敗しました。"));
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
			return false;
		}
	}

//	m_ProjectFilePaht = CFileUtil::FilePathCombine(m_ActiveRegisttedTestFolder,ProjectFileName);
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

		CString msg;
		msg.Format(_T("プロジェクトファイルの保存に失敗しました。(%s)"), static_cast<LPCWSTR>(m_ProjectFilePaht));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}
	if (m_OpenType == 2) {
		m_OpenType = 1;
	}

	CString msg;
	CString folder = GetRegistedFolder();
	if (!PopProject(folder, m_ActiveRegisttedTestName)) {
		if (!msg.LoadString(IDM_ERR_PROJECT_SAVE)) {
			msg = _T("登録に失敗しました。");
		}
	}
	else {
		if (!msg.LoadString(IDM_PRJREGIST_SUCCESS)) {
			msg = _T("登録が完了しました。");
		}
	}
	AfxMessageBox(msg, MB_ICONINFORMATION);
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
		CString msg;
		msg.Format(_T("解析結果の取得に失敗しました。(%s)"), static_cast<LPCWSTR>(path));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);

		return false;
	}
	
	data.clear();
	int h, v;
	GetScanDataSize(h, v);
	data.resize((__int64)h*v);

	string line;
	int id = 0;
	while (getline(ifs, line)) {
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
	}
	if (pView) {
		pView->SendMessage(WM_READ_RESULT_STATUS,READ_RESULT_END,NULL);
	}
	return true;
}

/// <summary>
/// 添付フォルダ名取得
/// </summary>
/// <returns>添付フォルダ名を返す</returns>
CString CWeldEvaluationDoc::GetTmpFolderName()
{
	return CString(_T("__temp__"));
}

/// <summary>
/// 添付フォルダへのパス取得
/// </summary>
/// <returns>添付フォルダへのパス名を返す</returns>
CString CWeldEvaluationDoc::GetTmpFolderPath()
{
	return CFileUtil::FilePathCombine(GetRegistedFolder(), GetTmpFolderName());
}

/// <summary>
/// スキャンデータ名の取得
/// </summary>
/// <param name="ScanID">対象スキャンID</param>
/// <param name="Prefix">プレフィックス</param>
/// <returns>スキャンデータ名を返す</returns>
CString CWeldEvaluationDoc::GetScanDataName(int ScanID, CString Prefix)
{
	CString ProjectName = m_PropatyIO.GetProjectName();
	CString fileName;
	switch (ScanID) {
	case	eResinSurface:	///< 樹脂
	{
		if (Prefix.IsEmpty()) {
			fileName.Format(_T("ResinScanImage"));
		}
		else {
			fileName.Format(_T("%s_ResinScanImage"), (LPCWSTR)ProjectName);
		}
	}
	break;
	case	eMetalSurface:	///< 金属
	{
		if (Prefix.IsEmpty()) {
			fileName.Format(_T("MetalScanImage"));
		}
		else {
			fileName.Format(_T("%s_MetalScanImage"), (LPCWSTR)ProjectName);
		}
	}
	break;
	case	eJoiningResult:	///< 接合結果
	{
		if (Prefix.IsEmpty()) {
			fileName.Format(_T("ResultScanImage"));
		}
		else {
			fileName.Format(_T("%s_ResultScanImage"), (LPCWSTR)ProjectName);
		}
	}
	break;
	default:
		return _T("");
	}
	return fileName;
}

/// <summary>
/// スキャンデータの保存
/// </summary>
/// <param name="ScanID">対象スキャンID</param>
/// <returns>成功した場合はtrue、失敗の場合はfalseを返す</returns>
bool CWeldEvaluationDoc::SaveScanImage(int ScanID)
{
//	CString ProjectName = m_PropatyIO.GetProjectName();
	CString ProjectName = _T("");		// ファイル名にプロジェクト名を付けない様に修正
	CString fileName = GetScanDataName(ScanID, ProjectName);
	switch (ScanID) {
	case	eResinSurface:	///< 樹脂
		if (!m_ProjectIO.SetResinScanImageFile(fileName)) {
			CString msg;
			msg.Format(_T("樹脂面スキャンデータの設定に失敗しました。(%s)"), static_cast<LPCWSTR>(fileName));
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
			return false;
		}
		////////////////////////////////////////////////////////////////////
		//
		// 保存処理を行う(.hdr,.raw,.bmp)
		// 画像の保存に成功したらプロジェクトへ登録
		//
		////////////////////////////////////////////////////////////////////
		break;
	case	eMetalSurface:	///< 金属
		if (!m_ProjectIO.SetMetalScanImageFile(fileName)) {
			CString msg;
			msg.Format(_T("金属面スキャンデータの設定に失敗しました。(%s)"), static_cast<LPCWSTR>(fileName));
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
			return false;
		}
		////////////////////////////////////////////////////////////////////
		//
		// 保存処理を行う(.hdr,.raw,.bmp)
		// 画像の保存に成功したらプロジェクトへ登録
		//
		////////////////////////////////////////////////////////////////////
		break;
	case	eJoiningResult:	///< 接合結果
		if (!m_ProjectIO.SetResultScanImageFile(fileName)) {
			CString msg;
			msg.Format(_T("接合結果スキャンデータの設定に失敗しました。(%s)"), static_cast<LPCWSTR>(fileName));
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
			return false;
		}
		////////////////////////////////////////////////////////////////////
		//
		// 保存処理を行う(.hdr,.raw,.bmp)
		// 画像の保存に成功したらプロジェクトへ登録
		//
		////////////////////////////////////////////////////////////////////
		break;
	default:
		{
			CString msg;
			msg.Format(_T("スキャン対象ID値が不正です。(%d)"), ScanID);
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		}
		return false;
	}
	return true;
}

/// <summary>
/// 分類ファイルの有無判定
/// </summary>
/// <param name="ScanID">ファイル種別ID</param>
/// <param name="method">解析方法</param>
/// <returns>存在する場合はtrue、失敗の場合はfalseを返す</returns>
bool CWeldEvaluationDoc::IsExistClassificationResultFile(int ScanID, int method)
{
	bool bResult = false;
	CString path;

	path = getClassificationDataFilePath(ScanID, method);
	if (path.IsEmpty()) {
		bResult = false;
	}
	else {
		bResult = CFileUtil::fileExists(path);
	}
	return bResult;
}

/// <summary>
/// 解析結果の分類数と設定の分類数が異なるかの判定
/// </summary>
/// <param name="ScanID">ファイル種別ID</param>
/// <param name="AnalysisMethodID">解析方法</param>
/// <returns>異なっている場合はtrue、同じ場合はfalseを返す</returns>
bool CWeldEvaluationDoc::IsUpdateNumberOfClassifications(int ScanID, int AnalysisMethodID)
{
	int	nClass=0,CriatedNClass=0;
	if (ScanID == eResinSurface) {
		nClass = m_PropatyIO.ResinGetNumberOfClass(AnalysisMethodID);
		if (AnalysisMethodID == AnalizeTypeKMeans) {
			CriatedNClass = m_ProjectIO.GetNumbetOfResinKMeansClassificationClass();
		}
		else if (AnalysisMethodID == AnalizeTypeHiClustering) {
			CriatedNClass = m_ProjectIO.GetNumbetOfResinDendrogramClassificationClass();
		}
	}
	else if (ScanID == eMetalSurface) {
		nClass = m_PropatyIO.MetalGetNumberOfClass(AnalysisMethodID);
		if (AnalysisMethodID == AnalizeTypeKMeans) {
			CriatedNClass = m_ProjectIO.GetNumbetOfMetalKMeansClassificationClass();
		}
		else if (AnalysisMethodID == AnalizeTypeHiClustering) {
			CriatedNClass = m_ProjectIO.GetNumbetOfMetalDendrogramClassificationClass();
		}
	}
	else if (ScanID == eJoiningResult) {
		nClass = m_PropatyIO.ResultGetNumberOfClass(AnalysisMethodID);
		if (AnalysisMethodID == AnalizeTypeKMeans) {
			CriatedNClass = m_ProjectIO.GetNumbetOfResultKMeansClassificationClass();
		}
		else if (AnalysisMethodID == AnalizeTypeHiClustering) {
			CriatedNClass = m_ProjectIO.GetNumbetOfResultDendrogramClassificationClass();
		}
	}
	return (nClass != CriatedNClass);
}


/// <summary>
/// 分類結果の保存
/// </summary>
/// <param name="ScanID">対象スキャンID</param>
/// <param name="method">解析方法</param>
/// <returns>成功した場合はtrue、失敗の場合はfalseを返す</returns>
bool CWeldEvaluationDoc::SaveClassificationResultFile(int ScanID, int method)
{
	CString fileName;
	switch(ScanID) {
	case	eResinSurface	:	// 樹脂
		if (method == AnalizeTypeKMeans)
		{
			fileName = _T("ResinDendrogramClassification.csv");
			if (!m_ProjectIO.SetResinDendrogramClassFile(fileName)) {
				CString msg;
				msg.Format(_T("樹脂面階層クラスタリング分類結果ファイル名の設定に失敗しました。(%s)"), static_cast<LPCWSTR>(fileName));
				writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
				return false;
			}
		}
		else if (method == AnalizeTypeHiClustering)
		{
			fileName = _T("ResinkmeansClassification.csv");
			if (!m_ProjectIO.SetResinKmeansClassFile(fileName)) {
				CString msg;
				msg.Format(_T("樹脂面k-means分類結果ファイル名の設定に失敗しました。(%s)"), static_cast<LPCWSTR>(fileName));
				writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
				return false;
			}
		}
		break;
	case	eMetalSurface	:	// 金属
		if (method == AnalizeTypeKMeans)
		{
			fileName = _T("MetalDendrogramClassification.csv");
			if (!m_ProjectIO.SetMetalDendrogramClassFile(fileName)) {
				CString msg;
				msg.Format(_T("金属面階層クラスタリング分類結果ファイル名の設定に失敗しました。(%s)"), static_cast<LPCWSTR>(fileName));
				writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
				return false;
			}
		}
		else if (method == AnalizeTypeHiClustering) {
			fileName = _T("MetalkmeansClassification.csv");
			if (!m_ProjectIO.SetMetalKmeansClassFile(fileName)) {
				CString msg;
				msg.Format(_T("金属面k-means分類結果ファイル名の設定に失敗しました。(%s)"), static_cast<LPCWSTR>(fileName));
				writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
				return false;
			}
		}
		break;
	case	eJoiningResult:	// 接合結果
		if (method == AnalizeTypeKMeans)
		{
			fileName = _T("ResultDendrogramClassification.csv");
			if (!m_ProjectIO.SetResultDendrogramClassFile(fileName)) {
				CString msg;
				msg.Format(_T("接合結果階層クラスタリングフ分類ァイル名の設定に失敗しました。(%s)"), static_cast<LPCWSTR>(fileName));
				writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
				return false;
			}
		}
		else if (method == AnalizeTypeHiClustering) {
			fileName = _T("ResultkmeansClassification.csv");
			if (!m_ProjectIO.SetResultKmeansClassFile(fileName)) {
				CString msg;
				msg.Format(_T("接合結果k-meansファイル名の設定に失敗しました。(%s)"), static_cast<LPCWSTR>(fileName));
				writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
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
		CString msg;
		msg.Format(_T("登録用フォルダが存在しません。(%s)"), static_cast<LPCWSTR>(path));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}

	CFileFind    cFileFind;
	CString      strSearchDir = CFileUtil::FilePathCombine(path,_T("*"));
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
				if (patnname.CollateNoCase(GetTmpFolderName()) == 0) {
					continue;
				}
				else if (patnname.CollateNoCase(GetWorkProjectFolderName()) == 0) {
					continue;
				}
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

	int dataw, datah;
	GetScanDataSize(dataw, datah);
	if ((int)data.size() != ((__int64)dataw * datah)) {
		CString msg;
		msg.Format(_T("データサイズがスキャンデータのサイズと一致しません。widht=%d Height=%d :データサイズ=%d"), dataw, datah, (int)data.size());
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}
	CPoint AnzLTPos;
	CSize AnzSize;
	if (!getAnalizeArea(AnzLTPos, AnzSize)) {
		CString msg;
		msg.Format(_T("解析エリアが取得できませんでした。"));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}

	std::map<int,int> sum;
	for (int i = 0; i < nClass; i++) {
		sum[i] = 0;
	}

#if 0
	for (int i = 0; i < (int)data.size(); i++) {
		if ((data[i] < 0) || (data[i] >= nClass)) {
			continue;
		}
		sum[data[i]] += 1;
	}

	for (int i = 0; i < nClass; i++) {
		retio[i] = (double)sum[i] / (double)data.size();
	}
#else
	int nData = 0;
	int id = 0;
	int minH = AnzLTPos.y - 1;
	int minW = AnzLTPos.x - 1;
	int maxH = AnzLTPos.y + AnzSize.cy - 1;
	int maxW = AnzLTPos.x + AnzSize.cx - 1;
	for (int h = 0; h < datah; h++) {
		if ((h < minH) || (h >= maxH)) {
			continue;
		}
		id = h * dataw;
		for (int w = 0; w < dataw; w++) {
			if ((w < minW) || (w >= maxW)) {
				continue;
			}
			if ((data[(__int64)id+w] < 0) || (data[(__int64)id+w] >= nClass)) {
				continue;
			}
			sum[data[(__int64)id+w]] += 1;
			nData++;
		}
	}

	for (int i = 0; i < nClass; i++) {
		retio[i] = (double)sum[i] / (double)nData;
	}
#endif
	return true;
}

bool CWeldEvaluationDoc::getAnalizeArea(CPoint &tlPos, CSize &size)
{
	CConfigrationIO sys(m_SystemFilePathName);
	int AnalysisAreaTLPointX = sys.getInt(_T("System"), _T("AnalysisAreaTLPointX"));
	int AnalysisAreaTLPointY = sys.getInt(_T("System"), _T("AnalysisAreaTLPointY"));
	int AnalysisAreaWidth = sys.getInt(_T("System"), _T("AnalysisAreaWidth"));
	int AnalysisAreaHeight = sys.getInt(_T("System"), _T("AnalysisAreaHeight"));
	int dataw, datah;
	m_ProjectIO.GetScanDataSize(dataw, datah);
	if ((dataw == 0) || (datah == 0)) {
		CString msg;
		msg.Format(_T("結合されたスキャンデータのサイズが正しくありません。Width=%d Height=%d"), dataw, datah);
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}

	if ((AnalysisAreaWidth <= 0) || (AnalysisAreaHeight <= 0)) {
		AnalysisAreaTLPointX = 0;
		AnalysisAreaTLPointY = 0;
		AnalysisAreaWidth = dataw;
		AnalysisAreaHeight = datah;
		CString msg;
		msg.Format(_T("解析エリアの値が正しくありません。解析エリアを全領域に設定します"));
		writeLog(CLog::Warning, CString(__FILE__), __LINE__, msg);
	}
	if ((AnalysisAreaTLPointX + AnalysisAreaWidth) > dataw) {
		AnalysisAreaWidth = dataw - AnalysisAreaTLPointX;
	}
	if ((AnalysisAreaTLPointY + AnalysisAreaHeight) > datah) {
		AnalysisAreaHeight = datah - AnalysisAreaTLPointY;
	}

	tlPos.x = AnalysisAreaTLPointX;
	tlPos.y = AnalysisAreaTLPointY;
	size.cx = AnalysisAreaWidth;
	size.cy = AnalysisAreaHeight;
	return true;
}

/// <summary>
/// 解析の実施
/// </summary>
/// <param name="ScanID">判定素材ID</param>
/// <param name="AnalysisMethodID">解析方法ID</param>
/// <returns>成功の場合はtrue、失敗の場合はfalseを返す</returns>
bool CWeldEvaluationDoc::Analize(int ScanID, int AnalysisMethodID)
{
	int nClass = 1;
	CString ScanDataFilePath, ClassificationDataFilePath;
	////////////////////////////////////////////////////////////////////
	// 解析を実施
	////////////////////////////////////////////////////////////////////
	bool (CProjectFileIO::*SetNumbetOfClass)(int) = nullptr;

	ScanDataFilePath = getScanDataFilePath(ScanID);
	ClassificationDataFilePath = getClassificationDataFilePath(ScanID, AnalysisMethodID);
	switch (ScanID) {
	case	eResinSurface	:	// 樹脂
		nClass = ResinGetNumberOfClass(AnalysisMethodID);
		if (AnalysisMethodID == AnalizeTypeHiClustering) {
			SetNumbetOfClass = &CProjectFileIO::SetNumbetOfResinDendrogramClassificationClass;
		}
		else if (AnalysisMethodID == AnalizeTypeKMeans) {
			SetNumbetOfClass = &CProjectFileIO::SetNumbetOfResinKMeansClassificationClass;
		}
		break;
	case	eMetalSurface	:	// 金属
		nClass = MetalGetNumberOfClass(AnalysisMethodID);
		if (AnalysisMethodID == AnalizeTypeHiClustering) {
			SetNumbetOfClass = &CProjectFileIO::SetNumbetOfMetalDendrogramClassificationClass;
		}
		else if (AnalysisMethodID == AnalizeTypeKMeans) {
			SetNumbetOfClass = &CProjectFileIO::SetNumbetOfMetalKMeansClassificationClass;
		}
		break;
	case	eJoiningResult	:	// 接合結果
		nClass = ResultGetNumberOfClass(AnalysisMethodID);
		if (AnalysisMethodID == AnalizeTypeHiClustering) {
			SetNumbetOfClass = &CProjectFileIO::SetNumbetOfResultDendrogramClassificationClass;
		}
		else if (AnalysisMethodID == AnalizeTypeKMeans) {
			SetNumbetOfClass = &CProjectFileIO::SetNumbetOfResultKMeansClassificationClass;
		}
		break;
	}

	CConfigrationIO sys(m_SystemFilePathName);
	CString modulePath = sys.getString(_T("Common"), _T("AIModuleFolder"));
	if (modulePath.IsEmpty()) {
		CString msg;
		msg.Format(_T("解析モジュール格納フォルダが存在しません。(%s):ScanID=%d,解析方法=%d"), static_cast<LPCWSTR>(modulePath), ScanID, AnalysisMethodID);
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}
	CString returnfile = CFileUtil::FilePathCombine(modulePath, _T("return.txt"));
	CFileUtil::fileDelete(returnfile);

	CString execpath;
	PROCESS_INFORMATION p;
	STARTUPINFO s;
	ZeroMemory(&s, sizeof(s));
	s.cb = sizeof(s);

	CString prm;
	switch (AnalysisMethodID) {
	case	AnalizeTypeKMeans:				// k-means
		execpath = CFileUtil::FilePathCombine(modulePath, _T("kmeans.bat"));
		break;
	case	AnalizeTypeHiClustering:		// 階層クラスタリング
		execpath = CFileUtil::FilePathCombine(modulePath, _T("dendrogram.bat"));
		break;
	default:
		{
			CString msg;
			msg.Format(_T("未知の解析手法が設定されています。(%d):ScanID=%d"), AnalysisMethodID, ScanID);
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		}
		return false;
	}

	// 解析エリアの取得
#if 0
	int AnalysisAreaTLPointX = sys.getInt(_T("System"), _T("AnalysisAreaTLPointX"));
	int AnalysisAreaTLPointY = sys.getInt(_T("System"), _T("AnalysisAreaTLPointY"));
	int AnalysisAreaWidth = sys.getInt(_T("System"), _T("AnalysisAreaWidth"));
	int AnalysisAreaHeight = sys.getInt(_T("System"), _T("AnalysisAreaHeight"));
	int dataw, datah;
	m_ProjectIO.GetScanDataSize(dataw, datah);
	if ((dataw == 0) || (datah == 0)) {
		CString msg;
		msg.Format(_T("結合されたスキャンデータのサイズが正しくありません。Width=%d Height=%d"), dataw, datah);
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}

	if ((AnalysisAreaWidth <= 0) || (AnalysisAreaHeight <= 0)) {
		AnalysisAreaTLPointX = 0;
		AnalysisAreaTLPointY = 0;
		AnalysisAreaWidth = dataw;
		AnalysisAreaHeight = datah;
		CString msg;
		msg.Format(_T("解析エリアの値が正しくありません。全領域の解析をおこないます"));
		writeLog(CLog::Warning, CString(__FILE__), __LINE__, msg);
	}
	if ((AnalysisAreaTLPointX + AnalysisAreaWidth) > dataw) {
		AnalysisAreaWidth = dataw - AnalysisAreaTLPointX;
	}
	if ((AnalysisAreaTLPointY + AnalysisAreaHeight) > datah) {
		AnalysisAreaHeight = datah - AnalysisAreaTLPointY;
	}
	if ((AnalysisAreaWidth < (int)m_PropatyIO.NumberOfOverridePixel()) || (AnalysisAreaHeight <= 0)) {
		CString msg;
		msg.Format(_T("解析エリアの値が正しくありません。Stert Point(%d, %d) Width=%d Height=%d"), AnalysisAreaTLPointX, AnalysisAreaTLPointY, AnalysisAreaWidth, AnalysisAreaHeight);
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}
#else
	CPoint AnalizetlPos;
	CSize AnalizeSze;
	if (!getAnalizeArea(AnalizetlPos, AnalizeSze)) {
		CString msg;
		msg.Format(_T("解析エリアの値が正しくありません"));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}
	if ((AnalizeSze.cx < (int)m_PropatyIO.NumberOfOverridePixel()) || (AnalizeSze.cy <= 0)) {
		CString msg;
		msg.Format(_T("解析エリアの値が正しくありません。Stert Point(%d, %d) Width=%d Height=%d"), AnalizetlPos.x, AnalizetlPos.y, AnalizeSze.cx, AnalizeSze.cy);
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}
#endif

	////////////////////////////////////////////////////////////////////
	// 解析の実施
	if (CFileUtil::fileExists(execpath)) {
		prm.Format(_T("%s %s %d %d %d %d %d"), (LPCTSTR)ScanDataFilePath, (LPCTSTR)ClassificationDataFilePath, nClass, AnalizetlPos.x, AnalizetlPos.y, AnalizeSze.cx, AnalizeSze.cy);
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
			CString msg;
			msg.Format(_T("解析モジュールの実行に失敗しました。(%s):ScanID=%d,解析方法=%d"), static_cast<LPCWSTR>(execpath), ScanID, AnalysisMethodID);
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
			return false;
		}
		else {
			CloseHandle(p.hThread);
			WaitForSingleObject(p.hProcess, INFINITE);
			CloseHandle(p.hProcess);
		}
	}
	else {
		CString msg;
		msg.Format(_T("解析モジュールが存在しません。(%s):ScanID=%d,解析方法=%d"), static_cast<LPCWSTR>(execpath), ScanID, AnalysisMethodID);
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
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

	// 解析結果の合否チェック（結果ファイル解析）
	CStdioFile fd;
	UINT nOpenFlags = CFile::modeRead;
	if (fd.Open(returnfile, nOpenFlags))
	{
		CString read;
		if (!fd.ReadString(read)) {
			fd.Close();
			CString msg;
			msg.Format(_T("解析結果判定結果を読み込めませんでした。:ScanID=%d,解析方法=%d"), ScanID, AnalysisMethodID);
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
			return false;
		}
		int val = _ttol(read);
		fd.Close();
		if (val != 0) {
			CString msg;
			msg.Format(_T("解析に失敗しました。(result=%d):ScanID=%d,解析方法=%d"), val, ScanID, AnalysisMethodID);
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
			return false;
		}
	}
	else {
		CString msg;
		msg.Format(_T("解析結果判定ファイルが開けませんでした。(%s):ScanID=%d,解析方法=%d"), static_cast<LPCWSTR>(returnfile), ScanID, AnalysisMethodID);
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}

	// 解析実施情報の保存
	if (SetNumbetOfClass) {
		if ((m_ProjectIO.*SetNumbetOfClass)(nClass)) {
			CString msg;
			msg.Format(_T("解析実施情報の保存に失敗しました。:ScanID=%d,解析方法=%d"), ScanID, AnalysisMethodID);
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		}
	}

	////////////////////////////////////////////////////////////////////
	// 接合率を求める
	CString path;
	vector<int> data;
	vector<double> retio;
	path = getClassificationDataFilePath(ScanID, AnalysisMethodID);
	if (ScanID == eResinSurface) {
		nClass = m_PropatyIO.ResinGetNumberOfClass(AnalysisMethodID);
	}
	else if (ScanID == eMetalSurface) {
		nClass = m_PropatyIO.MetalGetNumberOfClass(AnalysisMethodID);
	}
	else if (ScanID == eJoiningResult) {
		nClass = m_PropatyIO.ResultGetNumberOfClass(AnalysisMethodID);
	}

	if (!CWeldEvaluationDoc::getResultFile(path, data)) {
		CString msg;
		msg.Format(_T("解析結果ファイルの読み込みに失敗しました。(%s):ScanID=%d,解析方法=%d"), static_cast<LPCWSTR>(path), ScanID, AnalysisMethodID);
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}
	else {
		int dataw, datah;
		GetScanDataSize(dataw, datah);
		if ((int)data.size() != ((__int64)dataw * datah)) {
			CString msg;
			msg.Format(_T("データサイズがスキャンデータのサイズと一致しません。:ScanID=%d,widht=%d Height=%d :データサイズ=%d"), ScanID, dataw, datah, (int)data.size());
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
			return false;
		}

		if (!CalcJoJointRetio(data, nClass, retio)) {
			CString msg;
			msg.Format(_T("接合率の取得に失敗しました。:ScanID=%d,解析方法=%d"), ScanID, AnalysisMethodID);
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
			return false;
		}
	}

	if (ScanID == eResinSurface) {
		for (int ViewJointRatioNo = 0; ViewJointRatioNo < (int)retio.size(); ViewJointRatioNo++) {
			if (!ResinSetJointRetio(AnalysisMethodID, ViewJointRatioNo,retio[ViewJointRatioNo]*100.0)) {
				CString msg;
				msg.Format(_T("樹脂面の接合割合の設定に失敗しました。(ScanID=%d、解析方法=%d、接合面番号=%d,樹脂面の接合割合=%lf)"), ScanID, AnalysisMethodID, ViewJointRatioNo, retio[ViewJointRatioNo] * 100.0);
				writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
				bResult = false;
			}
		}
	} else if (ScanID == eMetalSurface) {
		for (int ViewJointRatioNo = 0; ViewJointRatioNo < (int)retio.size(); ViewJointRatioNo++) {
			if (!MetalSetJointRetio(AnalysisMethodID, ViewJointRatioNo,retio[ViewJointRatioNo]*100.0)) {
				CString msg;
				msg.Format(_T("金属面の接合割合の設定に失敗しました。(ScanID=%d、解析方法=%d、接合面番号=%d,金属面の接合割合=%lf)"), ScanID, AnalysisMethodID, ViewJointRatioNo, retio[ViewJointRatioNo] * 100.0);
				writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
				bResult = false;
			}
		}
	} else if (ScanID == eJoiningResult) {
		for (int ViewJointRatioNo = 0; ViewJointRatioNo < (int)retio.size(); ViewJointRatioNo++) {
			if (!ResultSetJointRetio(AnalysisMethodID,ViewJointRatioNo, retio[ViewJointRatioNo] * 100.0)) {
				CString msg;
				msg.Format(_T("接合結果の接合割合の設定に失敗しました。(ScanID=%d、解析方法=%d、接合面番号=%d,接合結果の接合割合=%lf)"), ScanID, AnalysisMethodID, ViewJointRatioNo, retio[ViewJointRatioNo] * 100.0);
				writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
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
	if ((renew) || (!CFileUtil::fileExists(imagefile))) {
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
		double r, g, b;
		GetRGBWavelength(r,g,b); 
		pSdio->SetRGBWavelength(r,g,b);
		if (pSdio->open(scanfile,renew)) {
			if (!pSdio->LoadImage(height, width, nBand, img)) {
				CString msg;
				msg.Format(_T("画像データ取得の取得に失敗しました。(%s)"), static_cast<LPCWSTR>(scanfile));
				writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
				return false;
			}
			img.Save(imagefile);
		}
		else {
			CString msg;
			msg.Format(_T("画像データ取得の取得に失敗しました。(%s)"), static_cast<LPCWSTR>(scanfile));
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
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

/// <summary>
/// スキャンデータイメージの削除
/// </summary>
/// <param name="ScanID">対象スキャンID</param>
/// <returns>成功の場合はtrue、失敗の場合はfalseを返す</returns>
bool CWeldEvaluationDoc::DeleteScanImageFilePath(int ScanID)
{
	CString scanImageFile = getScanImageFilePath(ScanID);
	if (CFileUtil::fileExists(scanImageFile)) {
		return CFileUtil::fileDelete(scanImageFile);
	}
	return true;
}

/// <summary>
/// スキャンデータの反転
/// </summary>
/// <param name="ScanID">対象スキャンID</param>
/// <returns>成功の場合はtrue、失敗の場合はfalseを返す</returns>
bool CWeldEvaluationDoc::InversScanData(int ScanID)
{
	CScanDataIO *pSdio = nullptr;
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

	bool bResult = true;
	CString pathName = getScanDataPath(ScanID);
	CString fPathName = pathName + _T(".raw");
	if (pSdio->InversData()) {
		CString tmp = fPathName + _T(".tmp");
		bResult = pSdio->saveRawData(tmp);
		if (bResult) {
			CString bak = fPathName + _T(".bak");
			CFileUtil::fileDelete(bak);
			CFile::Rename(fPathName, bak);
			CFile::Rename(tmp, fPathName);
			CFileUtil::fileDelete(bak);
		}
	}
	else {
		bResult = false;
		if (!pSdio->readRawData(fPathName)) {
		}
	}
	return bResult;
}

/// <summary>
/// 解析データの反転
/// </summary>
/// <param name="ScanID">対象スキャンID</param>
/// <returns>成功の場合はtrue、失敗の場合はfalseを返す</returns>
bool CWeldEvaluationDoc::InversAnalizeData(int ScanID)
{
	CString ScanDataFilePath, ClassificationDataFilePath, imgPaht;
	ScanDataFilePath = getScanDataFilePath(ScanID);
	if (!CFileUtil::fileExists(ScanDataFilePath)) {
		return true;
	}

	int method[] = { AnalizeTypeKMeans ,AnalizeTypeHiClustering };

	bool bResult = true;
	int width, height;
	GetScanDataSize(width, height);
#if 0
	CStdioFile tfd;
	CString buf;
	vector<CString> lines;
	lines.resize(height);
	for (int metodID = 0; metodID < 2; metodID++) {
		ClassificationDataFilePath = getClassificationDataFilePath(ScanID, method[metodID]);
		if (!CFileUtil::fileExists(ClassificationDataFilePath)) {
			continue;
		}

		if (tfd.Open(ClassificationDataFilePath, CFile::modeReadWrite | CFile::typeText)) {
			CString key, val;
			int i = height - 1;
			bool bOverflow = false;
			lines.clear();
			lines.resize(height);
			while (tfd.ReadString(buf)) {
				if (i < 0) {
					bOverflow = true;
					bResult = false;
					break;
				}
				lines[i] = buf;
				i--;
			}
			if (!bOverflow) {
				tfd.SeekToBegin();
				for (i = 0; i < height; i++) {
					tfd.WriteString(lines[i] + _T("\n"));
				}
				tfd.Close();

				imgPaht = ClassificationDataFilePath + _T(".bmp");
				if (CFileUtil::fileExists(imgPaht)) {
					CFileUtil::fileDelete(imgPaht);
				}
			}
			else {
				tfd.Close();
			}
		}
		else {
			bResult = false;
		}
	}
	lines.clear();
	lines.shrink_to_fit();

#else
	float ***ana = nullptr;
	try {
		CString buf, val;
		CStdioFile tfd;
		ana = new float **[2]();
		if (ana) {
			for (int i = 0; i < 2; i++) {
				ana[i] = new float *[height]();
				for (int h = 0; h < height; h++) {
					ana[i][h] = new float[width]();
				}
			}
		}

		CScanDataIO scn;
		double **mat = scn.MatrixInit();
		scn.MatrixMove(mat, width / 2, height / 2);
		scn.MatrixRotete(mat, 180);
		scn.MatrixMove(mat, -width / 2, -height / 2);
		scn.MatrixInvers(mat);

		for (int metodID = 0; metodID < 2; metodID++) {
			ClassificationDataFilePath = getClassificationDataFilePath(ScanID, method[metodID]);
			if (!CFileUtil::fileExists(ClassificationDataFilePath)) {
				continue;
			}

			if (tfd.Open(ClassificationDataFilePath, CFile::modeReadWrite | CFile::typeText)) {
				int id, w = 0, h = 0;
				while (tfd.ReadString(buf)) {
					if (!buf.IsEmpty()) {
						w = 0;
						while (w < width) {
							id = buf.FindOneOf(_T(","));
							if (id >= 0) {
								val = buf.Mid(0, id);
								ana[0][h*width][w++] = (float)_ttof(val);
								if (buf.GetLength() < (id + 1)) {
									val = buf;
									ana[0][h*width][w] = (float)_ttof(val);
									break;
								}
								else {
									buf = buf.Mid(id + 1);
								}
							}
							else {
								buf.Trim();
								if (buf.GetLength() > 1) {
									ana[0][h*width][w] = (float)_ttof(val);
								}
								break;
							}
						}
					}
					if (w != (width-1)) {
							// error
							scn.MatrixRelease(mat);
							bResult = false;
							goto InversAnalizeDataFinal;
					}
					h++;
				}
				if (h != (height-1)) {
					// error
					tfd.Close();
					scn.MatrixRelease(mat);
					bResult = false;
					goto InversAnalizeDataFinal;
				}
			}
			if (scn.affine(width, height, &ana[0], width, height, &ana[1], 1, mat, false)) {
				CString tmp;
				tfd.SeekToBegin();
				for (int h = 0; h < height; h++) {
					buf.Format(_T("%d,"),(int)(ana[1][h][0]+.5));
					for (int w = 1; w < width; w++) {
						tmp.Format(_T("%s,%d"), static_cast<LPCWSTR>(buf), (int)(ana[1][h][w]+.5));
						buf = tmp;
					}
					tfd.WriteString(buf);
				}
				tfd.Close();
			}
			else {
				// error
				tfd.Close();
				scn.MatrixRelease(mat);
				bResult = false;
				goto InversAnalizeDataFinal;
			}
		}
		scn.MatrixRelease(mat);
	}
	catch (...) {
		bResult = false;
	}
#endif // 0

InversAnalizeDataFinal:
	if (ana) {
		for (int i = 0; i < 2; i++) {
			if (ana[i]) {
				for (int h = 0; h < height; h++) {
					if (ana[i][h]) {
						delete[] ana[i][h];
						ana[i][h] = nullptr;
					}
				}
				delete[] ana[i];
				ana[i] = nullptr;
			}
		}
		delete[] ana;
		ana = nullptr;
	}

	return bResult;
}

/// <summary>
/// 解析データの反転可否判定
/// </summary>
/// <param name="ScanID">対象スキャンID</param>
/// <returns>可能の場合はtrue、不可能の場合はfalseを返す</returns>
///@remark 解析エリアの上と下または右と左の幅が異なる場合は反転できない
bool CWeldEvaluationDoc::IsInversAnalizeData(int ScanID)
{
	int width, height;
	GetScanDataSize(width, height);
	CPoint tlPos;
	CSize size;
	if (!getAnalizeArea(tlPos, size)) {
		return false;
	}

	if ((height - (tlPos.y + size.cy)) == tlPos.y) {
		if ((width - (tlPos.x + size.cx)) == tlPos.x) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}

/// <summary>
/// 解析データの削除
/// </summary>
/// <param name="ScanID">対象スキャンID</param>
void CWeldEvaluationDoc::DeleteAnalizeData(int ScanID)
{
	CString ClassificationDataFilePath, imgPaht;
	int method[] = { AnalizeTypeKMeans ,AnalizeTypeHiClustering };
	for (int metodID = 0; metodID < 2; metodID++) {
		ClassificationDataFilePath = getClassificationDataFilePath(ScanID, method[metodID]);
		if (ClassificationDataFilePath.IsEmpty()) {
			continue;
		}
		if (CFileUtil::fileExists(ClassificationDataFilePath)) {
			CFileUtil::fileDelete(ClassificationDataFilePath);
		}

		imgPaht = ClassificationDataFilePath + _T(".bmp");
		if (CFileUtil::fileExists(imgPaht)) {
			CFileUtil::fileDelete(imgPaht);
		}
	}
}

/// <summary>
/// 輝度－RGB変換
/// </summary>
/// <param name="h">輝度</param>
/// <param name="r">赤</param>
/// <param name="g">緑</param>
/// <param name="b">青</param>
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

/// <summary>
/// クラスに対応する色の取得
/// </summary>
/// <param name="id">対象クラスID</param>
/// <param name="nClass">クラス数</param>
/// <returns>成功の場合は対応色を返す</returns>
COLORREF CWeldEvaluationDoc::GetClassColor(int id, int nClass)
{
	COLORREF col;
	unsigned char r, g, b;
	int h = (int)((double)id * (240.0 / (double)((__int64)nClass - 1) + 0.5));
	H2RGB(h, r, g, b);
	col = RGB(r, g, b);
	return col;
}

/// <summary>
/// 分類結果の画像データ取得
/// </summary>
/// <param name="targetID">対象スキャンID</param>
/// <param name="method">分類種別ID</param>
/// <param name="img">画像データ</param>
/// <param name="renew">再読込フラグ</param>
/// <returns>存在する場合はパスを返す、失敗した場合は空文字を返す</returns>
bool CWeldEvaluationDoc::LoadClassificationResultImage(int targetID, int method, CImage &img, bool renew/* = false*/)
{
	bool bResult = true;
	CString root, name;
	CString classfile, imagefile;
	int nClass = 0;

	classfile = getClassificationDataFilePath(targetID, method);
	if (!CFileUtil::fileExists(classfile)) {
		CString msg;
		msg.Format(_T("解析結果ファイルが存在しません。(%s)"), static_cast<LPCWSTR>(classfile));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}
	imagefile = getClassificationImageFilePath(targetID, method);
	if ((renew) || (!CFileUtil::fileExists(imagefile))) {
		vector<int> data;
		if (!CWeldEvaluationDoc::getResultFile(classfile, data)) {
			CString msg;
			msg.Format(_T("解析結果ファイルの読み込みに失敗しました。(%s)"), static_cast<LPCWSTR>(classfile));
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
			return false;
		}
		else {
			switch (targetID) {
			case	eResinSurface:	///< 樹脂
				nClass = m_PropatyIO.ResinGetNumberOfClass(method);
				break;
			case	eMetalSurface:	///< 金属
				nClass = m_PropatyIO.MetalGetNumberOfClass(method);
				break;
			case	eJoiningResult:	///< 接合結果
				nClass = m_PropatyIO.ResultGetNumberOfClass(method);
				break;
			default:
				{
					CString msg;
					msg.Format(_T("未定義のスキャンIDが設定されました。(%d)"), targetID);
					writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
				}
				return false;
			}

			int Bpp = 3;
#if 0
			int width = m_PropatyIO.GetHorizontalResolution();
			int height = m_PropatyIO.GetVerticalResolution();
#else
			int width, height;
			GetScanDataSize(width, height);
#endif
			if ((int)data.size() != (width * height)) {
				CString msg;
				msg.Format(_T("データサイズが異常です。(size=%d)"), (int)data.size());
				writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
				return false;
			}
			// イメージの横サイズバウンダリ調整
			int imageWidth = width;
			if ((imageWidth % 8) != 0) {
				imageWidth = (int)(imageWidth / 8) * 8 + 8;
			}
			unsigned char * p24Img = new unsigned char[(__int64)imageWidth * (__int64)height * Bpp];
			BYTE *ptr = p24Img;
			COLORREF *col = new COLORREF[nClass];
			for (int id = 0; id < nClass; id++) {
				col[id] = GetClassColor(id, nClass);
			}

			switch (targetID) {
			case	eResinSurface:	///< 樹脂
				for (int id = 0; id < nClass; id++) {
					ResinSetJointColor(method,id, col[id]);
				}
				break;
			case	eMetalSurface:	///< 金属
				for (int id = 0; id < nClass; id++) {
					MetalSetJointColor(method,id, col[id]);
				}
				break;
			case	eJoiningResult:	///< 接合結果
				for (int id = 0; id < nClass; id++) {
					ResultSetJointColor(method,id, col[id]);
				}
				break;
			}

			int id = 0;
			for (int y = 0; y < height; y++) {
				ptr = p24Img + (__int64)((__int64)y*imageWidth)*Bpp;
				for (int x = 0; x < width; x++) {
					id = (y*width) + x;
					if (data[id] < 0) {
						*(ptr++) = 0;
						*(ptr++) = 0;
						*(ptr++) = 0;
					}
					else {
						*(ptr++) = GetBValue(col[data[id]]);
						*(ptr++) = GetGValue(col[data[id]]);
						*(ptr++) = GetRValue(col[data[id]]);
					}
				}
				for (int x = width; x < imageWidth; x++) {
					*(ptr++) = 0;
					*(ptr++) = 0;
					*(ptr++) = 0;
				}
			}

			if (col) {
				delete[] col;
				col = nullptr;
			}

			BITMAPINFOHEADER    bmInfohdr;
			// Create the header info
			bmInfohdr.biSize = sizeof(BITMAPINFOHEADER);
			bmInfohdr.biWidth = imageWidth;
			bmInfohdr.biHeight = -height;
			bmInfohdr.biPlanes = 1;
			bmInfohdr.biBitCount = (WORD)(Bpp * 8);
			bmInfohdr.biCompression = BI_RGB;
			bmInfohdr.biSizeImage = imageWidth * height * Bpp;
			bmInfohdr.biXPelsPerMeter = 0;
			bmInfohdr.biYPelsPerMeter = 0;
			bmInfohdr.biClrUsed = 0;
			bmInfohdr.biClrImportant = 0;

			BITMAPINFO bmInfo;
			bmInfo.bmiHeader = bmInfohdr;
			bmInfo.bmiColors[0].rgbBlue = 255;

			if (img.Create(imageWidth, height, 8 * Bpp, NULL)) {
				HDC dc = img.GetDC();
				SetDIBitsToDevice(dc, 0, 0, imageWidth, height, 0, 0, 0, height, p24Img, &bmInfo, DIB_RGB_COLORS);
				img.ReleaseDC();
				img.Save(imagefile);
			}
			else {
				DWORD err = GetLastError();
				LPVOID lpMsgBuf;
				FormatMessage(
					FORMAT_MESSAGE_ALLOCATE_BUFFER							// テキストのメモリ割り当てを要求する
					| FORMAT_MESSAGE_FROM_SYSTEM							// エラーメッセージはWindowsが用意しているものを使用
					| FORMAT_MESSAGE_IGNORE_INSERTS,						// 次の引数を無視してエラーコードに対するエラーメッセージを作成する
					NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),	// 言語を指定
					(LPTSTR)&lpMsgBuf,										// メッセージテキストが保存されるバッファへのポインタ
					0,
					NULL);
//				AfxMessageBox((LPCTSTR)lpMsgBuf, MB_OK | MB_ICONINFORMATION);
				LocalFree(lpMsgBuf);

				CString msg,fmt;
				if (!fmt.LoadString(IDM_ERR_FAILDCREATEIMAGE)) {
					fmt = _T("画像の作成に失敗しました。:%s");
				}
				msg.Format(fmt, static_cast<LPCWSTR>(lpMsgBuf));
				writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
				bResult = false;
			}

			if (p24Img) {
				delete[] p24Img;
				p24Img = nullptr;
			}
		}
	}
	else {
		HRESULT ret;
		ret = img.Load(imagefile);
		if (FAILED(ret)) {
			CString msg;
			msg.Format(CString(_T("画像の読み込みに失敗しました。(%s)")), static_cast<LPCWSTR>(imagefile));
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
			return false;
		}
	}

	return bResult;
}

/// <summary>
/// 指定位置スペクトル群の取得
/// </summary>
/// <param name="ScanID">対象スキャンID</param>
/// <param name="pos">スペクトル取得位置</param>
/// <param name="data">スペクトル群</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CWeldEvaluationDoc::GetSpectrumData(int ScanID, CPoint &pos, std::vector<double> &data)
{
	CString root, name;
	CString scanfile, imagefile;

	root = GetWorkProjectPath();
	switch (ScanID) {
	case	eResinSurface:	// 樹脂
		if (!m_ResinScanData.GetSpectrumData(pos, data)) {
			CString msg;
			msg.Format(_T("樹脂面の指定位置スペクトル群の取得に失敗しました。(スキャンID=%d、スペクトル取得位置=(%d,%d))"), ScanID, pos.x, pos.y);
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
			return false;
		}
		break;
	case	eMetalSurface:	// 金属
		if (!m_MetalScanData.GetSpectrumData(pos, data)) {
			CString msg;
			msg.Format(_T("金属面の指定位置スペクトル群の取得に失敗しました。(スキャンID=%d、スペクトル取得位置=(%d,%d))"), ScanID, pos.x, pos.y);
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
			return false;
		}
		break;
	case	eJoiningResult:	// 接合結果
		if (!m_ResultScanData.GetSpectrumData(pos, data)) {
			CString msg;
			msg.Format(_T("接合結果の指定位置スペクトル群の取得に失敗しました。(スキャンID=%d、スペクトル取得位置=(%d,%d))"), ScanID, pos.x, pos.y);
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
			return false;
		}
		break;
	default:
		{
			CString msg;
			msg.Format(_T("不明のスキャンIDが指定されています(ScanID=%d)"), ScanID);
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		}
		return false;
	}
	return true;
}

/// <summary>
/// スペクトルレンジの取得
/// </summary>
/// <param name="ScanID">対象スキャンID</param>
/// <param name="min">最小波形値[nm]</param>
/// <param name="max">最大波形値[nm]</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CWeldEvaluationDoc::GetSpectrumRange(int ScanID, double &min, double &max)
{
	int nBand = 0;
	switch (ScanID) {
	case	eResinSurface:	// 樹脂
		nBand = m_ResinScanData.GetNumberOfBand();
		min = m_ResinScanData.getWaveLength(0);
		max = m_ResinScanData.getWaveLength(nBand - 1);
		break;
	case	eMetalSurface:	// 金属
		nBand = m_MetalScanData.GetNumberOfBand();
		min = m_MetalScanData.getWaveLength(0);
		max = m_MetalScanData.getWaveLength(nBand - 1);
		break;
	case	eJoiningResult:	// 接合結果
		nBand = m_ResultScanData.GetNumberOfBand();
		min = m_ResultScanData.getWaveLength(0);
		max = m_ResultScanData.getWaveLength(nBand-1);
		break;
	default:
		return false;
	}
	return true;
}

/// <summary>
/// スキャンイメージの保存
/// </summary>
/// <param name="writePath">保存パス</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CWeldEvaluationDoc::WriteImage(CString writePath)
{
	int ScanID[3] = { eResinSurface ,eMetalSurface , eJoiningResult};
	int method[2] = { AnalizeTypeKMeans, AnalizeTypeHiClustering };

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

			for (int methodID = 0; methodID < 2; methodID++) {
				// 結果画像は解析したもののみ表示
				classfile = getClassificationDataFilePath(ScanID[ID], method[methodID]);
				if (CFileUtil::fileExists(classfile)) {
					path = getClassificationImageFilePath(ScanID[ID], method[methodID]);
					bexist = true;
					if (!CFileUtil::fileExists(path)) {
						CImage img;
						if (!LoadClassificationResultImage(ScanID[ID], method[methodID], img)) {
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
							fname.Format(_T("%d_%d_%s%s"), ScanID[ID], method[methodID], (LPCTSTR)fname, (LPCTSTR)ext);
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
	if (!bResult) {
		CString msg;
		msg.Format(_T("スキャンイメージの保存に失敗しました。(%s)"), static_cast<LPCWSTR>(writePath));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
	}
	return bResult;
}


//////////////////////////////////////////////////////////////////////////////////////////
// プロジェクトワークファイル処理

/// <summary>
/// ワークプロジェクト名の取得
/// </summary>
/// <returns>ワークプロジェクト名を返す</returns>
CString CWeldEvaluationDoc::GetWorkProjectFolderName()
{
	CConfigrationIO sys(m_SystemFilePathName);
	CString folder = sys.getString(_T("Common"), _T("ProjectWorkFolderName"));
	if (folder.IsEmpty()) {
		folder = m_ModulePath + _T("__ProjectWork__");
	}
	return folder;
}

/// <summary>
/// ワークプロジェクトのルートパス取得
/// </summary>
/// <returns>ワークプロジェクトのルートパスを返す</returns>
CString CWeldEvaluationDoc::GetWorkProjectFolderPath()
{
	CString folderName = GetWorkProjectFolderName();
	CConfigrationIO sys(m_SystemFilePathName);
	CString folder = sys.getString(_T("Common"), _T("ProjectWorkPath"));
	if (folder.IsEmpty()) {
		folder = m_ModulePath + _T("__ProjectWork__");
	}
	CString path = CFileUtil::FilePathCombine(folder, folderName);
	return path;
}

/// <summary>
/// プロジェクトステータス設定
/// </summary>
/// <param name="WKPrjPath">ワークプロジェクトへのパス</param>
/// <param name="key">ステータスキー</param>
/// <param name="value">ステータスキーに対応する値</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CWeldEvaluationDoc::SetWorkProjectStatus(CString WKPrjPath, CString key, CString value)
{
	CString stsPathName = CFileUtil::FilePathCombine(WKPrjPath, StatysFileName);
	CConfigrationIO sys(stsPathName);
	return sys.setString(_T("Status"), key, value);
}

/// <summary>
/// プロジェクトステータス取得
/// </summary>
/// <param name="WKPrjPath">ワークプロジェクトへのパス</param>
/// <param name="key">ステータスキー</param>
/// <returns>成功場合はキーに対応する値を返す</returns>
CString CWeldEvaluationDoc::GetWorkProjectStatus(CString WKPrjPath, CString key)
{
	CString stsPathName = CFileUtil::FilePathCombine(WKPrjPath, StatysFileName);
	CConfigrationIO sys(stsPathName);
	CString val = sys.getString(_T("Status"), key);
	return val;
}

/// <summary>
/// ワークプロジェクトパスの取得
/// </summary>
/// <returns>ワークプロジェクトパスを返す</returns>
CString CWeldEvaluationDoc::GetWorkProjectPath()
{
	CString rootPath = GetWorkProjectFolderPath();
	CString prjName = GetWorkProjectStatus(rootPath, _T("ProjectName"));
	if (prjName.IsEmpty()) {
		prjName = NewProjectFolder;
	}
	return CFileUtil::FilePathCombine(rootPath, prjName);
}

/// <summary>
/// プロジェクトの更新判定
/// </summary>
/// <returns>更新がある場合はtrue、無い場合はfalseを返す</returns>
bool CWeldEvaluationDoc::IsWorkProjectUpdated()
{
	CString rootPath = GetWorkProjectFolderPath();
	CString status = GetWorkProjectStatus(rootPath, _T("Update"));
	if (status.MakeUpper() == _T("TRUE")) {
		return true;
	}
	else {
		return false;
	}
}

/// <summary>
/// プロジェクトの更新ステータス設定
/// </summary>
/// <param name="bUpdate">更新フラグ</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CWeldEvaluationDoc::SetWorkProjectUpdteStatus(bool bUpdate)
{
	CString rootPath = GetWorkProjectFolderPath();
	CString status;
	if (bUpdate) {
		status = _T("TRUE");
	}
	else {
		status = _T("FALSE");
	}
	return SetWorkProjectStatus(rootPath, _T("Update"), status);
}

/// <summary>
/// ワークプロジェクトの削除
/// </summary>
void CWeldEvaluationDoc::ClrWorkProject()
{
	CString folder = GetWorkProjectFolderPath();
	if (CFileUtil::fileExists(folder)) {
		CFileUtil::fileDeleteEx(folder);
	}
}

/// <summary>
/// プロジェクトをワークへ移動
/// </summary>
/// <param name="ResistPath">移動元プロジェクトのルートパス</param>
/// <param name="ProjentName">プロジェクト名</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CWeldEvaluationDoc::PushProject(CString ResistPath, CString ProjentName)
{
	CString rootPath = GetWorkProjectFolderPath();
	if (!CFileUtil::fileExists(rootPath)) {
		if (!CFileUtil::MakeDir(rootPath)) {
			CString msg;
			msg.Format(_T("ディレクトリの作成に失敗しました。(%s)"), static_cast<LPCWSTR>(rootPath));
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
			return false;
		}
	}
	if (ProjentName.IsEmpty()) {
		// 新規プロジェクト
		SetWorkProjectStatus(rootPath, _T("ProjectName"), NewProjectFolder);
		SetWorkProjectStatus(rootPath, _T("NewProject"), _T("TRUE"));
	}
	else {
		// 既存プロジェクト
		SetWorkProjectStatus(rootPath, _T("ProjectName"), ProjentName);
		SetWorkProjectStatus(rootPath, _T("NewProject"), _T("FALSE"));
		CString srcPath = CFileUtil::FilePathCombine(ResistPath, ProjentName);
		CString dstPath = GetWorkProjectPath();
		if (!CFileUtil::Copy(srcPath, dstPath, false)) {
			ClrWorkProject();
			CString msg;
			msg.Format(_T("プロジェクトからワークへ移動に失敗しました。(プロジェクト[%s]、ワーク[%s])"), static_cast<LPCWSTR>(rootPath), static_cast<LPCWSTR>(dstPath));
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
			return false;
		}
	}
	SetWorkProjectUpdteStatus(false);

	return true;
}

/// <summary>
/// ワークからプロジェクトへ移動
/// </summary>
/// <param name="ResistPath">移動先プロジェクトのルートパス</param>
/// <param name="ProjentName">プロジェクト名</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CWeldEvaluationDoc::PopProject(CString ResistPath, CString ProjentName)
{
	CString rootPath = GetWorkProjectFolderPath();
	CString srcPath = GetWorkProjectPath();
	CString dstPath = CFileUtil::FilePathCombine(ResistPath, ProjentName);
	SetWorkProjectStatus(rootPath, _T("ProjectName"), ProjentName);
	SetWorkProjectStatus(rootPath, _T("NewProject"), _T("FALSE"));
	SetWorkProjectUpdteStatus(false);

	bool bBackup = false;
	CString bak = dstPath + _T(".bak__");
	if (CFileUtil::fileExists(dstPath)) {
		CFile::Rename(dstPath, bak);
		bBackup = true;
	}

	if (!CFileUtil::Copy(srcPath, dstPath, true)) {
		if (bBackup) {
			CFile::Rename(bak, dstPath);
		}
		CString msg;
		msg.Format(_T("ワークからプロジェクトへ移動に失敗しました。(ワーク[%s]、プロジェクト[%s])"), static_cast<LPCWSTR>(srcPath), static_cast<LPCWSTR>(dstPath));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}
	if (bBackup) {
		CFileUtil::fileDeleteEx(bak);
	}

	CString ParmaterFileName;
	if (!ParmaterFileName.LoadString(IDS_PROPATYFILENAME)) {
		ParmaterFileName = _T("parameter.inf");
	}
	CString projectFileName;
	if (!projectFileName.LoadString(IDS_PROJECTFILENAME)) {
		projectFileName = _T("WeldEvalution.prj");
	}

	CString PropatyFilePath = CFileUtil::FilePathCombine(ResistPath, ProjentName);
	PropatyFilePath = CFileUtil::FilePathCombine(PropatyFilePath, ParmaterFileName);
	CPropatyIO::WriteProjectName(PropatyFilePath, projectFileName);
	CPropatyIO::WriteTestName(PropatyFilePath, ProjentName);

	m_ActiveRegisttedTestFolder = GetWorkProjectPath();
	m_ParamaterFilePaht = CFileUtil::FilePathCombine(m_ActiveRegisttedTestFolder, ParmaterFileName);
	m_ProjectFilePaht = CFileUtil::FilePathCombine(m_ActiveRegisttedTestFolder, projectFileName);

	m_PropatyIO.SetParamaterFilePath(m_ParamaterFilePaht);
	m_ProjectIO.SetProjectFilePath(m_ProjectFilePaht);

	dstPath = CFileUtil::FilePathCombine(rootPath, ProjentName);
	CFile::Rename(srcPath, dstPath);

	return true;
}

/// <summary>
/// スキャン情報の削除
/// </summary>
/// <param name="ScanID">対象スキャンID</param>
void CWeldEvaluationDoc::DeleteContents(int ScanID)
{
	CString path, imgPaht;
	path = getScanDataPath(ScanID);
	CString headerPath = path + _T(".hdr");
	if (CFileUtil::fileExists(headerPath)) {
		CFileUtil::fileDelete(headerPath);
	}
	CString rawPath = path + _T(".raw");
	if (CFileUtil::fileExists(rawPath)) {
		CFileUtil::fileDelete(rawPath);
	}
	imgPaht = GetScanImagePath(ScanID);
	if (CFileUtil::fileExists(imgPaht)) {
		CFileUtil::fileDelete(imgPaht);
	}
	path = GetClassificationResultPath(ScanID, AnalizeTypeKMeans);
	if (CFileUtil::fileExists(path)) {
		CFileUtil::fileDelete(path);
	}
	imgPaht = path + _T(".bmp");
	if (CFileUtil::fileExists(imgPaht)) {
		CFileUtil::fileDelete(imgPaht);
	}
	path = GetClassificationResultPath(ScanID, AnalizeTypeHiClustering);
	if (CFileUtil::fileExists(path)) {
		CFileUtil::fileDelete(path);
	}
	imgPaht = path + _T(".bmp");
	if (CFileUtil::fileExists(imgPaht)) {
		CFileUtil::fileDelete(imgPaht);
	}

	CConfigrationIO sys(m_SystemFilePathName);
	double dval;
	int uval;
	COLORREF color = RGB(0, 0, 0);
	switch (ScanID) {
	case	eResinSurface:	///< 樹脂
		for (int method = 0; method < 2; method++) {
			uval = m_PropatyIO.ResinGetNumberOfClass(method);
			for (int i = 0; i < (int)uval; i++) {
				dval = sys.getDouble(_T("ParamDefault"), _T("ResinSurface_Joining_ratio"));
				m_PropatyIO.ResinSetJointRetio(method, i, dval);
				m_PropatyIO.ResinSetJointColor(method, i,color);
			}
		}
		break;
	case	eMetalSurface:	///< 金属
		for (int method = 0; method < 2; method++) {
			uval = m_PropatyIO.MetalGetNumberOfClass(method);
			for (int i = 0; i < (int)uval; i++) {
				dval = sys.getDouble(_T("ParamDefault"), _T("ResinSurface_Joining_ratio"));
				m_PropatyIO.MetalSetJointRetio(method, i, dval);
				m_PropatyIO.MetalSetJointColor(method, i, color);
			}
		}
		break;
	case	eJoiningResult:	///< 接合結果
		for (int method = 0; method < 2; method++) {
			uval = m_PropatyIO.ResultGetNumberOfClass(method);
			for (int i = 0; i < (int)uval; i++) {
				dval = sys.getDouble(_T("ParamDefault"), _T("ResinSurface_Joining_ratio"));
				m_PropatyIO.ResultSetJointRetio(method, i, dval);
				m_PropatyIO.ResultSetJointColor(method, i, color);
			}
		}
		break;
	}
}

/// <summary>
/// ホワイトバランスファイルの削除
/// </summary>
void CWeldEvaluationDoc::DeleteWBFile(CString title/* = _T("")*/)
{
	CString registedFolde = GetRegistedFolder();
	CString WBFileName;
	if (title.IsEmpty()) {
		WBFileName = GetWBFileName();
	}
	else {
		WBFileName = title;
	}
	CString WBFileNameHdr = WBFileName + _T(".hdr");
	CString WBFileNameRaw = WBFileName + _T(".raw");
	CString path;
	path = CFileUtil::FilePathCombine(registedFolde, WBFileNameHdr);
	if (CFileUtil::fileExists(path)) {
		CFileUtil::fileDelete(path);
	}
	path = CFileUtil::FilePathCombine(registedFolde, WBFileNameRaw);
	if (CFileUtil::fileExists(path)) {
		CFileUtil::fileDelete(path);
	}
}

/// <summary>
/// プロジェクトの削除
/// </summary>
/// <param name="ProjentName">プロジェクト名</param>
/// <returns>成功場合はtrue、失敗場合はfalseを返す</returns>
bool CWeldEvaluationDoc::DeleteProject(CString ProjentName)
{
	bool bResult = true;
	CString rootPath = GetWorkProjectFolderPath();
	CString workPath = CFileUtil::FilePathCombine(rootPath, ProjentName);

	CString resistFolder = GetRegistedFolder();
	CString dstPath = CFileUtil::FilePathCombine(resistFolder, ProjentName);
	if (CFileUtil::fileExists(dstPath)) {
		if (CFileUtil::fileDeleteEx(dstPath)) {
			if (CFileUtil::fileExists(workPath)) {
				if (CFileUtil::fileDeleteEx(workPath)) {
					if (!NewProject()) {
						bResult = false;
					}
				}
				else {
					bResult = false;
				}
			}
		}
		else {
			bResult = false;
		}
	}
	return bResult;
}
