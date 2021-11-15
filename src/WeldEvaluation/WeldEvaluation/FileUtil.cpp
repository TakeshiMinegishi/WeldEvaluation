#include "stdafx.h"
#include "FileUtil.h"


/// <summary>
/// コンストラクタ
/// </summary>
CFileUtil::CFileUtil(void)
{
}

/// <summary>
/// デストラクタ
/// </summary>
CFileUtil::~CFileUtil(void)
{
}

/// <summary>
/// 実行モジュールのカレントパスの取得
/// </summary>
/// <returns>実行モジュールのカレントパスを返す</returns>
CString CFileUtil::GetModulePath()
{
	CString	    path = _T("");     // テーブルへのパス
	DWORD nSize = 1024;
	TCHAR* szPath = nullptr;
	DWORD rSize;

	szPath = new WCHAR[nSize];
	rSize = GetModuleFileNameW(NULL, szPath, nSize);
	if ((rSize < nSize) && (ERROR_SUCCESS == GetLastError())) {
		CString drive, dir, fname, ext;
		if (CFileUtil::splitPath(szPath, drive, dir, fname, ext)) {
			path = drive + dir;
		}
	}
	if (szPath) {
		delete szPath;
		szPath = nullptr;
	}
	return path;
}

/// <summary>
/// ファイルが存在するかを確認する
/// </summary>
/// <param name="pathname">ファイルへのフルパス</param>
/// <returns>存在する場合はtrue、存在しない場合はfalseを返す</returns>
bool CFileUtil::fileExists(CString& pathname)
{
	bool bResult = false;
	if (PathFileExists(pathname)) {
		bResult = true;
	}
	return bResult;
}

/// <summary>
/// ファイルを削除する
/// </summary>
/// <param name="pathname">ファイルへのフルパス</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CFileUtil::fileDelete(CString& pathname)
{
	bool bResult = false;
	if (CFileUtil::fileExists(pathname)) {
		CFileStatus rStatus;
		if (CFile::GetStatus(pathname,rStatus)) {
			if ((rStatus.m_attribute & CFile::directory) == CFile::directory ) {
				if (RemoveDirectory(pathname)) {
					bResult = true;
				}
			} else {
				if (_tremove(pathname) == 0) {
					bResult = true;
				}
			}
		} else {
			if (_tremove(pathname) == 0) {
				bResult = true;
			}
		}
	}
	else {
		bResult = true;
	}
	return bResult;
}

/// <summary>
/// フィルパスを分解する
/// </summary>
/// <param name="path">ファイルパス</param>
/// <param name="drive">ドライブレター</param>
/// <param name="dir">パス</param>
/// <param name="fname">ファイルタイトル</param>
/// <param name="ext">拡張子</param>
/// <returns>成功した場合はture、失敗した場合はfalseを返す</returns>
bool CFileUtil::splitPath(CString path, CString &drive, CString &dir, CString &fname, CString &ext)
{
	bool bResult = false;
	TCHAR szFullPath[MAX_PATH];
	TCHAR szDrive[MAX_PATH] = _T(""), szPath[MAX_PATH] = _T(""), szFName[MAX_PATH] = _T(""), szExt[MAX_PATH] = _T("");
	drive = _T("");
	dir = _T("");
	fname = _T("");
	ext = _T("");
	_tcscpy_s(szFullPath, path);
	errno_t err = _tsplitpath_s(szFullPath, szDrive, sizeof(szDrive)/sizeof(TCHAR), szPath, sizeof(szPath)/sizeof(TCHAR), szFName, sizeof(szFName)/sizeof(TCHAR), szExt, sizeof(szExt)/sizeof(TCHAR));
	if (err == 0) {
		drive = szDrive;
		dir = szPath;
		fname = szFName;
		ext = szExt;
		bResult = true;
	}
	return bResult;
}

/// <summary>
/// ディレクトリの作成
/// </summary>
/// <param name="path">ディレクトリパス</param>
/// <returns>成功した場合はture、失敗した場合はfalseを返す</returns>
bool CFileUtil::MakeDir(CString path)
{
	bool bResult = true;

	if (!fileExists(path)) {
		CString dir = getDir(path);
		if (dir.IsEmpty()) {
			return false;
		}
		if (!MakeDir(dir)) {
			return false;
		}
		if (!CreateDirectory(path,NULL)) {
			return false;
		}
	}

	return bResult;
}

