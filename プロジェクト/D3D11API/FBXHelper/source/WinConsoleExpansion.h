/*!
	@file	WinConsoleExpansion.h
	@date	2019/05/30
	@author	番場 宥輝
	@brief	Windowsコンソール機能の拡張
*/
#pragma once
#include <Windows.h>
#include <iostream>

namespace ConsoleExpansion
{
	/*!
		@enum	""(empty)
		@brief	色
		@detail	WORD(unsigned short)型の別名。
				2進数のbit演算?
	*/
	enum :unsigned short
	{
		/*!
			@var	Red
			@brief	赤
		*/
		Red = 0x0004,

		/*!
			@var	Green
			@brief	緑
		*/
		Green = 0x0002,


		/*!
			@var	Blue
			@brief	青
		*/
		Blue = 0x0001,

		/*!
			@var	Yellow
			@brief	黄色
		*/
		Yellow = Red | Green,

		/*!
			@var	Purple
			@brief	紫
		*/
		Purple = Red | Blue,
		/*!
			@var	Cyan
			@brief	シアン
		*/
		Cyan = Green | Blue,

		/*!
			@var	White
			@brief	白
		*/
		White = Red | Green | Blue,
	};
	class WinConsoleExpansion
	{
	public:
		/*!
			@fn			SetColor
			@brief		コンソールに出力する文字列色の変更
			@param[in]	文字色
		*/
		static void SetColor(WORD color);
	};
}