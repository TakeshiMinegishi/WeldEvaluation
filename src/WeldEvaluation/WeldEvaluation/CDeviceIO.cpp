#include "stdafx.h"
#include "CDeviceIO.h"
#include "CLog.h"
#include "CDio.h"

#define		MovePort		0
#define		CtrlPort		1
#define		InpputPort		0
#define		TimeOverCount	12000

CDeviceIO::CDeviceIO()
{
	m_demoMode = false;
}

CDeviceIO::~CDeviceIO()
{

}

void CDeviceIO::logOut(CString filePath, long lineNo, CString msg)
{
	CLog log;
	CString ErrMsg;
	ErrMsg.Format(_T(" File:%s Line:%ld:%s"), (LPCTSTR)filePath, lineNo, (LPCTSTR)msg);
	log.logWrite(CLog::LOGLEVEL::Error, ErrMsg);
	AfxMessageBox(ErrMsg, MB_OK | MB_ICONSTOP);
}

bool CDeviceIO::IsReady(int id)
{
	bool bResult = false;
	BYTE InpPortData;
	int wcnt = 0;
	int ret;
	while (wcnt < TimeOverCount) {
		ret = DioInpByte((short)id, 0, &InpPortData);
		if ((InpPortData & 0x0f) == 0x01) {
			bResult = true;
			break;
		}
		Sleep(10);
		wcnt++;
	}
	return bResult;
}

bool CDeviceIO::PortOut(int id, int portNo, int value) 
{
	bool bResult = false;
	if (m_demoMode) {
		Sleep(300);
		bResult = true;
	}
	else {
		//-----------------------------
		// ポート出力
		//-----------------------------
		long ret = DioOutByte((short)id, (short)portNo, (BYTE)value);

		//-----------------------------
		// エラー処理
		//-----------------------------
		CString ErrMsg = _T("");
		if (ret == DIO_ERR_SUCCESS) {
			Sleep(100);
			if (IsReady(id)) {
				bResult = true;
			}
			else {
				ErrMsg.Format(_T("The signal did not go ready(Time over)."));
			}
		}
		else {
			char	szError[256];
			DioGetErrorString(ret, szError);
			ErrMsg.Format(_T("Ret = %d : %S"), ret, szError);
		}

		if (!bResult) {
			CLog log;
			CString msg;
			msg.Format(_T("CDeviceIO::PortOut():ID(%d):%s"), id, (LPCTSTR)ErrMsg);
			logOut(CString(__FILE__), __LINE__, msg);
		}
	}
	return bResult;
}

bool CDeviceIO::sendPals(int id)
{
	bool bResult = false;
	bResult = PortOut(id, CtrlPort, 0);
	if (bResult) {
		Sleep(100);
		bResult = PortOut(id, CtrlPort, 1);
	}
	if (!bResult) {
		CLog log;
		CString msg;
		msg.Format(_T("CDeviceIO::sendPals():ID(%d)"), id);
		logOut(CString(__FILE__), __LINE__, msg);
	}
	return bResult;
}

int CDeviceIO::Init(CString DeviceName)
{
	long			ret;
	const size_t	textSize = 100;
	char			pszDeviceName[textSize];
	short			Id;

	if (!DeviceName.IsEmpty()) {
		m_demoMode = false;
		//-----------------------------
		// 文字列変換
		//-----------------------------
		WideCharToMultiByte(CP_ACP, 0, DeviceName.GetString(), -1, pszDeviceName, textSize, NULL, NULL);
		//-----------------------------
		// 初期化処理を行ない、IDを取得する
		//-----------------------------
		ret = DioInit(pszDeviceName, &Id);
		//-----------------------------
		// エラー処理
		//-----------------------------
		if (ret != DIO_ERR_SUCCESS) {
			char	szError[256];
			DioGetErrorString(ret, szError);
			CString msg;
			msg.Format(_T("CDeviceIO::Init():DeviceName(%s):Ret = %d : %S"), (LPCTSTR)DeviceName, ret, (LPCTSTR)szError);
			logOut(CString(__FILE__), __LINE__, msg);
			return -1;
		}
		else {
#if 0
			ret = DioResetDevice(Id);
			if (ret != DIO_ERR_SUCCESS) {
				char	szError[256];
				DioGetErrorString(ret, szError);
				CString msg;
				msg.Format(_T("CDeviceIO::Init():DeviceName(%s): ID(%d) :Ret = %d : %S"), DeviceName, Id, ret, szError);
				logOut(CString(__FILE__), __LINE__, msg);
				return -1;
			}
#else
			if (!IsReady(Id)) {
				Id = -1;
			}
#endif
		}
	}
	else {
		m_demoMode = true;
		Id = 999;
	}
	return (int)Id;
}

void CDeviceIO::Close(int id)
{
	if (!m_demoMode) {
		DioExit((short)id);
	}
}

bool CDeviceIO::Move(int id, int pos)
{
	if ((pos < 0) || (pos > 0x19)) {
		return false;
	}

	bool bResult = PortOut(id, MovePort, pos);
	if (bResult) {
		Sleep(150);
		bResult = sendPals(id);
	}
	if (!bResult) {
		CString msg;
		msg.Format(_T("CDeviceIO::Move():ID(%d):Position(%d)"),id,pos);
		logOut(CString(__FILE__), __LINE__, msg);
	}
	return bResult;
}

bool CDeviceIO::ToHome(int id)
{
	bool bResult = Move(id, 0);
	if (!bResult) {
		CLog log;
		CString msg;
		msg.Format(_T("CDeviceIO::ToHome():ID(%d)"), id);
		logOut(CString(__FILE__), __LINE__, msg);
	}
	return bResult;
}