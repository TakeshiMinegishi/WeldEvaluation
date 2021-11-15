#include "stdafx.h"
#include "CCameraIO.h"
#include "FileUtil.h"

CCameraIO::CCameraIO()
{
	CameraPrmInit();
	m_width				= 2048;
	m_height			= 1088;
	m_band				= 150;
	m_integrationTime	= 1.0;

	InitLlogger();

}

CCameraIO::CCameraIO(int widht, int height, int band)
{
	InitLlogger();
	CameraPrmInit();
	m_width				= widht;
	m_height			= height;
	m_band				= band;
	m_integrationTime	= 1.0;
}

CCameraIO::~CCameraIO()
{
	FleeCameraPrm();
}

void CCameraIO::CameraPrmInit()
{
	m_handle			= 0x0;
	m_cube				= { 0 };
	m_correction_matrix = { 0 };
	m_cube_format		= { 0 };
}

bool CCameraIO::InitLlogger()
{
	CLog log;
	CString logPath = log.getLogPath();
	HSI_RETURN return_val = commonInitializeLogger(logPath, LV_WARNING);
	if (HSI_OK != return_val)
	{
		errorLog(CString(__FILE__), __LINE__, _T("InitializeLogger"), return_val);
		return false;
	}
	return true;
}

bool CCameraIO::Open(CString snapscan_file, bool dummyApi/* = true*/)
{
	HSI_RETURN return_val;
	{
		int o_p_major, o_p_minor, o_p_patch, o_p_build;
		return_val = GetAPIVersion(&o_p_major, &o_p_minor, &o_p_patch, &o_p_build);
		if (HSI_OK != return_val)
		{
			errorLog(CString(__FILE__), __LINE__, _T("OpenDevice"), return_val);
			return false;
		}
		CString version;
		version.Format(_T("major:%d minor:%d patch:%d build:%d"), o_p_major, o_p_minor, o_p_patch, o_p_build);
		writeLog(CLog::LOGLEVEL::Info, CString(__FILE__), __LINE__, version);
		AfxMessageBox(version, MB_OK | MB_ICONSTOP);
	}

CString sval;
sval.Format(_T("Open:IN dummyApi:%d file:%s"), dummyApi, (LPCTSTR)snapscan_file);
AfxMessageBox(sval, MB_OK | MB_ICONSTOP);

	m_handle = 0x0;
	//snapscan_file = _T("./resources/snapscan_dummy.xml");
	return_val = OpenDevice(&m_handle, snapscan_file, dummyApi);
	if (HSI_OK != return_val)
	{
		errorLog(CString(__FILE__), __LINE__, _T("OpenDevice"), return_val);
		return false;
	}
AfxMessageBox(_T("Open:OpenDevice(Success)"), MB_OK | MB_ICONSTOP);

	// Width,Height,Band ÇÃê›íË
	if (!setFormat(m_width, m_height, m_band)) {
		writeLog(CLog::LOGLEVEL::Error, CString(__FILE__), __LINE__, _T("Open Error"));
		return false;
	}
AfxMessageBox(_T("Open:setFormat(Success)"), MB_OK | MB_ICONSTOP);

	// Initialize
	return_val = Initialize(m_handle);
	if (HSI_OK != return_val)
	{
		errorLog(CString(__FILE__), __LINE__, _T("Initialize"), return_val);
		return false;
	}
AfxMessageBox(_T("Open:Initialize(Success)"), MB_OK | MB_ICONSTOP);

	// allocate cube data format(mandatory)
	return_val = GetOutputCubeDataFormat(m_handle, &m_cube_format);
	if (HSI_OK != return_val)
	{
		errorLog(CString(__FILE__), __LINE__, _T("GetOutputCubeDataFormat"), return_val);
		return false;
	}
AfxMessageBox(_T("Open:GetOutputCubeDataFormat(Success)"), MB_OK | MB_ICONSTOP);

	// allocate cube (mandatory)
	m_cube = { 0 };
	return_val = commonAllocateCube(&m_cube, m_cube_format);
	if (HSI_OK != return_val)
	{
		errorLog(CString(__FILE__), __LINE__, _T("AllocateCube (cube)"), return_val);
		return false;
	}
AfxMessageBox(_T("Open:commonAllocateCube(Success)"), MB_OK | MB_ICONSTOP);
AfxMessageBox(_T("Open:OUT(Success)"), MB_OK | MB_ICONSTOP);
return true;
}

