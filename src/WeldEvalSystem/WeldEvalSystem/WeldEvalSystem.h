
// WeldEvalSystem.h : WeldEvalSystem �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C��
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH �ɑ΂��Ă��̃t�@�C�����C���N���[�h����O�� 'stdafx.h' ���C���N���[�h���Ă�������"
#endif

#include "resource.h"       // ���C�� �V���{��


// CWeldEvalSystemApp:
// ���̃N���X�̎����ɂ��ẮAWeldEvalSystem.cpp ���Q�Ƃ��Ă��������B
//

class CWeldEvalSystemApp : public CWinApp
{
public:
	CWeldEvalSystemApp();


// �I�[�o�[���C�h
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ����

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CWeldEvalSystemApp theApp;
