#pragma once

// 名前空間
using namespace std;

/// <summary>
/// ファイルユーティリティクラス
/// </summary>
class CFileUtil
{
public:
	CFileUtil(void);
	~CFileUtil(void);

	static CString GetModulePath();
	static bool fileExists(CString& pathname);
	static bool fileDelete(CString& pathname);
	static bool fileDeleteEx(CString pathname);
	static bool Copy(CString src, CString dst, bool bOverwrite);
	static bool splitPath(CString path, CString &drive, CString &dir, CString &fname, CString &ext);
	static bool MakeDir(CString path);
	static CString getDir(CString path);
	static CString FilePathCombine(CString path, CString filename);
	static bool GetFileCreateDay(CString path,COleDateTime &cdate);
	static bool GetFileUpdateDay(CString path,COleDateTime &mdate);
	static bool SelectFolder(HWND hWnd, LPCTSTR lpDefFolder, LPTSTR lpSelectPath, UINT iFlg, CString ttl);
	static bool WriteUTF8ToSJIS(CStdioFile &fd, CString str);
};