/// <summary>
/// ファイルパスからディレクトリを取得する
/// </summary>
/// <param name="path">ファイルパス</param>
/// <returns>ディレクトリを返す</returns>
CString CFileUtil::getDir(CString path)
{
	CString  dirpath = _T("");
	CString drive, dir, fname, ext;
	if (CFileUtil::splitPath(path, drive, dir, fname, ext)) {
		dirpath = drive + dir;
		if (dirpath != "") {
			if (dirpath[dirpath.GetLength() - 1] != '\\') {
				dirpath += "\\";
			}
		}
	}
	return dirpath;
}

/// <summary>
/// ファイルパスの作成
/// </summary>
/// <param name="path">ファイルフォルダ</param>
/// <param name="filename">ファイル名</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
CString CFileUtil::FilePathCombine(CString path, CString filename)
{
	TCHAR filePath[MAX_PATH];
	ZeroMemory(&filePath[0], sizeof(filePath));
	PathCombine(filePath, path, filename);
	return CString(filePath);
}


/// <summary>
/// ファイルの作成日の取得
/// </summary>
/// <param name="path">ファイルパス</param>
/// <param name="cdate">作成日</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CFileUtil::GetFileCreateDay(CString path,COleDateTime &cdate)
{
	if (!CFileUtil::fileExists(path)) {
		return false;
	}
	CFileStatus tStatus;
	if (CFile::GetStatus(path,tStatus)) {
		CTime tm(tStatus.m_ctime);
		cdate.SetDate(tm.GetYear(),tm.GetMonth(),tm.GetDay());
		return true;
	} else {
		COleDateTime date;
		cdate.SetDate(0,0,0);
		return false;
	}
}

/// <summary>
/// ファイルの更新日の取得
/// </summary>
/// <param name="path">ファイルパス</param>
/// <param name="mdate">更新日日</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CFileUtil::GetFileUpdateDay(CString path,COleDateTime &mdate)
{
	if (!CFileUtil::fileExists(path)) {
		return false;
	}
	CFileStatus tStatus;
	if (CFile::GetStatus(path,tStatus)) {
		CTime tm(tStatus.m_mtime);
		mdate.SetDate(tm.GetYear(),tm.GetMonth(),tm.GetDay());
		return true;
	} else {
		COleDateTime date;
		mdate.SetDate(0,0,0);
		return false;
	}
}


int CALLBACK BrowseCallbackProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	switch (uMsg)
	{
	case BFFM_INITIALIZED:
		//初期化処理(初期表示フォルダ指定)
		SendMessage(hWnd, BFFM_SETSELECTION, (WPARAM)TRUE, lpData);
		break;
	case BFFM_SELCHANGED:
		//フォルダ選択ダイアログでフォルダが選択された時に処理が必要であればここに記載
		break;
	}
	return 0;
}

bool CFileUtil::SelectFolder(HWND hWnd, LPCTSTR lpDefFolder, LPTSTR lpSelectPath, UINT iFlg, CString ttl)
{
	LPMALLOC    pMalloc;
	BOOL        brtn = FALSE;


	//IMallocインターフェイスへのポインタを取得
	if (SUCCEEDED(SHGetMalloc(&pMalloc)))
	{
		BROWSEINFO        brows;
		ITEMIDLIST        *pIDlist;
		//BROWSEINFO構造体の初期化
		memset(&brows, NULL, sizeof(brows));
		//BROWSEINFO構造体に値をセット
		brows.hwndOwner = hWnd;
		brows.pszDisplayName = lpSelectPath;
		brows.lpszTitle = ttl;
		brows.lParam = (LPARAM)lpDefFolder;
		brows.pidlRoot = NULL;
		brows.ulFlags = iFlg;
		brows.lpfn = &BrowseCallbackProc;
		brows.iImage = (int)NULL;
		//フォルダ選択ダイアログの表示
		pIDlist = SHBrowseForFolder(&brows);
		if (pIDlist == NULL) {
			// キャンセル
			return false;
		}
		else {
			//ITEMIDLIST からフルパスを得る
			SHGetPathFromIDList(pIDlist, lpSelectPath);
			brtn = TRUE;
			//ITEMIDLIST の開放
			pMalloc->Free(pIDlist);
		}
		pMalloc->Release();
	}	
	return true;
}
