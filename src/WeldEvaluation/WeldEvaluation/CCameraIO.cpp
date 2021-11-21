#include "stdafx.h"
#include "CCameraIO.h"
#include "FileUtil.h"
#include "ScanDataIO.h"


/// <summary>
/// コンストラクタ
/// </summary>
CCameraIO::CCameraIO()
{
	CameraPrmInit();
	m_width				= 2048;
	m_height			= 1088;
	m_band				= 150;
	m_integrationTime	= 1.0;
	m_bDemoMode			= false;
	InitLlogger();

}

/// <summary>
/// 初期化コンストラクタ
/// </summary>
/// <param name="widht">データ幅</param>
/// <param name="height">データ高さ</param>
/// <param name="band">バンドサイズ</param>
CCameraIO::CCameraIO(int widht, int height, int band)
{
	InitLlogger();
	CameraPrmInit();
	m_width				= widht;
	m_height			= height;
	m_band				= band;
	m_integrationTime	= 1.0;
	m_bDemoMode			= false;
}

/// <summary>
/// デストラクタ
/// </summary>
CCameraIO::~CCameraIO()
{
	FleeCameraPrm();
}

/// <summary>
/// カメラパラメータの初期化
/// </summary>
void CCameraIO::CameraPrmInit()
{
	m_handle			= 0x0;
	m_cube				= { 0 };
	m_correction_matrix = { 0 };
	m_cube_format		= { 0 };
	m_dark_reference	= { 0 };
}

/// <summary>
/// ロガーの初期化
/// </summary>
bool CCameraIO::InitLlogger()
{
	CLog log;
	CString logPath = log.getLogPath();
	HSI_RETURN return_val = commonInitializeLogger(logPath, LV_WARNING);
	if (HSI_OK != return_val)
	{
		CScanDataIO::errorLog(CString(__FILE__), __LINE__, _T("InitializeLogger"), return_val);
		return false;
	}
	return true;
}

/// <summary>
/// カメラのオープン
/// </summary>
/// <param name="snapscan_file">スナップスキャンファイルへのパス</param>
/// <param name="dummyApi">ダミーAPIフラグ</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CCameraIO::Open(CString snapscan_file, bool dummyApi/* = true*/)
{
	HSI_RETURN return_val;
	{
		int major, minor, patch, build;
		return_val = GetAPIVersion(&major, &minor, &patch, &build);
		if (HSI_OK != return_val)
		{
			CScanDataIO::errorLog(CString(__FILE__), __LINE__, _T("OpenDevice"), return_val);
			return false;
		}
		CString version;
		version.Format(_T("API Version %d.%d.%d.%d\n"), major, minor, patch, build);
		CScanDataIO::writeLog(CLog::LOGLEVEL::Info, CString(__FILE__), __LINE__, version);
	}

	if (dummyApi == true) {
		m_bDemoMode = true;
	}
	else {
		m_bDemoMode = false;
	}

#ifdef _DEBUG
	CScanDataIO::writeLog(CLog::LOGLEVEL::Info, CString(__FILE__), __LINE__, L"Connecting device ...");
#endif
	m_handle = 0x0;
	return_val = OpenDevice(&m_handle, snapscan_file, dummyApi);
	if (HSI_OK != return_val)
	{
		CScanDataIO::errorLog(CString(__FILE__), __LINE__, _T("OpenDevice"), return_val);
		return false;
	}

#ifdef _DEBUG
	SystemProperties system_properties = { 0 };
	return_val = GetSystemProperties(m_handle, &system_properties);
	if (HSI_OK == return_val)
	{
		CString msg;
		msg.Format(_T("Max cube dimensions : %d x %d"), system_properties.cube_height_max, system_properties.cube_width_max);
		CScanDataIO::writeLog(CLog::LOGLEVEL::Info, CString(__FILE__), __LINE__, msg);
		msg.Format(_T("Memory requiremenets (Mb) : %f"), 0.000001 * (double)(system_properties.memory_requirements_bytes));
		CScanDataIO::writeLog(CLog::LOGLEVEL::Info, CString(__FILE__), __LINE__, msg);
		msg.Format(_T("Max pixel step: %d"), system_properties.pixel_step_max);
		CScanDataIO::writeLog(CLog::LOGLEVEL::Info, CString(__FILE__), __LINE__, msg);
		msg.Format(_T("Max stage position: %d"), system_properties.stage_position_px_max);
		CScanDataIO::writeLog(CLog::LOGLEVEL::Info, CString(__FILE__), __LINE__, msg);
	}
#endif

	// Width,Height,Band の設定
	if (!setFormat(m_width, m_height, m_band)) {
		CScanDataIO::writeLog(CLog::LOGLEVEL::Error, CString(__FILE__), __LINE__, _T("Open Error"));
		return false;
	}

	// Initialize
#ifdef _DEBUG
	CScanDataIO::writeLog(CLog::LOGLEVEL::Info, CString(__FILE__), __LINE__, _T("Initializing ..."));
#endif
	return_val = Initialize(m_handle);
	if (HSI_OK != return_val)
	{
		CScanDataIO::errorLog(CString(__FILE__), __LINE__, _T("Initialize"), return_val);
		return false;
	}

	// allocate cube data format(mandatory)
	return_val = GetOutputCubeDataFormat(m_handle, &m_cube_format);
	if (HSI_OK != return_val)
	{
		CScanDataIO::errorLog(CString(__FILE__), __LINE__, _T("GetOutputCubeDataFormat"), return_val);
		return false;
	}

	// allocate cube (mandatory)
	commonDeallocateCube(&m_cube);
	m_cube = { 0 };
	return_val = commonAllocateCube(&m_cube, m_cube_format);
	if (HSI_OK != return_val)
	{
		CScanDataIO::errorLog(CString(__FILE__), __LINE__, _T("AllocateCube (cube)"), return_val);
		return false;
	}
return true;
}

