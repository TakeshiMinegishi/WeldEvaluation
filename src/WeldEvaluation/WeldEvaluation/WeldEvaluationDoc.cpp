
// WeldEvaluationDoc.cpp : CWeldEvaluationDoc �N���X�̎���
//

#include "stdafx.h"
// SHARED_HANDLERS �́A�v���r���[�A�T���l�C���A����ь����t�B���^�[ �n���h���[���������Ă��� ATL �v���W�F�N�g�Œ�`�ł��A
// ���̃v���W�F�N�g�Ƃ̃h�L�������g �R�[�h�̋��L���\�ɂ��܂��B
#ifndef SHARED_HANDLERS
#include "WeldEvaluation.h"
#endif

#include "WeldEvaluationDoc.h"

#include <propkey.h>
#include <map>
#include <iomanip>
#include <atlimage.h>
#include "message.h"
#include "FileUtil.h"
#include "ConfigrationIO.h"
#include "CLog.h"

#pragma warning(disable:4800)
#pragma warning(disable:4100)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define StatysFileName _T("__Status__.sys")
#define NewProjectFolder _T("NewProject")

// CWeldEvaluationDoc

IMPLEMENT_DYNCREATE(CWeldEvaluationDoc, CDocument)

BEGIN_MESSAGE_MAP(CWeldEvaluationDoc, CDocument)
END_MESSAGE_MAP()


// CWeldEvaluationDoc �R���X�g���N�V����/�f�X�g���N�V����

/// <summary>
/// �R���X�g���N�^
/// </summary>
CWeldEvaluationDoc::CWeldEvaluationDoc()
{
	m_ModulePath = CFileUtil::GetModulePath();
	m_SystemFilePathName		= m_ModulePath + CString("system.cnf");
	m_ActiveRegisttedTestName	= _T("");
	m_ParamaterFilePaht			= _T("");
	m_ProjectFilePaht			= _T("");
	m_OpenType					= 0;
	m_bVisible					= false;
	m_bInitialized				= false;
	m_minRect					= CSize(0, 0);
	m_CalcResultStopRequest		= false;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
CWeldEvaluationDoc::~CWeldEvaluationDoc()
{
	m_ResinScanData.release();
	m_MetalScanData.release();
	m_ResultScanData.release();
}

/// <summary>
/// ���O�o��
/// </summary>
/// <param name="level">���O���x��</param>
/// <param name="filePath">�G���[�t�@�C��</param>
/// <param name="lineNo">�G���[�s</param>
/// <param name="msg">���O���b�Z�[�W</param>
void CWeldEvaluationDoc::writeLog(CLog::LOGLEVEL level, CString filePath, long lineNo, CString msg)
{
	CLog log;
	CString ErrMsg;
	ErrMsg.Format(_T(" File:%s Line:%ld:%s"), (LPCTSTR)filePath, lineNo, (LPCTSTR)msg);
	log.logWrite(level, ErrMsg);
}

/// <summary>
/// �r�W�u���t���O�ݒ�
/// </summary>
/// <param name="bVisible">���</param>
void CWeldEvaluationDoc::SetVisible(bool bVisible)
{
	m_bVisible = bVisible;
}

/// <summary>
/// �V�K�v���W�F�N�g�쐬
/// </summary>
/// <returns>���������ꍇ��TRUE�A���s�����ꍇ��FALSE��Ԃ�</returns>
BOOL CWeldEvaluationDoc::OnNewDocument()
{
	if (IsWorkProjectUpdated()) {
		CString msg,fmt;
		if (!m_bVisible) {
			CString rootPath = GetWorkProjectFolderPath();
			CString prjName = GetWorkProjectStatus(rootPath, _T("ProjectName"));
			if (!fmt.LoadString(IDS_EXIST_REPAIRABLEDATA)) {
				fmt = _T("�C���\�ȓo�^�f�[�^[%s]�����݂��܂��B\n�C�����܂����H");
			}
			msg.Format(fmt, (LPCTSTR)prjName);
			if (AfxMessageBox(msg, MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON1) != IDNO) {
				CString resistFolder = GetRegistedFolder();
				if (!PopProject(resistFolder, prjName)) {
					if (!msg.LoadString(IDM_ERR_FAIL_REGIST)) {
						msg = _T("�C���Ɏ��s�������܂����B");
					}
					AfxMessageBox(msg, MB_ICONSTOP);
				}
				else {
					if (!msg.LoadString(IDS_REPAIR_COMPLEAT)) {
						msg = _T("�C�����������܂����B");
					}
					AfxMessageBox(msg, MB_ICONINFORMATION);
				}
			}
		}
	}

	if (!CDocument::OnNewDocument()) {
		CString msg;
		msg.Format(_T("�V�K�v���W�F�N�g�̍쐬�Ɏ��s���܂����B"));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return FALSE;
	}

	if (!NewProject()) {
		CString msg;
		msg.Format(_T("�V�K�v���W�F�N�g�̍쐬�Ɏ��s���܂����B"));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return FALSE;
	}
	return TRUE;
}

// CWeldEvaluationDoc �V���A����

/// <summary>
/// �V���A���C�Y
/// </summary>
/// <param name="ar">�A�[�J�C�u�I�u�W�F�N�g</param>
void CWeldEvaluationDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �i�[����R�[�h�������ɒǉ����Ă��������B
	}
	else
	{
		// TODO: �ǂݍ��ރR�[�h�������ɒǉ����Ă��������B
	}
}

#ifdef SHARED_HANDLERS

// �T���l�C���̃T�|�[�g
void CWeldEvaluationDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// ���̃R�[�h��ύX���ăh�L�������g�̃f�[�^��`�悵�܂�
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// �����n���h���[�̃T�|�[�g
void CWeldEvaluationDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// �h�L�������g�̃f�[�^���猟���R���e���c��ݒ肵�܂��B 
	// �R���e���c�̊e������ ";" �ŋ�؂�K�v������܂�

	// ��:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CWeldEvaluationDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CWeldEvaluationDoc �f�f

#ifdef _DEBUG
/// <summary>
/// �I�u�W�F�N�g�̑Ó��������̎��{
/// </summary>
void CWeldEvaluationDoc::AssertValid() const
{
	CDocument::AssertValid();
}

/// <summary>
/// CDumpContext�I�u�W�F�N�g�̓��e���_���v
/// </summary>
/// <param name="dc">afxDump�f�f�_���v �R���e�L�X�g</param>
void CWeldEvaluationDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CWeldEvaluationDoc �R�}���h


////////////////////////////////////////////////////////////////////////
// �v���p�e�B�h�^�n

/// <summary>
/// �v���W�F�N�g���̎擾
/// </summary>
/// <returns>�v���W�F�N�g����Ԃ�</returns>
CString CWeldEvaluationDoc::GetProjectName()
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		return _T("");
	} else {
		return m_PropatyIO.GetProjectName();
	}
}

/// <summary>
/// �v���W�F�N�g���̐ݒ�
/// </summary>
/// <param name="projectname">�v���W�F�N�g��</param>
/// <returns>�����ꍇ��true�A���s�ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::SetProjectName(CString projectname)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		CString msg;
		msg.Format(_T("�v���W�F�N�g���̐ݒ�̐ݒ�Ɏ��s���܂����B"));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	} else {
		return m_PropatyIO.SetProjectName(projectname);
	}
}

/// <summary>
/// �o�^�ςݎ����i�[�t�H���_�̎擾
/// </summary>
/// <returns>�o�^�ςݎ����i�[�t�H���_��Ԃ�</returns>
CString CWeldEvaluationDoc::GetRegistedFolder(void)
{
	CConfigrationIO sys(m_SystemFilePathName);
	CString folder = sys.getString(_T("Common"),_T("RegistedFolder"));
	if (folder.IsEmpty()) {
		folder = m_ModulePath + _T("Registed");
	}
	if (!CFileUtil::fileExists(folder)) {
		if (!CreateDirectory(folder,NULL)) {
			folder.Empty();
		}
	} else {
		if (!sys.setString(_T("Common"),_T("RegistedFolder"),folder)) {
			folder.Empty();
		}
	}
	return folder;
}

/// <summary>
/// �o�^�ςݎ����i�[�t�H���_�̐ݒ�
/// </summary>
/// <param name="RegistedFolder">�o�^�ςݎ����i�[�t�H���_</param>
/// <returns>�����ꍇ��true�A���s�ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::SetRegistedFolder(CString RegistedFolder)
{
	CConfigrationIO sys(m_SystemFilePathName);
	return sys.setString(_T("Common"),_T("RegistedFolder"),RegistedFolder);
}

/// <summary>
/// �c�����̉𑜓x�̎擾
/// </summary>
/// <returns>�c�����̉𑜓x��Ԃ�</returns>
UINT CWeldEvaluationDoc::GetVerticalResolution(void)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		CConfigrationIO sys(m_SystemFilePathName);
		return sys.getInt(_T("ParamDefault"), _T("Vertical_resolution"));
	}
	else {
		UINT val = m_PropatyIO.GetVerticalResolution();
		if (val == 0) {
			CConfigrationIO sys(m_SystemFilePathName);
			val = sys.getInt(_T("ParamDefault"), _T("Vertical_resolution"));
		}
		return val;
	}
}

/// <summary>
/// �c�����̉𑜓x�̐ݒ�
/// </summary>
/// <param name="VerticalResolution">�c�����̉𑜓x</param>
/// <returns>�����ꍇ��true�A���s�ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::SetVerticalResolution(UINT VerticalResolution)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		CString msg;
		msg.Format(_T("�c�����̉𑜓x�̐ݒ�Ɏ��s���܂����B"));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}
	else {
		return m_PropatyIO.SetVerticalResolution(VerticalResolution);
	}
}

/// <summary>
/// �������̉𑜓x�̎擾
/// </summary>
/// <returns>�������̉𑜓x��Ԃ�</returns>
UINT CWeldEvaluationDoc::GetHorizontalResolution(void)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		CConfigrationIO sys(m_SystemFilePathName);
		return sys.getInt(_T("ParamDefault"), _T("Horizontal_resolution"));
	}
	else {
		UINT val = m_PropatyIO.GetHorizontalResolution();
		if (val == 0) {
			CConfigrationIO sys(m_SystemFilePathName);
			val = sys.getInt(_T("ParamDefault"), _T("Horizontal_resolution"));
		}
		return val;
	}
}

/// <summary>
/// �������̉𑜓x�̐ݒ�
/// </summary>
/// <param name="HorizontalResolution">�������̉𑜓x</param>
/// <returns>�����ꍇ��true�A���s�ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::SetHorizontalResolution(UINT HorizontalResolution)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		CString msg;
		msg.Format(_T("�������̉𑜓x�̐ݒ�Ɏ��s���܂����B"));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}
	else {
		return m_PropatyIO.SetHorizontalResolution(HorizontalResolution);
	}
}

/// <summary>
/// �d�Ȃ�s�N�Z�����̎擾
/// </summary>
/// <returns>�d�Ȃ�s�N�Z������Ԃ�</returns>
UINT CWeldEvaluationDoc::NumberOfOverridePixel(void)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		CConfigrationIO sys(m_SystemFilePathName);
		return sys.getInt(_T("ParamDefault"), _T("Number_of_overlapping_pixels"));
	}
	else {
		UINT val = m_PropatyIO.NumberOfOverridePixel();
		if (val == 0) {
			CConfigrationIO sys(m_SystemFilePathName);
			val = sys.getInt(_T("ParamDefault"), _T("Number_of_overlapping_pixels"));
		}
		return val;
	}
	bool SetOverridePixelNumber(UINT num);
}

/// <summary>
/// �d�Ȃ�s�N�Z�����̐ݒ�
/// </summary>
/// <param name="num">�d�Ȃ�s�N�Z����</param>
/// <returns>�����ꍇ��true�A���s�ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::SetOverridePixelNumber(UINT num)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		CString msg;
		msg.Format(_T("�d�Ȃ�s�N�Z�����̐ݒ�Ɏ��s���܂����B"));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}
	else {
		return m_PropatyIO.SetOverridePixelNumber(num);
	}
}

/// <summary>
/// �z���C�g�o�����X�t�@�C�����̎擾
/// </summary>
/// <returns>�z���C�g�o�����X�t�@�C������Ԃ�</returns>
CString CWeldEvaluationDoc::GetWBFileName(void)
{
	CConfigrationIO sys(m_SystemFilePathName);
	CString file = sys.getString(_T("Common"),_T("WBFileName"));
	if (file.IsEmpty()) {
		file = _T("WhiteBalance");
	}
	return file;
}

/// <summary>
/// �z���C�g�o�����X�t�@�C�����̐ݒ�
/// </summary>
/// <param name="WBFileName">�z���C�g�o�����X�t�@�C����</param>
/// <returns>�����ꍇ��true�A���s�ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::SetWBFileName(CString WBFileName)
{
	CConfigrationIO sys(m_SystemFilePathName);
	return sys.setString(_T("Common"),_T("WBFileName"),WBFileName);
}

/// <summary>
/// ���̂̎擾
/// </summary>
/// <returns>���̂�Ԃ�</returns>
CString CWeldEvaluationDoc::GetTestName()
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		CConfigrationIO sys(m_SystemFilePathName);
		return sys.getString(_T("ParamDefault"),_T("Test_name"));
	} else {
		CString TestName = m_PropatyIO.GetTestName();
		if (TestName.IsEmpty()) {
			CConfigrationIO sys(m_SystemFilePathName);
			TestName = sys.getString(_T("ParamDefault"), _T("Test_name"));
		}
		return TestName;
	}
}

/// <summary>
/// ���̂̐ݒ�
/// </summary>
/// <param name="name">����</param>
/// <returns>�����ꍇ��true�A���s�ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::SetTestName(CString name)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		if (m_OpenType != 2) {
			CString msg;
			msg.Format(_T("�������̐ݒ�Ɏ��s���܂����B(name=[%s])"), static_cast<LPCWSTR>(name));
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
			return false;
		}
	}
	return m_PropatyIO.SetTestName(name);
}

/// <summary>
/// �����ʂ̕��ސ��̎擾
/// </summary>
/// <param name="method">��͕��@</param>
/// <returns>�����ʂ̕��ސ���Ԃ�</returns>
UINT CWeldEvaluationDoc::ResinGetNumberOfClass(int method)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		CConfigrationIO sys(m_SystemFilePathName);
		return sys.getInt(_T("ParamDefault"),_T("ResinSurface_Number_of_classifications"));
	} else {
		UINT val = m_PropatyIO.ResinGetNumberOfClass(method);
		if (val == 0) {
			CConfigrationIO sys(m_SystemFilePathName);
			val = sys.getInt(_T("ParamDefault"),_T("ResinSurface_Number_of_classifications"));
		}
		return val;
	}
}

/// <summary>
/// �����ʂ̕��ސ��̐ݒ�
/// </summary>
/// <param name="method">��͕��@</param>
/// <param name="nClass">�����ʂ̕��ސ�</param>
/// <returns>�����ꍇ��true�A���s�ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::ResinSetNumberOfClass(int method, UINT nClass)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		CString msg;
		msg.Format(_T("�����ʂ̕��ސ��̐ݒ�Ɏ��s���܂����B(method=%d, nClass=%d)"), method, nClass);
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	} else {
		return m_PropatyIO.ResinSetNumberOfClass(method,nClass);
	}
}

/// <summary>
/// �����ʂ̐ڍ������̎擾
/// </summary>
/// <param name="method">��͕��@</param>
/// <param name="ViewJointRatioNo">�ڍ��ʔԍ�</param>
/// <returns>�����ʂ̐ڍ�������Ԃ�</returns>
double CWeldEvaluationDoc::ResinGetJointRetio(int method, int ViewJointRatioNo)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		CConfigrationIO sys(m_SystemFilePathName);
		return sys.getDouble(_T("ParamDefault"),_T("ResinSurface_Joining_ratio"));
	} else {
		double val = m_PropatyIO.ResinGetJointRetio(method,ViewJointRatioNo);
		if (val == 0) {
			CConfigrationIO sys(m_SystemFilePathName);
			val = sys.getDouble(_T("ParamDefault"),_T("ResinSurface_Joining_ratio"));
		}
		return val;
	}
}

/// <summary>
/// �����ʂ̐ڍ������̐ݒ�
/// </summary>
/// <param name="method">��͕��@</param>
/// <param name="ViewJointRatioNo">�ڍ��ʔԍ�</param>
/// <param name="retio">�����ʂ̐ڍ�����</param>
/// <returns>�����ꍇ��true�A���s�ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::ResinSetJointRetio(int method, int ViewJointRatioNo, double retio)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		CString msg;
		msg.Format(_T("�����ʂ̐ڍ������̐ݒ�Ɏ��s���܂����B(method=%d, no=%d, retio=%lf)"), method, ViewJointRatioNo, retio);
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	} else {
		return m_PropatyIO.ResinSetJointRetio(method,ViewJointRatioNo, retio);
	}
}

/// <summary>
/// �����ʂ̐ڍ��F�̎擾
/// </summary>
/// <param name="method">��͕��@</param>
/// <param name="ViewJointRatioNo">�ڍ��ʔԍ�</param>
/// <returns>�����ʂ̐ڍ��F��Ԃ�</returns>
COLORREF CWeldEvaluationDoc::ResinGetJointColor(int method, int ViewJointRatioNo)
{
	COLORREF color = RGB(0, 0, 0);
	if (!m_ActiveRegisttedTestName.IsEmpty()) {
		color = m_PropatyIO.ResinGetJointColor(method,ViewJointRatioNo);
	}
	return color;
}

/// <summary>
/// �����ʂ̐ڍ��F�̐ݒ�
/// </summary>
/// <param name="method">��͕��@</param>
/// <param name="ViewJointRatioNo">�ڍ��ʔԍ�</param>
/// <param name="color">�ݒ�F</param>
/// <returns>�����ꍇ��true�A���s�ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::ResinSetJointColor(int method, int ViewJointRatioNo, COLORREF color)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		CString msg;
		msg.Format(_T("�����ʂ̐ڍ��F�̐ݒ�Ɏ��s���܂����B(method=%d, no=%d, color=%ld)"), method, ViewJointRatioNo, color);
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}
	else {
		return m_PropatyIO.ResinSetJointColor(method, ViewJointRatioNo, color);
	}
}

/// <summary>
/// �����ʂ̉�͕��@�̎擾
/// </summary>
/// <returns>�����ʂ̉�͕��@��Ԃ�</returns>
int CWeldEvaluationDoc::ResinGetAnalysisMethod()
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		CConfigrationIO sys(m_SystemFilePathName);
		return sys.getInt(_T("ParamDefault"),_T("ResinSurface_Analysis_method"));
	} else {
		int val = m_PropatyIO.ResinGetAnalysisMethod();
		return val;
	}
}

/// <summary>
/// �����ʂ̉�͕��@�̐ݒ�
/// </summary>
/// <param name="method">�����ʂ̉�͕��@</param>
/// <returns>�����ꍇ��true�A���s�ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::ResinSetAnalysisMethod(int method)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		CString msg;
		msg.Format(_T("�����ʂ̉�͕��@�̐ݒ�Ɏ��s���܂����B(method=%d)"), method);
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	} else {
		return m_PropatyIO.ResinSetAnalysisMethod(method);
	}
}

/// <summary>
/// �����ʂ̕��ސ��̎擾
/// </summary>
/// <param name="method">��͕��@</param>
/// <returns>�����ʂ̕��ސ���Ԃ�</returns>
UINT CWeldEvaluationDoc::MetalGetNumberOfClass(int method)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		CConfigrationIO sys(m_SystemFilePathName);
		return sys.getInt(_T("ParamDefault"),_T("MetalSurface_Number_of_classifications"));
	} else {
		UINT val = m_PropatyIO.MetalGetNumberOfClass(method);
		if (val == 0) {
			CConfigrationIO sys(m_SystemFilePathName);
			val = sys.getInt(_T("ParamDefault"),_T("MetalSurface_Number_of_classifications"));
		}
		return val;
	}
}

/// <summary>
/// �����ʂ̕��ސ��̐ݒ�
/// </summary>
/// <param name="method">��͕��@</param>
/// <param name="nClass">�����ʂ̕��ސ�</param>
/// <returns>�����ꍇ��true�A���s�ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::MetalSetNumberOfClass(int method, UINT nClass)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		CString msg;
		msg.Format(_T("�����ʂ̕��ސ��̐ݒ�Ɏ��s���܂����B(method=%d, nClass=%d)"), method, nClass);
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	} else {
		return m_PropatyIO.MetalSetNumberOfClass(method, nClass);
	}
}

/// <summary>
/// �����ʂ̐ڍ������̎擾
/// </summary>
/// <param name="method">��͕��@</param>
/// <param name="ViewJointRatioNo">�ڍ��ʔԍ�</param>
/// <returns>�����ʂ̐ڍ�������Ԃ�</returns>
double CWeldEvaluationDoc::MetalGetJointRetio(int method, int ViewJointRatioNo)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		CConfigrationIO sys(m_SystemFilePathName);
		return sys.getDouble(_T("ParamDefault"),_T("MetalSurface_Joining_ratio"));
	} else {
		double val = m_PropatyIO.MetalGetJointRetio(method,ViewJointRatioNo);
		if (val == 0) {
			CConfigrationIO sys(m_SystemFilePathName);
			val = sys.getDouble(_T("ParamDefault"),_T("MetalSurface_Joining_ratio"));
		}
		return val;
	}
}