void CCameraIO::FleeCameraPrm()
{
AfxMessageBox(_T("FleeCameraPrm:IN"), MB_OK | MB_ICONSTOP);
	// deallocate data format
	HSI_RETURN return_val = commonDeallocateCubeDataFormat(&m_cube_format);
	if (HSI_OK != return_val)
	{
		errorLog(CString(__FILE__), __LINE__, _T("DeallocateCubeDataFormat"), return_val);
	}
AfxMessageBox(_T("FleeCameraPrm:commonDeallocateCubeDataFormat(SUCCESS)"), MB_OK | MB_ICONSTOP);

	// deallocate correction matrix
	return_val = DeallocateCorrectionMatrix(&m_correction_matrix);
	if (HSI_OK != return_val)
	{
		errorLog(CString(__FILE__), __LINE__, _T("DeallocateCorrectionMatrix"), return_val);
	}
AfxMessageBox(_T("FleeCameraPrm:DeallocateCorrectionMatrix(SUCCESS)"), MB_OK | MB_ICONSTOP);
}

void CCameraIO::Close()
{
AfxMessageBox(_T("Close:IN"), MB_OK | MB_ICONSTOP);
	FleeCameraPrm();
AfxMessageBox(_T("Close:FleeCameraPrm(SUCCESS)"), MB_OK | MB_ICONSTOP);

	HSI_RETURN return_val = CloseDevice(&m_handle);
	if (HSI_OK != return_val)
	{
		errorLog(CString(__FILE__), __LINE__, _T("CloseDevice"), return_val);
	}
AfxMessageBox(_T("Close:CloseDevice(SUCCESS)"), MB_OK | MB_ICONSTOP);
}

bool CCameraIO::getSystemFormat(int &widht_max, int &height_max)
{
	if (m_handle == 0x00) {
		return false;
	}
	SystemProperties system_properties = { 0 };
	HSI_RETURN return_val = GetSystemProperties(m_handle, &system_properties);
	if (HSI_OK != return_val)
	{
		errorLog(CString(__FILE__), __LINE__, _T("GetSystemProperties"), return_val);
		return false;
	}
	widht_max	= system_properties.cube_height_max;
	height_max = system_properties.cube_width_max;
	return true;
}

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
			errorLog(CString(__FILE__), __LINE__, _T("GetConfigurationParameters"), return_val);
			return false;
		}
AfxMessageBox(_T("Open:GetConfigurationParameters(Success)"), MB_OK | MB_ICONSTOP);

		config.cube_width = width;
		config.cube_height = height;
		return_val = SetConfigurationParameters(m_handle, config);
		if (HSI_OK != return_val)
		{
			errorLog(CString(__FILE__), __LINE__, _T("SetConfigurationParameters"), return_val);
			return false;
		}
		else {
			m_width = width;
			m_height = height;
		}
AfxMessageBox(_T("Open:SetConfigurationParameters(Success)"), MB_OK | MB_ICONSTOP);

#if 0
		SpectralRegionOfInterest i_spectral_regions;
		i_spectral_regions.wavelength_start_nm = WavelengthStart_nm;
		i_spectral_regions.wavelength_end_nm = WavelengthEnd_nm;

		return_val = SetBandSelection(m_handle, &i_spectral_regions, band);
		if (HSI_OK != return_val)
		{
			errorLog(CString(__FILE__), __LINE__, _T("SetBandSelection"), return_val);
			return false;
		}
		else {
			m_band = band;
		}
