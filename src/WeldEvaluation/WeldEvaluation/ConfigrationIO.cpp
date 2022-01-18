#include "stdafx.h"
#include "ConfigrationIO.h"
#include "FileUtil.h"


/// <summary>
/// コンストラクタ
/// </summary>
CConfigrationIO::CConfigrationIO(void)
{
	Initialize();
}

/// <summary>
/// 初期化コンストラクタ
/// </summary>
/// <param name="pathname">ファイルパス</param>
CConfigrationIO::CConfigrationIO(CString pathname)
{
	Initialize();
	m_DataPath = pathname;
}

/// <summary>
/// デストラクタ
/// </summary>
CConfigrationIO::~CConfigrationIO(void)
{
}

/// <summary>
/// 初期化を行う
/// </summary>
void CConfigrationIO::Initialize()
{
	m_DataPath = "";
}


/// <summary>
/// 設定ファイルのパスを設定
/// </summary>
/// <param name="pathName">ファイルパス</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CConfigrationIO::setFilePath(CString pathName)
{
	m_DataPath = pathName;
	return CFileUtil::fileExists(m_DataPath);
}

/// <summary>
/// 設定ファイルのパスを取得
/// </summary>
/// <returns>ファイルパスを返す</returns>
CString CConfigrationIO::getFilePath(void)
{
	return m_DataPath;
}

/// <summary>
/// 指定セクションのキー値を取得
/// </summary>
/// <param name="section">セクション名</param>
/// <param name="key">キー名</param>
/// <param name="filePath">データパス名</param>
/// <returns>値を返す</returns>
CString CConfigrationIO::read(CString section, CString key, CString filePath)
{
	CString val;
	if (CFileUtil::fileExists(filePath)) {
		TCHAR waBuf[1024];
		DWORD ret = GetPrivateProfileString(section, key, nullptr, waBuf, _countof(waBuf), filePath);
		if (ret > 0) {
			val = CString(waBuf);
		}
	}
	return val;
}

/// <summary>
/// 指定セクションのキーに値を設定
/// </summary>
/// <param name="section">セクション名</param>
/// <param name="key">キー名</param>
/// <param name="value">値</param>
/// <param name="filePath">データパス名</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CConfigrationIO::write(CString section, CString key, CString value, CString filePath)
{
	bool bResult = false;
	if (0 != ::WritePrivateProfileString(section, key, value, filePath)) {
		bResult = true;
	}
	return bResult;
}

/// <summary>
/// 指定セクションのキー値（整数）を取得
/// </summary>
/// <param name="section">セクション名</param>
/// <param name="key">キー名</param>
/// <param name="default">デフォルト値</param>
/// <param name="filePath">データパス名</param>
/// <returns>値を返す</returns>
int CConfigrationIO::getInt(CString section, CString key, int default, CString filePath)
{
	if (filePath == _T("")) {
		filePath = m_DataPath;
	}
	CString str = read(section, key, filePath);
	int iVal = default;
	if (str != _T("")) {
		iVal = _ttol(str);
	}
	return iVal;
}

/// <summary>
/// 指定セクションのキー値（整数）を取得
/// </summary>
/// <param name="section">セクション名</param>
/// <param name="key">キー名</param>
/// <param name="filePath">データパス名</param>
/// <returns>値を返す</returns>
int CConfigrationIO::getInt(CString section, CString key, CString filePath)
{
	return getInt(section,key, 0, filePath);
}

/// <summary>
/// 指定セクションのキーに整数値を設定
/// </summary>
/// <param name="section">セクション名</param>
/// <param name="key">キー名</param>
/// <param name="value">整数値</param>
/// <param name="filePath">データパス名</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CConfigrationIO::setInt(CString section, CString key, int value, CString filePath)
{
	if (filePath == _T("")) {
		filePath = m_DataPath;
	}
	CString str;
	str.Format(_T("%d"), value);
	return write(section,key,str,filePath);
}

/// <summary>
/// 指定セクションのキー値（浮動小数点）を取得
/// </summary>
/// <param name="section">セクション名</param>
/// <param name="key">キー名</param>
/// <param name="filePath">データパス名</param>
/// <returns>値を返す</returns>
double CConfigrationIO::getDouble(CString section, CString key, CString filePath)
{
	if (filePath == _T("")) {
		filePath = m_DataPath;
	}
	CString str = read(section,key,filePath);
	double dVal = 0.0;
	if (str != _T("")) {
		dVal =_ttof(str);
	}
	return dVal;
}

/// <summary>
/// 指定セクションのキーに浮動小数点値を設定
/// </summary>
/// <param name="section">セクション名</param>
/// <param name="key">キー名</param>
/// <param name="value">浮動小数点値</param>
/// <param name="filePath">データパス名</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CConfigrationIO::setDouble(CString section, CString key, double value, CString filePath)
{
	if (filePath == _T("")) {
		filePath = m_DataPath;
	}
	CString str;
	str.Format(_T("%lf"), value);
	return write(section,key,str,filePath);
}

/// <summary>
/// 指定セクションのキー値（文字列）を取得
/// </summary>
/// <param name="section">セクション名</param>
/// <param name="key">キー名</param>
/// <param name="filePath">データパス名</param>
/// <returns>値を返す</returns>
CString CConfigrationIO::getString(CString section, CString key, CString filePath)
{
	if (filePath == _T("")) {
		filePath = m_DataPath;
	}
	CString str = read(section,key,filePath);
	return str;
}

/// <summary>
/// 指定セクションのキーに文字列を設定
/// </summary>
/// <param name="section">セクション名</param>
/// <param name="key">キー名</param>
/// <param name="value">文字列</param>
/// <param name="filePath">データパス名</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CConfigrationIO::setString(CString section, CString key, CString& value, CString filePath)
{
	if (filePath == _T("")) {
		filePath = m_DataPath;
	}
	return write(section,key,value,filePath);
}

/// <summary>
/// 指定セクションのキーにを削除
/// </summary>
/// <param name="section">セクション名</param>
/// <param name="key">キー名</param>
/// <param name="filePath">データパス名</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CConfigrationIO::deleteKey(CString section, CString key, CString filePath)
{
	if (filePath == _T("")) {
		filePath = m_DataPath;
	}
	return WritePrivateProfileString(section, key, NULL, filePath);
}