/// <summary>
/// �����ʂ̐ڍ������̐ݒ�
/// </summary>
/// <param name="method">��͕��@</param>
/// <param name="ViewJointRatioNo">�ڍ��ʔԍ�</param>
/// <param name="retio">�����ʂ̐ڍ�����</param>
/// <returns>�����ꍇ��true�A���s�ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::MetalSetJointRetio(int method, int ViewJointRatioNo,double retio)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		CString msg;
		msg.Format(_T("�����ʂ̐ڍ������̐ݒ�Ɏ��s���܂����B(method=%d, no=%d, retio=%lf)"), method, ViewJointRatioNo, retio);
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	} else {
		return m_PropatyIO.MetalSetJointRetio(method,ViewJointRatioNo,retio);
	}
}

/// <summary>
/// �����ʂ̐ڍ��F�̎擾
/// </summary>
/// <param name="method">��͕��@</param>
/// <param name="ViewJointRatioNo">�ڍ��ʔԍ�</param>
/// <returns>�����ʂ̐ڍ��F��Ԃ�</returns>
COLORREF CWeldEvaluationDoc::MetalGetJointColor(int method, int ViewJointRatioNo)
{
	COLORREF color = RGB(0, 0, 0);
	if (!m_ActiveRegisttedTestName.IsEmpty()) {
		color = m_PropatyIO.MetalGetJointColor(method, ViewJointRatioNo);
	}
	return color;
}

/// <summary>
/// �����ʂ̐ڍ��F�̐ݒ�
/// </summary>
/// <param name="method">��͕��@</param>
/// <param name="ViewJointRatioNo">�ڍ��ʔԍ�</param>
/// <param name="color">�ݒ�F</param>
/// <returns>�����ꍇ��true�A���s�ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::MetalSetJointColor(int method, int ViewJointRatioNo, COLORREF color)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		CString msg;
		msg.Format(_T("�����ʂ̐ڍ��F�̐ݒ�Ɏ��s���܂����B(method=%d, no=%d, color=%ld)"), method, ViewJointRatioNo, color);
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}
	else {
		return m_PropatyIO.MetalSetJointColor(method, ViewJointRatioNo, color);
	}
}

/// <summary>
/// �����ʂ̉�͕��@�̎擾
/// </summary>
/// <returns>�����ʂ̉�͕��@��Ԃ�</returns>
int CWeldEvaluationDoc::MetalGetAnalysisMethod()
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		CConfigrationIO sys(m_SystemFilePathName);
		return sys.getInt(_T("ParamDefault"),_T("MetalSurface_Analysis_method"));
	} else {
		int val = m_PropatyIO.MetalGetAnalysisMethod();
		return val;
	}
}

/// <summary>
/// �����ʂ̉�͕��@�̐ݒ�
/// </summary>
/// <param name="method">�����ʂ̉�͕��@</param>
/// <returns>�����ꍇ��true�A���s�ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::MetalSetAnalysisMethod(int method)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		CString msg;
		msg.Format(_T("�����ʂ̉�͕��@�̐ݒ�Ɏ��s���܂����B(method=%d)"), method);
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	} else {
		return m_PropatyIO.MetalSetAnalysisMethod(method);
	}
}

/// <summary>
/// �ڍ����ʂ̕��ސ��̎擾
/// </summary>
/// <param name="method">��͕��@</param>
/// <returns>�ڍ����ʂ̕��ސ���Ԃ�</returns>
UINT CWeldEvaluationDoc::ResultGetNumberOfClass(int method)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		CConfigrationIO sys(m_SystemFilePathName);
		return sys.getInt(_T("ParamDefault"),_T("JoiningResult_Number_of_classifications"));
	} else {
		UINT val = m_PropatyIO.ResultGetNumberOfClass(method);
		if (val == 0) {
			CConfigrationIO sys(m_SystemFilePathName);
			val = sys.getInt(_T("ParamDefault"),_T("JoiningResult_Number_of_classifications"));
		}
		return val;
	}
}

/// <summary>
/// �ڍ����ʂ̕��ސ��̐ݒ�
/// </summary>
/// <param name="method">��͕��@</param>
/// <param name="nClass">�ڍ����ʂ̕��ސ�</param>
/// <returns>�����ꍇ��true�A���s�ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::ResultSetNumberOfClass(int method, UINT nClass)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		CString msg;
		msg.Format(_T("�ڍ����ʂ̕��ސ��̐ݒ�Ɏ��s���܂����B(method=%d, nClass=%d)"), method, nClass);
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	} else {
		return m_PropatyIO.ResultSetNumberOfClass(method,nClass);
	}
}

/// <summary>
/// �ڍ����ʂ̐ڍ������̎擾
/// </summary>
/// <param name="method">��͕��@</param>
/// <param name="ViewJointRatioNo">�ڍ��ʔԍ�</param>
/// <returns>�ڍ����ʂ̐ڍ�������Ԃ�</returns>
double CWeldEvaluationDoc::ResultGetJointRetio(int method, int ViewJointRatioNo)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		CConfigrationIO sys(m_SystemFilePathName);
		return sys.getDouble(_T("ParamDefault"),_T("JoiningResult_Joining_ratio"));
	} else {
		double val = m_PropatyIO.ResultGetJointRetio(method, ViewJointRatioNo);
		if (val == 0) {
			CConfigrationIO sys(m_SystemFilePathName);
			val = sys.getDouble(_T("ParamDefault"),_T("JoiningResult_Joining_ratio"));
		}
		return val;
	}
}

/// <summary>
/// �ڍ����ʂ̐ڍ������̐ݒ�
/// </summary>
/// <param name="method">��͕��@</param>
/// <param name="ViewJointRatioNo">�ڍ��ʔԍ�</param>
/// <param name="retio">�ڍ����ʂ̐ڍ�����</param>
/// <returns>�����ꍇ��true�A���s�ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::ResultSetJointRetio(int method, int ViewJointRatioNo, double retio)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		CString msg;
		msg.Format(_T("�ڍ����ʂ̐ڍ������̐ݒ�Ɏ��s���܂����B(method=%d, no=%d, retio=%lf)"), method, ViewJointRatioNo, retio);
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	} else {
		return m_PropatyIO.ResultSetJointRetio(method, ViewJointRatioNo, retio);
	}
}

/// <summary>
/// �ڍ����ʂ̐ڍ��F�̎擾
/// </summary>
/// <param name="method">��͕��@</param>
/// <param name="ViewJointRatioNo">�ڍ��ʔԍ�</param>
/// <returns>�ڍ����ʂ̐ڍ��F��Ԃ�</returns>
COLORREF CWeldEvaluationDoc::ResultGetJointColor(int method, int ViewJointRatioNo)
{
	COLORREF color = RGB(0, 0, 0);
	if (!m_ActiveRegisttedTestName.IsEmpty()) {
		color = m_PropatyIO.ResultGetJointColor(method, ViewJointRatioNo);
	}
	return color;
}

/// <summary>
/// �ڍ����ʂ̐ڍ��F�̐ݒ�
/// </summary>
/// <param name="method">��͕��@</param>
/// <param name="ViewJointRatioNo">�ڍ��ʔԍ�</param>
/// <param name="color">�ݒ�F</param>
/// <returns>�����ꍇ��true�A���s�ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::ResultSetJointColor(int method, int ViewJointRatioNo, COLORREF color)
{
	if (m_ActiveRegisttedTestName.IsEmpty()) {
		CString msg;
		msg.Format(_T("�ڍ����ʂ̐ڍ��F�̐ݒ�Ɏ��s���܂����B(method=%d, no=%d, color=%ld)"), method, ViewJointRatioNo, color);
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}
	else {
		return m_PropatyIO.ResultSetJointColor(method, ViewJointRatioNo, color);
	}
}

/// <summary>
/// �\�����[�h�̎擾
/// </summary>
/// <param name="targetID">����f��ID</param>
/// <returns>�\�����[�h��Ԃ�</returns>
int CWeldEvaluationDoc::GetDisplayMode(int targetID)
{
	int DisplayMode = DisplayModeScan;
	switch(targetID) {
	case	eResinSurface	:	///< ����
		DisplayMode = m_ProjectIO.GetResinDisplayType();
		break;
	case	eMetalSurface	:	///< ����
		DisplayMode = m_ProjectIO.GetMetalDisplayType();
		break;
	case	eJoiningResult	:	///< �ڍ�����
		DisplayMode = m_ProjectIO.GetResultDisplayType();
		break;
	}
	if (DisplayMode == 0) {
		DisplayMode = DisplayModeScan;
	}
	return DisplayMode;
}

/// <summary>
/// �\�����[�h�̐ݒ�
/// </summary>
/// <param name="targetID">����f��ID</param>
/// <param name="DisplayModeID">�\�����[�h</param>
/// <returns>�����̏ꍇ��true�A���s�̏ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::SetDisplayMode(int targetID, int DisplayModeID)
{
	bool bResult = false;
	switch(targetID) {
	case	eResinSurface	:	///< ����
		bResult = m_ProjectIO.SetResinDisplayType(DisplayModeID);
		break;
	case	eMetalSurface	:	///< ����
		bResult = m_ProjectIO.SetMetalDisplayType(DisplayModeID);
		break;
	case	eJoiningResult	:	///< �ڍ�����
		bResult = m_ProjectIO.SetResultDisplayType(DisplayModeID);
		break;
	}
	return bResult;
}


/// <summary>
/// ��͕��@�̎擾
/// </summary>
/// <param name="targetID">����f��ID</param>
/// <returns>��̓��[�h��Ԃ�</returns>
int CWeldEvaluationDoc::GetAnalysisMethod(int targetID)
{
	int AnalizeMode = AnalizeTypeKMeans;
	switch(targetID) {
	case	eResinSurface	:	// ����
		AnalizeMode = m_PropatyIO.ResinGetAnalysisMethod();
		break;
	case	eMetalSurface	:	// ����
		AnalizeMode = m_PropatyIO.MetalGetAnalysisMethod();
		break;
	case	eJoiningResult:		// �ڍ�����
		AnalizeMode = m_PropatyIO.ResultGetAnalysisMethod();
		break;
	}
	return AnalizeMode;
}

/// <summary>
/// ��͕��@�̐ݒ�
/// </summary>
/// <param name="targetID">����f��ID</param>
/// <param name="AnalysisMethodID">��͕��@ID</param>
/// <returns>�����̏ꍇ��true�A���s�̏ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::SetAnalysisMethod(int targetID, int AnalysisMethodID)
{
	bool bResult = false;
	switch(targetID) {
	case	eResinSurface	:	// ����
		bResult = m_PropatyIO.ResinSetAnalysisMethod(AnalysisMethodID);
		break;
	case	eMetalSurface	:	// ����
		bResult = m_PropatyIO.MetalSetAnalysisMethod(AnalysisMethodID);
		break;
	case	eJoiningResult:		// �ڍ�����
		bResult = m_PropatyIO.ResultSetAnalysisMethod(AnalysisMethodID);
		break;
	}
	return bResult;
}


//////////////////////////////////////////////////////////////////////////////////
// �v���W�F�N�g�f�[�^I/O

#if 0
/// <summary>
/// �����ʃX�L�����摜�t�@�C���p�X�̎擾
/// </summary>
/// <returns>�����ʃX�L�����摜�t�@�C���p�X��Ԃ�</returns>
CString CWeldEvaluationDoc::GetResinScanImagePath()
{
	CString root = m_ProjectIO.GetImageDataRootPath();
	CString fname = m_ProjectIO.GetResinScanImageFile();
	fname = fname + _T(".hdr");
	if (root.IsEmpty() || fname.IsEmpty()) {
		return _T("");
	}
	CString path;
	path = CFileUtil::FilePathCombine(root,fname);
	if (!CFileUtil::fileExists(path)) {
		return _T("");
	}
	return path;
}

/// <summary>
/// �����ʕ��ތ��ʃt�@�C���p�X�̎擾
/// </summary>
/// <returns>�����ʕ��ތ��ʃt�@�C���p�X��Ԃ�</returns>
CString CWeldEvaluationDoc::GetResinClassificationResultPath()
{
	CString root = m_ProjectIO.GetImageDataRootPath();
	CString fname = m_ProjectIO.GetResinClassificationResultFile();
	if (root.IsEmpty() || fname.IsEmpty()) {
		return _T("");
	}
	CString path;
	path = CFileUtil::FilePathCombine(root,fname);
	if (!CFileUtil::fileExists(path)) {
		return _T("");
	}
	return path;
}

/// <summary>
/// �����ʃX�L�����摜�t�@�C���p�X�̎擾
/// </summary>
/// <returns>�����ʃX�L�����摜�t�@�C���p�X��Ԃ�</returns>
CString CWeldEvaluationDoc::GetMetalScanImagePath()
{
	CString root = m_ProjectIO.GetImageDataRootPath();
	CString fname = m_ProjectIO.GetMetalScanImageFile();
	fname = fname + _T(".hdr");
	if (root.IsEmpty() || fname.IsEmpty()) {
		return _T("");
	}
	CString path;
	path = CFileUtil::FilePathCombine(root,fname);
	if (!CFileUtil::fileExists(path)) {
		return _T("");
	}
	return path;
}

/// <summary>
/// �����ʕ��ތ��ʃt�@�C���p�X�̎擾
/// </summary>
/// <returns>�����ʕ��ތ��ʃt�@�C���p�X��Ԃ�</returns>
CString CWeldEvaluationDoc::GetMetalClassificationResultPath()
{
	CString root = m_ProjectIO.GetImageDataRootPath();
	CString fname = m_ProjectIO.GetMetalResultFile();
	if (root.IsEmpty() || fname.IsEmpty()) {
		return _T("");
	}
	CString path;
	path = CFileUtil::FilePathCombine(root,fname);
	if (!CFileUtil::fileExists(path)) {
		return _T("");
	}
	return path;
}

/// <summary>
/// ���ʉ摜�X�L�����摜�t�@�C���p�X�̎擾
/// </summary>
/// <returns>���ʉ摜�X�L�����摜�t�@�C���p�X��Ԃ�</returns>
CString CWeldEvaluationDoc::GetClassScanImagePath()
{
	CString root = m_ProjectIO.GetImageDataRootPath();
	CString fname = m_ProjectIO.GetResultScanImageFile();
	if (root.IsEmpty() || fname.IsEmpty()) {
		return _T("");
	}
	CString path;
	path = CFileUtil::FilePathCombine(root,fname);
	if (!CFileUtil::fileExists(path)) {
		return _T("");
	}
	return path;
}
#endif

/// <summary>
/// �v���W�F�N�g����̃X�L�����C���[�W�t�@�C���p�X�擾
/// </summary>
/// <param name="fileID">�t�@�C�����ID</param>
/// <returns>���݂���ꍇ�̓p�X��Ԃ��A���s�����ꍇ�͋󕶎���Ԃ�</returns>
CString CWeldEvaluationDoc::GetScanImagePath(int fileID)
{
	return getScanImageFilePath(fileID);
}

/// <summary>
/// �v���W�F�N�g�ւ̕��ތ��ʃf�[�^�t�@�C���p�X�擾
/// </summary>
/// <param name="fileID">�t�@�C�����ID</param>
/// <param name="method">��͕��@</param>
/// <returns>���݂���ꍇ�̓p�X��Ԃ��A���s�����ꍇ�͋󕶎���Ԃ�</returns>
CString CWeldEvaluationDoc::GetClassificationResultPath(int fileID, int method)
{
	return getClassificationDataFilePath(fileID, method);
}

/// <summary>
/// �v���W�F�N�g����̃X�y�N�g���O���t�i�_�w��j�̑Ώۂ̎擾
/// </summary>
/// <returns>�X�y�N�g���O���t�i�_�w��j�̑Ώۂ�Ԃ�</returns>
UINT CWeldEvaluationDoc::GetSpectralGraphPointTarget()
{
	return m_ProjectIO.GetSpectralGraphPointTarget();
}

/// <summary>
/// �v���W�F�N�g�ւ̃X�y�N�g���O���t�i�_�w��j�̑Ώۂ̐ݒ�
/// </summary>
/// <param name="targetID">�X�y�N�g���O���t�i�_�w��j�̑Ώ�</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::SetSpectralGraphPointTarget(UINT targetID)
{
	return m_ProjectIO.SetSpectralGraphPointTarget(targetID);
}

/// <summary>
/// �v���W�F�N�g����̃X�y�N�g���O���t�i�_�w��j�̈ʒu�̎擾
/// </summary>
/// <param name="holizontal">�����l</param>
/// <param name="vertical">�����l</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::GetSpectralGraphPointPosition(int &holizontal, int &vertical)
{
	return m_ProjectIO.GetSpectralGraphPointPosition(holizontal, vertical);
}

/// <summary>
/// �v���W�F�N�g�ւ̃X�y�N�g���O���t�i�_�w��j�̈ʒu�̐ݒ�
/// </summary>
/// <param name="holizontal">�����l</param>
/// <param name="vertical">�����l</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::SetSpectralGraphPointPosition(int holizontal, int vertical)
{
	return m_ProjectIO.SetSpectralGraphPointPosition(holizontal, vertical);
}


/// <summary>
/// �v���W�F�N�g����̃X�y�N�g���O���t�i��Ԏw��j�̑Ώۂ̎擾
/// </summary>
/// <returns>�X�y�N�g���O���t�i��Ԏw��j�̑Ώۂ�Ԃ�</returns>
UINT CWeldEvaluationDoc::GetSpectralGraphSectionTarget()
{
	return m_ProjectIO.GetSpectralGraphSectionTarget();
}

/// <summary>
/// �v���W�F�N�g�ւ̃X�y�N�g���O���t�i��Ԏw��j�̑Ώۂ̐ݒ�
/// </summary>
/// <param name="targetID">�X�y�N�g���O���t�i��Ԏw��j�̑Ώ�</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::SetSpectralGraphSectionTarget(UINT targetID)
{
	return m_ProjectIO.SetSpectralGraphSectionTarget(targetID);
}

/// <summary>
/// �v���W�F�N�g����̃X�y�N�g���O���t�i��Ԏw��j�̊J�n�ʒu�̎擾
/// </summary>
/// <param name="holizontal">�����l</param>
/// <param name="vertical">�����l</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::GetSpectralGraphSectionStartPosition(int &holizontal, int &vertical)
{
	return m_ProjectIO.GetSpectralGraphSectionStartPosition(holizontal, vertical);
}

/// <summary>
/// �v���W�F�N�g�ւ̃X�y�N�g���O���t�i��Ԏw��j�̊J�n�ʒu�̐ݒ�
/// </summary>
/// <param name="holizontal">�����l</param>
/// <param name="vertical">�����l</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::SetSpectralGraphSectionStartPosition(int holizontal, int vertical)
{
	return m_ProjectIO.SetSpectralGraphSectionStartPosition(holizontal, vertical);
}

/// <summary>
/// �v���W�F�N�g����̃X�y�N�g���O���t�i��Ԏw��j�̏I���ʒu�̎擾
/// </summary>
/// <param name="holizontal">�����l</param>
/// <param name="vertical">�����l</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::GetSpectralGraphSectionEndPosition(int &holizontal, int &vertical)
{
	return m_ProjectIO.GetSpectralGraphSectionEndPosition(holizontal, vertical);
}

/// <summary>
/// �v���W�F�N�g�ւ̃X�y�N�g���O���t�i��Ԏw��j�̏I���ʒu�̐ݒ�
/// </summary>
/// <param name="holizontal">�����l</param>
/// <param name="vertical">�����l</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::SetSpectralGraphSectionEndPosition(int holizontal, int vertical)
{
	return m_ProjectIO.SetSpectralGraphSectionEndPosition(holizontal, vertical);
}

/// <summary>
/// �v���W�F�N�g����X�L�����f�[�^�̏c���T�C�Y�̎擾
/// </summary>
/// <param name="holizontal">�����l</param>
/// <param name="vertical">�����l</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::GetScanDataSize(int &holizontal, int &vertical)
{
	return m_ProjectIO.GetScanDataSize(holizontal, vertical);
}

/// <summary>
/// �v���W�F�N�g�ւ̃X�L�����f�[�^�̏c���T�C�Y�̐ݒ�
/// </summary>
/// <param name="holizontal">�����l</param>
/// <param name="vertical">�����l</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::SetScanDataSize(int holizontal, int vertical)
{
	return m_ProjectIO.SetScanDataSize(holizontal, vertical);
}

//////////////////////////////////////////////////////////////////////////////////
// 

//############################################################################
//## Private function

/// <summary>
/// ���C���X�v���b�^
/// </summary>
/// <param name="input">������</param>
/// <param name="delimiter">�f���~�^</param>
/// <param name="size">�ő�f�[�^��</param>
/// <returns>�f���~�^�ŕ��������f�[�^</returns>
std::vector<int> CWeldEvaluationDoc::split(std::string& input, char delimiter, int size/*= 0*/)
{
	std::istringstream stream(input);
	std::string field;
	std::vector<int> result;
	result.resize(size);
	int i = 0;
	while (getline(stream, field, delimiter)) {
		result[i++] = stoi(field);
	}
	return result;
}

