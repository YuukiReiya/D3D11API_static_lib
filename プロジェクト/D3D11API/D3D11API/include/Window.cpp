/*!
	@file	Window.cpp
	@date	2019/03/06
	@author	番場 宥輝
	@brief	WindowsAPIのウィンドウ生成
*/
#include <memory>
#include "stdafx.h"
#include "Window.h"
#include "MemoryLeaks.h"
#include "MyGame.h"

/*!
	@var	g_pWindow
	@brief	変数のの参照
*/
Window*	g_pWindow = nullptr;

/*!
	@brief	プロトタイプ宣言
	@detail	コールバック関数WndProcのプロトタイプ宣言
*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

/*!
	@brief	ウィンドウプロシージャ
	@detail	コールバック関数WndProcのオーバーライド
*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	return g_pWindow->MsgProc(hWnd, msg, wParam, lParam);
}

/*!
	@brief	デストラクタ
*/
Window::~Window()
{
	g_pWindow = nullptr;
}

/*!
	@fn			メッセージプロシージャ
	@brief		ウィンドウプロシージャの中身
	@detail		win32のWndProcをオーバーライドしている
	@param[in]	ウィンドウのハンドラ
	@param[in]	メッセージの指定
	@param[in]	メッセージの追加
	@param[in]	メッセージの追加
	@return		メッセージ
*/
LRESULT Window::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	case WM_KEYDOWN:
		switch ((char)wParam)
		{
		// Escキーを押されたら
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
		break;
		// ウィンドウが破棄されたとき
	case WM_DESTROY:
		PostQuitMessage(0);	// WM_QUITメッセージをメッセージキューに送る
		break;
	}
	// デフォルトのメッセージ処理を行う
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

/*!
	@fn			イニシャライズ
	@brief		初期化
	@detail		win32のウィンドウ作成
	@param[in]	ウィンドウハンドラの参照
	@param[in]	インスタンスハンドラ
	@param[in]	生成位置x
	@param[in]	生成位置y
	@param[in]	横幅
	@param[in]	縦幅
	@param[in]	ウィンドウの名前
	@return		true:成功 false:失敗
*/
bool Window::Initialize(HWND* hWnd, HINSTANCE hInstance, INT iX, INT iY, INT iWidth, INT iHeight, LPCTSTR WindowName)
{
	g_pWindow = this;

	// ウィンドウの定義
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));
	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wc.lpszClassName = WindowName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&wc);
	// ウィンドウの作成
	*hWnd = CreateWindow(WindowName, WindowName, WS_OVERLAPPEDWINDOW,
		0, 0, iWidth, iHeight, 0, 0, hInstance, 0);
	if (!hWnd)
	{
		return false;
	}
	/*! ウィンドウの表示 */
	ShowWindow(*hWnd, SW_SHOW);
	UpdateWindow(*hWnd);

	return true;
}

/*!
	@fn			Create
	@brief		初期化
	@detail		win32のウィンドウ作成
	@param[in]	ウィンドウハンドラの参照
	@param[in]	インスタンスハンドラ
	@param[in]	生成位置x
	@param[in]	生成位置y
	@param[in]	横幅
	@param[in]	縦幅
	@param[in]	ウィンドウの名前
	@return		true:成功 false:失敗
*/
bool Window::Create(HWND hWnd, HINSTANCE hInstance, int x, int y, int width, int height, std::string name)
{
	//	自身をメッセージプロシージャに関連させる
	g_pWindow = this;

	//	ウィンドウの定義
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));
	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);

	//	文字コード変換
	auto cast = To_TString(name);
	const auto windowName = const_cast<LPTSTR>(cast.c_str());

	wc.lpszClassName = windowName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&wc);

	//	ウィンドウの作成
	hWnd = CreateWindow(windowName, windowName, WS_OVERLAPPEDWINDOW,
		0, 0, width, height, 0, 0, hInstance, 0);
	if (!hWnd)
	{
		return false;
	}

	//	ウィンドウの表示
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	return true;
}
