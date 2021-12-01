#pragma once

/// <summary>
/// �v���W�F�N�g�t�@�C��IO�N���X
/// </summary>
class CProjectFileIO
{
public:
	/// <summary>
	/// �Ώێ�ʗp��
	/// </summary>
	enum {
		eResinSurface	= 0,	///< ����
		eMetalSurface	= 1,	///< ����
		eJoiningResult	= 2,	///< �ڍ�����
	} ;

private:
	CString			m_ProjectFilePath;								///< �v���W�F�N�g�t�@�C���ւ̃p�X

	COleDateTime	m_CreeateDay;									///< �쐬��
	COleDateTime	m_UpdateDay;									///< �X�V��
	CString			m_ImageDataRootPath;							///< �C���[�W�f�[�^�ւ̃��[�g�p�X

	CString			m_ResinScanImageFile;							///< �����ʃX�L�����摜�t�@�C����
	CString			m_ResinDendrogramClassFile;						///< �����ʕ��ތ��ʊK�w�N���X�^�����O�t�@�C����
	CString			m_ResinKmeansClassFile;							///< �����ʕ��ތ���k-means�t�@�C����

	CString			m_MetalScanImageFile;							///< �����ʃX�L�����摜�t�@�C����
	CString			m_MetalDendrogramClassFile;						///< �����ʕ��ތ��ʊK�w�N���X�^�����O�t�@�C����
	CString			m_MetalKmeansClassFile;							///< �����ʕ��ތ���k-means�t�@�C����

	CString			m_ResultScanImageFile;							///< ���ʉ摜�X�L�����摜�t�@�C����
	CString			m_ResultDendrogramClassFile;					///< ���ʊK�w�N���X�^�����O�t�@�C����
	CString			m_ResultKmeansClassFile;						///< ����k-means�t�@�C����

/*
	int				m_ResinDisplayType;								///< �����摜�\���^�C�v
	int				m_MetalDisplayType;								///< �����摜�\���^�C�v
	int				m_ResultDisplayType;							///< ���ʉ摜�\���^�C�v
*/
	int				m_ResinActiveAnalizeMethod;						///< �����ʂ̕\���Ώۉ�͕��@
	int				m_MetalActiveAnalizeMethod;						///< �����ʂ̕\���Ώۉ�͕��@
	int				m_ResultActiveAnalizeMethod;					///< ���ʂ̕\���Ώۉ�͕��@

	int				m_nResinDendrogramClassificationClass;			///< �����ʕ��ތ��ʊK�w�N���X�^�����O���ސ�
	int				m_nResinKMeansClassificationClass;				///< �����ʕ��ތ���k-means���ސ�
	int				m_nMetalDendrogramClassificationClass;			///< �����ʕ��ތ��ʊK�w�N���X�^�����O���ސ�
	int				m_nMetalKMeansClassificationClass;				///< �����ʕ��ތ���k-means���ސ�
	int				m_nResultDendrogramClassificationClass;			///< ���ʊK�w�N���X�^�����O���ސ�
	int				m_nResultMeansClassificationClass;				///< ���ʊK�w�N���X�^�����O���ސ�

	UINT			m_SpectralGraphPointTarget;						///< �X�y�N�g���O���t�i�_�w��j�̑Ώ�
	int				m_SpectralGraphPointHolizontalPosition;			///< �X�y�N�g���O���t�i�_�w��j�̐����ʒu
	int				m_SpectralGraphPointVerticalPosition;			///< �X�y�N�g���O���t�i�_�w��j�̐����ʒu

	UINT			m_SpectralGraphSectionTarget;					///< �X�y�N�g���O���t�i��Ԏw��j�̑Ώ�
	int				m_SpectralGraphSectionHolizontalStartPosition;	///< �X�y�N�g���O���t�i�_�w��j�̐����J�n�ʒu
	int				m_SpectralGraphSectionVerticalStartPosition;	///< �X�y�N�g���O���t�i�_�w��j�̐����J�n�ʒu
	int				m_SpectralGraphSectionHolizontalEndPosition;	///< �X�y�N�g���O���t�i�_�w��j�̐����I���ʒu
	int				m_SpectralGraphSectionVerticalEndPosition;		///< �X�y�N�g���O���t�i�_�w��j�̐����I���ʒu

private:
	bool read(CString path);
	bool save(CString path);

public:
	CProjectFileIO(void);
	~CProjectFileIO(void);