bool CWeldEvaluationDoc::split(std::string& input, char delimiter, int &dataID, std::vector<int> &data)
{
	std::istringstream stream(input);
	std::string field;
	while (getline(stream, field, delimiter)) {
		data[dataID++] = stoi(field);
	}
	return true;
}

CString CWeldEvaluationDoc::SetRegistedTestFolder(void)
{
	CString folder = GetRegistedFolder();
	m_ActiveRegisttedTestFolder = CFileUtil::FilePathCombine(folder,m_ActiveRegisttedTestName);

	CString ParmaterFileName;
	if (!ParmaterFileName.LoadString(IDS_PROPATYFILENAME)) {
		ParmaterFileName = _T("parameter.inf");
	}
	CString projectFileName;
	if (!projectFileName.LoadString(IDS_PROJECTFILENAME)) {
		projectFileName = _T("WeldEvalution.prj");
	}
	m_ParamaterFilePaht = CFileUtil::FilePathCombine(m_ActiveRegisttedTestFolder,ParmaterFileName);
	m_ProjectFilePaht = CFileUtil::FilePathCombine(m_ActiveRegisttedTestFolder, projectFileName);

	return m_ActiveRegisttedTestFolder;
}

/// <summary>
/// �X�L�����t�@�C���̐ݒ�L������
/// </summary>
/// <param name="fileID">�t�@�C�����ID</param>
/// <returns>���݂���ꍇ��true�A���s�̏ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::ExistScanFile(int fileID)
{
	bool bResult = false;
	CString name;

	switch (fileID) {
	case	eResinSurface:	///< ����
		name = m_ProjectIO.GetResinScanImageFile();
		break;
	case	eMetalSurface:	///< ����
		name = m_ProjectIO.GetMetalScanImageFile();
		break;
	case	eJoiningResult:	///< �ڍ�����
		name = m_ProjectIO.GetResultScanImageFile();
		break;
	default:
		return false;
	}
	if (name.IsEmpty()) {
		bResult = false;
	}
	else {
		name = name + _T(".hdr");
		CString root = GetWorkProjectPath();
//		CString root = m_ProjectIO.GetImageDataRootPath();
		CString path = CFileUtil::FilePathCombine(root, name);
		bool bExist = CFileUtil::fileExists(path);

		bResult = bExist;
	}
	return bResult;
}

/// <summary>
/// �X�L�����t�@�C���p�X�擾�i�g���q�Ȃ��j
/// </summary>
/// <param name="ScanID">�ΏۃX�L����ID</param>
/// <returns>���݂���ꍇ�̓p�X��Ԃ��A���s�����ꍇ�͋󕶎���Ԃ�</returns>
CString CWeldEvaluationDoc::getScanDataPath(int ScanID)
{
	CString root, name;

	root = GetWorkProjectPath();
//	root = m_ProjectIO.GetImageDataRootPath();
	switch (ScanID) {
	case	eResinSurface:	///< ����
		name = m_ProjectIO.GetResinScanImageFile();
		break;
	case	eMetalSurface:	///< ����
		name = m_ProjectIO.GetMetalScanImageFile();
		break;
	case	eJoiningResult:	///< �ڍ�����
		name = m_ProjectIO.GetResultScanImageFile();
		break;
	default:
		return _T("");
	}
	if (root.IsEmpty() || name.IsEmpty()) {
		return _T("");
	}
	return CFileUtil::FilePathCombine(root, name);
}

/// <summary>
/// �X�L�����f�[�^�t�@�C���p�X�擾
/// </summary>
/// <param name="ScanID">�ΏۃX�L����ID</param>
/// <returns>���݂���ꍇ�̓p�X��Ԃ��A���s�����ꍇ�͋󕶎���Ԃ�</returns>
CString CWeldEvaluationDoc::getScanDataFilePath(int ScanID)
{
	CString pathName = getScanDataPath(ScanID);
	CString fPathName = pathName + _T(".hdr");
	return fPathName;
}

/// <summary>
/// �X�L�����C���[�W�t�@�C���p�X�擾
/// </summary>
/// <param name="ScanID">�t�@�C�����ID</param>
/// <returns>���݂���ꍇ�̓p�X��Ԃ��A���s�����ꍇ�͋󕶎���Ԃ�</returns>
CString CWeldEvaluationDoc::getScanImageFilePath(int ScanID)
{
	CString pathName = getScanDataPath(ScanID);
	CString fPathName = pathName + _T(".bmp");
	return fPathName;
}

/// <summary>
/// ���ތ��ʃf�[�^�t�@�C���p�X�擾
/// </summary>
/// <param name="ScanID">�ΏۃX�L����ID</param>
/// <param name="method">��͕��@</param>
/// <returns>���݂���ꍇ�̓p�X��Ԃ��A���s�����ꍇ�͋󕶎���Ԃ�</returns>
CString CWeldEvaluationDoc::getClassificationDataFilePath(int ScanID, int method)
{
	CString root, name;
	CString classfile, imagefile;

	root = GetWorkProjectPath();
//	root = m_ProjectIO.GetImageDataRootPath();
	switch (ScanID) {
	case	eResinSurface:	///< ����
		if (method == AnalizeTypeKMeans) {
			name = m_ProjectIO.GetResinKmeansClassFile();
		}
		else {
			name = m_ProjectIO.GetResinDendrogramClassFile();
		}
		break;
	case	eMetalSurface:	///< ����
		if (method == AnalizeTypeKMeans) {
			name = m_ProjectIO.GetMetalKmeansClassFile();
		}
		else {
			name = m_ProjectIO.GetMetalDendrogramClassFile();
		}
		break;
	case	eJoiningResult:	///< �ڍ�����
		if (method == AnalizeTypeKMeans) {
			name = m_ProjectIO.GetResultKmeansClassFile();
		}
		else {
			name = m_ProjectIO.GetResultDendrogramClassFile();
		}
		break;
	default:
		return false;
	}
	if (root.IsEmpty() || name.IsEmpty()) {
		return _T("");
	}

	classfile = CFileUtil::FilePathCombine(root, name);
	return classfile;
}

/// <summary>
/// ���ތ��ʃC���[�W�t�@�C���p�X�擾
/// </summary>
/// <param name="ScanID">�ΏۃX�L����ID</param>
/// <param name="method">��͕��@</param>
/// <returns>���݂���ꍇ�̓p�X��Ԃ��A���s�����ꍇ�͋󕶎���Ԃ�</returns>
CString CWeldEvaluationDoc::getClassificationImageFilePath(int ScanID, int method)
{
	CString classfile = getClassificationDataFilePath(ScanID, method);
	if (classfile.IsEmpty()) {
		return _T("");
	}
	CString imagePath = classfile + _T(".bmp");
	return imagePath;
}

/// <summary>
/// ���C���E�C���h�̈ʒu�ƃT�C�Y�̎擾
/// </summary>
/// <param name="rect">�E�C���h�̈ʒu�ƃT�C�Y</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::GetWindRect(CRect &rect)
{
	CConfigrationIO sys(m_SystemFilePathName);
	int top,left,width,hight;

	top = sys.getInt(_T("System"),_T("wind_top"));
	left = sys.getInt(_T("System"),_T("wind_left"));
	width = sys.getInt(_T("System"),_T("form_width"));
	hight = sys.getInt(_T("System"),_T("form_height"));
	rect.top = top;
	rect.left = left;
	rect.bottom = top + hight;
	rect.right = left + width;
	if ((width <= 0) || (hight <= 0) || (top < 0) || (left < 0)) {
		CString msg;
		msg.Format(_T("���C���E�C���h�̈ʒu�ƃT�C�Y�̎擾�Ɏ��s���܂����B(pos=(%d,%d) width=%d height=%d"), left, top, width, hight);
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	} else {
		return true;
	}
}

/// <summary>
/// ���C���E�C���h�̈ʒu�ƃT�C�Y�̐ݒ�
/// </summary>
/// <param name="rect">�E�C���h�̈ʒu�ƃT�C�Y</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::SetWindRect(CRect &rect)
{
	CConfigrationIO sys(m_SystemFilePathName);
	sys.setInt(_T("System"),_T("wind_top"),rect.top);
	sys.setInt(_T("System"),_T("wind_left"),rect.left);
	sys.setInt(_T("System"),_T("form_width"),rect.Width());
	sys.setInt(_T("System"),_T("form_height"),rect.Height());
	return true;
}

/// <summary>
/// ��ԃX�y�N�g���_�C�A���O�̈ʒu�ƃT�C�Y�̎擾
/// </summary>
/// <param name="rect">�E�C���h�̈ʒu�ƃT�C�Y</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::GetSpectralDlgRect(CRect &rect)
{
	CConfigrationIO sys(m_SystemFilePathName);
	int top, left, width, hight;

	top = sys.getInt(_T("System"), _T("SpectralDlg_top"));
	left = sys.getInt(_T("System"), _T("SpectralDlg_left"));
	width = sys.getInt(_T("System"), _T("SpectralDlg_width"));
	hight = sys.getInt(_T("System"), _T("SpectralDlg_height"));
	rect.top = top;
	rect.left = left;
	rect.bottom = top + hight;
	rect.right = left + width;
	if ((width <= 0) || (hight <= 0) || (top < 0) || (left < 0)) {
		CString msg;
		msg.Format(_T("��ԃX�y�N�g���_�C�A���O�̈ʒu�ƃT�C�Y�̎擾�Ɏ��s���܂����B(pos=(%d,%d) width=%d height=%d"), left, top, width, hight);
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}
	else {
		return true;
	}
}

/// <summary>
/// ��ԃX�y�N�g���_�C�A���O�̈ʒu�ƃT�C�Y�̎擾
/// </summary>
/// <param name="rect">�E�C���h�̈ʒu�ƃT�C�Y</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::SetSpectralDlgRect(CRect &rect)
{
	CConfigrationIO sys(m_SystemFilePathName);
	sys.setInt(_T("System"), _T("SpectralDlg_top"), rect.top);
	sys.setInt(_T("System"), _T("SpectralDlg_left"), rect.left);
	sys.setInt(_T("System"), _T("SpectralDlg_width"), rect.Width());
	sys.setInt(_T("System"), _T("SpectralDlg_height"), rect.Height());
	return true;
}

/// <summary>
/// RGB�ɑΉ�����g���̎擾
/// </summary>
/// <param name="r">�Ԃ̔g��</param>
/// <param name="g">�΂̔g��</param>
/// <param name="b">�̔g��</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::GetRGBWavelength(double &r, double &g, double &b)
{
	if (!CFileUtil::fileExists(m_SystemFilePathName)) {
		CString msg;
		msg.Format(_T("RGB�ɑΉ�����g���̎擾�Ɏ��s���܂���"));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}
	CConfigrationIO sys(m_SystemFilePathName);
	r = sys.getDouble(_T("System"), _T("RedWavelength"));
	g = sys.getDouble(_T("System"), _T("GreenWavelength"));
	b = sys.getDouble(_T("System"), _T("BlueWavelength"));
	return true;
}

/// <summary>
/// �ˉe�ϊ��p���_�̎擾
/// </summary>
/// <param name="vOrignPt">�ˉe���̒��_</param>
/// <param name="vTransPt">�ˉe��̒��_</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
/// @remark ���_�͍���A�������A�E���A�E��Ŋi�[
bool CWeldEvaluationDoc::GetHomographyPoint(vector<CPoint> &vOrignPt, vector<CPoint> &vTransPt)
{
	if (!CFileUtil::fileExists(m_SystemFilePathName)) {
		CString msg;
		msg.Format(_T("�ˉe�ϊ��p���_�̎擾�Ɏ��s���܂���"));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}
	vOrignPt.resize(4);
	vTransPt.resize(4);
	CConfigrationIO sys(m_SystemFilePathName);
	vOrignPt[0].x = sys.getInt(_T("System"), _T("HomographyOrignTLPtX"));
	vOrignPt[0].y = sys.getInt(_T("System"), _T("HomographyOrignTLPtY"));
	vOrignPt[1].x = sys.getInt(_T("System"), _T("HomographyOrignBLPtX"));
	vOrignPt[1].y = sys.getInt(_T("System"), _T("HomographyOrignBLPtY"));
	vOrignPt[2].x = sys.getInt(_T("System"), _T("HomographyOrignBRPtX"));
	vOrignPt[2].y = sys.getInt(_T("System"), _T("HomographyOrignBRPtY"));
	vOrignPt[3].x = sys.getInt(_T("System"), _T("HomographyOrignTRPtX"));
	vOrignPt[3].y = sys.getInt(_T("System"), _T("HomographyOrignTRPtY"));

	vTransPt[0].x = sys.getInt(_T("System"), _T("HomographyTransTLPtX"));
	vTransPt[0].y = sys.getInt(_T("System"), _T("HomographyTransTLPtY"));
	vTransPt[1].x = sys.getInt(_T("System"), _T("HomographyTransBLPtX"));
	vTransPt[1].y = sys.getInt(_T("System"), _T("HomographyTransBLPtY"));
	vTransPt[2].x = sys.getInt(_T("System"), _T("HomographyTransBRPtX"));
	vTransPt[2].y = sys.getInt(_T("System"), _T("HomographyTransBRPtY"));
	vTransPt[3].x = sys.getInt(_T("System"), _T("HomographyTransTRPtX"));
	vTransPt[3].y = sys.getInt(_T("System"), _T("HomographyTransTRPtY"));

	return true;
}

/// <summary>
/// �f�o�C�X���̎擾
/// </summary>
/// <returns>�f�o�C�X����Ԃ�</returns>
CString CWeldEvaluationDoc::GetDeviceName()
{
	CConfigrationIO sys(m_SystemFilePathName);
	CString name;
	name = sys.getString(_T("System"), _T("DeviceName"), _T(""));
	return name;
}

/// <summary>
/// �J�����_�~�[���W���[���g�p�L������
/// </summary>
/// <returns>�_�~�[���W���[�����g�p����ꍇ��true�A�����łȂ��ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::IsCameraDummyApi()
{
	CConfigrationIO sys(m_SystemFilePathName);
	int use = sys.getInt(_T("System"), _T("UseCameraDummyApi"));
	if (use == 0) {
		return false;
	}
	else {
		return true;
	}
}

/// <summary>
/// �B�e���̎擾
/// </summary>
/// <returns>�B�e����Ԃ�</returns>
int CWeldEvaluationDoc::GetShootingWidth()
{
	CConfigrationIO sys(m_SystemFilePathName);
	int width = sys.getInt(_T("System"), _T("ShootingWidth"));
	if (width == 0) {
		CString msg;
		msg.Format(_T("�B�e���̎擾�̎擾�Ɏ��s���܂����B�f�t�H���g�̎B�e��[%d]���g�p���܂��B"),width);
		writeLog(CLog::Warning, CString(__FILE__), __LINE__, msg);
		return 2048;
	}
	else {
		return width;
	}
}

/// <summary>
/// �B�e�����̎擾
/// </summary>
/// <returns>�B�e������Ԃ�</returns>
int CWeldEvaluationDoc::GetShootingHeight()
{
	CConfigrationIO sys(m_SystemFilePathName);
	int height = sys.getInt(_T("System"), _T("ShootingHeight"));
	if (height == 0) {
		CString msg;
		msg.Format(_T("�B�e�����̎擾�̎擾�Ɏ��s���܂����B�f�t�H���g�̎B�e����[%d]���g�p���܂��B"), height);
		writeLog(CLog::Warning, CString(__FILE__), __LINE__, msg);
		return 1088;
	}
	else {
		return height;
	}
}

/// <summary>
/// �o���h���̎擾
/// </summary>
/// <returns>�o���h����Ԃ�</returns>
UINT CWeldEvaluationDoc::NumberOfBand(void)
{
	if (!CFileUtil::fileExists(m_SystemFilePathName)) {
		CString msg;
		msg.Format(_T("�o���h���̎擾�Ɏ��s���܂����B"));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return 0;
	}
	CConfigrationIO sys(m_SystemFilePathName);
	return sys.getInt(_T("System"), _T("Number_of_band"));
}

/// <summary>
/// �o���h���̐ݒ�
/// </summary>
/// <param name="band">�o���h��</param>
/// <returns>�����ꍇ��true�A���s�ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::SetNumberOfBand(UINT band)
{
	if (!CFileUtil::fileExists(m_SystemFilePathName)) {
		CString msg;
		msg.Format(_T("�o���h���̐ݒ�Ɏ��s���܂����B(�o���h��=%d)"),band);
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}
	CConfigrationIO sys(m_SystemFilePathName);
	if (!sys.setInt(_T("Setting"), _T("Number_of_band"), band)) {
		return false;
	}
	return true;
}

/// <summary>
/// Integration_time_ms�̎擾
/// </summary>
/// <returns>Integration_time_ms��Ԃ�</returns>
double CWeldEvaluationDoc::GetIntegrationTimeMs(void)
{
	if (!CFileUtil::fileExists(m_SystemFilePathName)) {
		CString msg;
		msg.Format(_T("Integration_time_ms�̎擾�Ɏ��s���܂����B"));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return 0;
	}
	CConfigrationIO sys(m_SystemFilePathName);
	return sys.getDouble(_T("System"), _T("Integration_time_ms"));
}

/// <summary>
/// Integration_time_ms�̐ݒ�
/// </summary>
/// <param name="IntegrationTime_ms">Integration_time_ms</param>
/// <returns>�����ꍇ��true�A���s�ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::SetIntegrationTimeMs(double IntegrationTime_ms)
{
	if (!CFileUtil::fileExists(m_SystemFilePathName)) {
		CString msg;
		msg.Format(_T("Integration_time_ms�̐ݒ�Ɏ��s���܂����B(IntegrationTime_ms=%lf)"), IntegrationTime_ms);
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}
	CConfigrationIO sys(m_SystemFilePathName);
	if (!sys.setDouble(_T("System"), _T("Integration_time_ms"), IntegrationTime_ms)) {
		CString msg;
		msg.Format(_T("Integration_time_ms�̐ݒ�Ɏ��s���܂����B(IntegrationTime_ms=%lf)"), IntegrationTime_ms);
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}
	return true;
}

/// <summary>
/// �擾�f�[�^�̏c�����g�k���̎擾
/// </summary>
/// <returns>�����ꍇ�͊g�k���A���s�ꍇ��0.0��Ԃ�</returns>
double CWeldEvaluationDoc::GetVerticalScale()
{
	if (!CFileUtil::fileExists(m_SystemFilePathName)) {
		CString msg;
		msg.Format(_T("�擾�f�[�^�̏c�����g�k���̎擾�Ɏ��s���܂����B"));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return 0.0;
	}
	int ShootingHeight = GetShootingHeight();
	int VerticalResolution = GetVerticalResolution();
	double retio;
	if ((ShootingHeight == 0) || (VerticalResolution == 0)) {
		CString msg;
		msg.Format(_T("�擾�f�[�^�̏c�����g�k���̎擾�Ɏ��s���܂����B(ShootingHeight=%d, VerticalResolution=%d)"), ShootingHeight, VerticalResolution);
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		retio = 0.0;
	}
	else {
		retio = (double)VerticalResolution / (double)ShootingHeight;
	}
	return retio;
}

/// <summary>
/// �擾�f�[�^�̉������g�k���̎擾
/// </summary>
/// <returns>�����ꍇ�͊g�k���A���s�ꍇ��0.0��Ԃ�</returns>
double CWeldEvaluationDoc::GetHorizontalScale()
{
	if (!CFileUtil::fileExists(m_SystemFilePathName)) {
		CString msg;
		msg.Format(_T("�擾�f�[�^�̉������g�k���̎擾�Ɏ��s���܂����B"));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return 0.0;
	}
	int ShootingWidth = GetShootingWidth();
	int HorizontalResolution = GetHorizontalResolution();
	double retio;
	if ((ShootingWidth == 0) || (HorizontalResolution == 0)) {
		CString msg;
		msg.Format(_T("�擾�f�[�^�̉������g�k���̎擾�Ɏ��s���܂����B(ShootingWidth=%d, HorizontalResolution=%d)"), ShootingWidth, HorizontalResolution);
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		retio = 0.0;
	}
	else {
		retio = (double)HorizontalResolution / (double)ShootingWidth;
	}
	return retio;
}

/// <summary>
/// �E�C���h�̍ŏ��T�C�Y�̎擾
/// </summary>
/// <param name="size">�ŏ��T�C�Y</param>
/// <returns>�ŏ��T�C�Y���m�肵�Ă���ꍇ��true�A����̏ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::GetMinWndSize(CSize &size)
{
	if (!m_bInitialized) {
		size = CSize(0, 0);
		return false;
	}
	else {
		size = m_minRect;
		return true;
	}
}

/// <summary>
/// �E�C���h�̍ŏ��T�C�Y�̐ݒ�
/// </summary>
/// <param name="size">�ŏ��T�C�Y</param>
/// <returns>���������ꍇ�͂�true�A���s�����ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::SetMinWndSize(CSize size)
{
	m_bInitialized = true;
	m_minRect = size;
	return true;
}


