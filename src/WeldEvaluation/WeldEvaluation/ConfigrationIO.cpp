#include "stdafx.h"
#include "ConfigrationIO.h"
#include "FileUtil.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
CConfigrationIO::CConfigrationIO(void)
{
	Initialize();
}

/// <summary>
/// �������R���X�g���N�^
/// </summary>
/// <param name="pathname">�t�@�C���p�X</param>
CConfigrationIO::CConfigrationIO(CString pathname)
{
	Initialize();
	m_DataPath = pathname;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
CConfigrationIO::~CConfigrationIO(void)
{
}

/// <summary>
/// ���������s��
/// </summary>
void CConfigrationIO::Initialize()
{
	m_DataPath = "";
}


/// <summary>
/// �ݒ�t�@�C���̃p�X��ݒ�
/// </summary>
/// <param name="pathName">�t�@�C���p�X</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CConfigrationIO::setFilePath(CString pathName)
{
	m_DataPath = pathName;
	return CFileUtil::fileExists(m_DataPath);
}

/// <summary>
/// �ݒ�t�@�C���̃p�X���擾
/// </summary>
/// <returns>�t�@�C���p�X��Ԃ�</returns>
CString CConfigrationIO::getFilePath(void)
{
	return m_DataPath;
}

/// <summary>
/// �w��Z�N�V�����̃L�[�l���擾
/// </summary>
/// <param name="section">�Z�N�V������</param>
/// <param name="key">�L�[��</param>
/// <param name="filePath">�f�[�^�p�X��</param>
/// <returns>�l��Ԃ�</returns>
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
/// �w��Z�N�V�����̃L�[�ɒl��ݒ�
/// </summary>
/// <param name="section">�Z�N�V������</param>
/// <param name="key">�L�[��</param>
/// <param name="value">�l</param>
/// <param name="filePath">�f�[�^�p�X��</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CConfigrationIO::write(CString section, CString key, CString value, CString filePath)
{
	bool bResult = false;
	if (0 != ::WritePrivateProfileString(section, key, value, filePath)) {
		bResult = true;
	}
	return bResult;
}

/// <summary>
/// �w��Z�N�V�����̃L�[�l�i�����j���擾
/// </summary>
/// <param name="section">�Z�N�V������</param>
/// <param name="key">�L�[��</param>
/// <param name="default">�f�t�H���g�l</param>
/// <param name="filePath">�f�[�^�p�X��</param>
/// <returns>�l��Ԃ�</returns>
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
/// �w��Z�N�V�����̃L�[�l�i�����j���擾
/// </summary>
/// <param name="section">�Z�N�V������</param>
/// <param name="key">�L�[��</param>
/// <param name="filePath">�f�[�^�p�X��</param>
/// <returns>�l��Ԃ�</returns>
int CConfigrationIO::getInt(CString section, CString key, CString filePath)
{
	return getInt(section,key, 0, filePath);
}

/// <summary>
/// �w��Z�N�V�����̃L�[�ɐ����l��ݒ�
/// </summary>
/// <param name="section">�Z�N�V������</param>
/// <param name="key">�L�[��</param>
/// <param name="value">�����l</param>
/// <param name="filePath">�f�[�^�p�X��</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
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
/// �w��Z�N�V�����̃L�[�l�i���������_�j���擾
/// </summary>
/// <param name="section">�Z�N�V������</param>
/// <param name="key">�L�[��</param>
/// <param name="filePath">�f�[�^�p�X��</param>
/// <returns>�l��Ԃ�</returns>
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
/// �w��Z�N�V�����̃L�[�ɕ��������_�l��ݒ�
/// </summary>
/// <param name="section">�Z�N�V������</param>
/// <param name="key">�L�[��</param>
/// <param name="value">���������_�l</param>
/// <param name="filePath">�f�[�^�p�X��</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
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
/// �w��Z�N�V�����̃L�[�l�i������j���擾
/// </summary>
/// <param name="section">�Z�N�V������</param>
/// <param name="key">�L�[��</param>
/// <param name="filePath">�f�[�^�p�X��</param>
/// <returns>�l��Ԃ�</returns>
CString CConfigrationIO::getString(CString section, CString key, CString filePath)
{
	if (filePath == _T("")) {
		filePath = m_DataPath;
	}
	CString str = read(section,key,filePath);
	return str;
}

/// <summary>
/// �w��Z�N�V�����̃L�[�ɕ������ݒ�
/// </summary>
/// <param name="section">�Z�N�V������</param>
/// <param name="key">�L�[��</param>
/// <param name="value">������</param>
/// <param name="filePath">�f�[�^�p�X��</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CConfigrationIO::setString(CString section, CString key, CString& value, CString filePath)
{
	if (filePath == _T("")) {
		filePath = m_DataPath;
	}
	return write(section,key,value,filePath);
}

/// <summary>
/// �w��Z�N�V�����̃L�[�ɂ��폜
/// </summary>
/// <param name="section">�Z�N�V������</param>
/// <param name="key">�L�[��</param>
/// <param name="filePath">�f�[�^�p�X��</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CConfigrationIO::deleteKey(CString section, CString key, CString filePath)
{
	if (filePath == _T("")) {
		filePath = m_DataPath;
	}
	return WritePrivateProfileString(section, key, NULL, filePath);
}
