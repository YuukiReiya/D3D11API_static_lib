/*
	@file	CustomShaderBin.cpp
	@date	2019/04/16
	@author	番場 宥輝
	@brief	.cso,cfxといったコンパイル済みシェーダーファイルのデータ
*/
#include "stdafx.h"
#include "CustomShaderBin.h"
#include "MyGame.h"

/*!
	@brief		引数付きコンストラクタ
	@param[in]	読み込むバイナリデータのパス
*/
D3D11::Graphic::CustomShaderBin::CustomShaderBin(std::string path)
	: m_Size(0)
	, m_Bin(nullptr)
{
	std::ifstream bin(path.c_str(), std::ios::in | std::ios::binary);

	if (!bin.is_open()) {
		std::string error = "\"" + path + "\" " + "is not found!";
		ErrorLog(error);
		return;
	}

	int size = static_cast<int>(bin.seekg(0, std::ios::end).tellg());
	bin.seekg(0, std::ios::beg);
	std::unique_ptr<char>data(new char[size]);
	m_Size = size;
	m_Bin = std::move(data);
}

D3D11::Graphic::CustomShaderBin::CustomShaderBin(const wchar_t * path)
{
	std::ifstream binfile(path, std::ios::in | std::ios::binary);

	if (binfile.is_open()) {
		int fsize = static_cast<int>(binfile.seekg(0, std::ios::end).tellg());
		binfile.seekg(0, std::ios::beg);
		std::unique_ptr<char> code(new char[fsize]);
		binfile.read(code.get(), fsize);
		m_Size = fsize;
		m_Bin = std::move(code);
	}

}
