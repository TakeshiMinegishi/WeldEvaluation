
// WeldEvaluation.h : WeldEvaluation �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C��
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH �ɑ΂��Ă��̃t�@�C�����C���N���[�h����O�� 'stdafx.h' ���C���N���[�h���Ă�������"
#endif

#include "resource.h"       // ���C�� �V���{��


// CWeldEvaluationApp:
// ���̃N���X�̎����ɂ��ẮAWeldEvaluation.cpp ���Q�Ƃ��Ă��������B
//

/// <summary>
/// CWeldEvaluatio�A�v���P�[�V�����N���X
/// </summary>
class CWeldEvaluationApp : public CWinApp
{
public:
	CWeldEvaluationApp();


// �I�[�o�[���C�h
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ����
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CWeldEvaluationApp theApp;