/// <summary>
/// �v���W�F�N�g�̃I�[�v������
/// </summary>
/// <returns>�I�[�v�����Ă���ꍇ��true�A�����łȂ��ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::IsOpen(void)
{
	return m_OpenType!=0?true:false;
}

/// <summary>
/// �v���W�F�N�g�̐V�K����
/// </summary>
/// <returns>�V�K�̏ꍇ��true�A�����łȂ��ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::IsNew(void)
{
	return m_OpenType==2?true:false;
}

/// <summary>
/// �V�K�v���W�F�N�g�̍\�z
/// </summary>
/// <returns>���������ꍇ��true�A�����łȂ��ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::NewProject()
{
#if 0
	////////////////////////////////////////////////////////////////
	// �����Ă�
	// �ˉe�ϊ��̊m�F�p�R�[�f�B���O
	////////////////////////////////////////////////////////////////
	{
		AtlTrace(_T("##### �ˉe�ϊ��̊m�F #####\n"));
		double aParam[8], ppt[2];
		CScanDataIO sc;
		vector<CPoint> vOrign,vTrans;
		GetHomographyPoint(vOrign, vTrans);

		double tolerance = 0.000001;
		bool bProjResult = true;
		sc.Calc_ProjectionParam(vOrign, vTrans, aParam);
		for (int i = 0; i < 4; i++) {
			sc.Projection(vOrign[i].x, vOrign[i].y, aParam, ppt[0], ppt[1]);
			if ((fabs(ppt[0] - (double)vTrans[i].x) > tolerance) || (fabs(ppt[1] - (double)vTrans[i].y) > tolerance) ) {
				AtlTrace(_T("FAIL:Orign->Trans:(%.2lf,%2.lf) -> (%.2lf,%2.lf):(%.2lf,%2.lf)\n"), vOrign[i].x, vOrign[i].y, vTrans[i].x, vTrans[i].y, ppt[0], ppt[1]);
				bProjResult = false;
			}
			sc.ProjectionInvPos(vTrans[i].x, vTrans[i].y, aParam, ppt[0], ppt[1]);
			if ((fabs(ppt[0] - (double)vOrign[i].x) > tolerance) || (fabs(ppt[1] - (double)vOrign[i].y) > tolerance)) {
				AtlTrace(_T("FAIL:Trans->Orign:(%.2lf,%2.lf) -> (%.2lf,%2.lf):(%.2lf,%2.lf)\n"), vTrans[i].x, vTrans[i].y, vOrign[i].x, vOrign[i].y, ppt[0], ppt[1]);
				bProjResult = false;
			}
		}
		if (bProjResult) {
			AtlTrace(_T("##### �ˉe�ϊ�:���� #####\n"));
		}
		else {
			AtlTrace(_T("##### �ˉe�ϊ�:���s #####\n"));
		}
	}
	////////////////////////////////////////////////////////////////
#endif

	ClrWorkProject();

	m_ResinScanData.close();
	m_MetalScanData.close();
	m_ResultScanData.close();

	CString tmpFolder = GetRegistedFolder();
	PushProject(tmpFolder,_T(""));
	CString WrokPrjectPath = GetWorkProjectPath();
	CString version;

	m_ActiveRegisttedTestName = tmpFolder;
	m_ActiveRegisttedTestFolder = WrokPrjectPath;
	if (!CFileUtil::fileExists(m_ActiveRegisttedTestFolder)) {
		if (!CFileUtil::MakeDir(m_ActiveRegisttedTestFolder)) {
			CString msg;
			msg.Format((LPCTSTR)_T("�f�B���N�g���̍쐬�Ɏ��s���܂����B(folder=%s)"), static_cast<LPCWSTR>(m_ActiveRegisttedTestFolder));
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
			return false;
		}
	}

	CString paramFileName;
	if (!paramFileName.LoadString(IDS_PROPATYFILENAME)) {
		paramFileName = _T("parameter.inf");
	}
	CString projectFileName;
	if (!projectFileName.LoadString(IDS_PROJECTFILENAME)) {
		projectFileName = _T("WeldEvalution.prj");
	}

	m_ParamaterFilePaht = CFileUtil::FilePathCombine(m_ActiveRegisttedTestFolder, paramFileName);
	m_ProjectFilePaht	= CFileUtil::FilePathCombine(m_ActiveRegisttedTestFolder, projectFileName);

	UINT uval;
	CString str;
	double dval;
	//////////////////////////////////////////////////////////
	// �p�����[�^�̐ݒ�
	m_PropatyIO.SetParamaterFilePath(m_ParamaterFilePaht);

	// Default
	CConfigrationIO sys(m_SystemFilePathName);
	str = sys.getString(_T("ParamDefault"),_T("Test_name"));
	CString prjName;
	{
		COleDateTime date = COleDateTime::GetCurrentTime();
		if (str.IsEmpty()) {
			str = _T("Test");
		}

		CString folder = GetRegistedFolder();
		CString ProjectFileName;
		int i = 1;
		while (true) {
			prjName = m_PropatyIO.MakeProjectName(str, date, i);
			CString PathName = CFileUtil::FilePathCombine(folder, prjName);
			if (!CFileUtil::fileExists(PathName)) {
				break;
			}
			i++;
		}
	}

	if (!version.LoadString(IDS_PARAMETERFILE_VERSION)) {
		version = _T("");
	}
	m_PropatyIO.SetVersion(version);
		
	m_PropatyIO.Save(false);
	m_PropatyIO.SetTestName(prjName);

#if 0
	m_PropatyIO.SetTestName(prjName);
	uval = sys.getInt(_T("ParamDefault"),_T("Number_of_overlapping_pixels"));
	m_PropatyIO.SetOverridePixelNumber(uval);
	uval = sys.getInt(_T("ParamDefault"),_T("Integration_time_ms"));
	m_PropatyIO.SetIntegrationTimeMs(uval);
	uval = sys.getInt(_T("ParamDefault"),_T("Vertical_resolution"));
	m_PropatyIO.SetVerticalResolution(uval);
	uval = sys.getInt(_T("ParamDefault"),_T("Horizontal_resolution"));
	m_PropatyIO.SetHorizontalResolution(uval);
#endif
	uval = sys.getInt(_T("ParamDefault"),_T("ResinSurface_Number_of_classifications"));

	for (int method = 0; method < 2; method++) {
		m_PropatyIO.ResinSetNumberOfClass(method,uval);
		for (int i = 0; i < (int)uval; i++) {
			dval = sys.getDouble(_T("ParamDefault"), _T("ResinSurface_Joining_ratio"));
			m_PropatyIO.ResinSetJointRetio(method, i, dval);
		}
	}
	uval = sys.getInt(_T("ParamDefault"),_T("ResinSurface_Analysis_method"));
	m_PropatyIO.ResinSetAnalysisMethod(uval);

	uval = sys.getInt(_T("ParamDefault"),_T("MetalSurface_Number_of_classifications"));
	for (int method = 0; method < 2; method++) {
		m_PropatyIO.MetalSetNumberOfClass(method,uval);
		for (int i = 0; i < (int)uval; i++) {
			dval = sys.getDouble(_T("ParamDefault"), _T("MetalSurface_Joining_ratio"));
			m_PropatyIO.MetalSetJointRetio(method, i, dval);
		}
	}
	uval = sys.getInt(_T("ParamDefault"),_T("MetalSurface_Analysis_method"));
	m_PropatyIO.MetalSetAnalysisMethod(uval);

	uval = sys.getInt(_T("ParamDefault"),_T("JoiningResult_Number_of_classifications"));
	for (int method = 0; method < 2; method++) {
		m_PropatyIO.ResultSetNumberOfClass(method,uval);
		for (int i = 0; i < (int)uval; i++) {
			dval = sys.getInt(_T("ParamDefault"), _T("JoiningResult_Joining_ratio"));
			m_PropatyIO.ResultSetJointRetio(method,i, dval);
		}
	}

	uval = NumberOfOverridePixel();
	m_PropatyIO.SetOverridePixelNumber(uval);
	uval = GetHorizontalResolution();
	m_PropatyIO.SetHorizontalResolution(uval);
	uval = GetVerticalResolution();
	m_PropatyIO.SetVerticalResolution(uval);

	//////////////////////////////////////////////////////////
	// �v���W�F�N�g�̐ݒ�
	m_ProjectIO.Initialze();
	m_ProjectIO.SetImageDataRootPath(WrokPrjectPath);
	m_ProjectIO.SetProjectFilePath(m_ProjectFilePaht);
	CString name;
	name = GetScanDataName(eResinSurface,_T(""));
	m_ProjectIO.SetResinScanImageFile(name);
	name = GetScanDataName(eMetalSurface, _T(""));
	m_ProjectIO.SetMetalScanImageFile(name);
	name = GetScanDataName(eJoiningResult, _T(""));
	m_ProjectIO.SetResultScanImageFile(name);

	SaveClassificationResultFile(eResinSurface, AnalizeTypeKMeans);			// k-means
	SaveClassificationResultFile(eResinSurface, AnalizeTypeHiClustering);	// �K�w�N���X�^�����O

	SaveClassificationResultFile(eMetalSurface, AnalizeTypeKMeans);			// k-means
	SaveClassificationResultFile(eMetalSurface, AnalizeTypeHiClustering);	// �K�w�N���X�^�����O

	SaveClassificationResultFile(eJoiningResult, AnalizeTypeKMeans);		// k-means
	SaveClassificationResultFile(eJoiningResult, AnalizeTypeHiClustering);	// �K�w�N���X�^�����O

	if (!version.LoadString(IDS_PROJECTFILE_VERSION)) {
		version = _T("");
	}
	m_ProjectIO.SetVersion(version);
	m_ProjectIO.Save(false);

	m_OpenType = 2;

	return true;
}

/// <summary>
/// �����v���W�F�N�g�̃I�[�v��
/// </summary>
/// <param name="RegistedTestName">������</param>
/// <returns>���������ꍇ��true�A�����łȂ��ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::OpenProject(CString RegistedTestName)
{
	CWaitCursor waitCursol;
	CString PopatyFileName,ProjecName, ProjectFileName;
	if (!PopatyFileName.LoadString(IDS_PROPATYFILENAME)) {
		PopatyFileName = _T("parameter.inf");
	}
	if (!ProjecName.LoadString(IDS_PROJECTFILENAME)) {
		ProjecName = _T("WeldEvalution.prj");
	}

	m_ProjectIO.Initialze();
	m_PropatyIO.Initialze();

	// �v���W�F�N�g�t�H���_�擾
	CString folder = GetRegistedFolder();
	CString RegisttedTestFolder = CFileUtil::FilePathCombine(folder,RegistedTestName);
	if (!CFileUtil::fileExists(RegisttedTestFolder)) {
		CString msg;
		msg.Format((LPCTSTR)_T("�v���W�F�N�g�t�H���_�擾�����݂��܂���B(folder=%s)"), static_cast<LPCWSTR>(RegisttedTestFolder));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}

	if (!PushProject(folder, RegistedTestName)) {
		return false;
	}
	CString WorkProjectPath = GetWorkProjectPath();

	// �p�����[�^�擾
	CString ParamaterFilePaht;
	ParamaterFilePaht = CFileUtil::FilePathCombine(WorkProjectPath,PopatyFileName);
	if (!CFileUtil::fileExists(ParamaterFilePaht)) {
		CString msg;
		msg.Format((LPCTSTR)_T("�p�����[�^�t�@�C�������݂��܂���B(path=%s)"), static_cast<LPCWSTR>(ParamaterFilePaht));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}


	if (!m_PropatyIO.SetParamaterFilePath(ParamaterFilePaht)) {
		m_PropatyIO.SetParamaterFilePath(m_ParamaterFilePaht);
		CString msg;
		msg.Format((LPCTSTR)_T("�p�����[�^�t�@�C���̐ݒ�Ɏ��s���܂����B�B(path=%s)"), static_cast<LPCWSTR>(ParamaterFilePaht));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}
	if (!m_PropatyIO.Read()) {
		CString msg;
		msg.Format((LPCTSTR)_T("�p�����[�^�̓ǂݍ��݂Ɏ��s���܂����B�B(path=%s)"), static_cast<LPCWSTR>(ParamaterFilePaht));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}

	// �v���p�e�B�擾
	CString str = m_PropatyIO.GetProjectName();
	if (!str.IsEmpty()) {
		ProjecName = str;
	}
	CString ProjectFilePaht;
//	ProjectFileName.Format(_T("%s.prj"), (LPCWSTR)ProjecName);
	ProjectFilePaht = CFileUtil::FilePathCombine(WorkProjectPath, ProjecName);
	if (!CFileUtil::fileExists(ProjectFilePaht)) {
		m_ProjectIO.SetProjectFilePath(m_ProjectFilePaht);
		CString msg;
		msg.Format((LPCTSTR)_T("�v���W�F�N�g�t�@�C�������݂��܂���B(path=%s)"), static_cast<LPCWSTR>(ProjectFilePaht));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}
	if (!m_ProjectIO.SetProjectFilePath(ProjectFilePaht)) {
		m_ProjectIO.SetProjectFilePath(m_ProjectFilePaht);
		CString msg;
		msg.Format((LPCTSTR)_T("�v���W�F�N�g�t�@�C���̐ݒ�Ɏ��s���܂����B(path=%s)"), static_cast<LPCWSTR>(ProjectFilePaht));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}
	if (!m_ProjectIO.Read()) {
		CString msg;
		msg.Format((LPCTSTR)_T("�v���W�F�N�g�̓ǂݍ��݂Ɏ��s���܂����B(path=%s)"), static_cast<LPCWSTR>(ProjectFilePaht));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}

	m_ActiveRegisttedTestName = RegistedTestName;
	m_ActiveRegisttedTestFolder = WorkProjectPath;
	m_ParamaterFilePaht = ParamaterFilePaht;
	m_ProjectFilePaht = ProjectFilePaht;

	m_ResinScanData.close();
	m_MetalScanData.close();
	m_ResultScanData.close();

	CString path;
	path = getScanDataFilePath(eResinSurface);
	if (CFileUtil::fileExists(path)) {
		m_ResinScanData.open(path);
	}
	path = getScanDataFilePath(eMetalSurface);
	if (CFileUtil::fileExists(path)) {
		m_MetalScanData.open(path);
	}
	path = getScanDataFilePath(eJoiningResult);
	if (CFileUtil::fileExists(path)) {
		m_ResultScanData.open(path);
	}

	ProjectUpdate();

	m_OpenType = 1;
	return true;
}

/// <summary>
/// �v���W�F�N�g�̍X�V
/// </summary>
/// <returns>�����ꍇ��true�A���s�ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::ProjectUpdate()
{
	CConfigrationIO sys(m_SystemFilePathName);
	CConfigrationIO prm(m_PropatyIO.GetParamaterFilePath());
	CConfigrationIO prj(m_ProjectIO.GetProjectFilePath());

	CString prjVersion, proVersion, fileVersion, key;
	int iVal;
	if (!proVersion.LoadString(IDS_PARAMETERFILE_VERSION)) {
		proVersion = _T("");
	}
	fileVersion = m_PropatyIO.GetVersion();
	if (proVersion.CollateNoCase(fileVersion) != 0) {
		CString AnalizeTypeKMeansKey, AnalizeTypeHiClusteringKey;
		AnalizeTypeKMeansKey.Format(_T("%02d_Number_of_classifications"), AnalizeTypeKMeans);
		AnalizeTypeHiClusteringKey.Format(_T("%02d_Number_of_classifications"), AnalizeTypeHiClustering);

		iVal = prm.getInt(_T("ResinSurface"), _T("Number_of_classifications"));
		if (iVal == 0) {
			iVal = sys.getInt(_T("ParamDefault"), _T("ResinSurface_Number_of_classifications"));
		}
		prm.setInt(_T("ResinSurface"), AnalizeTypeKMeansKey, iVal);
		prm.setInt(_T("ResinSurface"), AnalizeTypeHiClusteringKey, iVal);
		prm.deleteKey(_T("ResinSurface"), _T("Number_of_classifications"));

		iVal = prm.getInt(_T("MetalSurface"), _T("Number_of_classifications"));
		if (iVal == 0) {
			iVal = sys.getInt(_T("ParamDefault"), _T("MetalSurface_Number_of_classifications"));
		}
		prm.setInt(_T("MetalSurface"), AnalizeTypeKMeansKey, iVal);
		prm.setInt(_T("MetalSurface"), AnalizeTypeHiClusteringKey, iVal);
		prm.deleteKey(_T("MetalSurface"), _T("Number_of_classifications"));

		iVal = prm.getInt(_T("JoiningResult"), _T("Number_of_classifications"));
		if (iVal == 0) {
			iVal = sys.getInt(_T("ParamDefault"), _T("JoiningResult_Number_of_classifications"));
		}
		prm.setInt(_T("JoiningResult"), AnalizeTypeKMeansKey, iVal);
		prm.setInt(_T("JoiningResult"), AnalizeTypeHiClusteringKey, iVal);
		prm.deleteKey(_T("JoiningResult"), _T("Number_of_classifications"));
	}

	if (!prjVersion.LoadString(IDS_PROJECTFILE_VERSION)) {
		prjVersion = _T("");
	}
	fileVersion = m_ProjectIO.GetVersion();
	if (prjVersion.CollateNoCase(fileVersion) != 0) {
		CString AnalizeTypeKMeansKey, AnalizeTypeHiClusteringKey, sVal;
		AnalizeTypeKMeansKey.Format(_T("%02d_Number_of_classifications"), AnalizeTypeKMeans);
		AnalizeTypeHiClusteringKey.Format(_T("%02d_Number_of_classifications"), AnalizeTypeHiClustering);

		// ������
		sVal = sys.getString(_T("ParamDefault"), _T("ResinSurface_Analysis_method"));
		if (sVal.CollateNoCase(_T("k_means")) == 0) {
			prj.setInt(_T("ResinSurface"), _T("analize_method"), AnalizeTypeKMeans);
		}
		else {
			prj.setInt(_T("ResinSurface"), _T("analize_method"), AnalizeTypeHiClustering);
		}
		iVal = prm.getInt(_T("ResinSurface"), AnalizeTypeKMeansKey);
		prj.setInt(_T("ResinSurface"), _T("kmeans_classification_nclass"), iVal);
		iVal = prm.getInt(_T("ResinSurface"), AnalizeTypeHiClusteringKey);
		prj.setInt(_T("ResinSurface"), _T("dendrogram_classification_nclass"), iVal);

		// ������
		sVal = sys.getString(_T("ParamDefault"), _T("MetalSurface_Analysis_method"));
		if (sVal.CollateNoCase(_T("k_means")) == 0) {
			prj.setInt(_T("MetalSurface"), _T("analize_method"), AnalizeTypeKMeans);
		}
		else {
			prj.setInt(_T("MetalSurface"), _T("analize_method"), AnalizeTypeHiClustering);
		}
		iVal = prm.getInt(_T("MetalSurface"), AnalizeTypeKMeansKey);
		prj.setInt(_T("MetalSurface"), _T("kmeans_classification_nclass"), iVal);
		iVal = prm.getInt(_T("MetalSurface"), AnalizeTypeHiClusteringKey);
		prj.setInt(_T("MetalSurface"), _T("dendrogram_classification_nclass"), iVal);

		// ��������
		sVal = sys.getString(_T("ParamDefault"), _T("MetalSurface_Analysis_method"));
		if (sVal.CollateNoCase(_T("k_means")) == 0) {
			prj.setInt(_T("MetalSurface"), _T("analize_method"), AnalizeTypeKMeans);
		}
		else {
			prj.setInt(_T("MetalSurface"), _T("analize_method"), AnalizeTypeHiClustering);
		}
		iVal = prm.getInt(_T("MetalSurface"), AnalizeTypeKMeansKey);
		prj.setInt(_T("MetalSurface"), _T("kmeans_classification_nclass"), iVal);
		iVal = prm.getInt(_T("MetalSurface"), AnalizeTypeHiClusteringKey);
		prj.setInt(_T("MetalSurface"), _T("dendrogram_classification_nclass"), iVal);
	}
	m_PropatyIO.SetVersion(proVersion);
	m_ProjectIO.SetVersion(prjVersion);

	return true;
}

/// <summary>
/// �v���W�F�N�g�̏I��
/// </summary>
/// <returns>�����ꍇ��true�A���s�ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::CloseProject(bool bSave)
{
	if (bSave) {
		if (!SaveProject()) {
			CString msg;
			msg.Format(_T("�v���W�F�N�g�̕ۑ��Ɏ��s���܂����B"));
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
			return false;
		}
	}

	m_ActiveRegisttedTestName = _T("");
	m_ActiveRegisttedTestFolder = _T("");
	m_ParamaterFilePaht = _T("");
	m_ProjectFilePaht = _T("");

	m_OpenType = 0;
	return true;
}

