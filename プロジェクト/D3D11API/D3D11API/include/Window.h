/*!
	@file	Window.h
	@date	2019/03/06
	@author	�ԏ� �G�P
	@brief	WindowsAPI�̃E�B���h�E����
*/
#pragma once
#pragma comment(lib,"winmm.lib")
#include "stdafx.h"
#include <Windows.h>
#include <string>

/*! API�֘A�̖��O��ԂɊ܂߂� */
namespace API {
	
	class Window
	{
	public:
		/*!
			@brief	�R���X�g���N�^
		*/
		Window() = default;

		/*!
			@brief	�f�X�g���N�^
		*/
		~Window();

		/*!
			@fn			MsgProc
			@brief		�E�B���h�E�v���V�[�W���̒��g
			@detail		win32��WndProc���I�[�o�[���C�h���Ă���
			@param[in]	�E�B���h�E�̃n���h��
			@param[in]	���b�Z�[�W�̎w��
			@param[in]	���b�Z�[�W�̒ǉ�
			@param[in]	���b�Z�[�W�̒ǉ�
			@return		���b�Z�[�W
		*/
		LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

		/*!
			@fn			Create
			@brief		������
			@detail		win32�̃E�B���h�E�쐬
			@param[in]	�E�B���h�E�n���h���̎Q��
			@param[in]	�C���X�^���X�n���h��
			@param[in]	�����ʒux
			@param[in]	�����ʒuy
			@param[in]	����
			@param[in]	�c��
			@param[in]	�E�B���h�E�̖��O
			@return		true:���� false:���s
		*/
		bool Create(HWND hWnd, HINSTANCE hInstance, int x, int y, int width, int height, std::string name);

	};
}
