#pragma once
class CConfigrationIO
{
private:
	CString					m_DataPath;     ///< テーブルへのパス

private:
	void Initialize();

protected:
	CString read(CString section, CString key, CString filePath);
	bool write(CString section, CString key, CString value, CString filePath);

public:
	CConfigrationIO(void);
	CConfigrationIO(CString pathname);
	~CConfigrationIO(void);

	bool setFilePath(CString pathName);
	CString getFilePath(void);

	int getInt(CString section, CString key, CString filePath=_T(""));
	bool setInt(CString section, CString key, int value, CString filePath=_T(""));
	double getDouble(CString section, CString key, CString filePath=_T(""));
	bool setDouble(CString section, CString key, double value, CString filePath=_T(""));
	CString getString(CString section, CString key, CString filePath=_T(""));
	bool setString(CString section, CString key, CString& value, CString filePath=_T(""));
};