/// <summary>
/// �v���W�F�N�g�̕ۑ�
/// </summary>
/// <returns>�����ꍇ��true�A���s�ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::SaveProject()
{
	CString TestName = m_PropatyIO.GetTestName();
	if (TestName.IsEmpty()) {
		CString msg;
		msg.Format(_T("�e�X�g�����ݒ肳��Ă��܂���B"));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}

	CString tmpParamFilePath = _T("");
	if (m_OpenType != 2) {
		if (m_ActiveRegisttedTestFolder.IsEmpty() || !CFileUtil::fileExists(m_ActiveRegisttedTestFolder)) {
			CString msg;
			msg.Format(_T("�o�^�t�H���_�����݂��܂���B(%s)"), static_cast<LPCWSTR>(m_ActiveRegisttedTestFolder));
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
			return false;
		}
	} else {
		tmpParamFilePath = m_ParamaterFilePaht;
	}

	COleDateTime date = COleDateTime::GetCurrentTime();

	CString rootPath = GetWorkProjectFolderPath();
	CString NewProject = GetWorkProjectStatus(rootPath, _T("NewProject"));
	if (NewProject.CollateNoCase(_T("TRUE")) == 0) {
		m_ProjectIO.SetCreateDay(date);
	}
	m_ActiveRegisttedTestName = TestName;

	CString projectFileName;
	if (!projectFileName.LoadString(IDS_PROJECTFILENAME)) {
		projectFileName = _T("WeldEvalution.prj");
	}
	CString ParmaterFileName;
	if (!ParmaterFileName.LoadString(IDS_PROPATYFILENAME)) {
		ParmaterFileName = _T("parameter.inf");
	}

	m_ParamaterFilePaht = CFileUtil::FilePathCombine(m_ActiveRegisttedTestFolder,ParmaterFileName);
	m_PropatyIO.SetParamaterFilePath(m_ParamaterFilePaht);
	if (!m_PropatyIO.Save(false)) {
		CFileUtil::fileDelete(m_ActiveRegisttedTestFolder);
		m_ActiveRegisttedTestName=_T("");
		m_ActiveRegisttedTestFolder=_T("");
		m_PropatyIO.SetProjectName(_T(""));
		CString tmpFolder	= m_ModulePath;
		m_ParamaterFilePaht = CFileUtil::FilePathCombine(tmpFolder,_T("paramater.tmp"));
		m_ProjectFilePaht	= CFileUtil::FilePathCombine(tmpFolder,_T("project.tmp"));

		CString msg;
		msg.Format(_T("�v���p�e�B�̕ۑ��Ɏ��s���܂����B"));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}
	if (m_OpenType == 2) {
		UINT uval;
		double dval;
		bool bResult = true;
		CPropatyIO pf;
		pf.SetParamaterFilePath(tmpParamFilePath);
		uval = pf.GetHorizontalResolution();
		if (!m_PropatyIO.SetHorizontalResolution(uval)) {
			CString msg;
			msg.Format(_T("�������̉𑜓x�̐ݒ�Ɏ��s���܂����B(%d)"),uval);
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
			bResult = false;
		}
		uval = pf.GetVerticalResolution();
		if (!m_PropatyIO.SetVerticalResolution(uval)) {
			CString msg;
			msg.Format(_T("�c�����̉𑜓x�̐ݒ�Ɏ��s���܂����B(%d)"), uval);
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
			bResult = false;
		}
		for (int method = 0; method < 2; method++) {
			uval = pf.ResinGetNumberOfClass(method);
			if (!m_PropatyIO.ResinSetNumberOfClass(method,uval)) {
				CString msg;
				msg.Format(_T("�����ʂ̕��ސ��̐ݒ�Ɏ��s���܂����B(%d)"), uval);
				writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
				bResult = false;
			}
			for (int i = 0; i < (int)uval; i++) {
				dval = pf.ResinGetJointRetio(method,i);
				if (!m_PropatyIO.ResinSetJointRetio(method,i, dval)) {
					CString msg;
					msg.Format(_T("�����ʂ̐ڍ������̐ݒ�B(%lf)"), dval);
					writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
					bResult = false;
				}
			}
		}
		for (int method = 0; method < 2; method++) {
			uval = pf.MetalGetNumberOfClass(method);
			if (!m_PropatyIO.MetalSetNumberOfClass(method,uval)) {
				CString msg;
				msg.Format(_T("�����ʂ̕��ސ��̐ݒ�Ɏ��s���܂����B(%d)"), uval);
				writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
				bResult = false;
			}
			for (int i = 0; i < (int)uval; i++) {
				dval = pf.MetalGetJointRetio(method,i);
				if (!m_PropatyIO.MetalSetJointRetio(method,i, dval)) {
					CString msg;
					msg.Format(_T("�����ʂ̐ڍ������̐ݒ�Ɏ��s���܂����B(%d)"), uval);
					writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
					bResult = false;
				}
			}
		}
		for (int method = 0; method < 2; method++) {
			uval = pf.ResultGetNumberOfClass(method);
			if (!m_PropatyIO.ResultSetNumberOfClass(method,uval)) {
				CString msg;
				msg.Format(_T("�ڍ����ʂ̕��ސ��̐ݒ�Ɏ��s���܂����B(%d)"), uval);
				writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
				bResult = false;
			}
			for (int i = 0; i < (int)uval; i++) {
				dval = pf.ResultGetJointRetio(method,i);
				if (!m_PropatyIO.ResultSetJointRetio(method,i, dval)) {
					CString msg;
					msg.Format(_T("�ڍ����ʂ̐ڍ������̐ݒ�Ɏ��s���܂����B(%d)"), uval);
					writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
					bResult = false;
				}
			}
		}
	}

	CString path;
	CString ImageRootPath = m_ActiveRegisttedTestFolder;

	m_ProjectIO.SetUpdateDay(date);
	CString filename;
	// �����ʃX�L�����C�摜������ꍇ
	{
		if (!SaveScanImage(eResinSurface)) {
			CString msg;
			msg.Format(_T("�����ʃX�L�����C�摜�̕ۑ��Ɏ��s���܂����B"));
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
			return false;
		}
	}

	// �����ʃX�L�����C�摜������ꍇ
	{
		if (!SaveScanImage(eMetalSurface)) {
			CString msg;
			msg.Format(_T("�����ʃX�L�����C�摜�̕ۑ��Ɏ��s���܂����B"));
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
			return false;
		}
	}

	// �ڍ����ʃX�L�����C�摜������ꍇ
	{
		if (!SaveScanImage(eJoiningResult)) {
			CString msg;
			msg.Format(_T("�ڍ����ʃX�L�����C�摜�̕ۑ��Ɏ��s���܂����B"));
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
			return false;
		}
	}

	// ������k-means���ތ��ʃt�@�C��������ꍇ
	{
		if (!SaveClassificationResultFile(eResinSurface, AnalizeTypeKMeans)) {
			CString msg;
			msg.Format(_T("������k-means���ތ��ʃt�@�C���̕ۑ��Ɏ��s���܂����B"));
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
			return false;
		}
	}
	// �����ʊK�w�N���X�^�����O���ތ��ʃt�@�C��������ꍇ
	{
		if (!SaveClassificationResultFile(eResinSurface, AnalizeTypeHiClustering)) {
			CString msg;
			msg.Format(_T("�����ʊK�w�N���X�^�����O���ތ��ʃt�@�C���̕ۑ��Ɏ��s���܂����B"));
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
			return false;
		}
	}

	// ������k-means���ތ��ʃt�@�C��������ꍇ
	{
		if (!SaveClassificationResultFile(eMetalSurface, AnalizeTypeKMeans)) {
			CString msg;
			msg.Format(_T("������k-means���ތ��ʃt�@�C���̕ۑ��Ɏ��s���܂����B"));
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
			return false;
		}
	}
	// �����ʊK�w�N���X�^�����O���ތ��ʃt�@�C��������ꍇ
	{
		if (!SaveClassificationResultFile(eMetalSurface, AnalizeTypeHiClustering)) {
			CString msg;
			msg.Format(_T("�����ʊK�w�N���X�^�����O���ތ��ʃt�@�C���̕ۑ��Ɏ��s���܂����B"));
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
			return false;
		}
	}

	
	// �ڍ�����k-means���ތ��ʃt�@�C��������ꍇ
	{
		if (!SaveClassificationResultFile(eJoiningResult, AnalizeTypeKMeans)) {
			CString msg;
			msg.Format(_T("�ڍ�����k-means���ތ��ʃt�@�C���̕ۑ��Ɏ��s���܂����B"));
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
			return false;
		}
	}
	// �ڍ����ʊK�w�N���X�^�����O���ތ��ʃt�@�C��������ꍇ
	{
		if (!SaveClassificationResultFile(eJoiningResult, AnalizeTypeHiClustering)) {
			CString msg;
			msg.Format(_T("�ڍ����ʊK�w�N���X�^�����O���ތ��ʃt�@�C���̕ۑ��Ɏ��s���܂����B"));
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
			return false;
		}
	}

//	m_ProjectFilePaht = CFileUtil::FilePathCombine(m_ActiveRegisttedTestFolder,ProjectFileName);
	m_ProjectIO.SetProjectFilePath(m_ProjectFilePaht);
	if (!m_ProjectIO.Save(false)) {
		CFileUtil::fileDelete(m_ParamaterFilePaht);
		CFileUtil::fileDelete(m_ActiveRegisttedTestFolder);
		m_ActiveRegisttedTestName=_T("");
		m_ActiveRegisttedTestFolder=_T("");
		m_PropatyIO.SetProjectName(_T(""));
		CString tmpFolder	= m_ModulePath;
		m_ParamaterFilePaht = CFileUtil::FilePathCombine(tmpFolder,_T("paramater.tmp"));
		m_ProjectFilePaht	= CFileUtil::FilePathCombine(tmpFolder,_T("project.tmp"));

		CString msg;
		msg.Format(_T("�v���W�F�N�g�t�@�C���̕ۑ��Ɏ��s���܂����B(%s)"), static_cast<LPCWSTR>(m_ProjectFilePaht));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}
	if (m_OpenType == 2) {
		m_OpenType = 1;
	}

	CString msg;
	CString folder = GetRegistedFolder();
	if (!PopProject(folder, m_ActiveRegisttedTestName)) {
		if (!msg.LoadString(IDM_ERR_PROJECT_SAVE)) {
			msg = _T("�o�^�Ɏ��s���܂����B");
		}
	}
	else {
		if (!msg.LoadString(IDM_PRJREGIST_SUCCESS)) {
			msg = _T("�o�^���������܂����B");
		}
	}
	AfxMessageBox(msg, MB_ICONINFORMATION);
	return true;
}

/// <summary>
/// �A�N�e�B�u�Ȍ������̎擾
/// </summary>
/// <returns>�A�N�e�B�u�Ȍ�������Ԃ�</returns>
CString CWeldEvaluationDoc::GetActiveRegistedTest()
{
	return m_ActiveRegisttedTestName;
}

/// <summary>
/// �X�i�b�v�X�L�����t�@�C���p�X�̎擾
/// </summary>
/// <returns>�X�i�b�v�X�L�����t�@�C���ւ̃p�X��Ԃ�</returns>
CString CWeldEvaluationDoc::getSnapscanFile()
{
	CConfigrationIO sys(m_SystemFilePathName);
	CString Snapscan_file = sys.getString(_T("Common"), _T("Snapscan_file"));
	return Snapscan_file;
}

/// <summary>
/// �X�L�����̕�����
/// </summary>
/// <returns>�X�L�����̕�������Ԃ�</returns>
int	CWeldEvaluationDoc::GetDivisionNumber()
{
	CConfigrationIO sys(m_SystemFilePathName);
	int DivisionNumber = sys.getInt(_T("System"), _T("DivisionNumber"));
	return DivisionNumber;
}

/// <summary>
/// ��͌��ʃt�@�C���̓ǂݍ���
/// </summary>
/// <param name="path">��̓t�@�C���p�X</param>
/// <param name="data">��̓f�[�^</param>
/// <returns>�����̏ꍇ��true�A���s�̏ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::getResultFile(CString path, vector<int>& data)
{
	m_CalcResultStopRequest = false;
	std::wstring spath((LPCTSTR)path);
	std::ifstream ifs(spath);

	UINT fileSize = 0;
	CView *pView = nullptr;
	POSITION p = GetFirstViewPosition();
	if (p != NULL) {
		pView = GetNextView(p);
		{ // �t�@�C���̃T�C�Y�����߂�
			ifs.seekg(0, fstream::end);			//�t�@�C��������T��
			UINT eofPos = (UINT)ifs.tellg();	//�t�@�C�������C���f�N�X���擾
			ifs.clear();						//�擪�ɂ��ǂ邽�߂Ɉ�xclear()��������B��������Ȃ��Ǝ���seekg()�ŃR�P��Ƃ�������B
			ifs.seekg(0, fstream::beg);			//�t�@�C���擪�ɖ߂�
			UINT begPos = (UINT)ifs.tellg();	//�t�@�C���擪�C���f�N�X���擾
			fileSize = eofPos - begPos;			//����-�擪�Ńt�@�C���T�C�Y���v�Z
		}
		pView->SendMessage(WM_READ_RESULT_STATUS,READ_RESULT_INIT,fileSize);
	}

	if (!ifs.is_open())
	{
		if (pView) {
			pView->SendMessage(WM_READ_RESULT_STATUS,READ_RESULT_END,NULL);
		}
		CString msg;
		msg.Format(_T("��͌��ʂ̎擾�Ɏ��s���܂����B(%s)"), static_cast<LPCWSTR>(path));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);

		return false;
	}
	
	data.clear();
	int h, v;
	GetScanDataSize(h, v);
	data.resize((__int64)h*v);

	string line;
	int id = 0;
	while (getline(ifs, line)) {
		if (split(line, ',', id, data)) {
			if (pView) {
				UINT readPos = (UINT)ifs.tellg();
				pView->SendMessage(WM_READ_RESULT_STATUS, READ_RESULT_READ, readPos);
			}

			if (m_CalcResultStopRequest) {	// ������~���N�G�X�g
				if (pView) {
					pView->SendMessage(WM_READ_RESULT_STATUS, READ_RESULT_END, NULL);
				}
				return false;
			}
		}
		else {
			return false;
		}
	}
	if (pView) {
		pView->SendMessage(WM_READ_RESULT_STATUS,READ_RESULT_END,NULL);
	}
	return true;
}

/// <summary>
/// �Y�t�t�H���_���擾
/// </summary>
/// <returns>�Y�t�t�H���_����Ԃ�</returns>
CString CWeldEvaluationDoc::GetTmpFolderName()
{
	return CString(_T("__temp__"));
}

/// <summary>
/// �Y�t�t�H���_�ւ̃p�X�擾
/// </summary>
/// <returns>�Y�t�t�H���_�ւ̃p�X����Ԃ�</returns>
CString CWeldEvaluationDoc::GetTmpFolderPath()
{
	return CFileUtil::FilePathCombine(GetRegistedFolder(), GetTmpFolderName());
}

/// <summary>
/// �X�L�����f�[�^���̎擾
/// </summary>
/// <param name="ScanID">�ΏۃX�L����ID</param>
/// <param name="Prefix">�v���t�B�b�N�X</param>
/// <returns>�X�L�����f�[�^����Ԃ�</returns>
CString CWeldEvaluationDoc::GetScanDataName(int ScanID, CString Prefix)
{
	CString ProjectName = m_PropatyIO.GetProjectName();
	CString fileName;
	switch (ScanID) {
	case	eResinSurface:	///< ����
	{
		if (Prefix.IsEmpty()) {
			fileName.Format(_T("ResinScanImage"));
		}
		else {
			fileName.Format(_T("%s_ResinScanImage"), (LPCWSTR)ProjectName);
		}
	}
	break;
	case	eMetalSurface:	///< ����
	{
		if (Prefix.IsEmpty()) {
			fileName.Format(_T("MetalScanImage"));
		}
		else {
			fileName.Format(_T("%s_MetalScanImage"), (LPCWSTR)ProjectName);
		}
	}
	break;
	case	eJoiningResult:	///< �ڍ�����
	{
		if (Prefix.IsEmpty()) {
			fileName.Format(_T("ResultScanImage"));
		}
		else {
			fileName.Format(_T("%s_ResultScanImage"), (LPCWSTR)ProjectName);
		}
	}
	break;
	default:
		return _T("");
	}
	return fileName;
}

/// <summary>
/// �X�L�����f�[�^�̕ۑ�
/// </summary>
/// <param name="ScanID">�ΏۃX�L����ID</param>
/// <returns>���������ꍇ��true�A���s�̏ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::SaveScanImage(int ScanID)
{
//	CString ProjectName = m_PropatyIO.GetProjectName();
	CString ProjectName = _T("");		// �t�@�C�����Ƀv���W�F�N�g����t���Ȃ��l�ɏC��
	CString fileName = GetScanDataName(ScanID, ProjectName);
	switch (ScanID) {
	case	eResinSurface:	///< ����
		if (!m_ProjectIO.SetResinScanImageFile(fileName)) {
			CString msg;
			msg.Format(_T("�����ʃX�L�����f�[�^�̐ݒ�Ɏ��s���܂����B(%s)"), static_cast<LPCWSTR>(fileName));
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
			return false;
		}
		////////////////////////////////////////////////////////////////////
		//
		// �ۑ��������s��(.hdr,.raw,.bmp)
		// �摜�̕ۑ��ɐ���������v���W�F�N�g�֓o�^
		//
		////////////////////////////////////////////////////////////////////
		break;
	case	eMetalSurface:	///< ����
		if (!m_ProjectIO.SetMetalScanImageFile(fileName)) {
			CString msg;
			msg.Format(_T("�����ʃX�L�����f�[�^�̐ݒ�Ɏ��s���܂����B(%s)"), static_cast<LPCWSTR>(fileName));
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
			return false;
		}
		////////////////////////////////////////////////////////////////////
		//
		// �ۑ��������s��(.hdr,.raw,.bmp)
		// �摜�̕ۑ��ɐ���������v���W�F�N�g�֓o�^
		//
		////////////////////////////////////////////////////////////////////
		break;
	case	eJoiningResult:	///< �ڍ�����
		if (!m_ProjectIO.SetResultScanImageFile(fileName)) {
			CString msg;
			msg.Format(_T("�ڍ����ʃX�L�����f�[�^�̐ݒ�Ɏ��s���܂����B(%s)"), static_cast<LPCWSTR>(fileName));
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
			return false;
		}
		////////////////////////////////////////////////////////////////////
		//
		// �ۑ��������s��(.hdr,.raw,.bmp)
		// �摜�̕ۑ��ɐ���������v���W�F�N�g�֓o�^
		//
		////////////////////////////////////////////////////////////////////
		break;
	default:
		{
			CString msg;
			msg.Format(_T("�X�L�����Ώ�ID�l���s���ł��B(%d)"), ScanID);
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		}
		return false;
	}
	return true;
}

/// <summary>
/// ���ރt�@�C���̗L������
/// </summary>
/// <param name="ScanID">�t�@�C�����ID</param>
/// <param name="method">��͕��@</param>
/// <returns>���݂���ꍇ��true�A���s�̏ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::IsExistClassificationResultFile(int ScanID, int method)
{
	bool bResult = false;
	CString path;

	path = getClassificationDataFilePath(ScanID, method);
	if (path.IsEmpty()) {
		bResult = false;
	}
	else {
		bResult = CFileUtil::fileExists(path);
	}
	return bResult;
}

/// <summary>
/// ��͌��ʂ̕��ސ��Ɛݒ�̕��ސ����قȂ邩�̔���
/// </summary>
/// <param name="ScanID">�t�@�C�����ID</param>
/// <param name="AnalysisMethodID">��͕��@</param>
/// <returns>�قȂ��Ă���ꍇ��true�A�����ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::IsUpdateNumberOfClassifications(int ScanID, int AnalysisMethodID)
{
	int	nClass=0,CriatedNClass=0;
	if (ScanID == eResinSurface) {
		nClass = m_PropatyIO.ResinGetNumberOfClass(AnalysisMethodID);
		if (AnalysisMethodID == AnalizeTypeKMeans) {
			CriatedNClass = m_ProjectIO.GetNumbetOfResinKMeansClassificationClass();
		}
		else if (AnalysisMethodID == AnalizeTypeHiClustering) {
			CriatedNClass = m_ProjectIO.GetNumbetOfResinDendrogramClassificationClass();
		}
	}
	else if (ScanID == eMetalSurface) {
		nClass = m_PropatyIO.MetalGetNumberOfClass(AnalysisMethodID);
		if (AnalysisMethodID == AnalizeTypeKMeans) {
			CriatedNClass = m_ProjectIO.GetNumbetOfMetalKMeansClassificationClass();
		}
		else if (AnalysisMethodID == AnalizeTypeHiClustering) {
			CriatedNClass = m_ProjectIO.GetNumbetOfMetalDendrogramClassificationClass();
		}
	}
	else if (ScanID == eJoiningResult) {
		nClass = m_PropatyIO.ResultGetNumberOfClass(AnalysisMethodID);
		if (AnalysisMethodID == AnalizeTypeKMeans) {
			CriatedNClass = m_ProjectIO.GetNumbetOfResultKMeansClassificationClass();
		}
		else if (AnalysisMethodID == AnalizeTypeHiClustering) {
			CriatedNClass = m_ProjectIO.GetNumbetOfResultDendrogramClassificationClass();
		}
	}
	return (nClass != CriatedNClass);
}


