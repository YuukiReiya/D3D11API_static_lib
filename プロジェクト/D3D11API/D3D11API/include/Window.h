/*!
	@file	Window.h
	@date	2019/03/06
	@author	番場 宥輝
	@brief	WindowsAPIのウィンドウ生成
*/
#pragma once
#pragma comment(lib,"winmm.lib")
#include "stdafx.h"
#include <Windows.h>
#include <string>

/*! API関連の名前空間に含める */
namespace API {
	
	class Window
	{
	public:
		/*!
			@brief	コンストラクタ
		*/
		Window() = default;

		/*!
			@brief	デストラクタ
		*/
		~Window();

		/*!
			@fn			MsgProc
			@brief		ウィンドウプロシージャの中身
			@detail		win32のWndProcをオーバーライドしている
			@param[in]	ウィンドウのハンドラ
			@param[in]	メッセージの指定
			@param[in]	メッセージの追加
			@param[in]	メッセージの追加
			@return		メッセージ
		*/
		LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

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
		bool Create(HWND hWnd, HINSTANCE hInstance, int x, int y, int width, int height, std::string name);

	};
}
