#pragma once
#include "hsi_api_common.h"
#include <vector>

/// <summary>
/// �X�L�����f�[�^IO�N���X
/// </summary>
class CScanDataIO
{
	///
	typedef struct {
		CubeFloat*	m_o_p_cube;		///< �X�L�����f�[�^
		int			m_band;			///< �o���h�C���f�b�N�X
		int			m_row;			///< ����
		int			m_col;			///< ��
		float		m_normalizer;	///< �o���h�ɂ����鐳�K���p�����[�^
		float		m_offset;		///< �o���h�ɂ�����I�t�Z�b�g�l
	} Normalizer;

private:
	CString		m_pathName;			///< �X�L�����f�[�^�t�@�C���p�X
	CubeFloat*	m_o_p_cube;			///< CubeFloat�I�u�W�F�N�g�ւ̃|�C���^
	std::vector<std::vector<std::vector<float>>>	m_data;


public:
	CScanDataIO(void);
	~CScanDataIO(void);

private:
	UINT __cdecl getNomalizer(LPVOID p);
	static void executeNomalizerLauncher(void* args);
	bool getRGBBandSpectrum(int &RBand, int &GBand, int &BBand);

public:
	bool open(CString pathName, bool bReload =false);
	void close();
	void release();
	bool LoadImage(int &height, int &width, int &bands, CImage &img);
	bool GetSpectrumData(CPoint &pos, std::vector<double> &data);
	bool getBandSpectrum(std::vector<double> &BandSpectrum);
	bool joinInit();
	bool join(CString pathName, int nOverlap);
	bool joinend(CString outpathName);
};

