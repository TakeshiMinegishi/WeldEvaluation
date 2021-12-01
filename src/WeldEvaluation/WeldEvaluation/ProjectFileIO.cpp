#include "stdafx.h"
#include "ProjectFileIO.h"
#include "ConfigrationIO.h"
#include "FileUtil.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
CProjectFileIO::CProjectFileIO(void)
{
	m_ProjectFilePath = _T("");///< �v���W�F�N�g�t�@�C���ւ̃p�X
	Initialze();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
CProjectFileIO::~CProjectFileIO(void)
{
}

/// <summary>
/// �v���W�F�N�g�t�@�C���p�X�̎擾
/// </summary>
/// <returns>�v���W�F�N�g�t�@�C���p�X��Ԃ�</returns>
CString CProjectFileIO::GetProjectFilePath()
{
	return m_ProjectFilePath;
}

/// <summary>
/// �v���W�F�N�g�t�@�C���p�X�̐ݒ�
/// </summary>
/// <param name="path">�v���W�F�N�g�t�@�C���p�X</param>
/// <returns>�����ꍇ��true�A���s�ꍇ��false��Ԃ�</returns>
bool CProjectFileIO::SetProjectFilePath(CString path)
{
	m_ProjectFilePath = path;
	return true;
}

/*
/// <summary>
/// �p�����[�^�t�@�C�����̍쐬
/// </summary>
/// <param name="TestName">����</param>
/// <param name="date">���t</param>
/// <param name="number">�T�u�ԍ�</param>
/// <returns>�p�����[�^�t�@�C������Ԃ�</returns>
CString CProjectFileIO::MakeProjectName(CString TestName, COleDateTime date, int number)
{
	CString name;
	name.Format(_T("%s%4d%02d%02d-%d"),TestName,date.GetYear(),date.GetMonth(),date.GetDay(),number);
	return name;
}

/// <summary>
/// �v���W�F�N�g���̎擾
/// </summary>
/// <returns>�v���W�F�N�g����Ԃ�</returns>
CString CProjectFileIO::GetProjectName()
{
	if (m_ProjectFilePath.IsEmpty()) {
		return _T("");
	}
	CConfigrationIO sys(m_ProjectFilePath);
	return sys.getString(_T("Common"),_T("project_name"));
}

/// <summary>
/// �v���W�F�N�g���̐ݒ�
/// </summary>
/// <param name="projectname">�v���W�F�N�g��</param>
/// <returns>�����ꍇ��true�A���s�ꍇ��false��Ԃ�</returns>
bool CProjectFileIO::SetProjectName(CString projectname)
{
	if (m_ProjectFilePath.IsEmpty()) {
		return false;
	}
	CConfigrationIO sys(m_ProjectFilePath);
	return sys.setString(_T("Common"),_T("project_name"),projectname);
}
*/

/// <summary>
/// �o�[�W�����̎擾�̎擾
/// </summary>
/// <returns>�o�[�W������Ԃ�</returns>
CString CProjectFileIO::GetVersion()
{
	return m_Version;
}

/// <summary>
/// �o�[�W�����̐ݒ�
/// </summary>
/// <param name="version">�o�[�W����</param>
/// <returns>�����ꍇ��true�A���s�ꍇ��false��Ԃ�</returns>
bool CProjectFileIO::SetVersion(CString version)
{
	m_Version = version;
	return true;
}

/// <summary>
/// �v���W�F�N�g�̏�����
/// </summary>
void CProjectFileIO::Initialze()
{
//	m_ProjectFilePath = _T("");///< �v���W�F�N�g�t�@�C���ւ̃p�X

	m_CreeateDay = COleDateTime(1960,11,18,0,0,0);				///< �쐬��
	m_UpdateDay = COleDateTime(1960,11,18,0,0,0);				///< �X�V��
	m_Version = _T("");
	m_ImageDataRootPath								= _T("");	///< �C���[�W�f�[�^�ւ̃��[�g�p�X

	m_ResinScanImageFile							= _T("");	///< �����ʃX�L�����摜�t�@�C����
	m_ResinDendrogramClassFile						= _T("");	///< �����ʊK�w�N���X�^�����O���ތ��ʃt�@�C����
	m_ResinKmeansClassFile							= _T("");	///< ������k-means���ތ��ʃt�@�C����

	m_MetalScanImageFile							= _T("");	///< �����ʃX�L�����摜�t�@�C����
	m_MetalDendrogramClassFile						= _T("");	///< �����ʊK�w�N���X�^�����O���ތ��ʃt�@�C����
	m_MetalKmeansClassFile							= _T("");	///< ������k-means���ތ��ʃt�@�C����

	m_ResultScanImageFile							= _T("");	///< �ڍ����ʉ摜�X�L�����摜�t�@�C����
	m_ResultDendrogramClassFile						= _T("");	///< �ڍ����ʊK�w�N���X�^�����O���ތ��ʃt�@�C����
	m_ResultKmeansClassFile							= _T("");	///< �ڍ�����k-means���ތ��ʃt�@�C����

	m_ResinActiveAnalizeMethod						= 0;		///< �����ʂ̕\���Ώۉ�͕��@
	m_MetalActiveAnalizeMethod						= 0;		///< �����ʂ̕\���Ώۉ�͕��@
	m_ResultActiveAnalizeMethod						= 0;		///< ���ʂ̕\���Ώۉ�͕��@

	m_nResinDendrogramClassificationClass			= 0;		///< �����ʕ��ތ��ʊK�w�N���X�^�����O���ސ�
	m_nResinKMeansClassificationClass				= 0;		///< �����ʕ��ތ���k-means���ސ�
	m_nMetalDendrogramClassificationClass			= 0;		///< �����ʕ��ތ��ʊK�w�N���X�^�����O���ސ�
	m_nMetalKMeansClassificationClass				= 0;		///< �����ʕ��ތ���k-means���ސ�
	m_nResultDendrogramClassificationClass			= 0;		///< ���ʊK�w�N���X�^�����O���ސ�
	m_nResultMeansClassificationClass				= 0;		///< ���ʊK�w�N���X�^�����O���ސ�

	m_SpectralGraphPointTarget						= 0;		///< �X�y�N�g���O���t�i�_�w��j�̑Ώ�
	m_SpectralGraphPointHolizontalPosition			= 0;		///< �X�y�N�g���O���t�i�_�w��j�̐����ʒu
	m_SpectralGraphPointVerticalPosition			= 0;		///< �X�y�N�g���O���t�i�_�w��j�̐����ʒu

	m_SpectralGraphSectionTarget					= 0;		///< �X�y�N�g���O���t�i��Ԏw��j�̑Ώ�
	m_SpectralGraphSectionHolizontalStartPosition	= 0;		///< �X�y�N�g���O���t�i�_�w��j�̐����J�n�ʒu
	m_SpectralGraphSectionVerticalStartPosition		= 0;		///< �X�y�N�g���O���t�i�_�w��j�̐����J�n�ʒu
	m_SpectralGraphSectionHolizontalEndPosition		= 0;		///< �X�y�N�g���O���t�i�_�w��j�̐����I���ʒu
	m_SpectralGraphSectionVerticalEndPosition		= 0;		///< �X�y�N�g���O���t�i�_�w��j�̐����I���ʒu

	m_ScanDataHolizontalSize						= 0;
	m_ScanDataVerticalSize							= 0;

}


/// <summary>
/// �����񂩂���t�ւ̕ϊ�
/// </summary>
/// <param name="str">���t��\��������</param>
/// <param name="date">���t</param>
/// <returns>�����ꍇ��true�A���s�ꍇ��false��Ԃ�</returns>
bool CProjectFileIO::Str2Date(CString str,COleDateTime &date)
{
	int y,m,d;
	int pos = str.FindOneOf(_T("/"));
	if (pos > 0) {
		CString s = str.Left(pos);
		y = _ttoi(s);
		str = str.Mid(pos+1);
		pos = str.FindOneOf(_T("/"));
		if (pos > 0) {
			s = str.Left(pos);
			m = _ttoi(s);
			str = str.Mid(pos+1);
			d = _ttoi(str);
		} else {
			return false;
		}
	} else {
		return false;
	}

	date.SetDate(y,m,d);
	return true;
}

/// <summary>
/// ���t���當����ւ̕ϊ�
/// </summary>
/// <param name="date">���t</param>
/// <returns>���t��\���������Ԃ�</returns>
CString CProjectFileIO::Date2Str(COleDateTime date)
{
	CString str;
	str.Format(_T("%4d/%02d/%02d"),date.GetYear(),date.GetMonth(),date.GetDay());
	return str;
}

/// <summary>
/// �v���W�F�N�g�̓ǂݍ���
/// </summary>
/// <param name="path">�v���W�F�N�g�t�@�C���p�X��</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CProjectFileIO::read(CString path)
{
	CConfigrationIO sys(path);
	CString str;

	m_Version = sys.getString(_T("Common"), _T("Version"));

	// �쐬��
	str = sys.getString(_T("Common"),_T("create_date"));
	if (!str.IsEmpty()) {
		if (!Str2Date(str,m_CreeateDay)) {
			CFileUtil::GetFileCreateDay(m_ProjectFilePath,m_CreeateDay);
		}
	} else {
		CFileUtil::GetFileCreateDay(m_ProjectFilePath,m_CreeateDay);
	}
	// �X�V��
	str = sys.getString(_T("Common"),_T("update_date"));
	if (!str.IsEmpty()) {
		if (!Str2Date(str,m_UpdateDay)) {
			CFileUtil::GetFileCreateDay(m_ProjectFilePath,m_UpdateDay);
		}
	} else {
		CFileUtil::GetFileCreateDay(m_ProjectFilePath,m_UpdateDay);
	}

	m_ImageDataRootPath = sys.getString(_T("Common"),_T("image_data_root_path"));							// �C���[�W�f�[�^�ւ̃��[�g�p�X

	m_ResinScanImageFile = sys.getString(_T("ResinSurface"),_T("scan_image_file"));							// �����ʃX�L�����摜�t�@�C����
	m_ResinDendrogramClassFile = sys.getString(_T("ResinSurface"),_T("dendrogram_classification"));			// �����ʕ��ތ��ʊK�w�N���X�^�����O�t�@�C����
	m_ResinKmeansClassFile = sys.getString(_T("ResinSurface"), _T("kmeans_classification"));				// �����ʕ��ތ���k-means�t�@�C����

	m_MetalScanImageFile = sys.getString(_T("MetalSurface"),_T("scan_image_file"));							// �����ʃX�L�����摜�t�@�C����
	m_MetalDendrogramClassFile = sys.getString(_T("MetalSurface"),_T("dendrogram_classification"));			// �����ʕ��ތ��ʊK�w�N���X�^�����O�t�@�C����
	m_MetalKmeansClassFile = sys.getString(_T("MetalSurface"), _T("kmeans_classification"));				// �����ʕ��ތ���k-means�t�@�C����

	m_ResultScanImageFile = sys.getString(_T("JoiningResult"),_T("scan_image_file"));						// ���ʉ摜�X�L�����摜�t�@�C����
	m_ResultDendrogramClassFile = sys.getString(_T("JoiningResult"), _T("dendrogram_classification"));		// ���ʊK�w�N���X�^�����O�t�@�C����
	m_ResultKmeansClassFile = sys.getString(_T("JoiningResult"), _T("kmeans_classification"));				// ����k-means�t�@�C����

	m_ResinActiveAnalizeMethod				= sys.getInt(_T("ResinSurface"), _T("analize_method"));			// �����ʂ̕\���Ώۉ�͕��@
	m_MetalActiveAnalizeMethod				= sys.getInt(_T("MetalSurface"), _T("analize_method"));			// �����ʂ̕\���Ώۉ�͕��@
	m_ResultActiveAnalizeMethod				= sys.getInt(_T("JoiningResult"), _T("analize_method"));			// ���ʂ̕\���Ώۉ�͕��@

	m_nResinDendrogramClassificationClass	= sys.getInt(_T("ResinSurface"), _T("dendrogram_classification_nclass"));		// �����ʕ��ތ��ʊK�w�N���X�^�����O���ސ�
	m_nResinKMeansClassificationClass		= sys.getInt(_T("ResinSurface"), _T("kmeans_classification_nclass"));			// �����ʕ��ތ���k-means���ސ�
	m_nMetalDendrogramClassificationClass	= sys.getInt(_T("MetalSurface"), _T("dendrogram_classification_nclass"));		// �����ʕ��ތ��ʊK�w�N���X�^�����O���ސ�
	m_nMetalKMeansClassificationClass		= sys.getInt(_T("MetalSurface"), _T("kmeans_classification_nclass"));			// �����ʕ��ތ���k-means���ސ�
	m_nResultDendrogramClassificationClass	= sys.getInt(_T("JoiningResult"), _T("dendrogram_classification_nclass"));;		// ���ʊK�w�N���X�^�����O���ސ�
	m_nResultMeansClassificationClass		= sys.getInt(_T("JoiningResult"), _T("kmeans_classification_nclass"));;			// ����k-means���ސ�

	m_SpectralGraphPointTarget = sys.getInt(_T("SpectralGraphPoint"),_T("target"));							// �X�y�N�g���O���t�i�_�w��j�̑Ώ�
	// �X�y�N�g���O���t�i�_�w��j�̈ʒu
	str = sys.getString(_T("SpectralGraphPoint"),_T("position"));
	if (!Str2Pos(str,m_SpectralGraphPointHolizontalPosition,m_SpectralGraphPointVerticalPosition)) {
		m_SpectralGraphPointHolizontalPosition = 0;
		m_SpectralGraphPointVerticalPosition = 0;
	}

	m_SpectralGraphSectionTarget = sys.getInt(_T("SpectralGraphSection"),_T("target"));						// �X�y�N�g���O���t�i��Ԏw��j�̑Ώ�
	// �X�y�N�g���O���t�i�_�w��j�̊J�n�ʒu
	str = sys.getString(_T("SpectralGraphSection"),_T("start_position"));
	if (!Str2Pos(str,m_SpectralGraphSectionHolizontalStartPosition,m_SpectralGraphSectionVerticalStartPosition)) {
		m_SpectralGraphSectionHolizontalStartPosition = 0;
		m_SpectralGraphSectionVerticalStartPosition = 0;
	}
	///< �X�y�N�g���O���t�i�_�w��j�̏I���ʒu
	str = sys.getString(_T("SpectralGraphSection"),_T("end_position"));
	if (!Str2Pos(str,m_SpectralGraphSectionHolizontalEndPosition,m_SpectralGraphSectionVerticalEndPosition)) {
		m_SpectralGraphSectionHolizontalEndPosition = 0;
		m_SpectralGraphSectionVerticalEndPosition = 0;
	}

	m_ScanDataHolizontalSize = sys.getInt(_T("Common"), _T("scan_data_holizontal_size"));
	m_ScanDataVerticalSize = sys.getInt(_T("Common"), _T("scan_data_vertical_size"));

	return true;
}

/// <summary>
/// �v���W�F�N�g�̕ۑ�
/// </summary>
/// <param name="path">�v���W�F�N�g�t�@�C���p�X��</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CProjectFileIO::save(CString path)
{
	bool bResult = true;
	CConfigrationIO sys(path);
	CString str;

	if (!sys.setString(_T("Common"), _T("Version"), m_Version)) {
		bResult = false;
	}

	// �쐬��
	str = Date2Str(m_CreeateDay);
	if (str.IsEmpty() || (!sys.setString(_T("Common"),_T("create_date"),str))) {
		bResult = false;
	}
	// �X�V��
	str = Date2Str(m_UpdateDay);
	if (str.IsEmpty() || (!sys.setString(_T("Common"),_T("update_date"),str))) {
		bResult = false;
	}

	if (!sys.setInt(_T("Common"), _T("scan_data_holizontal_size"), m_ScanDataHolizontalSize)) {
		bResult = false;
	}

	if (!sys.setInt(_T("Common"), _T("scan_data_vertical_size"), m_ScanDataVerticalSize)) {
		bResult = false;
	}

/*
	// �C���[�W�f�[�^�ւ̃��[�g�p�X
	if (!sys.setString(_T("Common"),_T("image_data_root_path"),m_ImageDataRootPath)) {
		bResult = false;
	}
*/
	// �����ʃX�L�����摜�t�@�C����
	if (!sys.setString(_T("ResinSurface"),_T("scan_image_file"),m_ResinScanImageFile)) {
		bResult = false;
	}
	// �����ʊK�w�N���X�^�����O���ތ��ʃt�@�C����
	if (!sys.setString(_T("ResinSurface"),_T("dendrogram_classification"), m_ResinDendrogramClassFile)) {
		bResult = false;
	}
	// ������k-means���ތ��ʃt�@�C����
	if (!sys.setString(_T("ResinSurface"), _T("kmeans_classification"), m_ResinKmeansClassFile)) {
		bResult = false;
	}

	// �����ʃX�L�����摜�t�@�C����
	if (!sys.setString(_T("MetalSurface"),_T("scan_image_file"),m_MetalScanImageFile)) {
		bResult = false;
	}
	// �����ʊK�w�N���X�^�����O���ތ��ʃt�@�C����
	if (!sys.setString(_T("MetalSurface"),_T("dendrogram_classification"), m_MetalDendrogramClassFile)) {
		bResult = false;
	}
	// ������k-means���ތ��ʃt�@�C����
	if (!sys.setString(_T("MetalSurface"), _T("kmeans_classification"), m_MetalKmeansClassFile)) {
		bResult = false;
	}

	// ���ʉ摜�X�L�����摜�t�@�C����
	if (!sys.setString(_T("JoiningResult"),_T("scan_image_file"),m_ResultScanImageFile)) {
		bResult = false;
	}
	// �ڍ����ʊK�w�N���X�^�����O���ތ��ʃt�@�C����
	if (!sys.setString(_T("JoiningResult"), _T("dendrogram_classification"), m_ResultDendrogramClassFile)) {
		bResult = false;
	}
	// �ڍ�����k-means���ތ��ʃt�@�C����
	if (!sys.setString(_T("JoiningResult"), _T("kmeans_classification"), m_ResultKmeansClassFile)) {
		bResult = false;
	}

	// �����ʂ̕\���Ώۉ�͕��@
	if (!sys.setInt(_T("ResinSurface"), _T("analize_method"), m_ResinActiveAnalizeMethod)) {
		bResult = false;
	}
	// �����ʂ̕\���Ώۉ�͕��@
	if (!sys.setInt(_T("MetalSurface"), _T("analize_method"), m_MetalActiveAnalizeMethod)) {
		bResult = false;
	}
	// ���ʂ̕\���Ώۉ�͕��@
	if (!sys.setInt(_T("JoiningResult"), _T("analize_method"), m_ResultActiveAnalizeMethod)) {
		bResult = false;
	}
	// �����ʕ��ތ��ʊK�w�N���X�^�����O���ސ�
	if (!sys.setInt(_T("ResinSurface"), _T("dendrogram_classification_nclass"), m_nResinDendrogramClassificationClass)) {
		bResult = false;
	}
	// �����ʕ��ތ���k-means���ސ�
	if (!sys.setInt(_T("ResinSurface"), _T("kmeans_classification_nclass"), m_nResinKMeansClassificationClass)) {
		bResult = false;
	}
	// �����ʕ��ތ��ʊK�w�N���X�^�����O���ސ�
	if (!sys.setInt(_T("MetalSurface"), _T("dendrogram_classification_nclass"), m_nMetalDendrogramClassificationClass)) {
		bResult = false;
	}
	// �����ʕ��ތ���k-means���ސ�
	if (!sys.setInt(_T("MetalSurface"), _T("kmeans_classification_nclass"), m_nMetalKMeansClassificationClass)) {
		bResult = false;
	}
	// ���ʊK�w�N���X�^�����O���ސ�
	if (!sys.setInt(_T("JoiningResult"), _T("dendrogram_classification_nclass"), m_nResultDendrogramClassificationClass)) {
		bResult = false;
	}
	// ����k-means���ސ�
	if (!sys.setInt(_T("JoiningResult"), _T("kmeans_classification_nclass"), m_nResultMeansClassificationClass)) {
		bResult = false;
	}

	// �X�y�N�g���O���t�i�_�w��j�̑Ώ�
	if (!sys.setInt(_T("SpectralGraphPoint"),_T("target"),m_SpectralGraphPointTarget)) {
		bResult = false;
	}
	// �X�y�N�g���O���t�i�_�w��j�̈ʒu
	str.Format(_T("%d,%d"),m_SpectralGraphPointHolizontalPosition,m_SpectralGraphPointVerticalPosition);
	if (!sys.setString(_T("SpectralGraphPoint"),_T("position"),str)) {
		bResult = false;
	}

	// �X�y�N�g���O���t�i��Ԏw��j�̑Ώ�
	if (!sys.setInt(_T("SpectralGraphSection"),_T("target"),m_SpectralGraphSectionTarget)) {
		bResult = false;
	}
	// �X�y�N�g���O���t�i�_�w��j�̊J�n�ʒu
	str.Format(_T("%d,%d"),m_SpectralGraphSectionHolizontalStartPosition,m_SpectralGraphSectionVerticalStartPosition);
	if (!sys.setString(_T("SpectralGraphSection"),_T("start_position"),str)) {
		bResult = false;
	}
	///< �X�y�N�g���O���t�i�_�w��j�̏I���ʒu
	str.Format(_T("%d,%d"),m_SpectralGraphSectionHolizontalEndPosition,m_SpectralGraphSectionVerticalEndPosition);
	if (!sys.setString(_T("SpectralGraphSection"),_T("end_position"),str)) {
		bResult = false;
	}
	return bResult;
}

/// <summary>
/// �쐬���̎擾
/// </summary>
/// <returns>�쐬����Ԃ�</returns>
COleDateTime CProjectFileIO::GetCreeateDay()
{
	return m_CreeateDay;
}

/// <summary>
/// �쐬���̐ݒ�
/// </summary>
/// <param name="date">���t</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CProjectFileIO::SetCreateDay(COleDateTime date)
{
	m_CreeateDay = date;
	return true;
}

/// <summary>
/// �X�V���̎擾
/// </summary>
/// <returns>�X�V����Ԃ�</returns>
COleDateTime CProjectFileIO::GetUpdateDay()
{
	return m_UpdateDay;
}

/// <summary>
/// �X�V���̐ݒ�
/// </summary>
/// <param name="date">���t</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CProjectFileIO::SetUpdateDay(COleDateTime date)
{
	m_UpdateDay = date;
	return true;
}

/// <summary>
/// �C���[�W�f�[�^�ւ̃��[�g�p�X�̎擾
/// </summary>
/// <returns>�C���[�W�f�[�^�ւ̃��[�g�p�X��Ԃ�</returns>
CString CProjectFileIO::GetImageDataRootPath()
{
	return m_ImageDataRootPath;
}

/// <summary>
/// �C���[�W�f�[�^�ւ̃��[�g�p�X�̐ݒ�
/// </summary>
/// <param name="rootPath">���[�g�p�X</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CProjectFileIO::SetImageDataRootPath(CString rootPath)
{
	m_ImageDataRootPath = rootPath;
	return true;
}

/// <summary>
/// �����ʃX�L�����摜�t�@�C�����̎擾
/// </summary>
/// <returns>�����ʃX�L�����摜�t�@�C������Ԃ�</returns>
CString CProjectFileIO::GetResinScanImageFile()
{
	return m_ResinScanImageFile;
}

/// <summary>
/// �����ʃX�L�����摜�t�@�C�����̐ݒ�
/// </summary>
/// <param name="fileName">�����ʃX�L�����摜�t�@�C����</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CProjectFileIO::SetResinScanImageFile(CString fileName)
{
	m_ResinScanImageFile = fileName;
	return true;
}

/// <summary>
/// �����ʊK�w�N���X�^�����O���ތ��ʃt�@�C�����̎擾
/// </summary>
/// <returns>�����ʃN���X�^�����O���ތ��ʃt�@�C������Ԃ�</returns>
CString CProjectFileIO::GetResinDendrogramClassFile()
{
	return m_ResinDendrogramClassFile;
}

/// <summary>
/// �����ʊK�w�N���X�^�����O���ތ��ʃt�@�C�����̐ݒ�
/// </summary>
/// <param name="fileName">�����ʊK�w�N���X�^�����O���ތ��ʃt�@�C����</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CProjectFileIO::SetResinDendrogramClassFile(CString fileName)
{
	m_ResinDendrogramClassFile = fileName;
	return true;
}

/// <summary>
/// ������k-means���ތ��ʃt�@�C�����̎擾
/// </summary>
/// <returns>������k-means���ތ��ʃt�@�C������Ԃ�</returns>
CString CProjectFileIO::GetResinKmeansClassFile()
{
	return m_ResinKmeansClassFile;
}

/// <summary>
/// ������k-means���ތ��ʃt�@�C�����̐ݒ�
/// </summary>
/// <param name="fileName">������k-means���ތ��ʃt�@�C����</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CProjectFileIO::SetResinKmeansClassFile(CString fileName)
{
	m_ResinKmeansClassFile = fileName;
	return true;
}

/// <summary>
/// �����摜�\���^�C�v�̎擾
/// </summary>
/// <returns>�����摜�\���^�C�v��Ԃ�</returns>
int CProjectFileIO::GetResinDisplayType()
{
	if (!CFileUtil::fileExists(m_ProjectFilePath)) {
		return 0;
	}
	CConfigrationIO sys(m_ProjectFilePath);
	return sys.getInt(_T("ResinSurface"),_T("display_type"));
}

/// <summary>
/// �����摜�\���^�C�v�̐ݒ�
/// </summary>
/// <param name="displayType">�����摜�\���^�C�v</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CProjectFileIO::SetResinDisplayType(int displayType)
{
	if (!CFileUtil::fileExists(m_ProjectFilePath)) {
		return false;
	}
	CConfigrationIO sys(m_ProjectFilePath);
	return sys.setInt(_T("ResinSurface"),_T("display_type"),displayType);
}

/// <summary>
/// �����ʉ�͕��@�̎擾
/// </summary>
/// <returns>�����ʉ�͕��@��Ԃ�</returns>
int CProjectFileIO::GetResinAnalizeMethod()
{
	return m_ResinActiveAnalizeMethod;
}

/// <summary>
/// �����ʉ�͕��@�̐ݒ�
/// </summary>
/// <param name="method">��͕��@</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CProjectFileIO::SetResinAnalizeMethod(int method)
{
	m_ResinActiveAnalizeMethod = method;
	return true;
}

/// <summary>
/// �쐬���ꂽ�����ʊK�w�N���X�^�����O��͂̃N���X���̎擾
/// </summary>
/// <returns>�N���X����Ԃ�</returns>
int CProjectFileIO::GetNumbetOfResinDendrogramClassificationClass()
{
	return m_nResinDendrogramClassificationClass;
}

/// <summary>
/// �쐬���ꂽ�����ʊK�w�N���X�^�����O��͂̃N���X���̎擾
/// </summary>
/// <param name="nClass">�N���X��</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CProjectFileIO::SetNumbetOfResinDendrogramClassificationClass(int nClass)
{
	m_nResinDendrogramClassificationClass = nClass;
	return true;
}

/// <summary>
/// �쐬���ꂽ������K-Means��͂̃N���X���̎擾
/// </summary>
/// <returns>�N���X����Ԃ�</returns>
int CProjectFileIO::GetNumbetOfResinKMeansClassificationClass()
{
	return m_nResinKMeansClassificationClass;
}

/// <summary>
/// �쐬���ꂽ������K-Means��͂̃N���X���̎擾
/// </summary>
/// <param name="nClass">�N���X��</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CProjectFileIO::SetNumbetOfResinKMeansClassificationClass(int nClass)
{
	m_nResinKMeansClassificationClass = nClass;
	return true;
}

/// <summary>
/// �����ʃX�L�����摜�t�@�C�����̎擾
/// </summary>
/// <returns>�����ʃX�L�����摜�t�@�C������Ԃ�</returns>
CString CProjectFileIO::GetMetalScanImageFile()
{
	return m_MetalScanImageFile;
}

/// <summary>
/// �����ʃX�L�����摜�t�@�C�����̐ݒ�
/// </summary>
/// <param name="fileName">�����ʃX�L�����摜�t�@�C����</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CProjectFileIO::SetMetalScanImageFile(CString fileName)
{
	m_MetalScanImageFile = fileName;
	return true;
}

/// <summary>
/// �����ʊK�w�N���X�^�����O���ތ��ʃt�@�C�����̎擾
/// </summary>
/// <returns>�����ʊK�w�N���X�^�����O���ތ��ʃt�@�C������Ԃ�</returns>
CString CProjectFileIO::GetMetalDendrogramClassFile()
{
	return m_MetalDendrogramClassFile;
}

/// <summary>
/// �����ʊK�w�N���X�^�����O���ތ��ʃt�@�C�����̐ݒ�
/// </summary>
/// <param name="fileName">�����ʊK�w�N���X�^�����O���ތ��ʃt�@�C����</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CProjectFileIO::SetMetalDendrogramClassFile(CString fileName)
{
	m_MetalDendrogramClassFile = fileName;
	return true;
}

/// <summary>
/// ������k-means���ތ��ʃt�@�C�����̎擾
/// </summary>
/// <returns>�����ʕ��ތ���k-means�t�@�C������Ԃ�</returns>
CString CProjectFileIO::GetMetalKmeansClassFile()
{
	return m_MetalKmeansClassFile;
}

/// <summary>
/// ������k-means���ތ��ʃt�@�C�����̐ݒ�
/// </summary>
/// <param name="fileName">�����ʕ��ތ���k-means�t�@�C����</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CProjectFileIO::SetMetalKmeansClassFile(CString fileName)
{
	m_MetalKmeansClassFile = fileName;
	return true;
}

/// <summary>
/// �����摜�\���^�C�v�̎擾
/// </summary>
/// <returns>�����摜�\���^�C�v��Ԃ�</returns>
int CProjectFileIO::GetMetalDisplayType()
{
	if (!CFileUtil::fileExists(m_ProjectFilePath)) {
		return 0;
	}
	CConfigrationIO sys(m_ProjectFilePath);
	return sys.getInt(_T("MetalSurface"),_T("display_type"));
}

/// <summary>
/// �����摜�\���^�C�v�̐ݒ�
/// </summary>
/// <param name="displayType">�����摜�\���^�C�v</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CProjectFileIO::SetMetalDisplayType(int displayType)
{
	if (!CFileUtil::fileExists(m_ProjectFilePath)) {
		return false;
	}
	CConfigrationIO sys(m_ProjectFilePath);
	return sys.setInt(_T("MetalSurface"),_T("display_type"),displayType);
}

/// <summary>
/// �����ʉ�͕��@�̎擾
/// </summary>
/// <returns>�����ʉ�͕��@��Ԃ�</returns>
int CProjectFileIO::GetMetalAnalizeMethod()
{
	return m_MetalActiveAnalizeMethod;
}

/// <summary>
/// �����ʉ�͕��@�̐ݒ�
/// </summary>
/// <param name="method">��͕��@</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CProjectFileIO::SetMetalAnalizeMethod(int method)
{
	m_MetalActiveAnalizeMethod = method;
	return true;
}

/// <summary>
/// �쐬���ꂽ�����ʊK�w�N���X�^�����O��͂̃N���X���̎擾
/// </summary>
/// <returns>�N���X����Ԃ�</returns>
int CProjectFileIO::GetNumbetOfMetalDendrogramClassificationClass()
{
	return m_nMetalDendrogramClassificationClass;
}

/// <summary>
/// �쐬���ꂽ�����ʊK�w�N���X�^�����O��͂̃N���X���̎擾
/// </summary>
/// <param name="nClass">�N���X��</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CProjectFileIO::SetNumbetOfMetalDendrogramClassificationClass(int nClass)
{
	m_nMetalDendrogramClassificationClass = nClass;
	return true;
}

/// <summary>
/// �쐬���ꂽ������K-Means��͂̃N���X���̎擾
/// </summary>
/// <returns>�N���X����Ԃ�</returns>
int CProjectFileIO::GetNumbetOfMetalKMeansClassificationClass()
{
	if (!CFileUtil::fileExists(m_ProjectFilePath)) {
		return false;
	}
	CConfigrationIO sys(m_ProjectFilePath);
	return sys.getInt(_T("MetalSurface"), _T("kmeans_classification_nclass"));
}

/// <summary>
/// �쐬���ꂽ������K-Means��͂̃N���X���̎擾
/// </summary>
/// <param name="nClass">�N���X��</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CProjectFileIO::SetNumbetOfMetalKMeansClassificationClass(int nClass)
{
	if (!CFileUtil::fileExists(m_ProjectFilePath)) {
		return false;
	}
	CConfigrationIO sys(m_ProjectFilePath);
	return sys.setInt(_T("MetalSurface"), _T("kmeans_classification_nclass"), nClass);
}

/// <summary>
/// �ڍ����ʉ摜�X�L�����摜�t�@�C�����̎擾
/// </summary>
/// <returns>�ڍ����ʉ摜�X�L�����摜�t�@�C������Ԃ�</returns>
CString CProjectFileIO::GetResultScanImageFile()
{
	return m_ResultScanImageFile;
}

/// <summary>
/// �ڍ����ʉ摜�X�L�����摜�t�@�C�����̐ݒ�
/// </summary>
/// <param name="fileName">�ڍ����ʉ摜�X�L�����摜�t�@�C����</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CProjectFileIO::SetResultScanImageFile(CString fileName)
{
	m_ResultScanImageFile = fileName;
	return true;
}

/// <summary>
/// �ڍ����ʊK�w�N���X�^�����O���ރt�@�C�����̎擾
/// </summary>
/// <returns>�ڍ����ʊK�w�N���X�^�����O���ރt�@�C������Ԃ�</returns>
CString CProjectFileIO::GetResultDendrogramClassFile()
{
	return m_ResultDendrogramClassFile;
}

/// <summary>
/// �ڍ����ʊK�w�N���X�^�����O�t���ރ@�C�����̐ݒ�
/// </summary>
/// <param name="fileName">�ڍ����ʊK�w�N���X�^�����O���ރt�@�C����</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CProjectFileIO::SetResultDendrogramClassFile(CString fileName)
{
	m_ResultDendrogramClassFile = fileName;
	return true;
}

/// <summary>
/// �ڍ�����k-means�t�@�C�����̎擾
/// </summary>
/// <returns>�ڍ�����k-means�t�@�C������Ԃ�</returns>
CString CProjectFileIO::GetResultKmeansClassFile()
{
	return m_ResultKmeansClassFile;
}

/// <summary>
/// �ڍ�����k-means�t�@�C�����̐ݒ�
/// </summary>
/// <param name="fileName">�ڍ�����k-means�t�@�C����</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CProjectFileIO::SetResultKmeansClassFile(CString fileName)
{
	m_ResultKmeansClassFile = fileName;
	return true;
}
/// <summary>
/// �ڍ����ʉ摜�\���^�C�v�̎擾
/// </summary>
/// <returns>�ڍ����ʉ摜�\���^�C�v��Ԃ�</returns>
int CProjectFileIO::GetResultDisplayType()
{
	if (!CFileUtil::fileExists(m_ProjectFilePath)) {
		return 0;
	}
	CConfigrationIO sys(m_ProjectFilePath);
	return sys.getInt(_T("JoiningResult"),_T("display_type"));
}

/// <summary>
/// �ڍ����ʉ摜�\���^�C�v�̐ݒ�
/// </summary>
/// <param name="displayType">�ڍ����ʉ摜�\���^�C�v</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CProjectFileIO::SetResultDisplayType(int displayType)
{
	if (!CFileUtil::fileExists(m_ProjectFilePath)) {
		return false;
	}
	CConfigrationIO sys(m_ProjectFilePath);
	return sys.setInt(_T("JoiningResult"),_T("display_type"),displayType);
}


/// <summary>
/// �ڍ����ʉ�͕��@�̎擾
/// </summary>
/// <returns>�ڍ����ʉ�͕��@��Ԃ�</returns>
int CProjectFileIO::GetResultAnalizeMethod()
{
	return m_ResultActiveAnalizeMethod;
}

/// <summary>
/// �ڍ����ʉ�͕��@�̐ݒ�
/// </summary>
/// <param name="method">��͕��@</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CProjectFileIO::SetResultAnalizeMethod(int method)
{
	m_ResultActiveAnalizeMethod = method;
	return true;
}

/// <summary>
/// �쐬���ꂽ�ڍ����ʊK�w�N���X�^�����O��͂̃N���X���̎擾
/// </summary>
/// <returns>�N���X����Ԃ�</returns>
int CProjectFileIO::GetNumbetOfResultDendrogramClassificationClass()
{
	return m_nResultDendrogramClassificationClass;
}

/// <summary>
/// �쐬���ꂽ�ڍ����ʊK�w�N���X�^�����O��͂̃N���X���̎擾
/// </summary>
/// <param name="nClass">�N���X��</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CProjectFileIO::SetNumbetOfResultDendrogramClassificationClass(int nClass)
{
	m_nResultDendrogramClassificationClass = nClass;
	return true;
}

/// <summary>
/// �쐬���ꂽ�ڍ�����K-Means��͂̃N���X���̎擾
/// </summary>
/// <returns>�N���X����Ԃ�</returns>
int CProjectFileIO::GetNumbetOfResultKMeansClassificationClass()
{
	return m_nResultMeansClassificationClass;
}

/// <summary>
/// �쐬���ꂽ�ڍ�����K-Means��͂̃N���X���̎擾
/// </summary>
/// <param name="nClass">�N���X��</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CProjectFileIO::SetNumbetOfResultKMeansClassificationClass(int nClass)
{
	m_nResultMeansClassificationClass = nClass;
	return true;
}

/// <summary>
/// �X�y�N�g���O���t�i�_�w��j�̑Ώۂ̎擾
/// </summary>
/// <returns>�X�y�N�g���O���t�i�_�w��j�̑Ώۂ�Ԃ�</returns>
UINT CProjectFileIO::GetSpectralGraphPointTarget()
{
	return m_SpectralGraphPointTarget;
}

/// <summary>
/// �X�y�N�g���O���t�i�_�w��j�̑Ώۂ̐ݒ�
/// </summary>
/// <param name="targetID">�X�y�N�g���O���t�i�_�w��j�̑Ώ�</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CProjectFileIO::SetSpectralGraphPointTarget(UINT targetID)
{
	m_SpectralGraphPointTarget = targetID;
	return true;
}

/// <summary>
/// �����񂩂�|�C���g�ւ̕ϊ�
/// </summary>
/// <param name="str">������</param>
/// <param name="holizontal">�����l</param>
/// <param name="vertical">�����l</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CProjectFileIO::Str2Pos(CString str,int &holizontal, int &vertical)
{
	holizontal = 0;
	vertical = 0;
	int pos = str.FindOneOf(_T(","));
	if (pos > 0) {
		CString s = str.Left(pos);
		holizontal = _ttoi(s);
		str = str.Mid(pos+1);
		vertical =  _ttoi(str);
		return true;
	} else {
		return false;
	}
}

/// <summary>
/// �X�y�N�g���O���t�i�_�w��j�̈ʒu�̎擾
/// </summary>
/// <param name="holizontal">�����l</param>
/// <param name="vertical">�����l</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CProjectFileIO::GetSpectralGraphPointPosition(int &holizontal, int &vertical)
{
	holizontal	= m_SpectralGraphPointHolizontalPosition;
	vertical	= m_SpectralGraphPointVerticalPosition;
	return true;
}

/// <summary>
/// �X�y�N�g���O���t�i�_�w��j�̈ʒu�̐ݒ�
/// </summary>
/// <param name="holizontal">�����l</param>
/// <param name="vertical">�����l</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CProjectFileIO::SetSpectralGraphPointPosition(int holizontal, int vertical)
{
	m_SpectralGraphPointHolizontalPosition = holizontal;
	m_SpectralGraphPointVerticalPosition = vertical;
	return true;
}

/// <summary>
/// �X�y�N�g���O���t�i��Ԏw��j�̑Ώۂ̎擾
/// </summary>
/// <returns>�X�y�N�g���O���t�i��Ԏw��j�̑Ώۂ�Ԃ�</returns>
UINT CProjectFileIO::GetSpectralGraphSectionTarget()
{
	return m_SpectralGraphSectionTarget;
}

/// <summary>
/// �X�y�N�g���O���t�i��Ԏw��j�̑Ώۂ̐ݒ�
/// </summary>
/// <param name="targetID">�X�y�N�g���O���t�i��Ԏw��j�̑Ώ�</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CProjectFileIO::SetSpectralGraphSectionTarget(UINT targetID)
{
	m_SpectralGraphSectionTarget = targetID;
	return true;
}

/// <summary>
/// �X�y�N�g���O���t�i��Ԏw��j�̊J�n�ʒu�̎擾
/// </summary>
/// <param name="holizontal">�����l</param>
/// <param name="vertical">�����l</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CProjectFileIO::GetSpectralGraphSectionStartPosition(int &holizontal, int &vertical)
{
	holizontal	= m_SpectralGraphSectionHolizontalStartPosition;
	vertical	= m_SpectralGraphSectionVerticalStartPosition;
	return true;
}

/// <summary>
/// �X�y�N�g���O���t�i��Ԏw��j�̊J�n�ʒu�̐ݒ�
/// </summary>
/// <param name="holizontal">�����l</param>
/// <param name="vertical">�����l</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CProjectFileIO::SetSpectralGraphSectionStartPosition(int holizontal, int vertical)
{
	m_SpectralGraphSectionHolizontalStartPosition = holizontal;
	m_SpectralGraphSectionVerticalStartPosition = vertical;
	return true;
}

/// <summary>
/// �X�y�N�g���O���t�i��Ԏw��j�̏I���ʒu�̎擾
/// </summary>
/// <param name="holizontal">�����l</param>
/// <param name="vertical">�����l</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CProjectFileIO::GetSpectralGraphSectionEndPosition(int &holizontal, int &vertical)
{
	holizontal	= m_SpectralGraphSectionHolizontalEndPosition;
	vertical	= m_SpectralGraphSectionVerticalEndPosition;
	return true;
}

/// <summary>
/// �X�y�N�g���O���t�i��Ԏw��j�̏I���ʒu�̐ݒ�
/// </summary>
/// <param name="holizontal">�����l</param>
/// <param name="vertical">�����l</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CProjectFileIO::SetSpectralGraphSectionEndPosition(int holizontal, int vertical)
{
	m_SpectralGraphSectionHolizontalEndPosition = holizontal;
	m_SpectralGraphSectionVerticalEndPosition = vertical;
	return true;
}

/// <summary>
/// �X�L�����f�[�^�̃T�C�Y�擾
/// </summary>
/// <param name="holizontal">�����l</param>
/// <param name="vertical">�����l</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CProjectFileIO::GetScanDataSize(int &holizontal, int &vertical)
{
	holizontal	= m_ScanDataHolizontalSize;
	vertical	= m_ScanDataVerticalSize;
	return true;
}

/// <summary>
/// �X�L�����f�[�^�̃T�C�Y�ݒ�
/// </summary>
/// <param name="holizontal">�����l</param>
/// <param name="vertical">�����l</param>
/// <returns>���������ꍇ��true�A���s�����ꍇ��false��Ԃ�</returns>
bool CProjectFileIO::SetScanDataSize(int holizontal, int vertical)
{
	m_ScanDataHolizontalSize = holizontal;
	m_ScanDataVerticalSize = vertical;
	return true;
}

/// <summary>
/// �p�����[�^�t�@�C���̓ǂݍ���
/// </summary>
/// <param name="path">�p�����[�^�t�@�C���ւ̃p�X</param>
/// <returns>�����ꍇ��true�A���s�ꍇ��false��Ԃ�</returns>
bool CProjectFileIO::Read(CString path/* = _T("")*/)
{
	if (path.IsEmpty()) {
		path = m_ProjectFilePath;
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
bool CProjectFileIO::Save(bool bUpdate/*=true*/, CString path/* = _T("")*/)
{
	if (path.IsEmpty()) {
		path = m_ProjectFilePath;
	}
	if (bUpdate && (!CFileUtil::fileExists(path))) {
		return false;
	}
	return save(path);
}