/// <summary>
/// カメラのパラメータ開放
/// </summary>
void CCameraIO::FleeCameraPrm()
{
	HSI_RETURN return_val;
	return_val = commonDeallocateCube(&m_cube);
	if (HSI_OK != return_val)
	{
		CScanDataIO::errorLog(CString(__FILE__), __LINE__, _T("DeallocateCubeData"), return_val);
	}

	// deallocate data format
	return_val = commonDeallocateCubeDataFormat(&m_cube_format);
	if (HSI_OK != return_val)
	{
		CScanDataIO::errorLog(CString(__FILE__), __LINE__, _T("DeallocateCubeDataFormat"), return_val);
	}

	// deallocate dark_reference
	return_val = commonDeallocateFrame(&m_dark_reference);
	if (HSI_OK != return_val)
	{
		CScanDataIO::errorLog(CString(__FILE__), __LINE__, _T("DeallocateFrame (dark_reference)"), return_val);
	}

	// deallocate correction matrix
	return_val = DeallocateCorrectionMatrix(&m_correction_matrix);
	if (HSI_OK != return_val)
	{
		CScanDataIO::errorLog(CString(__FILE__), __LINE__, _T("DeallocateCorrectionMatrix"), return_val);
	}
}

/// <summary>
/// カメラのクローズ
/// </summary>
void CCameraIO::Close()
{
	FleeCameraPrm();

	HSI_RETURN return_val = CloseDevice(&m_handle);
	if (HSI_OK != return_val)
	{
		CScanDataIO::errorLog(CString(__FILE__), __LINE__, _T("CloseDevice"), return_val);
	}
}

/// <summary>
/// システムフォーマットの取得
/// </summary>
/// <param name="widht_max">最大幅</param>
/// <param name="height_max">最大高</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CCameraIO::getSystemFormat(int &widht_max, int &height_max)
{
	if (m_handle == 0x00) {
		return false;
	}
	SystemProperties system_properties = { 0 };
	HSI_RETURN return_val = GetSystemProperties(m_handle, &system_properties);
	if (HSI_OK != return_val)
	{
		CScanDataIO::errorLog(CString(__FILE__), __LINE__, _T("GetSystemProperties"), return_val);
		return false;
	}
	widht_max	= system_properties.cube_height_max;
	height_max = system_properties.cube_width_max;
	return true;
}

