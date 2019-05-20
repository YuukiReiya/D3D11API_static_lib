/*!
	@file	Main.cpp
	@date	2018/02/08
	@author	�ԏ� �G�P
	@brief	�A�v���P�[�V�������C���G���g���[�|�C���g
*/
#include <tchar.h>
#include <Direct3D11.h>
#include <MyGame.h>
#include <MemoryLeaks.h>
#include <Debug.h>
#include <AudioMaster.h>
#include <Camera.h>
#include <Keyboard.h>
#include "Main.h"
#include <StructShaderBase.h> 
#include "../Scene/Root/SceneRoot.h"

/*!
	@brief	using�f�B���N�e�B�u
	@using	D3D11
*/
using namespace D3D11;

/*!
	@brief	using�f�B���N�e�B�u
	@using	API
*/
using namespace API;

/*!
	@brief	�R���X�g���N�^
*/
Main::Main()
	:m_hWnd(nullptr),
	m_hInstance(nullptr),
	m_pWindow(nullptr)
{
}

/*!
	@brief	�f�X�g���N�^
*/
Main::~Main()
{
	Release();
}

/*!
	@fn			Init
	@brief		������
	@param[in]	�C���X�^���X�n���h��
	@return		true:���� false:���s
*/
bool Main::Init(HINSTANCE hInstance)
{
	m_pWindow = std::make_unique<Window>();
	m_hInstance = hInstance;

	//	�E�B���h�E����
	if (!m_pWindow->Create(&m_hWnd, m_hInstance, 0, 0,
		c_WindowWidth, c_WindowHeight, c_AppName.data())) {
		ErrorLog("Window generation failed!");
		return false;
	}

	//	�E�B���h�E�T�C�Y�̎w��
	Direct3D11::GetInstance().SetWindowSize(c_WindowWidth, c_WindowHeight);

	//	�����_�[�̃N���A�J���[
	Direct3D11::GetInstance().SetClearColor(Color(0.5f, 0.5f, 0.5f, 1));

	//	�f�o�C�X������
	HRESULT hr = NULL;
	hr = Direct3D11::GetInstance().Initialize(m_hWnd);
	if (FAILED(hr)) {
		ErrorLog("Device initialization failed!");
		return false;
	}

	//	�f�o�b�O�R���\�[���\��
#if defined DEBUG_CONSOLE

	//	�R���\�[���E�B���h�E�̕\��
	if (Debug::CreateConsoleWindow())
	{
		//	�f�o�b�O�R���\�[���E�B���h�E�̕���{�^��������
		Debug::DisableCloseButton();
		return true;
	}
	//	�R���\�[���̕\�����s
	else 
	{
		ErrorLog("Debug console generation failed!");
		return false;
	}
	
#endif // DEBUG

	return true;
}

/*!
	@fn		Release
	@brief	�����I�ȉ������
*/
void Main::Release()
{
	SceneRoot::GetInstance().Finalize();
	Direct3D11::GetInstance().Release();
	m_pWindow.reset(nullptr);
}

/*!	
	@fn		Loop
	@brief	�A�v���P�[�V���������̓����
*/
void Main::Loop()
{
	// FPS�̏�����
	SetUpFPS();

	// �J�����̏�����
	Camera::GetInstance().Initialize({ c_WindowWidth,c_WindowHeight }, { 0,0,-10 });

	//	�V�[���̏�����
	SceneRoot::GetInstance().Initialize();

	HRESULT hr = NULL;

	// ���b�Z�[�W���[�v
	MSG msg{ 0 };
	ZeroMemory(&msg, sizeof(&msg));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
		//	App����
			App(); 
		}
	}

}

/*
	@fn		HighQualityTimer
	@brief	���𑜓x�^�C�}�[�̑Ή����m�F
	@return	true:�Ή� false:���Ή�
*/
bool Main::HighQualityTimer()
{
	// QueryPerformanceFrequency�̑Ή�������
	if (!QueryPerformanceFrequency(&m_TimeFreq)) {
		ErrorLog("This PC is not fit QueryPerformanceFrequency!\n"
			"Sorry this project is exit!");
		return false;
	}

	return true;
}

/*!
	@fn		App
	@brief	�A�v���P�[�V�����̏���
*/
void Main::App()
{
	// FPS�m�F
#ifdef _DEBUG
	static DWORD time = 0;
	static int frame = 0;
	frame++;
	char str[50];
	sprintf_s(str, "fps = %d", frame);
	if (timeGetTime() - time > 1000) {
		time = timeGetTime();
		frame = 0;

		auto tmp = To_TString(str);
		const auto fps = const_cast<LPTSTR>(tmp.c_str());
		if (!SetWindowText(m_hWnd, fps)) {
			WindowsErrorLog();
		}
	}
#endif
	//	FPS�̒������@�ŕ���(Vsync)
	if (Direct3D11::GetInstance().isVsync) {

		// �X�V����
		Update();
		// �`�揈��
		Render();
	}
	//	�A�v���P�[�V��������
	else {

		// �X�V����
		Update();
		// �`�揈��
		Render();
		// FPS����
		SleepApp();
	}
}

/*!
	@fn		Update
	@brief	�X�V����
*/
void Main::Update()
{
	//	�L�[�{�[�h�̓��̓o�b�t�@�̍X�V
	Keyboard::UpdateBuffer();

	//	�V�[���̍X�V
	SceneRoot::GetInstance().Update();
}

/*!
	@fn		Render
	@brief	�`�揈��
*/
void Main::Render()
{
	// �`��^�[�Q�b�g�̃N���A
	Direct3D11::GetInstance().Clear();

	// �V�[���̕`��
	SceneRoot::GetInstance().Render();

	// ��ʍX�V 
	Direct3D11::GetInstance().Present();
}

/*!
	@fn		SetUpFPS
	@brief	FPS�����̏���
*/
void Main::SetUpFPS()
{
	m_FrameTime = 0;
	// ����̌v�Z
	QueryPerformanceCounter(&m_TimeStart);
}
/*!
	@fn		SleepApp
	@brief	�A�v���P�[�V���������̂��x��
*/
void Main::SleepApp()
{
	//	���̎��Ԃ��擾
	QueryPerformanceCounter(&m_TimeEnd);
	
	// �o�ߎ���
	m_FrameTime = static_cast<float>(m_TimeEnd.QuadPart - m_TimeStart.QuadPart) / static_cast<float>(m_TimeFreq.QuadPart);
	if (m_FrameTime < c_MinFrameTime)
	{
		DWORD SleepTime = static_cast<DWORD>((c_MinFrameTime - m_FrameTime) * 1000);// �Y����
		timeBeginPeriod(1);	// ���𐫔\���グ��
		Sleep(SleepTime);	// ���x��
		timeEndPeriod(1);	// �߂�
	}
	QueryPerformanceCounter(&m_TimeStart);
}
