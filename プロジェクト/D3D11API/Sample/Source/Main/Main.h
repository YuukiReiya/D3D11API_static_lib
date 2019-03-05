/*!
	@file	Main.h
	@date	2018/02/08
	@author	�ԏ� �G�P
	@brief	�A�v���P�[�V�������C���G���g���[�|�C���g
*/
#pragma once

#include <Windows.h>
#include <mmsystem.h>
#include <string_view>
#include <Window.h>

class Main
{
public:
	Main();
	~Main();

	/*!
		@fn			Init
		@brief		������
		@param[in]	�C���X�^���X�n���h��
		@return		true:���� false:���s
	*/
	bool Init(HINSTANCE hInstance);

	/*!
		@fn		Release
		@brief	�������
	*/
	void Release();

	/*!
		@fn		Loop
		@brief	���C�����[�v����
	*/
	void Loop();

	/*!
		@fn		HighQualityTimer
		@brief	���𑜓x�^�C�}�[�̑Ή����m�F
		@return	true:�Ή� false:���Ή�
	*/
	bool HighQualityTimer();

	/*!
		@fn		SetUpFPS
		@brief	FPS�����̏���
	*/
	void SetUpFPS();
private:
	/*!
		@fn		App
		@brief	�A�v���P�[�V�����̏���
	*/
	void App();

	/*!
		@fn		Update
		@brief	�X�V����
	*/
	void Update();

	/*!
		@fn		Render
		@brief	�`�揈��
	*/
	void Render();

	/*!
		@fn		SleepApp
		@brief	�A�v���P�[�V���������̂��x��
	*/
	void SleepApp();

	/*!
		@var	c_AppName
		@brief	�A�v���P�[�V�����̖��O
		@value	Sample
	*/
	static constexpr std::string_view c_AppName = "Sample";

	/*!
		@var	c_FPS
		@brief	�Q�[���̃^�[�Q�b�gFPS
		@value	60
	*/
	static constexpr int	c_FPS = 60;

	/*!
		@var	c_MinFrameTime
		@brief	1�t���[���ӂ�̌o�ߎ���
		@value	1.0f / Main::c_FPS
	*/
	static constexpr float	c_MinFrameTime = 1.0f / Main::c_FPS;

	/*!
		@var	c_WindowWidth
		@brief	�E�B���h�E�̉���
		@value	1920
	*/
	static constexpr int c_WindowWidth	= 1920;

	/*!
		@var	c_WindowHeight
		@brief	�E�B���h�E�̏c��
		@value	1080
	*/
	static constexpr int c_WindowHeight = 1080;

	/*!
		@var	m_hWnd
		@brief	�E�B���h�E�Y�n���h��
	*/
	HWND m_hWnd;

	/*!
		@var	m_FrameTime
		@brief	�o�ߎ���
	*/
	float m_FrameTime;

	/*!
		@var	m_TimeStart
		@brief	�J�n���Ԃ̃J�E���^
	*/
	LARGE_INTEGER m_TimeStart;

	/*!
		@var	m_TimeEnd
		@brief	�I�����Ԃ̃J�E���^
	*/
	LARGE_INTEGER m_TimeEnd;

	/*!
		@var	m_TimeFreq
		@brief	���𑜓x�^�C�}�[�̎��g��
	*/
	LARGE_INTEGER m_TimeFreq;

	Window* m_pWindow;
};