	void Initialze();

	CString GetProjectFilePath();
	bool SetProjectFilePath(CString path);

/*
	CString MakeProjectName(CString TestName, COleDateTime date, int nunber);
	CString GetProjectName();
	bool SetProjectName(CString projectname);
*/

	bool Str2Date(CString str,COleDateTime &date);
	CString Date2Str(COleDateTime date);

	COleDateTime GetCreeateDay();
	bool SetCreateDay(COleDateTime date);
	COleDateTime GetUpdateDay();
	bool SetUpdateDay(COleDateTime date);
	CString GetImageDataRootPath();
	bool SetImageDataRootPath(CString rootPath);

	CString GetResinScanImageFile();
	bool SetResinScanImageFile(CString fileName);
	CString GetResinDendrogramClassFile();
	bool SetResinDendrogramClassFile(CString fileName);
	CString GetResinKmeansClassFile();
	bool SetResinKmeansClassFile(CString fileName);
	int GetResinDisplayType();
	bool SetResinDisplayType(int displayType);
	int GetResinAnalizeMethod();
	bool SetResinAnalizeMethod(int method);
	int GetNumbetOfResinDendrogramClassificationClass();
	bool SetNumbetOfResinDendrogramClassificationClass(int nClass);
	int GetNumbetOfResinKMeansClassificationClass();
	bool SetNumbetOfResinKMeansClassificationClass(int nClass);

	CString GetMetalScanImageFile();
	bool SetMetalScanImageFile(CString fileName);
	CString GetMetalDendrogramClassFile();
	bool SetMetalDendrogramClassFile(CString fileName);
	CString GetMetalKmeansClassFile();
	bool SetMetalKmeansClassFile(CString fileName);
	int GetMetalDisplayType();
	bool SetMetalDisplayType(int displayType);
	int GetMetalAnalizeMethod();
	bool SetMetalAnalizeMethod(int method);
	int GetNumbetOfMetalDendrogramClassificationClass();
	bool SetNumbetOfMetalDendrogramClassificationClass(int nClass);
	int GetNumbetOfMetalKMeansClassificationClass();
	bool SetNumbetOfMetalKMeansClassificationClass(int nClass);

	CString GetResultScanImageFile();
	bool SetResultScanImageFile(CString fileName);
	CString GetResultDendrogramClassFile();
	bool SetResultDendrogramClassFile(CString fileName);
	CString GetResultKmeansClassFile();
	bool SetResultKmeansClassFile(CString fileName);
	int GetResultDisplayType();
	bool SetResultDisplayType(int displayType);
	int GetResultAnalizeMethod();
	bool SetResultAnalizeMethod(int method);
	int GetNumbetOfResultDendrogramClassificationClass();
	bool SetNumbetOfResultDendrogramClassificationClass(int nClass);
	int GetNumbetOfResultKMeansClassificationClass();
	bool SetNumbetOfResultKMeansClassificationClass(int nClass);

	bool Str2Pos(CString str,int &holizontal, int &bvertical);

	UINT GetSpectralGraphPointTarget();
	bool SetSpectralGraphPointTarget(UINT targetID);
	bool GetSpectralGraphPointPosition(int &holizontal, int &bvertical);
	bool SetSpectralGraphPointPosition(int holizontal, int bvertical);

	UINT GetSpectralGraphSectionTarget();
	bool SetSpectralGraphSectionTarget(UINT targetID);
	bool GetSpectralGraphSectionStartPosition(int &holizontal, int &bvertical);
	bool SetSpectralGraphSectionStartPosition(int holizontal, int bvertical);
	bool GetSpectralGraphSectionEndPosition(int &holizontal, int &bvertical);
	bool SetSpectralGraphSectionEndPosition(int holizontal, int bvertical);

#if 0
	bool GetScanDataSize(int &holizontal, int &vertical);
	bool SetScanDataSize(int holizontal, int vertical);
#endif

	bool Read(CString path = _T(""));
	bool Save(bool bUpdate=true, CString path = _T(""));
};
