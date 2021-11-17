#include "stdafx.h"
#include "CLog.h"
#include "FileUtil.h"
#include <Shlwapi.h>
#include <iostream>
#include <iomanip>
#include <codecvt>
#include <fstream>
#include <locale>

//#define NotLog

/// <summary>
/// std::string��LPCWSTR�ɕϊ�����
/// </summary>
/// <param name="s">std :: string������</param>
/// <returns>LPCWSTR�ɕϊ����ꂽ�������Ԃ�</returns>
std::wstring CLog::s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buff = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buff, len);
	std::wstring r(buff);
	if (buff) {
		delete[] buff;
		buff = nullptr;
	}
	return r;
}

/**
 *  �G���[�����p���O�o��
 *  @param ex catch�����G���[���e
 */
void CLog::outputErrLog(exception ex)
{
#ifndef NotLog
    try {
        // �������ݐݒ�
        time_t t = time(nullptr);
        tm localTime;
        localtime_s(&localTime, &t);
        std::stringstream s;
        s << "20" << localTime.tm_year - 100;
        s << setw(2) << setfill('0') << localTime.tm_mon + 1;
        s << setw(2) << setfill('0') << localTime.tm_mday;

        CString sName = _T("Err_") + CString(s.str().c_str()) + ".log";
		CString logPath = CFileUtil::FilePathCombine(CFileUtil::GetModulePath(), _T("log"));
		if (!CFileUtil::fileExists(logPath)) {
			::CreateDirectory(logPath, NULL);
			if (!CFileUtil::fileExists(logPath)) {
				logPath = CFileUtil::GetModulePath();
			}
		}

        CString errlogpath = CFileUtil::FilePathCombine(CFileUtil::GetModulePath(),_T("\\errorlog_") + CString(s.str().c_str()) + _T(".txt"));
        std::wofstream  ofs(errlogpath, std::ios::app);
		ofs.imbue({ {}, new std::codecvt_utf8<wchar_t, 0x10FFFF, std::generate_header> });

        CString drive, dir, fname, ext;
		string file = __FILE__;
        CString path = CString(file.c_str());
        if (CFileUtil::splitPath(path, drive, dir, fname, ext)) {
            path = fname + ext;
        }
        ofs << L"[" << L"20" << localTime.tm_year - 100 << L"/" << setw(2) << setfill(L'0') << localTime.tm_mon + 1 << L"/" << setw(2) << setfill(L'0') << localTime.tm_mday;
        ofs << L" " << setw(2) << setfill(L'0') << localTime.tm_hour << L":" << setw(2) << setfill(L'0') << localTime.tm_min << L":" << setw(2) << setfill(L'0') << localTime.tm_sec << L"]";
        ofs << L" " << L"(" << path << L")";
        ofs << L" " << ex.what() << endl;

        ofs.close();
    }
    catch (exception ex)
    {
    }
#endif
}