/// <summary>
/// ���ތ��ʂ̕ۑ�
/// </summary>
/// <param name="ScanID">�ΏۃX�L����ID</param>
/// <param name="method">��͕��@</param>
/// <returns>���������ꍇ��true�A���s�̏ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::SaveClassificationResultFile(int ScanID, int method)
{
	CString fileName;
	switch(ScanID) {
	case	eResinSurface	:	// ����
		if (method == AnalizeTypeKMeans)
		{
			fileName = _T("ResinDendrogramClassification.csv");
			if (!m_ProjectIO.SetResinDendrogramClassFile(fileName)) {
				CString msg;
				msg.Format(_T("�����ʊK�w�N���X�^�����O���ތ��ʃt�@�C�����̐ݒ�Ɏ��s���܂����B(%s)"), static_cast<LPCWSTR>(fileName));
				writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
				return false;
			}
		}
		else if (method == AnalizeTypeHiClustering)
		{
			fileName = _T("ResinkmeansClassification.csv");
			if (!m_ProjectIO.SetResinKmeansClassFile(fileName)) {
				CString msg;
				msg.Format(_T("������k-means���ތ��ʃt�@�C�����̐ݒ�Ɏ��s���܂����B(%s)"), static_cast<LPCWSTR>(fileName));
				writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
				return false;
			}
		}
		break;
	case	eMetalSurface	:	// ����
		if (method == AnalizeTypeKMeans)
		{
			fileName = _T("MetalDendrogramClassification.csv");
			if (!m_ProjectIO.SetMetalDendrogramClassFile(fileName)) {
				CString msg;
				msg.Format(_T("�����ʊK�w�N���X�^�����O���ތ��ʃt�@�C�����̐ݒ�Ɏ��s���܂����B(%s)"), static_cast<LPCWSTR>(fileName));
				writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
				return false;
			}
		}
		else if (method == AnalizeTypeHiClustering) {
			fileName = _T("MetalkmeansClassification.csv");
			if (!m_ProjectIO.SetMetalKmeansClassFile(fileName)) {
				CString msg;
				msg.Format(_T("������k-means���ތ��ʃt�@�C�����̐ݒ�Ɏ��s���܂����B(%s)"), static_cast<LPCWSTR>(fileName));
				writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
				return false;
			}
		}
		break;
	case	eJoiningResult:	// �ڍ�����
		if (method == AnalizeTypeKMeans)
		{
			fileName = _T("ResultDendrogramClassification.csv");
			if (!m_ProjectIO.SetResultDendrogramClassFile(fileName)) {
				CString msg;
				msg.Format(_T("�ڍ����ʊK�w�N���X�^�����O�t���ރ@�C�����̐ݒ�Ɏ��s���܂����B(%s)"), static_cast<LPCWSTR>(fileName));
				writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
				return false;
			}
		}
		else if (method == AnalizeTypeHiClustering) {
			fileName = _T("ResultkmeansClassification.csv");
			if (!m_ProjectIO.SetResultKmeansClassFile(fileName)) {
				CString msg;
				msg.Format(_T("�ڍ�����k-means�t�@�C�����̐ݒ�Ɏ��s���܂����B(%s)"), static_cast<LPCWSTR>(fileName));
				writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
				return false;
			}
		}
		break;
	default :
		return false;
	}
	return true;
}


/// <summary>
/// �o�^�ς݌����̎擾
/// </summary>
/// <param name="list">�o�^�ς݌����̃��X�g</param>
/// <returns>���������ꍇ��true�A���s�̏ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::GetRegistTestList(CStringArray &list)
{
	list.RemoveAll();
	CString path = GetRegistedFolder();
	if (path.IsEmpty() || (!CFileUtil::fileExists(path))) {
		CString msg;
		msg.Format(_T("�o�^�p�t�H���_�����݂��܂���B(%s)"), static_cast<LPCWSTR>(path));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}

	CFileFind    cFileFind;
	CString      strSearchDir = CFileUtil::FilePathCombine(path,_T("*"));
	if (!cFileFind.FindFile(strSearchDir)) {
		return false;	
	}

	bool bResult = false;
	do
	{
		bResult = cFileFind.FindNextFile();
		if (!cFileFind.IsDots()) {
			if (cFileFind.IsDirectory()) {
				CString patnname = cFileFind.GetFileTitle();
				if (patnname.CollateNoCase(GetTmpFolderName()) == 0) {
					continue;
				}
				else if (patnname.CollateNoCase(GetWorkProjectFolderName()) == 0) {
					continue;
				}
				list.Add(patnname);
			}
		}
	} while (bResult);
	return true;
}

/// <summary>
/// ��͏������~���N�G�X�g
/// </summary>
void CWeldEvaluationDoc::CalcResultStopRequest()
{
	m_CalcResultStopRequest = true;
}

/// <summary>
/// �ڍ��������߂�
/// </summary>
/// <param name="buff">�f�[�^</param>
/// <param name="ndata">�f�[�^��</param>
/// <param name="nClass">���ސ�</param>
/// <param name="retio">�ڍ���</param>
/// <returns>�����̏ꍇ��true�A���s�̏ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::CalcJoJointRetio(vector<int> data, int nClass, vector<double> &retio)
{
	retio.clear();
	retio.resize(nClass);
	retio.reserve(nClass);

	int dataw, datah;
	GetScanDataSize(dataw, datah);
	if ((int)data.size() != ((__int64)dataw * datah)) {
		CString msg;
		msg.Format(_T("�f�[�^�T�C�Y���X�L�����f�[�^�̃T�C�Y�ƈ�v���܂���Bwidht=%d Height=%d :�f�[�^�T�C�Y=%d"), dataw, datah, (int)data.size());
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}
	CPoint AnzLTPos;
	CSize AnzSize;
	if (!getAnalizeArea(AnzLTPos, AnzSize)) {
		CString msg;
		msg.Format(_T("��̓G���A���擾�ł��܂���ł����B"));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}

	std::map<int,int> sum;
	for (int i = 0; i < nClass; i++) {
		sum[i] = 0;
	}

#if 0
	for (int i = 0; i < (int)data.size(); i++) {
		if ((data[i] < 0) || (data[i] >= nClass)) {
			continue;
		}
		sum[data[i]] += 1;
	}

	for (int i = 0; i < nClass; i++) {
		retio[i] = (double)sum[i] / (double)data.size();
	}
#else
	int nData = 0;
	int id = 0;
	int minH = AnzLTPos.y - 1;
	int minW = AnzLTPos.x - 1;
	int maxH = AnzLTPos.y + AnzSize.cy - 1;
	int maxW = AnzLTPos.x + AnzSize.cx - 1;
	for (int h = 0; h < datah; h++) {
		if ((h < minH) || (h >= maxH)) {
			continue;
		}
		id = h * dataw;
		for (int w = 0; w < dataw; w++) {
			if ((w < minW) || (w >= maxW)) {
				continue;
			}
			if ((data[(__int64)id+w] < 0) || (data[(__int64)id+w] >= nClass)) {
				continue;
			}
			sum[data[(__int64)id+w]] += 1;
			nData++;
		}
	}

	for (int i = 0; i < nClass; i++) {
		retio[i] = (double)sum[i] / (double)nData;
	}
#endif
	return true;
}

bool CWeldEvaluationDoc::getAnalizeArea(CPoint &tlPos, CSize &size)
{
	CConfigrationIO sys(m_SystemFilePathName);
	int AnalysisAreaTLPointX = sys.getInt(_T("System"), _T("AnalysisAreaTLPointX"));
	int AnalysisAreaTLPointY = sys.getInt(_T("System"), _T("AnalysisAreaTLPointY"));
	int AnalysisAreaWidth = sys.getInt(_T("System"), _T("AnalysisAreaWidth"));
	int AnalysisAreaHeight = sys.getInt(_T("System"), _T("AnalysisAreaHeight"));
	int dataw, datah;
	m_ProjectIO.GetScanDataSize(dataw, datah);
	if ((dataw == 0) || (datah == 0)) {
		CString msg;
		msg.Format(_T("�������ꂽ�X�L�����f�[�^�̃T�C�Y������������܂���BWidth=%d Height=%d"), dataw, datah);
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}

	if ((AnalysisAreaWidth <= 0) || (AnalysisAreaHeight <= 0)) {
		AnalysisAreaTLPointX = 0;
		AnalysisAreaTLPointY = 0;
		AnalysisAreaWidth = dataw;
		AnalysisAreaHeight = datah;
		CString msg;
		msg.Format(_T("��̓G���A�̒l������������܂���B��̓G���A��S�̈�ɐݒ肵�܂�"));
		writeLog(CLog::Warning, CString(__FILE__), __LINE__, msg);
	}
	if ((AnalysisAreaTLPointX + AnalysisAreaWidth) > dataw) {
		AnalysisAreaWidth = dataw - AnalysisAreaTLPointX;
	}
	if ((AnalysisAreaTLPointY + AnalysisAreaHeight) > datah) {
		AnalysisAreaHeight = datah - AnalysisAreaTLPointY;
	}

	tlPos.x = AnalysisAreaTLPointX;
	tlPos.y = AnalysisAreaTLPointY;
	size.cx = AnalysisAreaWidth;
	size.cy = AnalysisAreaHeight;
	return true;
}

/// <summary>
/// ��͂̎��{
/// </summary>
/// <param name="ScanID">����f��ID</param>
/// <param name="AnalysisMethodID">��͕��@ID</param>
/// <returns>�����̏ꍇ��true�A���s�̏ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::Analize(int ScanID, int AnalysisMethodID)
{
	int nClass = 1;
	CString ScanDataFilePath, ClassificationDataFilePath;
	////////////////////////////////////////////////////////////////////
	// ��͂����{
	////////////////////////////////////////////////////////////////////
	bool (CProjectFileIO::*SetNumbetOfClass)(int) = nullptr;

	ScanDataFilePath = getScanDataFilePath(ScanID);
	ClassificationDataFilePath = getClassificationDataFilePath(ScanID, AnalysisMethodID);
	switch (ScanID) {
	case	eResinSurface	:	// ����
		nClass = ResinGetNumberOfClass(AnalysisMethodID);
		if (AnalysisMethodID == AnalizeTypeHiClustering) {
			SetNumbetOfClass = &CProjectFileIO::SetNumbetOfResinDendrogramClassificationClass;
		}
		else if (AnalysisMethodID == AnalizeTypeKMeans) {
			SetNumbetOfClass = &CProjectFileIO::SetNumbetOfResinKMeansClassificationClass;
		}
		break;
	case	eMetalSurface	:	// ����
		nClass = MetalGetNumberOfClass(AnalysisMethodID);
		if (AnalysisMethodID == AnalizeTypeHiClustering) {
			SetNumbetOfClass = &CProjectFileIO::SetNumbetOfMetalDendrogramClassificationClass;
		}
		else if (AnalysisMethodID == AnalizeTypeKMeans) {
			SetNumbetOfClass = &CProjectFileIO::SetNumbetOfMetalKMeansClassificationClass;
		}
		break;
	case	eJoiningResult	:	// �ڍ�����
		nClass = ResultGetNumberOfClass(AnalysisMethodID);
		if (AnalysisMethodID == AnalizeTypeHiClustering) {
			SetNumbetOfClass = &CProjectFileIO::SetNumbetOfResultDendrogramClassificationClass;
		}
		else if (AnalysisMethodID == AnalizeTypeKMeans) {
			SetNumbetOfClass = &CProjectFileIO::SetNumbetOfResultKMeansClassificationClass;
		}
		break;
	}

	CConfigrationIO sys(m_SystemFilePathName);
	CString modulePath = sys.getString(_T("Common"), _T("AIModuleFolder"));
	if (modulePath.IsEmpty()) {
		CString msg;
		msg.Format(_T("��̓��W���[���i�[�t�H���_�����݂��܂���B(%s):ScanID=%d,��͕��@=%d"), static_cast<LPCWSTR>(modulePath), ScanID, AnalysisMethodID);
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}
	CString returnfile = CFileUtil::FilePathCombine(modulePath, _T("return.txt"));
	CFileUtil::fileDelete(returnfile);

	CString execpath;
	PROCESS_INFORMATION p;
	STARTUPINFO s;
	ZeroMemory(&s, sizeof(s));
	s.cb = sizeof(s);

	CString prm;
	switch (AnalysisMethodID) {
	case	AnalizeTypeKMeans:				// k-means
		execpath = CFileUtil::FilePathCombine(modulePath, _T("kmeans.bat"));
		break;
	case	AnalizeTypeHiClustering:		// �K�w�N���X�^�����O
		execpath = CFileUtil::FilePathCombine(modulePath, _T("dendrogram.bat"));
		break;
	default:
		{
			CString msg;
			msg.Format(_T("���m�̉�͎�@���ݒ肳��Ă��܂��B(%d):ScanID=%d"), AnalysisMethodID, ScanID);
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		}
		return false;
	}

	// ��̓G���A�̎擾
#if 0
	int AnalysisAreaTLPointX = sys.getInt(_T("System"), _T("AnalysisAreaTLPointX"));
	int AnalysisAreaTLPointY = sys.getInt(_T("System"), _T("AnalysisAreaTLPointY"));
	int AnalysisAreaWidth = sys.getInt(_T("System"), _T("AnalysisAreaWidth"));
	int AnalysisAreaHeight = sys.getInt(_T("System"), _T("AnalysisAreaHeight"));
	int dataw, datah;
	m_ProjectIO.GetScanDataSize(dataw, datah);
	if ((dataw == 0) || (datah == 0)) {
		CString msg;
		msg.Format(_T("�������ꂽ�X�L�����f�[�^�̃T�C�Y������������܂���BWidth=%d Height=%d"), dataw, datah);
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}

	if ((AnalysisAreaWidth <= 0) || (AnalysisAreaHeight <= 0)) {
		AnalysisAreaTLPointX = 0;
		AnalysisAreaTLPointY = 0;
		AnalysisAreaWidth = dataw;
		AnalysisAreaHeight = datah;
		CString msg;
		msg.Format(_T("��̓G���A�̒l������������܂���B�S�̈�̉�͂������Ȃ��܂�"));
		writeLog(CLog::Warning, CString(__FILE__), __LINE__, msg);
	}
	if ((AnalysisAreaTLPointX + AnalysisAreaWidth) > dataw) {
		AnalysisAreaWidth = dataw - AnalysisAreaTLPointX;
	}
	if ((AnalysisAreaTLPointY + AnalysisAreaHeight) > datah) {
		AnalysisAreaHeight = datah - AnalysisAreaTLPointY;
	}
	if ((AnalysisAreaWidth < (int)m_PropatyIO.NumberOfOverridePixel()) || (AnalysisAreaHeight <= 0)) {
		CString msg;
		msg.Format(_T("��̓G���A�̒l������������܂���BStert Point(%d, %d) Width=%d Height=%d"), AnalysisAreaTLPointX, AnalysisAreaTLPointY, AnalysisAreaWidth, AnalysisAreaHeight);
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}
#else
	CPoint AnalizetlPos;
	CSize AnalizeSze;
	if (!getAnalizeArea(AnalizetlPos, AnalizeSze)) {
		CString msg;
		msg.Format(_T("��̓G���A�̒l������������܂���"));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}
	if ((AnalizeSze.cx < (int)m_PropatyIO.NumberOfOverridePixel()) || (AnalizeSze.cy <= 0)) {
		CString msg;
		msg.Format(_T("��̓G���A�̒l������������܂���BStert Point(%d, %d) Width=%d Height=%d"), AnalizetlPos.x, AnalizetlPos.y, AnalizeSze.cx, AnalizeSze.cy);
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}
#endif

	////////////////////////////////////////////////////////////////////
	// ��͂̎��{
	if (CFileUtil::fileExists(execpath)) {
		prm.Format(_T("%s %s %d %d %d %d %d"), (LPCTSTR)ScanDataFilePath, (LPCTSTR)ClassificationDataFilePath, nClass, AnalizetlPos.x, AnalizetlPos.y, AnalizeSze.cx, AnalizeSze.cy);
		CString cmd;
		cmd.Format(_T("%s %s"), (LPCTSTR)execpath, (LPCTSTR)prm);
		TCHAR pszText[1049], pszMpath[1049];
		ZeroMemory(pszText, 1049);
		ZeroMemory(pszMpath, 1049);
		_tcscpy_s(pszText, 1049, cmd);
		_tcscpy_s(pszMpath, 1049, modulePath);
		int ret = CreateProcess(
			NULL,
			pszText,
			NULL,
			NULL,
			FALSE,
			NULL,
			NULL,
			pszMpath,
			&s,
			&p
		);
		if (ret == 0) {
			CloseHandle(p.hProcess);
			CString msg;
			msg.Format(_T("��̓��W���[���̎��s�Ɏ��s���܂����B(%s):ScanID=%d,��͕��@=%d"), static_cast<LPCWSTR>(execpath), ScanID, AnalysisMethodID);
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
			return false;
		}
		else {
			CloseHandle(p.hThread);
			WaitForSingleObject(p.hProcess, INFINITE);
			CloseHandle(p.hProcess);
		}
	}
	else {
		CString msg;
		msg.Format(_T("��̓��W���[�������݂��܂���B(%s):ScanID=%d,��͕��@=%d"), static_cast<LPCWSTR>(execpath), ScanID, AnalysisMethodID);
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}

	bool bResult = false;
	int count = 0;
	int TimeOut = 500;
	while (count < TimeOut) {
		if (CFileUtil::fileExists(returnfile)) {
			bResult = true;
			break;
		}
		Sleep(10);
		count++;
	}
	if (!bResult) {
		// �^�C���A�E�g
		return false;
	}

	// ��͌��ʂ̍��ۃ`�F�b�N�i���ʃt�@�C����́j
	CStdioFile fd;
	UINT nOpenFlags = CFile::modeRead;
	if (fd.Open(returnfile, nOpenFlags))
	{
		CString read;
		if (!fd.ReadString(read)) {
			fd.Close();
			CString msg;
			msg.Format(_T("��͌��ʔ��茋�ʂ�ǂݍ��߂܂���ł����B:ScanID=%d,��͕��@=%d"), ScanID, AnalysisMethodID);
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
			return false;
		}
		int val = _ttol(read);
		fd.Close();
		if (val != 0) {
			CString msg;
			msg.Format(_T("��͂Ɏ��s���܂����B(result=%d):ScanID=%d,��͕��@=%d"), val, ScanID, AnalysisMethodID);
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
			return false;
		}
	}
	else {
		CString msg;
		msg.Format(_T("��͌��ʔ���t�@�C�����J���܂���ł����B(%s):ScanID=%d,��͕��@=%d"), static_cast<LPCWSTR>(returnfile), ScanID, AnalysisMethodID);
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}

	// ��͎��{���̕ۑ�
	if (SetNumbetOfClass) {
		if ((m_ProjectIO.*SetNumbetOfClass)(nClass)) {
			CString msg;
			msg.Format(_T("��͎��{���̕ۑ��Ɏ��s���܂����B:ScanID=%d,��͕��@=%d"), ScanID, AnalysisMethodID);
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		}
	}

	////////////////////////////////////////////////////////////////////
	// �ڍ��������߂�
	CString path;
	vector<int> data;
	vector<double> retio;
	path = getClassificationDataFilePath(ScanID, AnalysisMethodID);
	if (ScanID == eResinSurface) {
		nClass = m_PropatyIO.ResinGetNumberOfClass(AnalysisMethodID);
	}
	else if (ScanID == eMetalSurface) {
		nClass = m_PropatyIO.MetalGetNumberOfClass(AnalysisMethodID);
	}
	else if (ScanID == eJoiningResult) {
		nClass = m_PropatyIO.ResultGetNumberOfClass(AnalysisMethodID);
	}

	if (!CWeldEvaluationDoc::getResultFile(path, data)) {
		CString msg;
		msg.Format(_T("��͌��ʃt�@�C���̓ǂݍ��݂Ɏ��s���܂����B(%s):ScanID=%d,��͕��@=%d"), static_cast<LPCWSTR>(path), ScanID, AnalysisMethodID);
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}
	else {
		int dataw, datah;
		GetScanDataSize(dataw, datah);
		if ((int)data.size() != ((__int64)dataw * datah)) {
			CString msg;
			msg.Format(_T("�f�[�^�T�C�Y���X�L�����f�[�^�̃T�C�Y�ƈ�v���܂���B:ScanID=%d,widht=%d Height=%d :�f�[�^�T�C�Y=%d"), ScanID, dataw, datah, (int)data.size());
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
			return false;
		}

		if (!CalcJoJointRetio(data, nClass, retio)) {
			CString msg;
			msg.Format(_T("�ڍ����̎擾�Ɏ��s���܂����B:ScanID=%d,��͕��@=%d"), ScanID, AnalysisMethodID);
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
			return false;
		}
	}

	if (ScanID == eResinSurface) {
		for (int ViewJointRatioNo = 0; ViewJointRatioNo < (int)retio.size(); ViewJointRatioNo++) {
			if (!ResinSetJointRetio(AnalysisMethodID, ViewJointRatioNo,retio[ViewJointRatioNo]*100.0)) {
				CString msg;
				msg.Format(_T("�����ʂ̐ڍ������̐ݒ�Ɏ��s���܂����B(ScanID=%d�A��͕��@=%d�A�ڍ��ʔԍ�=%d,�����ʂ̐ڍ�����=%lf)"), ScanID, AnalysisMethodID, ViewJointRatioNo, retio[ViewJointRatioNo] * 100.0);
				writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
				bResult = false;
			}
		}
	} else if (ScanID == eMetalSurface) {
		for (int ViewJointRatioNo = 0; ViewJointRatioNo < (int)retio.size(); ViewJointRatioNo++) {
			if (!MetalSetJointRetio(AnalysisMethodID, ViewJointRatioNo,retio[ViewJointRatioNo]*100.0)) {
				CString msg;
				msg.Format(_T("�����ʂ̐ڍ������̐ݒ�Ɏ��s���܂����B(ScanID=%d�A��͕��@=%d�A�ڍ��ʔԍ�=%d,�����ʂ̐ڍ�����=%lf)"), ScanID, AnalysisMethodID, ViewJointRatioNo, retio[ViewJointRatioNo] * 100.0);
				writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
				bResult = false;
			}
		}
	} else if (ScanID == eJoiningResult) {
		for (int ViewJointRatioNo = 0; ViewJointRatioNo < (int)retio.size(); ViewJointRatioNo++) {
			if (!ResultSetJointRetio(AnalysisMethodID,ViewJointRatioNo, retio[ViewJointRatioNo] * 100.0)) {
				CString msg;
				msg.Format(_T("�ڍ����ʂ̐ڍ������̐ݒ�Ɏ��s���܂����B(ScanID=%d�A��͕��@=%d�A�ڍ��ʔԍ�=%d,�ڍ����ʂ̐ڍ�����=%lf)"), ScanID, AnalysisMethodID, ViewJointRatioNo, retio[ViewJointRatioNo] * 100.0);
				writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
				bResult = false;
			}
		}
	}
	return bResult;
}

