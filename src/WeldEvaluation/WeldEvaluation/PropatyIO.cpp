#include "stdafx.h"
#include "PropatyIO.h"
#include "ConfigrationIO.h"
#include "FileUtil.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
CPropatyIO::CPropatyIO(void)
{
	Initialze();
	m_joinRetioFormat			= _T("%02d_Joining_ratio%03d");
	m_joinColorFormat			= _T("%02d_Joining_color%03d");
	m_nClassFormat				= _T("%02d_Number_of_classifications");
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
CPropatyIO::~CPropatyIO(void)
{
}

/// <summary>
/// �p�����[�^�̏�����
/// </summary>
void CPropatyIO::Initialze(void)
{
	m_Version = _T("");
	m_ParamaterFilePath = _T("");
	m_ProjectName = _T("");	// �v���W�F�N�g��
	m_TestName = _T("");	// ����
	m_ResinAnalysisMethod = 0;		// ������ ��͕��@
	m_MetalAnalysisMethod = 0;		// ������ ��͕��@
	m_ResultAnalysisMethod = 0;		// �ڍ����� ��͕��@
}

/// <summary>
/// �p�����[�^�t�@�C���p�X�̎擾
/// </summary>
/// <returns>�p�����[�^�t�@�C���p�X��Ԃ�</returns>
CString CPropatyIO::GetParamaterFilePath()
{
	return m_ParamaterFilePath;
}

/// <summary>
/// �p�����[�^�t�@�C���p�X�̐ݒ�
/// </summary>
/// <param name="path">�p�����[�^�t�@�C���p�X</param>
/// <returns>�����ꍇ��true�A���s�ꍇ��false��Ԃ�</returns>
bool CPropatyIO::SetParamaterFilePath(CString path)
{
//	if (!CFileUtil::fileExists(path)) {
//		m_ParamaterFilePath	= _T("");
//		return false;
//	}
	m_ParamaterFilePath = path;
	return true;
}


/// <summary>
/// �p�����[�^�t�@�C�����̍쐬
/// </summary>
/// <param name="TestName">����</param>
/// <param name="date">���t</param>
/// <param name="number">�T�u�ԍ�</param>
/// <returns>�p�����[�^�t�@�C������Ԃ�</returns>
CString CPropatyIO::MakeProjectName(CString TestName, COleDateTime date, int number)
{
	CString name;
	name.Format(_T("%s%4d%02d%02d-%d"), (LPCWSTR)TestName,date.GetYear(),date.GetMonth(),date.GetDay(),number);
	return name;
}

/// <summary>
/// �p�����[�^�t�@�C���̓ǂݍ���
/// </summary>
/// <param name="path">�p�����[�^�t�@�C���ւ̃p�X</param>
/// <returns>�����ꍇ��true�A���s�ꍇ��false��Ԃ�</returns>
bool CPropatyIO::read(CString path)
{
	CConfigrationIO sys(path);
	m_Version				= sys.getString(_T("Common"), _T("Version"));
	m_ProjectName			= sys.getString(_T("Common"),_T("Projent_name"));					// �v���W�F�N�g��
	m_TestName				= sys.getString(_T("Common"),_T("Test_name"));						// ����
	m_ResinAnalysisMethod	= sys.getInt(_T("ResinSurface"),_T("Analysis_method"));				// ������ ��͕��@
	m_MetalAnalysisMethod	= sys.getInt(_T("MetalSurface"),_T("Analysis_method"));				// ������ ��͕��@
	m_ResultAnalysisMethod	= sys.getInt(_T("JoiningResult"), _T("Analysis_method"));			// �ڍ����� ��͕��@

	return true;
}

/// <summary>
/// �p�����[�^�t�@�C���̏�������
/// </summary>
/// <param name="name">�p�����[�^�t�@�C���ւ̃p�X</param>
/// <returns>�����ꍇ��true�A���s�ꍇ��false��Ԃ�</returns>
bool CPropatyIO::save(CString path)
{
	bool bResult = true;
	CConfigrationIO sys(path);

	// �o�[�W����
	if (!sys.setString(_T("Common"), _T("Version"), m_Version)) {
		bResult = false;
	}

	// �v���W�F�N�g��
	if (!sys.setString(_T("Common"),_T("Projent_name"),m_ProjectName)) {
		bResult = false;
	}
	// ����
	if (!sys.setString(_T("Common"),_T("Test_name"),m_TestName)) {
		bResult = false;
	}

	// Setting�̃_�~�[�o��
	double dval;
	UINT uval;
	uval = sys.getInt(_T("Setting"),_T("Number_of_overlapping_pixels"));
	if (!sys.setInt(_T("Setting"),_T("Number_of_overlapping_pixels"),uval)) {
		bResult = false;
	}
	uval = sys.getInt(_T("Setting"),_T("Integration_time_ms"));
	if (!sys.setInt(_T("Setting"),_T("Integration_time_ms"),uval)) {
		bResult = false;
	}
	uval = sys.getInt(_T("Setting"),_T("Vertical_resolution"));
	if (!sys.setInt(_T("Setting"),_T("Vertical_resolution"),uval)) {
		bResult = false;
	}
	uval = sys.getInt(_T("Setting"),_T("Horizontal_resolution"));
	if (!sys.setInt(_T("Setting"),_T("Horizontal_resolution"),uval)) {
		bResult = false;
	}

	// Setting�̃_�~�[�o��
#if 0
	uval = sys.getInt(_T("ResinSurface"),_T("Number_of_overlapping_pixels"));
	if (!sys.setInt(_T("ResinSurface"),_T("Number_of_overlapping_pixels"),uval)) {
		bResult = false;
	}
	dval = sys.getDouble(_T("ResinSurface"),_T("Joining_ratio"));
	if (!sys.setDouble(_T("ResinSurface"),_T("Joining_ratio"),uval)) {
		bResult = false;
	}
	uval = sys.getInt(_T("MetalSurface "),_T("Number_of_overlapping_pixels"));
	if (!sys.setInt(_T("MetalSurface "),_T("Number_of_overlapping_pixels"),uval)) {
		bResult = false;
	}
	dval = sys.getDouble(_T("MetalSurface "),_T("Joining_ratio"));
	if (!sys.setDouble(_T("MetalSurface "),_T("Joining_ratio"),uval)) {
		bResult = false;
	}
	uval = sys.getInt(_T("JoiningResult "),_T("Number_of_overlapping_pixels"));
	if (!sys.setInt(_T("JoiningResult "),_T("Number_of_overlapping_pixels"),uval)) {
		bResult = false;
	}
	dval = sys.getDouble(_T("JoiningResult "),_T("Joining_ratio"));
	if (!sys.setDouble(_T("JoiningResult "),_T("Joining_ratio"),uval)) {
		bResult = false;
	}
#endif

	// ������ ��͕��@
	if (!sys.setInt(_T("ResinSurface"),_T("Analysis_method"),m_ResinAnalysisMethod)) {
		bResult = false;
	}
	// ������ ��͕��@
	if (!sys.setInt(_T("MetalSurface"),_T("Analysis_method"),m_MetalAnalysisMethod)) {
		bResult = false;
	}
	// ���ʖ� ��͕��@
	if (!sys.setInt(_T("JoiningResult"), _T("Analysis_method"), m_ResultAnalysisMethod)) {
		bResult = false;
	}

	return bResult;
}

/// <summary>
/// �o�[�W�����̎擾�̎擾
/// </summary>
/// <returns>�o�[�W������Ԃ�</returns>
CString CPropatyIO::GetVersion()
{
	return m_Version;
}

/// <summary>
/// �o�[�W�����̐ݒ�
/// </summary>
/// <param name="version">�o�[�W����</param>
/// <returns>�����ꍇ��true�A���s�ꍇ��false��Ԃ�</returns>
bool CPropatyIO::SetVersion(CString version)
{
	m_Version = version;
	return true;
}

/// <summary>
/// �v���W�F�N�g���̎擾
/// </summary>
/// <returns>�v���W�F�N�g����Ԃ�</returns>
CString CPropatyIO::GetProjectName()
{
	return m_ProjectName;
}

/// <summary>
/// �v���W�F�N�g���̐ݒ�
/// </summary>
/// <param name="projectname">�v���W�F�N�g��</param>
/// <returns>�����ꍇ��true�A���s�ꍇ��false��Ԃ�</returns>
bool CPropatyIO::SetProjectName(CString projectname)
{
	m_ProjectName = projectname;
	return true;
}

/// <summary>
/// ���̂̎擾
/// </summary>
/// <returns>���̂�Ԃ�</returns>
CString CPropatyIO::GetTestName()
{
	return m_TestName;
}

/// <summary>
/// ���̂̐ݒ�
/// </summary>
/// <param name="name">����</param>
/// <returns>�����ꍇ��true�A���s�ꍇ��false��Ԃ�</returns>
bool CPropatyIO::SetTestName(CString name)
{
	m_TestName = name;
	return true;
}

/// <summary>
/// �v���W�F�N�g���̕ۑ�
/// </summary>
/// <param name="PropatyFilePath">�v���p�e�B�t�@�C���ւ̃p�X</param>
/// <param name="ProjectName">�v���W�F�N�g��</param>
/// <returns>�����ꍇ��true�A���s�ꍇ��false��Ԃ�</returns>
bool CPropatyIO::WriteProjectName(CString PropatyFilePath, CString ProjectName)
{
	if (!CFileUtil::fileExists(PropatyFilePath)) {
		return false;
	}

	bool bResult = true;
	CConfigrationIO sys(PropatyFilePath);
	// �v���W�F�N�g��
	if (!sys.setString(_T("Common"), _T("Projent_name"), ProjectName)) {
		bResult = false;
	}
	return bResult;
}

/// <summary>
/// �e�X�g���̕ۑ�
/// </summary>
/// <param name="PropatyFilePath">�v���p�e�B�t�@�C���ւ̃p�X</param>
/// <param name="TestName">�e�X�g��</param>
/// <returns>�����ꍇ��true�A���s�ꍇ��false��Ԃ�</returns>
bool CPropatyIO::WriteTestName(CString PropatyFilePath, CString TestName)
{
	if (!CFileUtil::fileExists(PropatyFilePath)) {
		return false;
	}

	bool bResult = true;
	CConfigrationIO sys(PropatyFilePath);
	// ����
	if (!sys.setString(_T("Common"), _T("Test_name"), TestName)) {
		bResult = false;
	}
	return bResult;
}


#if 0
/// <summary>
/// Integration_time_ms�̎擾
/// </summary>
/// <returns>Integration_time_ms��Ԃ�</returns>
double CPropatyIO::GetIntegrationTimeMs(void)
{
	if (!CFileUtil::fileExists(m_ParamaterFilePath)) {
		return 0;
	}
	CConfigrationIO sys(m_ParamaterFilePath);
	return sys.getDouble(_T("Setting"),_T("Integration_time_ms"));
}

/// <summary>
/// Integration_time_ms�̐ݒ�
/// </summary>
/// <param name="time">Integration_time_ms</param>
/// <returns>�����ꍇ��true�A���s�ꍇ��false��Ԃ�</returns>
bool CPropatyIO::SetIntegrationTimeMs(double time)
{
	if (!CFileUtil::fileExists(m_ParamaterFilePath)) {
		return false;
	}
	CConfigrationIO sys(m_ParamaterFilePath);
	if (!sys.setDouble(_T("Setting"),_T("Integration_time_ms"),time)) {
		return false;
	}
	return true;
}

/// <summary>
/// �o���h���̎擾
/// </summary>
/// <returns>�o���h����Ԃ�</returns>
UINT CPropatyIO::GetNumberOfBand(void)
{
	if (!CFileUtil::fileExists(m_ParamaterFilePath)) {
		return 0;
	}
	CConfigrationIO sys(m_ParamaterFilePath);
	return sys.getInt(_T("Setting"), _T("Number_of_band"));
}

/// <summary>
/// �o���h���̐ݒ�
/// </summary>
/// <param name="band">�o���h��</param>
/// <returns>�����ꍇ��true�A���s�ꍇ��false��Ԃ�</returns>
bool CPropatyIO::SetNumberOfBand(UINT band)
{
	if (!CFileUtil::fileExists(m_ParamaterFilePath)) {
		return false;
	}
	CConfigrationIO sys(m_ParamaterFilePath);
	if (!sys.setInt(_T("Setting"), _T("Number_of_band"), band)) {
		return false;
	}
	return true;
}
#endif

/// <summary>
/// �d�Ȃ�s�N�Z�����̎擾
/// </summary>
/// <returns>�d�Ȃ�s�N�Z������Ԃ�</returns>
UINT CPropatyIO::NumberOfOverridePixel(void)
{
	if (!CFileUtil::fileExists(m_ParamaterFilePath)) {
		return 0;
	}
	CConfigrationIO sys(m_ParamaterFilePath);
	return sys.getInt(_T("Setting"), _T("Number_of_overlapping_pixels"));
}

/// <summary>
/// �d�Ȃ�s�N�Z�����̐ݒ�
/// </summary>
/// <param name="num">�d�Ȃ�s�N�Z����</param>
/// <returns>�����ꍇ��true�A���s�ꍇ��false��Ԃ�</returns>
bool CPropatyIO::SetOverridePixelNumber(UINT num)
{
	if (!CFileUtil::fileExists(m_ParamaterFilePath)) {
		return false;
	}
	CConfigrationIO sys(m_ParamaterFilePath);
	if (!sys.setInt(_T("Setting"), _T("Number_of_overlapping_pixels"), num)) {
		return false;
	}
	return true;
}

/// <summary>
/// �c�����̉𑜓x�̎擾
/// </summary>
/// <returns>�c�����̉𑜓x��Ԃ�</returns>
UINT CPropatyIO::GetVerticalResolution(void)
{
	if (!CFileUtil::fileExists(m_ParamaterFilePath)) {
		return 0;
	}
	CConfigrationIO sys(m_ParamaterFilePath);
	return sys.getInt(_T("Setting"), _T("Vertical_resolution"));
}

/// <summary>
/// �c�����̉𑜓x�̐ݒ�
/// </summary>
/// <param name="resolution">�c�����̉𑜓x</param>
/// <returns>�����ꍇ��true�A���s�ꍇ��false��Ԃ�</returns>
bool CPropatyIO::SetVerticalResolution(UINT resolution)
{
	if (!CFileUtil::fileExists(m_ParamaterFilePath)) {
		return false;
	}
	CConfigrationIO sys(m_ParamaterFilePath);
	if (!sys.setInt(_T("Setting"), _T("Vertical_resolution"), resolution)) {
		return false;
	}
	return true;
}

/// <summary>
/// �������̉𑜓x�̎擾
/// </summary>
/// <returns>�������̉𑜓x��Ԃ�</returns>
UINT CPropatyIO::GetHorizontalResolution(void)
{
	if (!CFileUtil::fileExists(m_ParamaterFilePath)) {
		return 0;
	}
	CConfigrationIO sys(m_ParamaterFilePath);
	return sys.getInt(_T("Setting"), _T("Horizontal_resolution"));
}

/// <summary>
/// �������̉𑜓x�̐ݒ�
/// </summary>
/// <param name="resolution">�������̉𑜓x</param>
/// <returns>�����ꍇ��true�A���s�ꍇ��false��Ԃ�</returns>
bool CPropatyIO::SetHorizontalResolution(UINT resolution)
{
	if (!CFileUtil::fileExists(m_ParamaterFilePath)) {
		return false;
	}
	CConfigrationIO sys(m_ParamaterFilePath);
	if (!sys.setInt(_T("Setting"), _T("Horizontal_resolution"), resolution)) {
		return false;
	}
	return true;
}

#if 0
/// <summary>
/// �X�L�����f�[�^�̃T�C�Y�擾
/// </summary>
/// <param name="holizontal">�����l</param>
/// <param name="vertical">�����l</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CPropatyIO::GetScanDataSize(int &holizontal, int &vertical)
{
	holizontal = m_ScanDataHolizontalSize;
	vertical	= m_ScanDataVerticalSize;
	return true;
}

/// <summary>
/// �X�L�����f�[�^�̃T�C�Y�ݒ�
/// </summary>
/// <param name="holizontal">�����l</param>
/// <param name="vertical">�����l</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CPropatyIO::SetScanDataSize(int holizontal, int vertical)
{
	m_ScanDataHolizontalSize = holizontal;
	m_ScanDataVerticalSize = vertical;
	return true;
}
#endif

/// <summary>
/// �����ʂ̕��ސ��̎擾
/// </summary>
/// <param name="method">��͕��@</param>
/// <returns>�����ʂ̕��ސ���Ԃ�</returns>
UINT CPropatyIO::ResinGetNumberOfClass(int method)
{
	if (!CFileUtil::fileExists(m_ParamaterFilePath)) {
		return 0;
	}
	CConfigrationIO sys(m_ParamaterFilePath);
	// ������ ���ސ�
	CString key;
	key.Format(m_nClassFormat, method);
	UINT nClass = sys.getInt(_T("ResinSurface"), key);
	if (nClass == 0) {
		nClass = sys.getInt(_T("ResinSurface"), _T("Number_of_classifications"));
		if (nClass <= 0) {
			nClass = 10;
		}
		ResinSetNumberOfClass(method, nClass);
		sys.deleteKey(_T("ResinSurface"), _T("Number_of_classifications"));
	}
	return nClass;
}

/// <summary>
/// �����ʂ̕��ސ��̐ݒ�
/// </summary>
/// <param name="method">��͕��@</param>
/// <param name="nClass">�����ʂ̕��ސ�</param>
/// <returns>�����ꍇ��true�A���s�ꍇ��false��Ԃ�</returns>
bool CPropatyIO::ResinSetNumberOfClass(int method, UINT nClass)
{
	if (!CFileUtil::fileExists(m_ParamaterFilePath)) {
		return false;
	}
	CConfigrationIO sys(m_ParamaterFilePath);
	// ������ ���ސ�
	CString key;
	key.Format(m_nClassFormat, method);
	if (!sys.setInt(_T("ResinSurface"),key,nClass)) {
		return false;
	}
	return true;
}

/// <summary>
/// �����ʂ̐ڍ������̎擾
/// </summary>
/// <param name="method">��͕��@</param>
/// <param name="ViewJointRatioNo">�ڍ��ʔԍ�</param>
/// <returns>�����ʂ̐ڍ�������Ԃ�</returns>
double CPropatyIO::ResinGetJointRetio(int method, int ViewJointRatioNo)
{
	if (!CFileUtil::fileExists(m_ParamaterFilePath)) {
		return 0.0;
	}
	if (ViewJointRatioNo < 0) {
		return 0.0;
	}

	CString label;
	label.Format(m_joinRetioFormat, method,ViewJointRatioNo);
	CConfigrationIO sys(m_ParamaterFilePath);
	// ������ �ڍ�����
	return sys.getDouble(_T("ResinSurface"),label);
}

/// <summary>
/// �����ʂ̐ڍ������̐ݒ�
/// </summary>
/// <param name="method">��͕��@</param>
/// <param name="ViewJointRatioNo">�ڍ��ʔԍ�</param>
/// <param name="retio">�����ʂ̐ڍ�����</param>
/// <returns>�����ꍇ��true�A���s�ꍇ��false��Ԃ�</returns>
bool CPropatyIO::ResinSetJointRetio(int method, int ViewJointRatioNo, double retio)
{
	if (ViewJointRatioNo < 0) {
		return false;
	}
	CString label;
	label.Format(m_joinRetioFormat, method, ViewJointRatioNo);
	CConfigrationIO sys(m_ParamaterFilePath);
	// ������ �ڍ�����
	if (!sys.setDouble(_T("ResinSurface"),label,retio)) {
		return false;
	}
	return true;
}

/// <summary>
/// �����ʂ̐ڍ��F�̎擾
/// </summary>
/// <param name="method">��͕��@</param>
/// <param name="ViewJointRatioNo">�ڍ��ʔԍ�</param>
/// <returns>�����ʂ̐ڍ��F��Ԃ�</returns>
COLORREF CPropatyIO::ResinGetJointColor(int method, int ViewJointRatioNo)
{
	if (!CFileUtil::fileExists(m_ParamaterFilePath)) {
		return RGB(0, 0, 0);
	}
	if (ViewJointRatioNo < 0) {
		return RGB(0, 0, 0);
	}

	CString label;
	label.Format(m_joinColorFormat, method, ViewJointRatioNo);
	CConfigrationIO sys(m_ParamaterFilePath);
	// ������ �ڍ��F
	return (COLORREF)(sys.getInt(_T("ResinSurface"), label));
}

/// <summary>
/// �����ʂ̐ڍ��F�̐ݒ�
/// </summary>
/// <param name="method">��͕��@</param>
/// <param name="ViewJointRatioNo">�ڍ��ʔԍ�</param>
/// <param name="color">�����ʂ̐ڍ��F</param>
/// <returns>�����ꍇ��true�A���s�ꍇ��false��Ԃ�</returns>
bool CPropatyIO::ResinSetJointColor(int method, int ViewJointRatioNo, COLORREF color)
{
	if (ViewJointRatioNo < 0) {
		return false;
	}
	CString label;
	label.Format(m_joinColorFormat, method, ViewJointRatioNo);
	CConfigrationIO sys(m_ParamaterFilePath);
	// ������ �ڍ��F
	if (!sys.setInt(_T("ResinSurface"), label, (DWORD)color)) {
		return false;
	}
	return true;
}

/// <summary>
/// �����ʂ̉�͕��@�̎擾
/// </summary>
/// <returns>�����ʂ̉�͕��@��Ԃ�</returns>
int CPropatyIO::ResinGetAnalysisMethod()
{
	return m_ResinAnalysisMethod;
}

/// <summary>
/// �����ʂ̉�͕��@�̐ݒ�
/// </summary>
/// <param name="method">�����ʂ̉�͕��@</param>
/// <returns>�����ꍇ��true�A���s�ꍇ��false��Ԃ�</returns>
bool CPropatyIO::ResinSetAnalysisMethod(int method)
{
	m_ResinAnalysisMethod = method;
	return true;
}

/// <summary>
/// �����ʂ̕��ސ��̎擾
/// </summary>
/// <param name="method">��͕��@</param>
/// <returns>�����ʂ̕��ސ���Ԃ�</returns>
UINT CPropatyIO::MetalGetNumberOfClass(int method)
{
	if (!CFileUtil::fileExists(m_ParamaterFilePath)) {
		return 0;
	}
	CConfigrationIO sys(m_ParamaterFilePath);
	// �����ʂ̕��ސ�
	CString key;
	key.Format(m_nClassFormat, method);
	UINT nClass = sys.getInt(_T("MetalSurface"), key);
	if (nClass == 0) {
		nClass = sys.getInt(_T("MetalSurface"), _T("Number_of_classifications"));
		if (nClass <= 0) {
			nClass = 10;
		}
		MetalSetNumberOfClass(method, nClass);
		sys.deleteKey(_T("MetalSurface"), _T("Number_of_classifications"));
	}
	return nClass;
}

/// <summary>
/// �����ʂ̕��ސ��̐ݒ�
/// </summary>
/// <param name="method">��͕��@</param>
/// <param name="nClass">�����ʂ̕��ސ�</param>
/// <returns>�����ꍇ��true�A���s�ꍇ��false��Ԃ�</returns>
bool CPropatyIO::MetalSetNumberOfClass(int method,UINT nClass)
{
//	if (!CFileUtil::fileExists(m_ParamaterFilePath)) {
//		return false;
//	}
	CConfigrationIO sys(m_ParamaterFilePath);
	CString key;
	key.Format(m_nClassFormat, method);
	if (!sys.setInt(_T("MetalSurface"),key,nClass)) {
		return false;
	}
	return true;
}

/// <summary>
/// �����ʂ̐ڍ������̎擾
/// </summary>
/// <param name="method">��͕��@</param>
/// <param name="ViewJointRatioNo">�ڍ��ʔԍ�</param>
/// <returns>�����ʂ̐ڍ�������Ԃ�</returns>
double CPropatyIO::MetalGetJointRetio(int method, int ViewJointRatioNo)
{
	if (!CFileUtil::fileExists(m_ParamaterFilePath)) {
		return false;
	}
	CString label;
	label.Format(m_joinRetioFormat, method, ViewJointRatioNo);
	CConfigrationIO sys(m_ParamaterFilePath);
	return sys.getDouble(_T("MetalSurface"),label);
}

/// <summary>
/// �����ʂ̐ڍ������̐ݒ�
/// </summary>
/// <param name="method">��͕��@</param>
/// <param name="ViewJointRatioNo">�ڍ��ʔԍ�</param>
/// <param name="retio">�����ʂ̐ڍ�����</param>
/// <returns>�����ꍇ��true�A���s�ꍇ��false��Ԃ�</returns>
bool CPropatyIO::MetalSetJointRetio(int method, int ViewJointRatioNo, double retio)
{
	if (ViewJointRatioNo < 0) {
		return false;
	}
	CString label;
	label.Format(m_joinRetioFormat, method, ViewJointRatioNo);
	CConfigrationIO sys(m_ParamaterFilePath);
	if (!sys.setDouble(_T("MetalSurface"),label,retio)) {
		return false;
	}
	return true;
}

/// <summary>
/// �����ʂ̐ڍ��F�̎擾
/// </summary>
/// <param name="method">��͕��@</param>
/// <param name="ViewJointRatioNo">�ڍ��ʔԍ�</param>
/// <returns>�����ʂ̐ڍ��F��Ԃ�</returns>
COLORREF CPropatyIO::MetalGetJointColor(int method, int ViewJointRatioNo)
{
	if (!CFileUtil::fileExists(m_ParamaterFilePath)) {
		return RGB(0, 0, 0);
	}
	if (ViewJointRatioNo < 0) {
		return RGB(0, 0, 0);
	}

	CString label;
	label.Format(m_joinColorFormat, method, ViewJointRatioNo);
	CConfigrationIO sys(m_ParamaterFilePath);
	// ������ �ڍ��F
	return (COLORREF)(sys.getInt(_T("MetalSurface"), label));
}

/// <summary>
/// �����ʂ̐ڍ��F�̐ݒ�
/// </summary>
/// <param name="method">��͕��@</param>
/// <param name="ViewJointRatioNo">�ڍ��ʔԍ�</param>
/// <param name="color">�����ʂ̐ڍ��F</param>
/// <returns>�����ꍇ��true�A���s�ꍇ��false��Ԃ�</returns>
bool CPropatyIO::MetalSetJointColor(int method, int ViewJointRatioNo, COLORREF color)
{
	if (ViewJointRatioNo < 0) {
		return false;
	}
	CString label;
	label.Format(m_joinColorFormat, method, ViewJointRatioNo);
	CConfigrationIO sys(m_ParamaterFilePath);
	// ������ �ڍ��F
	if (!sys.setInt(_T("MetalSurface"), label, (DWORD)color)) {
		return false;
	}
	return true;
}

/// <summary>
/// �����ʂ̉�͕��@�̎擾
/// </summary>
/// <returns>�����ʂ̉�͕��@��Ԃ�</returns>
int CPropatyIO::MetalGetAnalysisMethod()
{
	return m_MetalAnalysisMethod;
}

/// <summary>
/// �����ʂ̉�͕��@�̐ݒ�
/// </summary>
/// <param name="method">�����ʂ̉�͕��@</param>
/// <returns>�����ꍇ��true�A���s�ꍇ��false��Ԃ�</returns>
bool CPropatyIO::MetalSetAnalysisMethod(int method)
{
	m_MetalAnalysisMethod = method;
	return true;
}

/// <summary>
/// �ڍ����ʂ̕��ސ��̎擾
/// </summary>
/// <param name="method">��͕��@</param>
/// <returns>�ڍ����ʂ̕��ސ���Ԃ�</returns>
UINT CPropatyIO::ResultGetNumberOfClass(int method)
{
	if (!CFileUtil::fileExists(m_ParamaterFilePath)) {
		return 0;
	}
	CConfigrationIO sys(m_ParamaterFilePath);
	// �ڍ����� ���ސ�
	CString key;
	key.Format(m_nClassFormat, method);
	UINT nClass = sys.getInt(_T("JoiningResult"), key);
	if (nClass == 0) {
		nClass = sys.getInt(_T("JoiningResult"), _T("Number_of_classifications"));
		if (nClass <= 0) {
			nClass = 10;
		}
		ResultSetNumberOfClass(method, nClass);
		sys.deleteKey(_T("JoiningResult"), _T("Number_of_classifications"));
	}
	return nClass;
}

/// <summary>
/// �ڍ����ʂ̕��ސ��̐ݒ�
/// </summary>
/// <param name="method">��͕��@</param>
/// <param name="nClass">�ڍ����ʂ̕��ސ�</param>
/// <returns>�����ꍇ��true�A���s�ꍇ��false��Ԃ�</returns>
bool CPropatyIO::ResultSetNumberOfClass(int method, UINT nClass)
{
//	if (!CFileUtil::fileExists(m_ParamaterFilePath)) {
//		return false;
//	}
	CConfigrationIO sys(m_ParamaterFilePath);
	// �ڍ����� ���ސ�
	CString key;
	key.Format(m_nClassFormat, method);
	if (!sys.setInt(_T("JoiningResult"),key,nClass)) {
		return false;
	}
	return true;
}

/// <summary>
/// �ڍ����ʂ̐ڍ������̎擾
/// </summary>
/// <param name="method">��͕��@</param>
/// <param name="ViewJointRatioNo">�ڍ��ʔԍ�</param>
/// <returns>�ڍ����ʂ̐ڍ�������Ԃ�</returns>
double CPropatyIO::ResultGetJointRetio(int method, int ViewJointRatioNo)
{
	if (!CFileUtil::fileExists(m_ParamaterFilePath)) {
		return false;
	}
	CString label;
	label.Format(m_joinRetioFormat, method, ViewJointRatioNo);
	CConfigrationIO sys(m_ParamaterFilePath);
	return sys.getDouble(_T("JoiningResult"),label);
}

/// <summary>
/// �ڍ����ʂ̐ڍ������̐ݒ�
/// </summary>
/// <param name="method">��͕��@</param>
/// <param name="ViewJointRatioNo">�ڍ��ʔԍ�</param>
/// <param name="retio">�ڍ����ʂ̐ڍ�����</param>
/// <returns>�����ꍇ��true�A���s�ꍇ��false��Ԃ�</returns>
bool CPropatyIO::ResultSetJointRetio(int method, int ViewJointRatioNo, double retio)
{
	if (ViewJointRatioNo < 0) {
		return false;
	}
	CString label;
	label.Format(m_joinRetioFormat, method, ViewJointRatioNo);
	CConfigrationIO sys(m_ParamaterFilePath);
	if (!sys.setDouble(_T("JoiningResult"),label,retio)) {
		return false;
	}
	return true;
}

/// <summary>
/// �ڍ����ʂ̐ڍ��F�̎擾
/// </summary>
/// <param name="method">��͕��@</param>
/// <param name="ViewJointRatioNo">�ڍ��ʔԍ�</param>
/// <returns>�ڍ����ʂ̐ڍ��F��Ԃ�</returns>
COLORREF CPropatyIO::ResultGetJointColor(int method, int ViewJointRatioNo)
{
	if (!CFileUtil::fileExists(m_ParamaterFilePath)) {
		return RGB(0, 0, 0);
	}
	if (ViewJointRatioNo < 0) {
		return RGB(0, 0, 0);
	}

	CString label;
	label.Format(m_joinColorFormat, method, ViewJointRatioNo);
	CConfigrationIO sys(m_ParamaterFilePath);
	// �ڍ����� �ڍ��F
	return (COLORREF)(sys.getInt(_T("JoiningResult"), label));
}

/// <summary>
/// �ڍ����ʂ̐ڍ��F�̐ݒ�
/// </summary>
/// <param name="method">��͕��@</param>
/// <param name="ViewJointRatioNo">�ڍ��ʔԍ�</param>
/// <param name="color">�ڍ����ʂ̐ڍ��F</param>
/// <returns>�����ꍇ��true�A���s�ꍇ��false��Ԃ�</returns>
bool CPropatyIO::ResultSetJointColor(int method, int ViewJointRatioNo, COLORREF color)
{
	if (ViewJointRatioNo < 0) {
		return false;
	}
	CString label;
	label.Format(m_joinColorFormat, method, ViewJointRatioNo);
	CConfigrationIO sys(m_ParamaterFilePath);
	// �ڍ����� �ڍ��F
	if (!sys.setInt(_T("JoiningResult"), label, (DWORD)color)) {
		return false;
	}
	return true;
}

/// <summary>
/// �ڍ����ʂ̉�͕��@�̎擾
/// </summary>
/// <returns>�ڍ����ʂ̉�͕��@��Ԃ�</returns>
int CPropatyIO::ResultGetAnalysisMethod()
{
	return m_ResultAnalysisMethod;
}

/// <summary>
/// �ڍ����ʂ̉�͕��@�̐ݒ�
/// </summary>
/// <param name="method">�ڍ����ʂ̉�͕��@</param>
/// <returns>�����ꍇ��true�A���s�ꍇ��false��Ԃ�</returns>
bool CPropatyIO::ResultSetAnalysisMethod(int method)
{
	m_ResultAnalysisMethod = method;
	return true;
}

/// <summary>
/// �p�����[�^�t�@�C���̓ǂݍ���
/// </summary>
/// <param name="path">�p�����[�^�t�@�C���ւ̃p�X</param>
/// <returns>�����ꍇ��true�A���s�ꍇ��false��Ԃ�</returns>
bool CPropatyIO::Read(CString path/* = _T("")*/)
{
	if (path.IsEmpty()) {
		path = m_ParamaterFilePath;
	}
	if (!CFileUtil::fileExists(path)) {
		return false;
	}
	return read(path);
}

/// <summary>
/// �p�����[�^�t�@�C���̏�������
/// </summary>
/// <param name="bUpdate">�X�V�t���O</param>
/// <param name="path">�p�����[�^�t�@�C���ւ̃p�X</param>
/// <returns>�����ꍇ��true�A���s�ꍇ��false��Ԃ�</returns>
bool CPropatyIO::Save(bool bUpdate/* = true*/, CString path/* = _T("")*/)
{
	if (path.IsEmpty()) {
		path = m_ParamaterFilePath;
	}
	if ((bUpdate) && (!CFileUtil::fileExists(path))) {
		return false;
	}
	return save(path);
}