/// <summary>
/// ����������
/// </summary>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CLog::Initiaize()
{
#ifndef NotLog
    try
    {
        m_sysLogLevel = LOGLEVEL::Non;
        m_linebuffs.clear();

		CString path = CFileUtil::FilePathCombine(CFileUtil::GetModulePath(), _T("log"));
		if (!CFileUtil::fileExists(path)) {
			::CreateDirectory(path, NULL);
			if (!CFileUtil::fileExists(path)) {
				path = CFileUtil::GetModulePath();
			}
		}

        m_logPath = path;
        m_prefix = "Log_";
        int level = Error | Warning;
        // CSystemConfigrationIO����̃��O���x�����V�X�e�����x���ɕϊ�
        if (level > 0)
        {
            m_sysLogLevel = LOGLEVEL::Non;

            if ((level & 0x10) == 0x10)        // trace
            {
                m_sysLogLevel = LOGLEVEL::Trace;
            }
            else if ((level & 0x04) == 0x04)        // Details
            {
                m_sysLogLevel = (LOGLEVEL)(LOGLEVEL::Error | LOGLEVEL::Warning | LOGLEVEL::Details);
            }
            else if ((level & 0x02) == 0x02)        // Warning 
            {
                m_sysLogLevel = (LOGLEVEL)(LOGLEVEL::Error | LOGLEVEL::Warning);
            }
            else if ((level & 0x01) == 0x01)             // Error
            {
                m_sysLogLevel = (LOGLEVEL)(LOGLEVEL::Error);
            }

            if ((level & 0x08) == 0x08)        // Info
            {
                m_sysLogLevel = (LOGLEVEL)(m_sysLogLevel | LOGLEVEL::Info);
            }
        }
		deleteLogFile(7);
		m_sysLogLevel = (LOGLEVEL)(LOGLEVEL::Error | LOGLEVEL::Warning | LOGLEVEL::Details | LOGLEVEL::Info);

	}
    catch (exception ex)
    {
        outputErrLog(ex);
        return false;
    }
#endif
    return true;
}

/// <summary>
/// �V�X�e���̃��O���x���ݒ�
/// </summary>
/// <param name="level">���O���x��</param>
void CLog::setSystemLogLevel(LOGLEVEL level)
{
#ifndef NotLog
    m_sysLogLevel = level;
#endif
}