#endif
	}
	return true;
}

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
			errorLog(CString(__FILE__), __LINE__, _T("GetConfigurationParameters"), return_val);
			return false;
		}
		width = config.cube_width;
		height = config.cube_height;
		band = m_band;
	}
	return true;
}

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
			errorLog(CString(__FILE__), __LINE__, _T("GetRuntimeParameters"), return_val);
			return 1.0;
		}

		runtime.integration_time_ms = integrationTime;

		return_val = SetRuntimeParameters(m_handle, runtime);
		if (HSI_OK != return_val)
		{
			errorLog(CString(__FILE__), __LINE__, _T("SetRuntimeParameters"), return_val);
			return false;
		}
		else {
			m_integrationTime = integrationTime;
		}
	}
	return true;
}

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
			errorLog(CString(__FILE__), __LINE__, _T("GetRuntimeParameters"), return_val);
			return 1.0;
		}
		integrationTime = runtime.integration_time_ms;
	}
	return integrationTime;
}

bool CCameraIO::StartScan()
{
AfxMessageBox(_T("StartScan:IN"), MB_OK | MB_ICONSTOP);
	if (m_handle == 0x00) {
		writeLog(CLog::LOGLEVEL::Error, CString(__FILE__), __LINE__, _T("Start Error:camera is not open."));
		return false;
	}
	else {
		// start
		HSI_RETURN return_val = Start(m_handle);
		if (HSI_OK != return_val)
		{
			errorLog(CString(__FILE__), __LINE__, _T("Start"), return_val);
			return false;
		}
AfxMessageBox(_T("StartScan:Start(SUCCESS)"), MB_OK | MB_ICONSTOP);

		// get correction matrix (mandatory)
		m_correction_matrix = { 0 };
		return_val = GetCorrectionMatrix(m_handle, &m_correction_matrix);
		if (HSI_OK != return_val)
		{
			errorLog(CString(__FILE__), __LINE__, _T("GetCorrectionMatrix"), return_val);
			return false;
		}
AfxMessageBox(_T("StartScan:GetCorrectionMatrix(SUCCESS)"), MB_OK | MB_ICONSTOP);
	}
AfxMessageBox(_T("StartScan:OUT(SUCCESS)"), MB_OK | MB_ICONSTOP);
	return true;
}

