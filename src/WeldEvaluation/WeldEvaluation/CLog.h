#pragma once
#pragma warning(disable:4251)

// インクルード
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

// 名前空間
using namespace std;

    /// <summary>
    /// ログ出力用クラス
    /// </summary>
class  CLog
{
public:
    /// <summary>
    /// ログ出力モードの列挙型
    /// </summary>
    enum LOGLEVEL
    {
        Non = 0x00,         ///< 未使用
        Error = 0x01,       ///< エラー出力
        Warning = 0x02,     ///< 警告
        Details = 0x04,     ///< 詳細
        Info = 0x08,        ///< 情報等出力
        Trace = 0x0F,       ///< 全情報
        Debug = 0x10        ///< デバッグモード時のみ出力
    };

private:
    vector<CString>  m_linebuffs;    ///< ラインバッファ
    LOGLEVEL        m_sysLogLevel;  ///< システムのログレベル
    CString          m_logPath;      ///< ログ出力ディレクトリ
    CString          m_prefix;       ///< ファイル名プレフィックス

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


