#pragma once
class CDeviceIO
{
private:
	bool	m_demoMode;		///< ƒfƒ‚ƒ‚[ƒh

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