/// <summary>
/// �o�͔���
/// </summary>
/// <param name="level">�Ώۃ��x��</param>
/// <returns>�o�͉̏ꍇ��true�A�����łȂ��ꍇ��false��Ԃ�</returns>
bool CLog::checkLogLevel(LOGLEVEL level)
{
    if ((level & m_sysLogLevel) != 0x00)
    {
        return true;
    }
    return false;
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
CLog::CLog()
{
#ifndef NotLog
    Initiaize();
#endif
}

/// <summary>
/// �������R���X�g���N�^
/// </summary>
/// <param name="path">���O�i�[��t�H���_�ւ̃p�X</param>
/// <param name="prefix">�t�@�C�����v���t�B�b�N�X</param>
CLog::CLog(CString path, CString prefix)
{
#ifndef NotLog
    Initiaize();
    if (prefix != "")
    {
        m_prefix = prefix;
    }
    setLogPath(path);
#endif
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
CLog::~CLog()
{
#ifndef NotLog
    if (m_linebuffs.size() > 0)
    {
        m_linebuffs.clear();
    }
#endif
}

/// <summary>
/// �t�@�C�����v���t�B�b�N�X�̐ݒ�
/// </summary>
/// <param name="prefix">�t�@�C�����v���t�B�b�N�X</param>
void CLog::setFilePrefix(CString prefix)
{
#ifndef NotLog
    m_prefix = prefix;
#endif
}

/// <summary>
/// �t�@�C�����v���t�B�b�N�X�̎擾
/// </summary>
/// <returns>�t�@�C�����v���t�B�b�N��Ԃ�</returns>
CString CLog::getFilePrefix()
{
    return m_prefix;
}

/// <summary>
/// ���O�t�@�C�����̎擾
/// </summary>
/// <returns>���O�t�@�C������Ԃ�</returns>
CString CLog::getLogName()
{
    time_t t = time(nullptr);
    tm localTime;
    localtime_s(&localTime ,&t);
    std::stringstream s;
    s << "20" << localTime.tm_year - 100;
    s << setw(2) << setfill('0') << localTime.tm_mon + 1;
    s << setw(2) << setfill('0') << localTime.tm_mday;

    CString sName = getFilePrefix() + CString(s.str().c_str()) + ".log";
    return sName;
}

/// <summary>
/// ���O�t�@�C���o�̓f�B���N�g���̐ݒ�
/// </summary>
/// <param name="path">���O�t�@�C���o�̓f�B���N�g��</param>
void CLog::setLogPath(CString path)
{
#ifndef NotLog
    m_logPath = path;
#endif
}

/// <summary>
/// ���O�t�@�C���o�̓f�B���N�g���̎擾
/// </summary>
/// <returns>���O�t�@�C���o�̓f�B���N�g����Ԃ�</returns>
CString CLog::getLogPath()
{
    return m_logPath;
}

/// <summary>
/// ���O�t�@�C���t���p�X���̎擾
/// </summary>
/// <returns>���O�t�@�C���t���p�X����Ԃ�</returns>
CString CLog::getLogPathName()
{
	CString logPath = CFileUtil::FilePathCombine(getLogPath(), getLogName());
    return logPath;
}

/// <summary>
/// ���O�t�@�C���폜
/// </summary>
/// <param name="saveday">�ۑ��L������</param>
/// @remark �Ăяo�������ۑ��L�����ԈȑO�̃t�@�C�����폜����B
void CLog::deleteLogFile(int saveday)
{
#ifndef NotLog
    try {
        time_t t = time(nullptr);
        tm localtm;
        localtime_s(&localtm ,&t);
        time_t ltime = mktime(&localtm);

        CString search_name = CFileUtil::FilePathCombine(getLogPath(), getFilePrefix() + _T("????????.log"));
        HANDLE hFind;
        WIN32_FIND_DATA win32fd;
        hFind = FindFirstFile(search_name, &win32fd);
        if (hFind == (HANDLE)-1) {
            return;
        }
        do {
            if (win32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                /* �f�B���N�g���̏ꍇ�͉������Ȃ� */
            }
            else {
                FILETIME ft = win32fd.ftCreationTime;
                LONGLONG ll;
                ll = ((LONGLONG)ft.dwHighDateTime << 32) + ft.dwLowDateTime;
                time_t ftime = (time_t)((ll - 116444736000000000) / 10000000);

                double diff = difftime(ltime, ftime);
                diff /= 60.0 * 60.0 * 24.0;
                if (INT(diff+0.5) > saveday) {
                    CString fname = CFileUtil::FilePathCombine(getLogPath(), win32fd.cFileName);
                    CFileUtil::fileDelete(fname);
                }
            }
        } while (FindNextFile(hFind, &win32fd));

        FindClose(hFind);
    }
    catch (exception ex)
    {
        outputErrLog(ex);
    }
#endif
}
/// <summary>
 /// ���C���o��
 /// </summary>
 /// <param name="ofsswLog">StreamWriter�C���X�^���X</param>
 /// <param name="level">���O���x��</param>
 /// <param name="prm">�o�͕�����</param>
void CLog::writeLine(std::wofstream  &ofsswLog, LOGLEVEL level, CString prm)
{
#ifndef NotLog
    ios::iostate stat = ofsswLog.exceptions();
    ofsswLog.exceptions(ios::failbit | ios::badbit);
    try
    {
        CString label;
        if ((level & LOGLEVEL::Error) == LOGLEVEL::Error)
        {
            label = "Error      ";
        }
        else if ((level & LOGLEVEL::Warning) == LOGLEVEL::Warning)
        {
            label = "Warning    ";
        }
        else if ((level & LOGLEVEL::Details) == LOGLEVEL::Details)
        {
            label = "Details    ";
        }
        else
        {
            label = "Information";

        }

        time_t t = time(nullptr);
        tm localTime;
        localtime_s(&localTime, &t);

		CString wrk = label + L":" + prm;
		int bufSZ = wrk.GetLength() + 2;
		TCHAR *buf = new TCHAR[bufSZ];
		_tcscpy_s(buf, bufSZ, wrk);

        ofsswLog << L"[" << "20" << localTime.tm_year - 100 << L"/" << setw(2) << setfill(L'0') << localTime.tm_mon + 1 << L"/" << setw(2) << setfill(L'0') << localTime.tm_mday;
        ofsswLog << L" " << setw(2) << setfill(L'0') << localTime.tm_hour << ":" << setw(2) << setfill(L'0') << localTime.tm_min << L":" << setw(2) << setfill(L'0') << localTime.tm_sec << "]";
        ofsswLog << L":" << buf << std::endl;
		delete buf;

        // ��O�����̃`�F�b�N
        std::ios_base::iostate state = ofsswLog.rdstate();
        if (state & std::ios_base::failbit) {
            exception ex("ios_base::failbit set : iostream stream error");
            throw ex;
        }
        else if (state & std::ios_base::badbit) {
            exception ex("ios_base::badbit set : iostream stream error");
            throw ex;
        }
        ofsswLog.exceptions(stat);
    }
    catch (exception ex)
    {
        outputErrLog(ex);
        ofsswLog.exceptions(stat);
    }
#endif
}

/// <summary>
/// ���O�̏o��
/// </summary>
/// <param name="level">���O���x��</param>
/// <param name="prm">�o�͕�����</param>
void CLog::logWrite(LOGLEVEL level, CString prm)
{
#ifndef NotLog
    if (!checkLogLevel(level))
    {
        return;
    }
	CString logPath = getLogPathName();
    try
    {
        std::wofstream  ofsLog(logPath, std::ios::app);
		ofsLog.imbue({ {}, new std::codecvt_utf8<wchar_t, 0x10FFFF, std::generate_header> });
		writeLine(ofsLog, level, prm);
        ofsLog.close();
    }
    catch (exception ex)
    {
        outputErrLog(ex);
    }
#endif
}

/// <summary>
/// ���O�̏o��
/// </summary>
/// <param name="level">���O���x��</param>
/// <param name="prms">�o�͕�����Q</param>
/// @remark �o�͕�����Q�͍s�P��
void CLog::logWrite(LOGLEVEL level, vector<CString> prms)
{
#ifndef NotLog
    if (!checkLogLevel(level))
    {
        return;
    }
    if (prms.size() > 0)
    {
        try
        {
			CString logPath = getLogPathName();
            std::wofstream  ofsLog(logPath, std::ios::app);
			ofsLog.imbue({ {}, new std::codecvt_utf8<wchar_t, 0x10FFFF, std::generate_header> });
			for(CString str : prms)
            {
                writeLine(ofsLog, level, str);
            }
            ofsLog.close();
        }
        catch (exception ex)
        {
            outputErrLog(ex);
        }
    }
#endif
}

/// <summary>
/// ���O�o�̓��C���o�b�t�@�̃N���A
/// </summary>
void CLog::clrLineBuff()
{
#ifndef NotLog
    m_linebuffs.clear();
#endif
}

/// <summary>
/// ���O�o�̓��C���o�b�t�@�ւ̏o�͕�����̓o�^
/// </summary>
/// <param name="buff">�o�͕�����</param>
void CLog::setLineBuff(CString buff)
{
#ifndef NotLog
    m_linebuffs.push_back(buff);
#endif
}

/// <summary>
/// ���O�o�̓��C���o�b�t�@�̃��O�o��
/// </summary>
/// <param name="level">���O���x��</param>
/// @remark ���O�o�̓��C���o�b�t�@�̓N���A�����
void CLog::writeLineBuff(LOGLEVEL level)
{
#ifndef NotLog
    if (!checkLogLevel(level))
    {
        return;
    }
    if (m_linebuffs.size() > 0)
    {
        try
        {
			CString logPath = getLogPathName();
            std::wofstream  ofsLog(logPath, std::ios::app);
			ofsLog.imbue({ {}, new std::codecvt_utf8<wchar_t, 0x10FFFF, std::generate_header> });
			for (CString str : m_linebuffs)
            {
                writeLine(ofsLog, level, str);
            }
            ofsLog.close();
            clrLineBuff();
        }
        catch (exception ex)
        {
            outputErrLog(ex);
        }
    }
#endif
}