bool CCameraIO::AcquireReference(CString refarenceFilePath, CString refarenceFileName)
{
AfxMessageBox(_T("AcquireReference:IN"), MB_OK | MB_ICONSTOP);
	// acquire dark reference
	FrameFloat dark_reference = { 0 };
	HSI_RETURN return_val = AcquireDarkReferenceFrame(m_handle, &dark_reference);
	if (HSI_OK != return_val)
	{
		errorLog(CString(__FILE__), __LINE__, _T("AcquireDarkReferenceFrame (dark reference)"), return_val);
		return false;
	}
AfxMessageBox(_T("AcquireReference:AcquireDarkReferenceFrame(SUCCESS)"), MB_OK | MB_ICONSTOP);

	// acquire cube (mandatory)
	return_val = AcquireCube(m_handle, &dark_reference, &m_cube);
	if (HSI_OK != return_val)
	{
		errorLog(CString(__FILE__), __LINE__, _T("AcquireCube (cube)"), return_val);
		return false;
	}
AfxMessageBox(_T("AcquireReference:AcquireCube(SUCCESS)"), MB_OK | MB_ICONSTOP);

#if 0
	/* --------------------------
	save cube (optional)
	--------------------------*/
	CString dummypath = _T("C:\\Users\\Project\\WeldEvaluation\\Data\\tmp");
	return_val = commonSaveCube(m_cube, dummypath, L"cube", FF_ENVI);
	if (HSI_OK != return_val)
	{
		errorLog(CString(__FILE__), __LINE__, _T("SaveCube (cube)"), return_val);
		return false;
	}
#endif

	return_val = commonDeallocateFrame(&dark_reference);
	if (HSI_OK != return_val)
	{
		errorLog(CString(__FILE__), __LINE__, _T("DeallocateFrame (dark_reference)"), return_val);
		return false;
	}
AfxMessageBox(_T("AcquireReference:commonDeallocateFrame(SUCCESS)"), MB_OK | MB_ICONSTOP);

	// allocate corrected cube (mandatory)
	CubeFloat reference_corrected = { 0 };
	 return_val = AllocateCubeCorrected(&reference_corrected, m_correction_matrix, m_cube_format);
	if (HSI_OK != return_val)
	{
		errorLog(CString(__FILE__), __LINE__, _T("AllocateCubeCorrected"), return_val);
		return false;
	}
AfxMessageBox(_T("AcquireReference:AllocateCubeCorrected(SUCCESS)"), MB_OK | MB_ICONSTOP);

	// apply spectral correction (mandatory)
	return_val = ApplySpectralCorrection(&reference_corrected, m_cube, m_correction_matrix);
	if (HSI_OK != return_val)
	{
		errorLog(CString(__FILE__), __LINE__, _T("ApplySpectralCorrection"), return_val);
		return false;
	}
AfxMessageBox(_T("AcquireReference:ApplySpectralCorrection(SUCCESS)"), MB_OK | MB_ICONSTOP);

	return_val = commonDeallocateCube(&m_cube);
	if (HSI_OK != return_val)
	{
		errorLog(CString(__FILE__), __LINE__, _T("DeallocateCube (reference)"), return_val);
		return false;
	}
AfxMessageBox(_T("AcquireReference:commonDeallocateCube(SUCCESS)"), MB_OK | MB_ICONSTOP);

	// save result (optional)
	return_val = commonSaveCube(reference_corrected, refarenceFilePath, refarenceFileName, FF_ENVI);
	if (HSI_OK != return_val)
	{
		errorLog(CString(__FILE__), __LINE__, _T("SaveCube (corrected)"), return_val);
		return false;
	}
AfxMessageBox(_T("AcquireReference:commonSaveCube(SUCCESS)"), MB_OK | MB_ICONSTOP);

	return_val = commonDeallocateCube(&reference_corrected);
	if (HSI_OK != return_val)
	{
		errorLog(CString(__FILE__), __LINE__, _T("DeallocateCube (reference)"), return_val);
		return false;
	}
AfxMessageBox(_T("AcquireReference:commonDeallocateCube(SUCCESS)"), MB_OK | MB_ICONSTOP);
AfxMessageBox(_T("AcquireReference:OUT(SUCCESS)"), MB_OK | MB_ICONSTOP);
return true;
}

bool CCameraIO::LoadReference(CubeFloat &reference_corrected, CString refarenceFilePath, CString refarenceFileName)
{
	CString filename = refarenceFileName + ".hdr";
	CString filepath = CFileUtil::FilePathCombine(refarenceFilePath, filename);
	HSI_RETURN return_val = commonLoadCube(&reference_corrected, filepath);
	if (HSI_OK != return_val)
	{
		errorLog(CString(__FILE__), __LINE__, _T("commonLoadCube (reference)"), return_val);
		return false;
	}
	return true;
}


