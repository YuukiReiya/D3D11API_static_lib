/*!
	@file	Window.cpp
	@date	2018/02/22
	@author	番場 宥輝
	@brief	WindowsAPIのウィンドウ生成
*/
#include "stdafx.h"
#include "Window.h"
#include "MemoryLeaks.h"

/*!
	@brief	プロトタイプ宣言
	@detail	コールバック関数WndProcのプロトタイプ宣言
*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

/*!
	@var	g_pWindow
	@brief	ウィンドウのインスタンス宣言
*/
Window*	g_pWindow = NULL;

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
			/*! Escキーを押されたら */
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
		break;
		/*! ウィンドウが破棄されたとき */
	case WM_DESTROY:
		PostQuitMessage(0);	/*!< WM_QUITメッセージをメッセージキューに送る */
		break;
	}
	/*! デフォルトのメッセージ処理を行う */
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
	/*! ウィンドウの定義 */
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
	/*! ウィンドウの作成 */
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

bool Window::Create(HWND hWnd, HINSTANCE hInstance, int x, int y, int width, int height, const wchar_t * name)
{	
	g_pWindow = this;
	/*! ウィンドウの定義 */
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));
	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wc.lpszClassName = name;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&wc);
	/*! ウィンドウの作成 */
	hWnd = CreateWindow(name, name, WS_OVERLAPPEDWINDOW,
		0, 0, width, height, 0, 0, hInstance, 0);
	if (!hWnd)
	{
		return false;
	}

	/*! ウィンドウの表示 */
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	return false;
}
