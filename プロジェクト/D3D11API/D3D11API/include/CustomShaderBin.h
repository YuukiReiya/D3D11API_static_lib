/*
	@file	CustomShaderBin.h
	@date	2019/04/16
	@author	番場 宥輝
	@brief	.cso,cfxといったコンパイル済みシェーダーファイルのバイナリデータ用クラス
*/
#pragma once
#include <fstream>
#include <string>
#include <memory>
#include <tchar.h>

namespace D3D11
{
	namespace Graphic {

		struct CustomShaderBin
		{
			/*!
				@brief		引数付きコンストラクタ
				@param[in]	読み込むバイナリデータのパス
			*/
			CustomShaderBin(std::string path);
			CustomShaderBin(const wchar_t* path);

			/*!
				@fn		GetPtr
				@brief	バイナリデータのポインタを取得
				@return	バイナリデータのポインタ
			*/
			inline const void*GetPtr() { return m_Bin.get(); }

			/*!
				@fn		GetSize
				@brief	読み込んだ文字列ストリームのサイズを取得
				@return	サイズ
			*/
			inline const int GetSize()const { return m_Size; }
		private:
			/*!
				@var	m_Size
				@brief	バッファサイズ
			*/
			int m_Size;

			/*!
				@var	m_Bin
				@brief	読み込んだバイナリデータ
			*/
			std::unique_ptr<char>m_Bin;
		};
	}
}