/// <summary>
/// �X�L�����f�[�^�C���[�W�̎擾
/// </summary>
/// <param name="ScanID">�ΏۃX�L����ID</param>
/// <param name="img">�擾�C���[�W</param>
/// <param name="renew">�ēǍ��t���O</param>
/// <returns>�����̏ꍇ��true�A���s�̏ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::LoadScanImage(int ScanID, CImage &img, bool renew/* = false*/)
{
	CString root,name;
	CString scanfile,imagefile;

	scanfile = getScanDataFilePath(ScanID);
	if (!CFileUtil::fileExists(scanfile)) {
		return false;
	}

	imagefile = getScanImageFilePath(ScanID);
	if ((renew) || (!CFileUtil::fileExists(imagefile))) {
		CScanDataIO *pSdio = nullptr;

		int width, height, nBand;
		switch (ScanID) {
		case	eResinSurface:	///< ����
			pSdio = &m_ResinScanData;
			break;
		case	eMetalSurface:	///< ����
			pSdio = &m_MetalScanData;
			break;
		case	eJoiningResult:	///< �ڍ�����
			pSdio = &m_ResultScanData;
			break;
		default:
			return false;
		}
		double r, g, b;
		GetRGBWavelength(r,g,b); 
		pSdio->SetRGBWavelength(r,g,b);
		if (pSdio->open(scanfile,renew)) {
			if (!pSdio->LoadImage(height, width, nBand, img)) {
				CString msg;
				msg.Format(_T("�摜�f�[�^�擾�̎擾�Ɏ��s���܂����B(%s)"), static_cast<LPCWSTR>(scanfile));
				writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
				return false;
			}
			img.Save(imagefile);
		}
		else {
			CString msg;
			msg.Format(_T("�摜�f�[�^�擾�̎擾�Ɏ��s���܂����B(%s)"), static_cast<LPCWSTR>(scanfile));
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
			return false;
		}
	}
	else {
		// jpeg����CBitmap�ɕϊ�
		HRESULT ret;
		ret = img.Load(imagefile);
		if (FAILED(ret)) {
			return false;
		}
	}
	return true;
}

/// <summary>
/// �X�L�����f�[�^�C���[�W�̍폜
/// </summary>
/// <param name="ScanID">�ΏۃX�L����ID</param>
/// <returns>�����̏ꍇ��true�A���s�̏ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::DeleteScanImageFilePath(int ScanID)
{
	CString scanImageFile = getScanImageFilePath(ScanID);
	if (CFileUtil::fileExists(scanImageFile)) {
		return CFileUtil::fileDelete(scanImageFile);
	}
	return true;
}

/// <summary>
/// �X�L�����f�[�^�̔��]
/// </summary>
/// <param name="ScanID">�ΏۃX�L����ID</param>
/// <returns>�����̏ꍇ��true�A���s�̏ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::InversScanData(int ScanID)
{
	CScanDataIO *pSdio = nullptr;
	switch (ScanID) {
	case	eResinSurface:	///< ����
		pSdio = &m_ResinScanData;
		break;
	case	eMetalSurface:	///< ����
		pSdio = &m_MetalScanData;
		break;
	case	eJoiningResult:	///< �ڍ�����
		pSdio = &m_ResultScanData;
		break;
	default:
		return false;
	}

	bool bResult = true;
	CString pathName = getScanDataPath(ScanID);
	CString fPathName = pathName + _T(".raw");
	if (pSdio->InversData()) {
		CString tmp = fPathName + _T(".tmp");
		bResult = pSdio->saveRawData(tmp);
		if (bResult) {
			CString bak = fPathName + _T(".bak");
			CFileUtil::fileDelete(bak);
			CFile::Rename(fPathName, bak);
			CFile::Rename(tmp, fPathName);
			CFileUtil::fileDelete(bak);
		}
	}
	else {
		bResult = false;
		if (!pSdio->readRawData(fPathName)) {
		}
	}
	return bResult;
}

/// <summary>
/// ��̓f�[�^�̔��]
/// </summary>
/// <param name="ScanID">�ΏۃX�L����ID</param>
/// <returns>�����̏ꍇ��true�A���s�̏ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::InversAnalizeData(int ScanID)
{
	CString ScanDataFilePath, ClassificationDataFilePath, imgPaht;
	ScanDataFilePath = getScanDataFilePath(ScanID);
	if (!CFileUtil::fileExists(ScanDataFilePath)) {
		return true;
	}

	int method[] = { AnalizeTypeKMeans ,AnalizeTypeHiClustering };

	bool bResult = true;
	int width, height;
	GetScanDataSize(width, height);
#if 0
	CStdioFile tfd;
	CString buf;
	vector<CString> lines;
	lines.resize(height);
	for (int metodID = 0; metodID < 2; metodID++) {
		ClassificationDataFilePath = getClassificationDataFilePath(ScanID, method[metodID]);
		if (!CFileUtil::fileExists(ClassificationDataFilePath)) {
			continue;
		}

		if (tfd.Open(ClassificationDataFilePath, CFile::modeReadWrite | CFile::typeText)) {
			CString key, val;
			int i = height - 1;
			bool bOverflow = false;
			lines.clear();
			lines.resize(height);
			while (tfd.ReadString(buf)) {
				if (i < 0) {
					bOverflow = true;
					bResult = false;
					break;
				}
				lines[i] = buf;
				i--;
			}
			if (!bOverflow) {
				tfd.SeekToBegin();
				for (i = 0; i < height; i++) {
					tfd.WriteString(lines[i] + _T("\n"));
				}
				tfd.Close();

				imgPaht = ClassificationDataFilePath + _T(".bmp");
				if (CFileUtil::fileExists(imgPaht)) {
					CFileUtil::fileDelete(imgPaht);
				}
			}
			else {
				tfd.Close();
			}
		}
		else {
			bResult = false;
		}
	}
	lines.clear();
	lines.shrink_to_fit();

#else
	float ***ana = nullptr;
	try {
		CString buf, val;
		CStdioFile tfd;
		ana = new float **[2]();
		if (ana) {
			for (int i = 0; i < 2; i++) {
				ana[i] = new float *[height]();
				for (int h = 0; h < height; h++) {
					ana[i][h] = new float[width]();
				}
			}
		}

		CScanDataIO scn;
		double **mat = scn.MatrixInit();
		scn.MatrixMove(mat, width / 2, height / 2);
		scn.MatrixRotete(mat, 180);
		scn.MatrixMove(mat, -width / 2, -height / 2);
		scn.MatrixInvers(mat);

		for (int metodID = 0; metodID < 2; metodID++) {
			ClassificationDataFilePath = getClassificationDataFilePath(ScanID, method[metodID]);
			if (!CFileUtil::fileExists(ClassificationDataFilePath)) {
				continue;
			}

			if (tfd.Open(ClassificationDataFilePath, CFile::modeReadWrite | CFile::typeText)) {
				int id, w = 0, h = 0;
				while (tfd.ReadString(buf)) {
					if (!buf.IsEmpty()) {
						w = 0;
						while (w < width) {
							id = buf.FindOneOf(_T(","));
							if (id >= 0) {
								val = buf.Mid(0, id);
								ana[0][h*width][w++] = (float)_ttof(val);
								if (buf.GetLength() < (id + 1)) {
									val = buf;
									ana[0][h*width][w] = (float)_ttof(val);
									break;
								}
								else {
									buf = buf.Mid(id + 1);
								}
							}
							else {
								buf.Trim();
								if (buf.GetLength() > 1) {
									ana[0][h*width][w] = (float)_ttof(val);
								}
								break;
							}
						}
					}
					if (w != (width-1)) {
							// error
							scn.MatrixRelease(mat);
							bResult = false;
							goto InversAnalizeDataFinal;
					}
					h++;
				}
				if (h != (height-1)) {
					// error
					tfd.Close();
					scn.MatrixRelease(mat);
					bResult = false;
					goto InversAnalizeDataFinal;
				}
			}
			if (scn.affine(width, height, &ana[0], width, height, &ana[1], 1, mat, false)) {
				CString tmp;
				tfd.SeekToBegin();
				for (int h = 0; h < height; h++) {
					buf.Format(_T("%d,"),(int)(ana[1][h][0]+.5));
					for (int w = 1; w < width; w++) {
						tmp.Format(_T("%s,%d"), static_cast<LPCWSTR>(buf), (int)(ana[1][h][w]+.5));
						buf = tmp;
					}
					tfd.WriteString(buf);
				}
				tfd.Close();
			}
			else {
				// error
				tfd.Close();
				scn.MatrixRelease(mat);
				bResult = false;
				goto InversAnalizeDataFinal;
			}
		}
		scn.MatrixRelease(mat);
	}
	catch (...) {
		bResult = false;
	}
#endif // 0

InversAnalizeDataFinal:
	if (ana) {
		for (int i = 0; i < 2; i++) {
			if (ana[i]) {
				for (int h = 0; h < height; h++) {
					if (ana[i][h]) {
						delete[] ana[i][h];
						ana[i][h] = nullptr;
					}
				}
				delete[] ana[i];
				ana[i] = nullptr;
			}
		}
		delete[] ana;
		ana = nullptr;
	}

	return bResult;
}

/// <summary>
/// ��̓f�[�^�̔��]�۔���
/// </summary>
/// <param name="ScanID">�ΏۃX�L����ID</param>
/// <returns>�\�̏ꍇ��true�A�s�\�̏ꍇ��false��Ԃ�</returns>
///@remark ��̓G���A�̏�Ɖ��܂��͉E�ƍ��̕����قȂ�ꍇ�͔��]�ł��Ȃ�
bool CWeldEvaluationDoc::IsInversAnalizeData(int ScanID)
{
	int width, height;
	GetScanDataSize(width, height);
	CPoint tlPos;
	CSize size;
	if (!getAnalizeArea(tlPos, size)) {
		return false;
	}

	if ((height - (tlPos.y + size.cy)) == tlPos.y) {
		if ((width - (tlPos.x + size.cx)) == tlPos.x) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}

/// <summary>
/// ��̓f�[�^�̍폜
/// </summary>
/// <param name="ScanID">�ΏۃX�L����ID</param>
void CWeldEvaluationDoc::DeleteAnalizeData(int ScanID)
{
	CString ClassificationDataFilePath, imgPaht;
	int method[] = { AnalizeTypeKMeans ,AnalizeTypeHiClustering };
	for (int metodID = 0; metodID < 2; metodID++) {
		ClassificationDataFilePath = getClassificationDataFilePath(ScanID, method[metodID]);
		if (ClassificationDataFilePath.IsEmpty()) {
			continue;
		}
		if (CFileUtil::fileExists(ClassificationDataFilePath)) {
			CFileUtil::fileDelete(ClassificationDataFilePath);
		}

		imgPaht = ClassificationDataFilePath + _T(".bmp");
		if (CFileUtil::fileExists(imgPaht)) {
			CFileUtil::fileDelete(imgPaht);
		}
	}
}

/// <summary>
/// �P�x�|RGB�ϊ�
/// </summary>
/// <param name="h">�P�x</param>
/// <param name="r">��</param>
/// <param name="g">��</param>
/// <param name="b">��</param>
void CWeldEvaluationDoc::H2RGB(int h, BYTE &r, BYTE &g, BYTE &b)
{
	double min = 0.;
	double max = 255.;
	if (h < 0) {
		h = h % 360;
		h = 360 + h;
	}
	if (h > 360) {
		h = h % 360;
	}
	double dh = (double)h;

	if (dh <= 60.) {
		r = (BYTE)max;
		g = (BYTE)((dh / 60.) * (max - min) + min);
		b = (BYTE)min;
	}
	else if (dh <= 120.) {
		r = (BYTE)(((120. - dh) / 60.) * (max - min) + min);
		g = (BYTE)max;
		b = (BYTE)min;
	}
	else if (dh <= 180.) {
		r = (BYTE)min;
		g = (BYTE)max;
		b = (BYTE)(((dh -120.) / 60.) * (max - min) + min);
	}
	else if (dh <= 240.) {
		r = (BYTE)min;
		g = (BYTE)(((240. - dh) / 60.) * (max - min) + min);
		b = (BYTE)max;
	}
	else if (dh <= 300.) {
		r = (BYTE)(((dh - 240.) / 60.) * (max - min) + min);
		g = (BYTE)min;
		b = (BYTE)max;
	}
	else if (dh <= 360.) {
		r = (BYTE)max;
		g = (BYTE)min;
		b = (BYTE)(((360. - dh) / 60.) * (max - min) + min);
	}
}

/// <summary>
/// �N���X�ɑΉ�����F�̎擾
/// </summary>
/// <param name="id">�ΏۃN���XID</param>
/// <param name="nClass">�N���X��</param>
/// <returns>�����̏ꍇ�͑Ή��F��Ԃ�</returns>
COLORREF CWeldEvaluationDoc::GetClassColor(int id, int nClass)
{
	COLORREF col;
	unsigned char r, g, b;
	int h = (int)((double)id * (240.0 / (double)((__int64)nClass - 1) + 0.5));
	H2RGB(h, r, g, b);
	col = RGB(r, g, b);
	return col;
}

/// <summary>
/// ���ތ��ʂ̉摜�f�[�^�擾
/// </summary>
/// <param name="targetID">�ΏۃX�L����ID</param>
/// <param name="method">���ގ��ID</param>
/// <param name="img">�摜�f�[�^</param>
/// <param name="renew">�ēǍ��t���O</param>
/// <returns>���݂���ꍇ�̓p�X��Ԃ��A���s�����ꍇ�͋󕶎���Ԃ�</returns>
bool CWeldEvaluationDoc::LoadClassificationResultImage(int targetID, int method, CImage &img, bool renew/* = false*/)
{
	bool bResult = true;
	CString root, name;
	CString classfile, imagefile;
	int nClass = 0;

	classfile = getClassificationDataFilePath(targetID, method);
	if (!CFileUtil::fileExists(classfile)) {
		CString msg;
		msg.Format(_T("��͌��ʃt�@�C�������݂��܂���B(%s)"), static_cast<LPCWSTR>(classfile));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}
	imagefile = getClassificationImageFilePath(targetID, method);
	if ((renew) || (!CFileUtil::fileExists(imagefile))) {
		vector<int> data;
		if (!CWeldEvaluationDoc::getResultFile(classfile, data)) {
			CString msg;
			msg.Format(_T("��͌��ʃt�@�C���̓ǂݍ��݂Ɏ��s���܂����B(%s)"), static_cast<LPCWSTR>(classfile));
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
			return false;
		}
		else {
			switch (targetID) {
			case	eResinSurface:	///< ����
				nClass = m_PropatyIO.ResinGetNumberOfClass(method);
				break;
			case	eMetalSurface:	///< ����
				nClass = m_PropatyIO.MetalGetNumberOfClass(method);
				break;
			case	eJoiningResult:	///< �ڍ�����
				nClass = m_PropatyIO.ResultGetNumberOfClass(method);
				break;
			default:
				{
					CString msg;
					msg.Format(_T("����`�̃X�L����ID���ݒ肳��܂����B(%d)"), targetID);
					writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
				}
				return false;
			}

			int Bpp = 3;
#if 0
			int width = m_PropatyIO.GetHorizontalResolution();
			int height = m_PropatyIO.GetVerticalResolution();
#else
			int width, height;
			GetScanDataSize(width, height);
#endif
			if ((int)data.size() != (width * height)) {
				CString msg;
				msg.Format(_T("�f�[�^�T�C�Y���ُ�ł��B(size=%d)"), (int)data.size());
				writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
				return false;
			}
			// �C���[�W�̉��T�C�Y�o�E���_������
			int imageWidth = width;
			if ((imageWidth % 8) != 0) {
				imageWidth = (int)(imageWidth / 8) * 8 + 8;
			}
			unsigned char * p24Img = new unsigned char[(__int64)imageWidth * (__int64)height * Bpp];
			BYTE *ptr = p24Img;
			COLORREF *col = new COLORREF[nClass];
			for (int id = 0; id < nClass; id++) {
				col[id] = GetClassColor(id, nClass);
			}

			switch (targetID) {
			case	eResinSurface:	///< ����
				for (int id = 0; id < nClass; id++) {
					ResinSetJointColor(method,id, col[id]);
				}
				break;
			case	eMetalSurface:	///< ����
				for (int id = 0; id < nClass; id++) {
					MetalSetJointColor(method,id, col[id]);
				}
				break;
			case	eJoiningResult:	///< �ڍ�����
				for (int id = 0; id < nClass; id++) {
					ResultSetJointColor(method,id, col[id]);
				}
				break;
			}

			int id = 0;
			for (int y = 0; y < height; y++) {
				ptr = p24Img + (__int64)((__int64)y*imageWidth)*Bpp;
				for (int x = 0; x < width; x++) {
					id = (y*width) + x;
					if (data[id] < 0) {
						*(ptr++) = 0;
						*(ptr++) = 0;
						*(ptr++) = 0;
					}
					else {
						*(ptr++) = GetBValue(col[data[id]]);
						*(ptr++) = GetGValue(col[data[id]]);
						*(ptr++) = GetRValue(col[data[id]]);
					}
				}
				for (int x = width; x < imageWidth; x++) {
					*(ptr++) = 0;
					*(ptr++) = 0;
					*(ptr++) = 0;
				}
			}

			if (col) {
				delete[] col;
				col = nullptr;
			}

			BITMAPINFOHEADER    bmInfohdr;
			// Create the header info
			bmInfohdr.biSize = sizeof(BITMAPINFOHEADER);
			bmInfohdr.biWidth = imageWidth;
			bmInfohdr.biHeight = -height;
			bmInfohdr.biPlanes = 1;
			bmInfohdr.biBitCount = (WORD)(Bpp * 8);
			bmInfohdr.biCompression = BI_RGB;
			bmInfohdr.biSizeImage = imageWidth * height * Bpp;
			bmInfohdr.biXPelsPerMeter = 0;
			bmInfohdr.biYPelsPerMeter = 0;
			bmInfohdr.biClrUsed = 0;
			bmInfohdr.biClrImportant = 0;

			BITMAPINFO bmInfo;
			bmInfo.bmiHeader = bmInfohdr;
			bmInfo.bmiColors[0].rgbBlue = 255;

			if (img.Create(imageWidth, height, 8 * Bpp, NULL)) {
				HDC dc = img.GetDC();
				SetDIBitsToDevice(dc, 0, 0, imageWidth, height, 0, 0, 0, height, p24Img, &bmInfo, DIB_RGB_COLORS);
				img.ReleaseDC();
				img.Save(imagefile);
			}
			else {
				DWORD err = GetLastError();
				LPVOID lpMsgBuf;
				FormatMessage(
					FORMAT_MESSAGE_ALLOCATE_BUFFER							// �e�L�X�g�̃��������蓖�Ă�v������
					| FORMAT_MESSAGE_FROM_SYSTEM							// �G���[���b�Z�[�W��Windows���p�ӂ��Ă�����̂��g�p
					| FORMAT_MESSAGE_IGNORE_INSERTS,						// ���̈����𖳎����ăG���[�R�[�h�ɑ΂���G���[���b�Z�[�W���쐬����
					NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),	// ������w��
					(LPTSTR)&lpMsgBuf,										// ���b�Z�[�W�e�L�X�g���ۑ������o�b�t�@�ւ̃|�C���^
					0,
					NULL);
//				AfxMessageBox((LPCTSTR)lpMsgBuf, MB_OK | MB_ICONINFORMATION);
				LocalFree(lpMsgBuf);

				CString msg,fmt;
				if (!fmt.LoadString(IDM_ERR_FAILDCREATEIMAGE)) {
					fmt = _T("�摜�̍쐬�Ɏ��s���܂����B:%s");
				}
				msg.Format(fmt, static_cast<LPCWSTR>(lpMsgBuf));
				writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
				bResult = false;
			}

			if (p24Img) {
				delete[] p24Img;
				p24Img = nullptr;
			}
		}
	}
	else {
		HRESULT ret;
		ret = img.Load(imagefile);
		if (FAILED(ret)) {
			CString msg;
			msg.Format(CString(_T("�摜�̓ǂݍ��݂Ɏ��s���܂����B(%s)")), static_cast<LPCWSTR>(imagefile));
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
			return false;
		}
	}

	return bResult;
}

