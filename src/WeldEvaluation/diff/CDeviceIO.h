#pragma once

/// <summary>
/// 機器IOクラス
/// </summary>
class CDeviceIO
{
private:
	bool	m_demoMode;		///< デモモード

private:
	bool IsReady(int id);
	bool PortOut(int id, int portNo, int value);
	bool sendPals(int id);
	void logOut(CString filePath, long lineNo, CString msg);

public:
	CDeviceIO();
	~CDeviceIO();

	int Init(CString DeviceName);
	bool ToHome(int id);
	bool Move(int id, int pos);
	void Close(int id);
};