/// <summary>
/// フォーマットの設定
/// </summary>
/// <param name="width">幅</param>
/// <param name="height">高さ</param>
/// <param name="band">バンド</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CCameraIO::setFormat(int width, int height, int band)
{
	if (m_handle == 0x00) {
		m_width = width;
		m_height = height;
		m_band = band;
	}
	else {
		ConfigurationParameters config = { 0 };
		HSI_RETURN return_val = GetConfigurationParameters(m_handle, &config);
		if (HSI_OK != return_val)
		{
			CScanDataIO::errorLog(CString(__FILE__), __LINE__, _T("GetConfigurationParameters"), return_val);
			return false;
		}

		config.cube_width = width;
		config.cube_height = height;
#ifdef _DEBUG
		CString msg;
		CScanDataIO::writeLog(CLog::LOGLEVEL::Info, CString(__FILE__), __LINE__, _T("Setting configuration parameters ..."));
		msg.Format(_T("Dimensions : %d x %d"), config.cube_height, config.cube_width);
		CScanDataIO::writeLog(CLog::LOGLEVEL::Info, CString(__FILE__), __LINE__, msg);
#endif
		return_val = SetConfigurationParameters(m_handle, config);
		if (HSI_OK != return_val)
		{
			CScanDataIO::errorLog(CString(__FILE__), __LINE__, _T("SetConfigurationParameters"), return_val);
			return false;
		}
		else {
			m_width = width;
			m_height = height;
		}

#if 0
		SpectralRegionOfInterest i_spectral_regions;
		i_spectral_regions.wavelength_start_nm = WavelengthStart_nm;
		i_spectral_regions.wavelength_end_nm = WavelengthEnd_nm;

		return_val = SetBandSelection(m_handle, &i_spectral_regions, band);
		if (HSI_OK != return_val)
		{
			CScanDataIO::errorLog(CString(__FILE__), __LINE__, _T("SetBandSelection"), return_val);
			return false;
		}
		else {
			m_band = band;
		}
#endif
	}
	return true;
}

/// <summary>
/// フォーマットの取得
/// </summary>
/// <param name="width">幅</param>
/// <param name="height">高さ</param>
/// <param name="band">バンド</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CCameraIO::getFormat(int &width, int &height, int &band)
{
	if (m_handle == 0x00) {
		width = m_width;
		height = m_height;
		band = m_band;
	}
	else {
		ConfigurationParameters config = { 0 };
		HSI_RETURN return_val = GetConfigurationParameters(m_handle, &config);
		if (HSI_OK != return_val)
		{
			CScanDataIO::errorLog(CString(__FILE__), __LINE__, _T("GetConfigurationParameters"), return_val);
			return false;
		}
		width = config.cube_width;
		height = config.cube_height;
		band = m_band;
	}
	return true;
}

/// <summary>
/// IntegrationTimeの設定
/// </summary>
/// <param name="integrationTime">integrationTime</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CCameraIO::setIntegrationTime(double integrationTime)
{
	if (m_handle == 0x00) {
		m_integrationTime = integrationTime;
	}
	else {
		RuntimeParameters runtime = { 0 };
		HSI_RETURN return_val = GetRuntimeParameters(m_handle, &runtime);
		if (HSI_OK != return_val)
		{
			CScanDataIO::errorLog(CString(__FILE__), __LINE__, _T("GetRuntimeParameters"), return_val);
			return 1.0;
		}

		runtime.integration_time_ms = integrationTime;
#ifdef _DEBUG
		CString msg;
		CScanDataIO::writeLog(CLog::LOGLEVEL::Info, CString(__FILE__), __LINE__, _T("Setting runtime parameters ..."));
		msg.Format(_T("Integration time : %f"), runtime.integration_time_ms);
		CScanDataIO::writeLog(CLog::LOGLEVEL::Info, CString(__FILE__), __LINE__, msg);
#endif

		return_val = SetRuntimeParameters(m_handle, runtime);
		if (HSI_OK != return_val)
		{
			CScanDataIO::errorLog(CString(__FILE__), __LINE__, _T("SetRuntimeParameters"), return_val);
			return false;
		}
		else {
			m_integrationTime = integrationTime;
		}
	}
	return true;
}

/// <summary>
/// IntegrationTimeの取得
/// </summary>
/// <returns>IntegrationTimeを返す</returns>
double CCameraIO::getIntegrationTime()
{
	double integrationTime = 1.0;
	if (m_handle == 0x00) {
		integrationTime = m_integrationTime;
	}
	else {
		RuntimeParameters runtime = { 0 };
		HSI_RETURN return_val = GetRuntimeParameters(m_handle, &runtime);
		if (HSI_OK != return_val)
		{
			CScanDataIO::errorLog(CString(__FILE__), __LINE__, _T("GetRuntimeParameters"), return_val);
			return 1.0;
		}
		integrationTime = runtime.integration_time_ms;
	}
	return integrationTime;
}