/// <summary>
/// �w��ʒu�X�y�N�g���Q�̎擾
/// </summary>
/// <param name="ScanID">�ΏۃX�L����ID</param>
/// <param name="pos">�X�y�N�g���擾�ʒu</param>
/// <param name="data">�X�y�N�g���Q</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::GetSpectrumData(int ScanID, CPoint &pos, std::vector<double> &data)
{
	CString root, name;
	CString scanfile, imagefile;

	root = GetWorkProjectPath();
	switch (ScanID) {
	case	eResinSurface:	// ����
		if (!m_ResinScanData.GetSpectrumData(pos, data)) {
			CString msg;
			msg.Format(_T("�����ʂ̎w��ʒu�X�y�N�g���Q�̎擾�Ɏ��s���܂����B(�X�L����ID=%d�A�X�y�N�g���擾�ʒu=(%d,%d))"), ScanID, pos.x, pos.y);
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
			return false;
		}
		break;
	case	eMetalSurface:	// ����
		if (!m_MetalScanData.GetSpectrumData(pos, data)) {
			CString msg;
			msg.Format(_T("�����ʂ̎w��ʒu�X�y�N�g���Q�̎擾�Ɏ��s���܂����B(�X�L����ID=%d�A�X�y�N�g���擾�ʒu=(%d,%d))"), ScanID, pos.x, pos.y);
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
			return false;
		}
		break;
	case	eJoiningResult:	// �ڍ�����
		if (!m_ResultScanData.GetSpectrumData(pos, data)) {
			CString msg;
			msg.Format(_T("�ڍ����ʂ̎w��ʒu�X�y�N�g���Q�̎擾�Ɏ��s���܂����B(�X�L����ID=%d�A�X�y�N�g���擾�ʒu=(%d,%d))"), ScanID, pos.x, pos.y);
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
			return false;
		}
		break;
	default:
		{
			CString msg;
			msg.Format(_T("�s���̃X�L����ID���w�肳��Ă��܂�(ScanID=%d)"), ScanID);
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		}
		return false;
	}
	return true;
}

/// <summary>
/// �X�y�N�g�������W�̎擾
/// </summary>
/// <param name="ScanID">�ΏۃX�L����ID</param>
/// <param name="min">�ŏ��g�`�l[nm]</param>
/// <param name="max">�ő�g�`�l[nm]</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::GetSpectrumRange(int ScanID, double &min, double &max)
{
	int nBand = 0;
	switch (ScanID) {
	case	eResinSurface:	// ����
		nBand = m_ResinScanData.GetNumberOfBand();
		min = m_ResinScanData.getWaveLength(0);
		max = m_ResinScanData.getWaveLength(nBand - 1);
		break;
	case	eMetalSurface:	// ����
		nBand = m_MetalScanData.GetNumberOfBand();
		min = m_MetalScanData.getWaveLength(0);
		max = m_MetalScanData.getWaveLength(nBand - 1);
		break;
	case	eJoiningResult:	// �ڍ�����
		nBand = m_ResultScanData.GetNumberOfBand();
		min = m_ResultScanData.getWaveLength(0);
		max = m_ResultScanData.getWaveLength(nBand-1);
		break;
	default:
		return false;
	}
	return true;
}

/// <summary>
/// �X�L�����C���[�W�̕ۑ�
/// </summary>
/// <param name="writePath">�ۑ��p�X</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::WriteImage(CString writePath)
{
	int ScanID[3] = { eResinSurface ,eMetalSurface , eJoiningResult};
	int method[2] = { AnalizeTypeKMeans, AnalizeTypeHiClustering };

	bool bResult = true;
	CString drive, dir, fname, ext;
	CString path,classfile, outPath;
	bool bexist = true;
	for (int ID = 0; ID < 3; ID++) {
		if (ExistScanFile(ScanID[ID])) {
			bexist = true;
			path = getScanImageFilePath(ScanID[ID]);
			if (!CFileUtil::fileExists(path)) {
				CImage img;
				if (!LoadScanImage(ScanID[ID], img)) {
					bexist = false;
				}
				else {
					HRESULT iret = img.Save(path);
					if (iret != 0) {
						bexist = false;
					}
				}
			}
			if (bexist) {
				if (CFileUtil::splitPath(path, drive, dir, fname, ext)) {
					fname.Format(_T("%d_%s%s"), ScanID[ID], (LPCTSTR)fname, (LPCTSTR)ext);
					outPath = CFileUtil::FilePathCombine(writePath, fname);
					if (!CopyFile(path, outPath, true)) {
						bResult = false;
					}
				}
				else {
					bResult = false;
				}
			}

			for (int methodID = 0; methodID < 2; methodID++) {
				// ���ʉ摜�͉�͂������̂̂ݕ\��
				classfile = getClassificationDataFilePath(ScanID[ID], method[methodID]);
				if (CFileUtil::fileExists(classfile)) {
					path = getClassificationImageFilePath(ScanID[ID], method[methodID]);
					bexist = true;
					if (!CFileUtil::fileExists(path)) {
						CImage img;
						if (!LoadClassificationResultImage(ScanID[ID], method[methodID], img)) {
							bexist = false;
						}
						else {
							HRESULT iret = img.Save(path);
							if (iret != 0) {
								bexist = false;
							}
						}
					}

					if (bexist) {
						if (CFileUtil::splitPath(path, drive, dir, fname, ext)) {
							fname.Format(_T("%d_%d_%s%s"), ScanID[ID], method[methodID], (LPCTSTR)fname, (LPCTSTR)ext);
							outPath = CFileUtil::FilePathCombine(writePath, fname);
							if (!CopyFile(path, outPath, true)) {
								bResult = false;
							}
						}
						else {
							bResult = false;
						}
					}
				}
			}
		}
	}
	if (!bResult) {
		CString msg;
		msg.Format(_T("�X�L�����C���[�W�̕ۑ��Ɏ��s���܂����B(%s)"), static_cast<LPCWSTR>(writePath));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
	}
	return bResult;
}


//////////////////////////////////////////////////////////////////////////////////////////
// �v���W�F�N�g���[�N�t�@�C������

/// <summary>
/// ���[�N�v���W�F�N�g���̎擾
/// </summary>
/// <returns>���[�N�v���W�F�N�g����Ԃ�</returns>
CString CWeldEvaluationDoc::GetWorkProjectFolderName()
{
	CConfigrationIO sys(m_SystemFilePathName);
	CString folder = sys.getString(_T("Common"), _T("ProjectWorkFolderName"));
	if (folder.IsEmpty()) {
		folder = m_ModulePath + _T("__ProjectWork__");
	}
	return folder;
}

/// <summary>
/// ���[�N�v���W�F�N�g�̃��[�g�p�X�擾
/// </summary>
/// <returns>���[�N�v���W�F�N�g�̃��[�g�p�X��Ԃ�</returns>
CString CWeldEvaluationDoc::GetWorkProjectFolderPath()
{
	CString folderName = GetWorkProjectFolderName();
	CConfigrationIO sys(m_SystemFilePathName);
	CString folder = sys.getString(_T("Common"), _T("ProjectWorkPath"));
	if (folder.IsEmpty()) {
		folder = m_ModulePath + _T("__ProjectWork__");
	}
	CString path = CFileUtil::FilePathCombine(folder, folderName);
	return path;
}

/// <summary>
/// �v���W�F�N�g�X�e�[�^�X�ݒ�
/// </summary>
/// <param name="WKPrjPath">���[�N�v���W�F�N�g�ւ̃p�X</param>
/// <param name="key">�X�e�[�^�X�L�[</param>
/// <param name="value">�X�e�[�^�X�L�[�ɑΉ�����l</param>
/// <returns>�����ꍇ��true�A���s�ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::SetWorkProjectStatus(CString WKPrjPath, CString key, CString value)
{
	CString stsPathName = CFileUtil::FilePathCombine(WKPrjPath, StatysFileName);
	CConfigrationIO sys(stsPathName);
	return sys.setString(_T("Status"), key, value);
}

/// <summary>
/// �v���W�F�N�g�X�e�[�^�X�擾
/// </summary>
/// <param name="WKPrjPath">���[�N�v���W�F�N�g�ւ̃p�X</param>
/// <param name="key">�X�e�[�^�X�L�[</param>
/// <returns>�����ꍇ�̓L�[�ɑΉ�����l��Ԃ�</returns>
CString CWeldEvaluationDoc::GetWorkProjectStatus(CString WKPrjPath, CString key)
{
	CString stsPathName = CFileUtil::FilePathCombine(WKPrjPath, StatysFileName);
	CConfigrationIO sys(stsPathName);
	CString val = sys.getString(_T("Status"), key);
	return val;
}

/// <summary>
/// ���[�N�v���W�F�N�g�p�X�̎擾
/// </summary>
/// <returns>���[�N�v���W�F�N�g�p�X��Ԃ�</returns>
CString CWeldEvaluationDoc::GetWorkProjectPath()
{
	CString rootPath = GetWorkProjectFolderPath();
	CString prjName = GetWorkProjectStatus(rootPath, _T("ProjectName"));
	if (prjName.IsEmpty()) {
		prjName = NewProjectFolder;
	}
	return CFileUtil::FilePathCombine(rootPath, prjName);
}

/// <summary>
/// �v���W�F�N�g�̍X�V����
/// </summary>
/// <returns>�X�V������ꍇ��true�A�����ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::IsWorkProjectUpdated()
{
	CString rootPath = GetWorkProjectFolderPath();
	CString status = GetWorkProjectStatus(rootPath, _T("Update"));
	if (status.MakeUpper() == _T("TRUE")) {
		return true;
	}
	else {
		return false;
	}
}

/// <summary>
/// �v���W�F�N�g�̍X�V�X�e�[�^�X�ݒ�
/// </summary>
/// <param name="bUpdate">�X�V�t���O</param>
/// <returns>�����ꍇ��true�A���s�ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::SetWorkProjectUpdteStatus(bool bUpdate)
{
	CString rootPath = GetWorkProjectFolderPath();
	CString status;
	if (bUpdate) {
		status = _T("TRUE");
	}
	else {
		status = _T("FALSE");
	}
	return SetWorkProjectStatus(rootPath, _T("Update"), status);
}

/// <summary>
/// ���[�N�v���W�F�N�g�̍폜
/// </summary>
void CWeldEvaluationDoc::ClrWorkProject()
{
	CString folder = GetWorkProjectFolderPath();
	if (CFileUtil::fileExists(folder)) {
		CFileUtil::fileDeleteEx(folder);
	}
}

/// <summary>
/// �v���W�F�N�g�����[�N�ֈړ�
/// </summary>
/// <param name="ResistPath">�ړ����v���W�F�N�g�̃��[�g�p�X</param>
/// <param name="ProjentName">�v���W�F�N�g��</param>
/// <returns>�����ꍇ��true�A���s�ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::PushProject(CString ResistPath, CString ProjentName)
{
	CString rootPath = GetWorkProjectFolderPath();
	if (!CFileUtil::fileExists(rootPath)) {
		if (!CFileUtil::MakeDir(rootPath)) {
			CString msg;
			msg.Format(_T("�f�B���N�g���̍쐬�Ɏ��s���܂����B(%s)"), static_cast<LPCWSTR>(rootPath));
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
			return false;
		}
	}
	if (ProjentName.IsEmpty()) {
		// �V�K�v���W�F�N�g
		SetWorkProjectStatus(rootPath, _T("ProjectName"), NewProjectFolder);
		SetWorkProjectStatus(rootPath, _T("NewProject"), _T("TRUE"));
	}
	else {
		// �����v���W�F�N�g
		SetWorkProjectStatus(rootPath, _T("ProjectName"), ProjentName);
		SetWorkProjectStatus(rootPath, _T("NewProject"), _T("FALSE"));
		CString srcPath = CFileUtil::FilePathCombine(ResistPath, ProjentName);
		CString dstPath = GetWorkProjectPath();
		if (!CFileUtil::Copy(srcPath, dstPath, false)) {
			ClrWorkProject();
			CString msg;
			msg.Format(_T("�v���W�F�N�g���烏�[�N�ֈړ��Ɏ��s���܂����B(�v���W�F�N�g[%s]�A���[�N[%s])"), static_cast<LPCWSTR>(rootPath), static_cast<LPCWSTR>(dstPath));
			writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
			return false;
		}
	}
	SetWorkProjectUpdteStatus(false);

	return true;
}

/// <summary>
/// ���[�N����v���W�F�N�g�ֈړ�
/// </summary>
/// <param name="ResistPath">�ړ���v���W�F�N�g�̃��[�g�p�X</param>
/// <param name="ProjentName">�v���W�F�N�g��</param>
/// <returns>�����ꍇ��true�A���s�ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::PopProject(CString ResistPath, CString ProjentName)
{
	CString rootPath = GetWorkProjectFolderPath();
	CString srcPath = GetWorkProjectPath();
	CString dstPath = CFileUtil::FilePathCombine(ResistPath, ProjentName);
	SetWorkProjectStatus(rootPath, _T("ProjectName"), ProjentName);
	SetWorkProjectStatus(rootPath, _T("NewProject"), _T("FALSE"));
	SetWorkProjectUpdteStatus(false);

	bool bBackup = false;
	CString bak = dstPath + _T(".bak__");
	if (CFileUtil::fileExists(dstPath)) {
		CFile::Rename(dstPath, bak);
		bBackup = true;
	}

	if (!CFileUtil::Copy(srcPath, dstPath, true)) {
		if (bBackup) {
			CFile::Rename(bak, dstPath);
		}
		CString msg;
		msg.Format(_T("���[�N����v���W�F�N�g�ֈړ��Ɏ��s���܂����B(���[�N[%s]�A�v���W�F�N�g[%s])"), static_cast<LPCWSTR>(srcPath), static_cast<LPCWSTR>(dstPath));
		writeLog(CLog::Error, CString(__FILE__), __LINE__, msg);
		return false;
	}
	if (bBackup) {
		CFileUtil::fileDeleteEx(bak);
	}

	CString ParmaterFileName;
	if (!ParmaterFileName.LoadString(IDS_PROPATYFILENAME)) {
		ParmaterFileName = _T("parameter.inf");
	}
	CString projectFileName;
	if (!projectFileName.LoadString(IDS_PROJECTFILENAME)) {
		projectFileName = _T("WeldEvalution.prj");
	}

	CString PropatyFilePath = CFileUtil::FilePathCombine(ResistPath, ProjentName);
	PropatyFilePath = CFileUtil::FilePathCombine(PropatyFilePath, ParmaterFileName);
	CPropatyIO::WriteProjectName(PropatyFilePath, projectFileName);
	CPropatyIO::WriteTestName(PropatyFilePath, ProjentName);

	m_ActiveRegisttedTestFolder = GetWorkProjectPath();
	m_ParamaterFilePaht = CFileUtil::FilePathCombine(m_ActiveRegisttedTestFolder, ParmaterFileName);
	m_ProjectFilePaht = CFileUtil::FilePathCombine(m_ActiveRegisttedTestFolder, projectFileName);

	m_PropatyIO.SetParamaterFilePath(m_ParamaterFilePaht);
	m_ProjectIO.SetProjectFilePath(m_ProjectFilePaht);

	dstPath = CFileUtil::FilePathCombine(rootPath, ProjentName);
	CFile::Rename(srcPath, dstPath);

	return true;
}

/// <summary>
/// �X�L�������̍폜
/// </summary>
/// <param name="ScanID">�ΏۃX�L����ID</param>
void CWeldEvaluationDoc::DeleteContents(int ScanID)
{
	CString path, imgPaht;
	path = getScanDataPath(ScanID);
	CString headerPath = path + _T(".hdr");
	if (CFileUtil::fileExists(headerPath)) {
		CFileUtil::fileDelete(headerPath);
	}
	CString rawPath = path + _T(".raw");
	if (CFileUtil::fileExists(rawPath)) {
		CFileUtil::fileDelete(rawPath);
	}
	imgPaht = GetScanImagePath(ScanID);
	if (CFileUtil::fileExists(imgPaht)) {
		CFileUtil::fileDelete(imgPaht);
	}
	path = GetClassificationResultPath(ScanID, AnalizeTypeKMeans);
	if (CFileUtil::fileExists(path)) {
		CFileUtil::fileDelete(path);
	}
	imgPaht = path + _T(".bmp");
	if (CFileUtil::fileExists(imgPaht)) {
		CFileUtil::fileDelete(imgPaht);
	}
	path = GetClassificationResultPath(ScanID, AnalizeTypeHiClustering);
	if (CFileUtil::fileExists(path)) {
		CFileUtil::fileDelete(path);
	}
	imgPaht = path + _T(".bmp");
	if (CFileUtil::fileExists(imgPaht)) {
		CFileUtil::fileDelete(imgPaht);
	}

	CConfigrationIO sys(m_SystemFilePathName);
	double dval;
	int uval;
	COLORREF color = RGB(0, 0, 0);
	switch (ScanID) {
	case	eResinSurface:	///< ����
		for (int method = 0; method < 2; method++) {
			uval = m_PropatyIO.ResinGetNumberOfClass(method);
			for (int i = 0; i < (int)uval; i++) {
				dval = sys.getDouble(_T("ParamDefault"), _T("ResinSurface_Joining_ratio"));
				m_PropatyIO.ResinSetJointRetio(method, i, dval);
				m_PropatyIO.ResinSetJointColor(method, i,color);
			}
		}
		break;
	case	eMetalSurface:	///< ����
		for (int method = 0; method < 2; method++) {
			uval = m_PropatyIO.MetalGetNumberOfClass(method);
			for (int i = 0; i < (int)uval; i++) {
				dval = sys.getDouble(_T("ParamDefault"), _T("ResinSurface_Joining_ratio"));
				m_PropatyIO.MetalSetJointRetio(method, i, dval);
				m_PropatyIO.MetalSetJointColor(method, i, color);
			}
		}
		break;
	case	eJoiningResult:	///< �ڍ�����
		for (int method = 0; method < 2; method++) {
			uval = m_PropatyIO.ResultGetNumberOfClass(method);
			for (int i = 0; i < (int)uval; i++) {
				dval = sys.getDouble(_T("ParamDefault"), _T("ResinSurface_Joining_ratio"));
				m_PropatyIO.ResultSetJointRetio(method, i, dval);
				m_PropatyIO.ResultSetJointColor(method, i, color);
			}
		}
		break;
	}
}

/// <summary>
/// �z���C�g�o�����X�t�@�C���̍폜
/// </summary>
void CWeldEvaluationDoc::DeleteWBFile(CString title/* = _T("")*/)
{
	CString registedFolde = GetRegistedFolder();
	CString WBFileName;
	if (title.IsEmpty()) {
		WBFileName = GetWBFileName();
	}
	else {
		WBFileName = title;
	}
	CString WBFileNameHdr = WBFileName + _T(".hdr");
	CString WBFileNameRaw = WBFileName + _T(".raw");
	CString path;
	path = CFileUtil::FilePathCombine(registedFolde, WBFileNameHdr);
	if (CFileUtil::fileExists(path)) {
		CFileUtil::fileDelete(path);
	}
	path = CFileUtil::FilePathCombine(registedFolde, WBFileNameRaw);
	if (CFileUtil::fileExists(path)) {
		CFileUtil::fileDelete(path);
	}
}

/// <summary>
/// �v���W�F�N�g�̍폜
/// </summary>
/// <param name="ProjentName">�v���W�F�N�g��</param>
/// <returns>�����ꍇ��true�A���s�ꍇ��false��Ԃ�</returns>
bool CWeldEvaluationDoc::DeleteProject(CString ProjentName)
{
	bool bResult = true;
	CString rootPath = GetWorkProjectFolderPath();
	CString workPath = CFileUtil::FilePathCombine(rootPath, ProjentName);

	CString resistFolder = GetRegistedFolder();
	CString dstPath = CFileUtil::FilePathCombine(resistFolder, ProjentName);
	if (CFileUtil::fileExists(dstPath)) {
		if (CFileUtil::fileDeleteEx(dstPath)) {
			if (CFileUtil::fileExists(workPath)) {
				if (CFileUtil::fileDeleteEx(workPath)) {
					if (!NewProject()) {
						bResult = false;
					}
				}
				else {
					bResult = false;
				}
			}
		}
		else {
			bResult = false;
		}
	}
	return bResult;
}
