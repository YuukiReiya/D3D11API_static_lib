/*!
	@file	Window.cpp
	@date	2019/03/06
	@author	�ԏ� �G�P
	@brief	WindowsAPI�̃E�B���h�E����
	@detail	http://kaitei.net/winapi/window-procedures/
*/
#include <memory>
#include "stdafx.h"
#include "Window.h"
#include "MemoryLeaks.h"
#include "MyGame.h"

/*!
	@brief	using�f�B���N�e�B�u
	@using	API
*/
using namespace API;

/*!
	@var	g_pWindow
	@brief	�ϐ��̂̎Q��(�C���X�^���X�̐����͍s��Ȃ�)
*/
Window*	g_pWindow = nullptr;

/*!
	@brief	�v���g�^�C�v�錾
	@detail	�R�[���o�b�N�֐�WndProc�̃v���g�^�C�v�錾
*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

/*!
	@brief	�E�B���h�E�v���V�[�W��
	@detail	�R�[���o�b�N�֐�WndProc�̃I�[�o�[���C�h
*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	return g_pWindow->MsgProc(hWnd, msg, wParam, lParam);
}

/*!
	@brief	�f�X�g���N�^
*/
Window::~Window()
{
	g_pWindow = nullptr;
}

/*!
	@fn			���b�Z�[�W�v���V�[�W��
	@brief		�E�B���h�E�v���V�[�W���̒��g
	@detail		win32��WndProc���I�[�o�[���C�h���Ă���
	@param[in]	�E�B���h�E�̃n���h��
	@param[in]	���b�Z�[�W�̎w��
	@param[in]	���b�Z�[�W�̒ǉ�
	@param[in]	���b�Z�[�W�̒ǉ�
	@return		���b�Z�[�W
*/
LRESULT Window::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	case WM_KEYDOWN:
		switch ((char)wParam)
		{
		// Esc�L�[�������ꂽ��
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
		break;
		// �E�B���h�E���j�����ꂽ�Ƃ�
	case WM_DESTROY:
		PostQuitMessage(0);	// WM_QUIT���b�Z�[�W�����b�Z�[�W�L���[�ɑ���
		break;
	}
	// �f�t�H���g�̃��b�Z�[�W�������s��
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

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
bool Window::Create(HWND* hWnd, HINSTANCE hInstance, int x, int y, int width, int height, std::string name)
{
	//	���g�����b�Z�[�W�v���V�[�W���Ɋ֘A������
	g_pWindow = this;

	//	�E�B���h�E�̒�`
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));
	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);

	//	�����R�[�h�ϊ�
	auto cast = To_TString(name);
	const auto windowName = const_cast<LPTSTR>(cast.c_str());

	wc.lpszClassName = windowName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&wc);

	//	�E�B���h�E�̍쐬
	*hWnd = CreateWindow(windowName, windowName, WS_OVERLAPPEDWINDOW,
		0, 0, width, height, 0, 0, hInstance, 0);
	if (!*hWnd)
	{
		return false;
	}

	//	�E�B���h�E�̕\��
	ShowWindow(*hWnd, SW_SHOW);
	UpdateWindow(*hWnd);

	return true;
}
