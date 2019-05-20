/*!
	@file	Entry.cpp
	@date	2019/03/06
	@author	�ԏ� �G�P
	@brief	Windows�̃G���g���[�|�C���g
*/
#pragma once
#include "Main/Main.h"
#include <MemoryLeaks.h>
#include <stdio.h>
#include <tchar.h>
#include <locale>
#include <memory>


#include <Direct3D11.h>
/*!
	@var	g_pMain
	@brief	�ϐ��錾
	@detail	���j�[�N�|�C���^���g�����������[�N��}��
*/
std::unique_ptr<Main>g_pMain;

/*!
	@fn			_tWinMain
	@brief		�G���g���[�|�C���g
	@param[in]	�C���X�^���X�n���h��
*/
INT WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, INT)
{
	//	���������[�N�ӏ�����
#if defined DEBUG||_DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // DEBUG||_DEBUG

#ifdef _UNICODE
	// �����R�[�h�ݒ�
	setlocale(LC_ALL, "JPN");
#endif // _UNICODE

	g_pMain = std::make_unique<Main>();

	//	���𑜓x�^�C�}�[�ւ̑Ή��𒲂ׁA���Ή��Ȃ�v���O�������I��������
	if (!g_pMain->HighQualityTimer())
	{
		g_pMain.reset(nullptr);
		return 0;
	}

	//	�A�v���P�[�V���������������A���C�����[�v��
	if (g_pMain->Init(hInstance)) {
		//g_pMain->Loop();
	}


	//	�����I�J��
	g_pMain.reset(nullptr);

	D3D11::Direct3D11::GetInstance().ReportCOMs("�J���͂���");
	return 0;
}