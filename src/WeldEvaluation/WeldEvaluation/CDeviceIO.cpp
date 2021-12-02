#include "stdafx.h"
#include "CDeviceIO.h"
#include "CLog.h"
#include "CDio.h"

#define		MovePort		0
#define		CtrlPort		1
#define		InpputPort		0
#define		TimeOverCount	12000

/// <summary>
/// コンストラクタ
/// </summary>
CDeviceIO::CDeviceIO()
{
	m_demoMode = false;
}

/// <summary>
/// デストラクタ
/// </summary>
CDeviceIO::~CDeviceIO()
{

}

/// <summary>
/// ログ出力
/// </summary>
/// <param name="filePath">ソースファイル名</param>
/// <param name="lineNo">ソースライン番号</param>
/// <param name="msg">メッセージ</param>
void CDeviceIO::logOut(CString filePath, long lineNo, CString msg)
{
	CLog log;
	CString ErrMsg;
	ErrMsg.Format(_T(" File:%s Line:%ld:%s"), (LPCTSTR)filePath, lineNo, (LPCTSTR)msg);
	log.logWrite(CLog::LOGLEVEL::Error, ErrMsg);
	AfxMessageBox(ErrMsg, MB_OK | MB_ICONSTOP);
}

/// <summary>
/// レディー判定
/// </summary>
/// <param name="id">機器ＩＤ</param>
/// <returns>レディー状態ならばtrue、そうで無ければfalseを返す</returns>
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

/// <summary>
/// ポート出力
/// </summary>
/// <param name="id">機器ＩＤ</param>
/// <param name="portNo">ポート番号</param>
/// <param name="value">設定値</param>
/// <returns>成功した場合はtrue、そう無ければfalseを返す</returns>
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

/// <summary>
/// パルス出力
/// </summary>
/// <param name="id">機器ＩＤ</param>
/// <returns>成功した場合はtrue、そう無ければfalseを返す</returns>
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

/// <summary>
/// 機器の初期化
/// </summary>
/// <param name="DeviceName">機器名</param>
/// <returns>成功した場合は機器IDを返す、失敗した場合は-1を返す</returns>
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
			msg.Format(_T("CDeviceIO::Init():DeviceName(%s):Ret = %d : %s"), static_cast<LPCWSTR>(DeviceName), ret, static_cast<LPCWSTR>(CString(szError)));
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

/// <summary>
/// 機器のクローズ
/// </summary>
void CDeviceIO::Close(int id)
{
	if (!m_demoMode) {
		DioExit((short)id);
	}
}

/// <summary>
/// カメラの移動
/// </summary>
/// <param name="id">機器ID</param>
/// <param name="pos">移動位置</param>
/// <returns>成功した場合は機器IDを返す、失敗した場合は-1を返す</returns>
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

/// <summary>
/// カメラをホームポジションへ移動
/// </summary>
/// <param name="id">機器ID</param>
/// <returns>成功した場合は機器IDを返す、失敗した場合は-1を返す</returns>
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