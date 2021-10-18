#pragma once

// –¼‘O‹óŠÔ
using namespace std;

class CFileUtil
{
public:
	CFileUtil(void);
	~CFileUtil(void);

	static CString GetModulePath();
	static bool fileExists(CString& pathname);
	static bool fileDelete(CString& pathname);
	static bool splitPath(CString path, CString &drive, CString &dir, CString &fname, CString &ext);
	static bool MakeDir(CString path);
	static CString getDir(CString path);
	static CString FilePathCombine(CString path, CString filename);
	static bool GetFileCreateDay(CString path,COleDateTime &cdate);
	static bool GetFileUpdateDay(CString path,COleDateTime &mdate);
};

