/*
	@file	CustomShaderBin.cpp
	@date	2019/04/16
	@author	�ԏ� �G�P
	@brief	.cso,cfx�Ƃ������R���p�C���ς݃V�F�[�_�[�t�@�C���̃f�[�^
*/
#include "stdafx.h"
#include "CustomShaderBin.h"
#include "MyGame.h"

/*!
	@brief		�����t���R���X�g���N�^
	@param[in]	�ǂݍ��ރo�C�i���f�[�^�̃p�X
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
