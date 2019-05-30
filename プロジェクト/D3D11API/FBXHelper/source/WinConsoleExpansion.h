/*!
	@file	WinConsoleExpansion.h
	@date	2019/05/30
	@author	�ԏ� �G�P
	@brief	Windows�R���\�[���@�\�̊g��
*/
#pragma once
#include <Windows.h>
#include <iostream>

namespace ConsoleExpansion
{
	/*!
		@enum	""(empty)
		@brief	�F
		@detail	WORD(unsigned short)�^�̕ʖ��B
				2�i����bit���Z?
	*/
	enum :unsigned short
	{
		/*!
			@var	Red
			@brief	��
		*/
		Red = 0x0004,

		/*!
			@var	Green
			@brief	��
		*/
		Green = 0x0002,


		/*!
			@var	Blue
			@brief	��
		*/
		Blue = 0x0001,

		/*!
			@var	Yellow
			@brief	���F
		*/
		Yellow = Red | Green,

		/*!
			@var	Purple
			@brief	��
		*/
		Purple = Red | Blue,
		/*!
			@var	Cyan
			@brief	�V�A��
		*/
		Cyan = Green | Blue,

		/*!
			@var	White
			@brief	��
		*/
		White = Red | Green | Blue,
	};
	class WinConsoleExpansion
	{
	public:
		/*!
			@fn			SetColor
			@brief		�R���\�[���ɏo�͂��镶����F�̕ύX
			@param[in]	�����F
		*/
		static void SetColor(WORD color);
	};
}