/// <summary>
/// スキャンの開始
/// </summary>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CCameraIO::StartScan()
{
	if (m_handle == 0x00) {
		CScanDataIO::writeLog(CLog::LOGLEVEL::Error, CString(__FILE__), __LINE__, _T("Start Error:camera is not open."));
		return false;
	}
	else {
		HSI_RETURN return_val;
		// start
#ifdef _DEBUG
		CScanDataIO::writeLog(CLog::LOGLEVEL::Info, CString(__FILE__), __LINE__, _T("Starting"));
#endif
		return_val = Start(m_handle);
		if (HSI_OK != return_val)
		{
			CScanDataIO::errorLog(CString(__FILE__), __LINE__, _T("Start"), return_val);
			return false;
		}

		// acquire dark reference
#ifdef _DEBUG
		CScanDataIO::writeLog(CLog::LOGLEVEL::Info, CString(__FILE__), __LINE__, _T("Acquiring dark reference ..."));
#endif
		FrameFloat dark_reference = { 0 };
		return_val = AcquireDarkReferenceFrame(m_handle, &m_dark_reference);
		if (HSI_OK != return_val)
		{
			CScanDataIO::errorLog(CString(__FILE__), __LINE__, _T("AcquireDarkReferenceFrame (dark reference)"), return_val);
			return false;
		}

		// get correction matrix (mandatory)
#ifdef _DEBUG
		CScanDataIO::writeLog(CLog::LOGLEVEL::Info, CString(__FILE__), __LINE__, _T("Getting correction matrix ..."));
#endif
		m_correction_matrix = { 0 };
		return_val = GetCorrectionMatrix(m_handle, &m_correction_matrix);
		if (HSI_OK != return_val)
		{
			CScanDataIO::errorLog(CString(__FILE__), __LINE__, _T("GetCorrectionMatrix"), return_val);
			return false;
		}

		// deallocate dark_reference
		return_val = commonDeallocateFrame(&m_dark_reference);
		if (HSI_OK != return_val)
		{
			CScanDataIO::errorLog(CString(__FILE__), __LINE__, _T("DeallocateFrame (dark_reference)"), return_val);
		}

	}
	return true;
}

/// <summary>
/// スキャンの停止
/// </summary>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CCameraIO::StopScan()
{
	if (m_handle == 0x00) {
		return true;
	}
	else {
		HSI_RETURN return_val;
		return_val = Stop(m_handle);
		if (HSI_OK != return_val)
		{
			CScanDataIO::errorLog(CString(__FILE__), __LINE__, _T("Stop"), return_val);
			return false;
		}
	}
	return true;
}

/// <summary>
/// リファレンス（ホワイトバランス）の取得
/// </summary>
/// <param name="refarenceFilePath">リファレンスファイルの保存フォルダ</param>
/// <param name="refarenceFileName">リファレンスファイル名（ホワイトバランスファイル）</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CCameraIO::AcquireReference(CString refarenceFilePath, CString refarenceFileName)
{
	HSI_RETURN return_val;
	// acquire cube (mandatory)
#ifdef _DEBUG
	CScanDataIO::writeLog(CLog::LOGLEVEL::Info, CString(__FILE__), __LINE__, _T("Acquiring cube ..."));
#endif
	return_val = AcquireCube(m_handle, &m_dark_reference, &m_cube);
	if (HSI_OK != return_val)
	{
		CScanDataIO::errorLog(CString(__FILE__), __LINE__, _T("AcquireCube (cube)"), return_val);
		return false;
	}

#if 0
	/* --------------------------
	save cube (optional)
	--------------------------*/
	CString dummypath = _T("C:\\Users\\Project\\WeldEvaluation\\Data\\tmp");
	return_val = commonSaveCube(m_cube, dummypath, L"cube", FF_ENVI);
	if (HSI_OK != return_val)
	{
		CScanDataIO::errorLog(CString(__FILE__), __LINE__, _T("SaveCube (cube)"), return_val);
		return false;
	}
#endif

	// allocate corrected cube (mandatory)
	CubeFloat reference_corrected = { 0 };
	 return_val = AllocateCubeCorrected(&reference_corrected, m_correction_matrix, m_cube_format);
	if (HSI_OK != return_val)
	{
		CScanDataIO::errorLog(CString(__FILE__), __LINE__, _T("AllocateCubeCorrected"), return_val);
		return false;
	}

	// apply spectral correction (mandatory)
	return_val = ApplySpectralCorrection(&reference_corrected, m_cube, m_correction_matrix);
	if (HSI_OK != return_val)
	{
		CScanDataIO::errorLog(CString(__FILE__), __LINE__, _T("ApplySpectralCorrection"), return_val);
		return false;
	}

	return_val = commonDeallocateCube(&m_cube);
	if (HSI_OK != return_val)
	{
		CScanDataIO::errorLog(CString(__FILE__), __LINE__, _T("DeallocateCube (reference)"), return_val);
		return false;
	}

	// save result (optional)
	return_val = commonSaveCube(reference_corrected, refarenceFilePath, refarenceFileName, FF_ENVI);
	if (HSI_OK != return_val)
	{
		CScanDataIO::errorLog(CString(__FILE__), __LINE__, _T("SaveCube (corrected)"), return_val);
		return false;
	}

	return_val = commonDeallocateCube(&reference_corrected);
	if (HSI_OK != return_val)
	{
		CScanDataIO::errorLog(CString(__FILE__), __LINE__, _T("DeallocateCube (reference)"), return_val);
		return false;
	}
	return true;
}

