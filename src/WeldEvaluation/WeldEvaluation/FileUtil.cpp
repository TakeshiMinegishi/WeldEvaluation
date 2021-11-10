#include "stdafx.h"
#include "FileUtil.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
CFileUtil::CFileUtil(void)
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
CFileUtil::~CFileUtil(void)
{
}

/// <summary>
/// ���s���W���[���̃J�����g�p�X�̎擾
/// </summary>
/// <returns>���s���W���[���̃J�����g�p�X��Ԃ�</returns>
CString CFileUtil::GetModulePath()
{
	CString	    path = _T("");     // �e�[�u���ւ̃p�X
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
/// �t�@�C�������݂��邩���m�F����
/// </summary>
/// <param name="pathname">�t�@�C���ւ̃t���p�X</param>
/// <returns>���݂���ꍇ��true�A���݂��Ȃ��ꍇ��false��Ԃ�</returns>
bool CFileUtil::fileExists(CString& pathname)
{
	bool bResult = false;
	if (PathFileExists(pathname)) {
		bResult = true;
	}
	return bResult;
}

/// <summary>
/// �t�@�C�����폜����
/// </summary>
/// <param name="pathname">�t�@�C���ւ̃t���p�X</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
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
/// �t�B���p�X�𕪉�����
/// </summary>
/// <param name="path">�t�@�C���p�X</param>
/// <param name="drive">�h���C�u���^�[</param>
/// <param name="dir">�p�X</param>
/// <param name="fname">�t�@�C���^�C�g��</param>
/// <param name="ext">�g���q</param>
/// <returns>���������ꍇ��ture�A���s�����ꍇ��false��Ԃ�</returns>
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
/// �f�B���N�g���̍쐬
/// </summary>
/// <param name="path">�f�B���N�g���p�X</param>
/// <returns>���������ꍇ��ture�A���s�����ꍇ��false��Ԃ�</returns>
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
/// �t�@�C���p�X����f�B���N�g�����擾����
/// </summary>
/// <param name="path">�t�@�C���p�X</param>
/// <returns>�f�B���N�g����Ԃ�</returns>
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
/// �t�@�C���p�X�̍쐬
/// </summary>
/// <param name="path">�t�@�C���t�H���_</param>
/// <param name="filename">�t�@�C����</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
CString CFileUtil::FilePathCombine(CString path, CString filename)
{
	TCHAR filePath[MAX_PATH];
	ZeroMemory(&filePath[0], sizeof(filePath));
	PathCombine(filePath, path, filename);
	return CString(filePath);
}


/// <summary>
/// �t�@�C���̍쐬���̎擾
/// </summary>
/// <param name="path">�t�@�C���p�X</param>
/// <param name="cdate">�쐬��</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
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
/// �t�@�C���̍X�V���̎擾
/// </summary>
/// <param name="path">�t�@�C���p�X</param>
/// <param name="mdate">�X�V����</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
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
