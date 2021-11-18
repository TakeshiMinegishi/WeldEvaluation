#pragma once
#pragma warning(disable:4251)

// �C���N���[�h
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

// ���O���
using namespace std;

    /// <summary>
    /// ���O�o�͗p�N���X
    /// </summary>
class  CLog
{
public:
    /// <summary>
    /// ���O�o�̓��[�h�̗񋓌^
    /// </summary>
    enum LOGLEVEL
    {
        Non = 0x00,         ///< ���g�p
        Error = 0x01,       ///< �G���[�o��
        Warning = 0x02,     ///< �x��
        Details = 0x04,     ///< �ڍ�
        Info = 0x08,        ///< ��񓙏o��
        Trace = 0x0F,       ///< �S���
        Debug = 0x10        ///< �f�o�b�O���[�h���̂ݏo��
    };

private:
    vector<CString>  m_linebuffs;    ///< ���C���o�b�t�@
    LOGLEVEL        m_sysLogLevel;  ///< �V�X�e���̃��O���x��
    CString          m_logPath;      ///< ���O�o�̓f�B���N�g��
    CString          m_prefix;       ///< �t�@�C�����v���t�B�b�N�X

private:
    bool Initiaize();
    void outputErrLog(exception ex);
    bool checkLogLevel(LOGLEVEL level);
    CString PathConbine(CString path, CString name);
	std::wstring s2ws(const std::string& s);


public:
    CLog();
    CLog(CString	path, CString prefix);
    ~CLog();

    void setFilePrefix(CString prefix);
	CString getFilePrefix();

    void setSystemLogLevel(LOGLEVEL level);
	CString getLogName();
    void setLogPath(CString	path);
	CString getLogPath();
	CString getLogPathName();

    void deleteLogFile(int saveday);
    void writeLine(std::wofstream & ofsswLog, LOGLEVEL level, CString prm);
    void logWrite(LOGLEVEL level, CString prm);
    void logWrite(LOGLEVEL level, vector<CString> prms);
    void clrLineBuff();
    void setLineBuff(CString buff);
    void writeLineBuff(LOGLEVEL level);
};