bool CCameraIO::AcquireSpectralCube(CString spectralFilePath, CString spectralFileName, CubeFloat &reference_corrected)
{
	// acquire dark reference
	FrameFloat dark_reference = { 0 };
	HSI_RETURN return_val = AcquireDarkReferenceFrame(m_handle, &dark_reference);
	if (HSI_OK != return_val)
	{
		errorLog(CString(__FILE__), __LINE__, _T("AcquireDarkReferenceFrame (dark reference)"), return_val);
		return false;
	}

	// acquire cube (mandatory)
	return_val = AcquireCube(m_handle, &dark_reference, &m_cube);
	if (HSI_OK != return_val)
	{
		errorLog(CString(__FILE__), __LINE__, _T("AcquireCube (cube)"), return_val);
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
		errorLog(CString(__FILE__), __LINE__, _T("SaveCube (cube)"), return_val);
		return false;
	}
#endif

	return_val = commonDeallocateFrame(&dark_reference);
	if (HSI_OK != return_val)
	{
		errorLog(CString(__FILE__), __LINE__, _T("DeallocateFrame (dark_reference)"), return_val);
		return false;
	}

	// allocate corrected cube(mandatory)
	CubeFloat cube_corrected = { 0 };
	return_val = AllocateCubeCorrected(&cube_corrected, m_correction_matrix, m_cube_format);
	if (HSI_OK != return_val)
	{
		errorLog(CString(__FILE__), __LINE__, _T("AllocateCubeCorrected"), return_val);
		return false;
	}

	// apply spectral correction (mandatory)
	return_val = ApplySpectralCorrection(&cube_corrected, m_cube, m_correction_matrix);
	if (HSI_OK != return_val)
	{
		errorLog(CString(__FILE__), __LINE__, _T("ApplySpectralCorrection"), return_val);
		return false;
	}

	return_val = ApplyWhiteReference(&cube_corrected, cube_corrected, reference_corrected, 0.95);
	if (HSI_OK != return_val)
	{
		errorLog(CString(__FILE__), __LINE__, _T("Normalize"), return_val);
		return false;
	}

	// save result (optional)
	return_val = commonSaveCube(cube_corrected, spectralFilePath, spectralFileName, FF_ENVI);
	if (HSI_OK != return_val)
	{
		errorLog(CString(__FILE__), __LINE__, _T("SaveCube (corrected)"), return_val);
		return false;
	}


	return true;
}

void CCameraIO::writeLog(CLog::LOGLEVEL level, CString filePath, long lineNo, CString msg)
{
	CLog log;
	CString ErrMsg;
	ErrMsg.Format(_T(" File:%s Line:%ld:%s"), (LPCTSTR)filePath, lineNo, (LPCTSTR)msg);
	log.logWrite(level, ErrMsg);
}

void CCameraIO::errorLog(CString filePath, long lineNo, CString i_caller_name, HSI_RETURN i_return_val)
{
	if (i_return_val == HSI_OK)
	{
		return;
	}

	CString errs;
	switch (i_return_val)
	{
	case HSI_HANDLE_INVALID:
		errs = _T("Invalid device handle specified.");
		break;
	case HSI_ARGUMENT_INVALID:
		errs = _T("Invalid argument provided in function call.");
		break;
	case HSI_CALL_ILLEGAL:
		errs = _T("Function call illegal given the current snapscan state.");
		break;
	case HSI_FILE_NOT_FOUND:
		errs = _T("A file could not be found.");
		break;
	case HSI_CALIBRATION_FILE_NOT_FOUND:
		errs = _T("Sensor calibration file could not be found.");
		break;
	case HSI_CONNECTION_FAILED:
		errs = _T("Snapscan system could not be connected.");
		break;
	case HSI_ALLOCATION_ERROR:
		errs = _T("Allocation of resources failed.");
		break;
	case HSI_ACQUISITION_FAILED:
		errs = _T("Unable to fulfill acquisition.");
		break;
	case HSI_DATA_NOT_ALLOCATED:
		errs = _T("Provided data structure is not allocated.");
		break;
	case HSI_DATA_NOT_VALID:
		errs = _T("Data with valid flag false provided as input for operation.");
		break;
	case HSI_DATA_NOT_COMPATIBLE:
		errs = _T("Data provided is not compatible.");
		break;
	case HSI_FILE_SYSTEM_ERROR:
		errs = _T("Specified directory doesn't exist and could not be created.");
		break;
	case HSI_FILE_IO_ERROR:
		errs = _T("Could not read or write data from the filesystem.");
		break;
	case HSI_INTERNAL_ERROR:
		errs = _T("An undexpected internal error occurred.");
		break;
	default:
		errs = _T("Unknown error.");
		break;
	}

	CLog log;
	CString ErrMsg;
	ErrMsg.Format(_T(" File:%s Line:%ld:%s:%s"), (LPCTSTR)filePath, lineNo, (LPCTSTR)i_caller_name, (LPCTSTR)errs);
	log.logWrite(CLog::LOGLEVEL::Error, ErrMsg);
	return;
}
