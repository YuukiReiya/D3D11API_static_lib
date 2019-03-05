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
#include <AudioMaster.h>
#include <Camera.h>
#include "Main.h"

/*!
	@brief	���O���
*/
using namespace D3D11;

/*!
	@brief	�R���X�g���N�^
*/
Main::Main() :m_pWindow(nullptr)
{
}

/*!
	@brief	�f�X�g���N�^
*/
Main::~Main()
{
	//delete(wave);
	Release();
	//SAFE_DELETE(m_pWindow);
}

/*!
	@fn			Init
	@brief		������
	@param[in]	�C���X�^���X�n���h��
*/

bool Main::Init(HINSTANCE hInstance)
{
	m_pWindow = new Window;

	// UNICODE�A�}���`�o�C�g���Ή��p������ϊ�
	auto tmp = To_TString(c_AppName.data());
	const auto appName = const_cast<LPTSTR>(tmp.c_str());

	if (!m_pWindow->Initialize(&m_hWnd, hInstance,
		0, 0, c_WindowWidth, c_WindowHeight, appName)) {
		return false;
	}

	// �f�o�C�X������

	return true;
}

/*!
	@brief	���
*/
void Main::Release()
{
}

/*!	
	@brief	�A�v���P�[�V���������̓����
*/
void Main::Loop()
{
	// FPS�̏�����
	SetUpFPS();

	// �J�����̏�����
	Camera::GetInstance().Initialize({ 0,0,-1 });

	HRESULT hr = NULL;

	if (FAILED(hr)) { ErrorLog(""); }

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
	@brief	���𑜓x�^�C�}�[���Ή����Ă��邩�m�F
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
	@brief	�A�v���P�[�V��������
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
	// FPS�̒������@�ŕ���
#ifdef Vsync
	// �X�V����
	Update();
	// �`�揈��
	Render();
#else
	// �X�V����
	Update();
	// �`�揈��
	Render();
	// FPS����
	SleepApp();
#endif // Vsync
}

/*!
	@brief	�X�V
*/
void Main::Update()
{

}

/*!
	@brief	�`��
*/
void Main::Render()
{
	// �`��^�[�Q�b�g�̃N���A
	Direct3D11::GetInstance().Clear();

	// �V�[���̕`��

	// ��ʍX�V 
	Direct3D11::GetInstance().Present();
}

/*!
	@brief	FPS�����̏���
*/
void Main::SetUpFPS()
{
	m_FrameTime = 0;
	// ����̌v�Z
	QueryPerformanceCounter(&m_TimeStart);
}
/*!
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