/// <summary>
/// リファレンス（ホワイトバランス）の読み込み
/// </summary>
/// <param name="reference_corrected">リファレンスファイルデータ</param>
/// <param name="refarenceFilePath">リファレンスファイルの保存フォルダ</param>
/// <param name="refarenceFileName">リファレンスファイル名（ホワイトバランスファイル）</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CCameraIO::LoadReference(CubeFloat &reference_corrected, CString refarenceFilePath, CString refarenceFileName)
{
	CString filename = refarenceFileName + ".hdr";
	CString filepath = CFileUtil::FilePathCombine(refarenceFilePath, filename);
	HSI_RETURN return_val = commonLoadCube(&reference_corrected, filepath);
	if (HSI_OK != return_val)
	{
		CScanDataIO::errorLog(CString(__FILE__), __LINE__, _T("commonLoadCube (reference)"), return_val);
		return false;
	}
	return true;
}

/// <summary>
/// キューブデータ（最終データ）の取得
/// </summary>
/// <param name="spectralFilePath">キューブデータファイルの保存フォルダ</param>
/// <param name="spectralFileName">キューブデータファイル名</param>
/// <param name="refarenceFilePath">リファレンスファイルの保存フォルダ</param>
/// <param name="refarenceFileName">リファレンスファイル名（ホワイトバランスファイル）</param>
/// <returns>成功した場合はtrue、失敗した場合はfalseを返す</returns>
bool CCameraIO::AcquireSpectralCube(CString spectralFilePath, CString spectralFileName, CubeFloat &reference_corrected, CubeFloat  &cube_corrected)
{
	if (m_bDemoMode) {
		CString fname = spectralFileName + _T(".hdr");
		CString scanDataFilePath = CFileUtil::FilePathCombine(spectralFilePath, fname);
		HSI_RETURN result = commonLoadCube(&cube_corrected, scanDataFilePath);
		if (result != HSI_OK) {
			return false;
		}
	}
	else {
		HSI_RETURN return_val;
		// acquire cube (mandatory)
		return_val = AcquireCube(m_handle, &m_dark_reference, &m_cube);
		if (HSI_OK != return_val)
		{
			CScanDataIO::errorLog(CString(__FILE__), __LINE__, _T("AcquireCube (cube)"), return_val);
			return false;
		}

#if 0
		// save cube (optional)
		CString cubeName = _T("cube");
		return_val = commonSaveCube(m_cube, spectralFilePath, cubeName, FF_ENVI);
		if (HSI_OK != return_val)
		{
			CScanDataIO::errorLog(CString(__FILE__), __LINE__, _T("SaveCube (cube)"), return_val);
			return false;
		}
#endif

		// allocate corrected cube(mandatory)
		commonDeallocateCube(&cube_corrected);
		cube_corrected = { 0 };
		return_val = AllocateCubeCorrected(&cube_corrected, m_correction_matrix, m_cube_format);
		if (HSI_OK != return_val)
		{
			CScanDataIO::errorLog(CString(__FILE__), __LINE__, _T("AllocateCubeCorrected"), return_val);
			return false;
		}

		// apply spectral correction (mandatory)
		return_val = ApplySpectralCorrection(&cube_corrected, m_cube, m_correction_matrix);
		if (HSI_OK != return_val)
		{
			CScanDataIO::errorLog(CString(__FILE__), __LINE__, _T("ApplySpectralCorrection"), return_val);
			return false;
		}

		return_val = ApplyWhiteReference(&cube_corrected, cube_corrected, reference_corrected, 0.95);
		if (HSI_OK != return_val)
		{
			CScanDataIO::CScanDataIO::errorLog(CString(__FILE__), __LINE__, _T("Normalize"), return_val);
			return false;
		}

		// save result (optional)
		return_val = commonSaveCube(cube_corrected, spectralFilePath, spectralFileName, FF_ENVI);
		if (HSI_OK != return_val)
		{
			CScanDataIO::errorLog(CString(__FILE__), __LINE__, _T("SaveCube (corrected)"), return_val);
			return false;
		}
	}
	return true;
}
