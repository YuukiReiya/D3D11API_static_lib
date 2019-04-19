/*
	@file	CustomShaderBin.h
	@date	2019/04/16
	@author	�ԏ� �G�P
	@brief	.cso,cfx�Ƃ������R���p�C���ς݃V�F�[�_�[�t�@�C���̃o�C�i���f�[�^�p�N���X
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
				@brief		�����t���R���X�g���N�^
				@param[in]	�ǂݍ��ރo�C�i���f�[�^�̃p�X
			*/
			CustomShaderBin(std::string path);
			CustomShaderBin(const wchar_t* path);

			/*!
				@fn		GetPtr
				@brief	�o�C�i���f�[�^�̃|�C���^���擾
				@return	�o�C�i���f�[�^�̃|�C���^
			*/
			inline const void*GetPtr() { return m_Bin.get(); }

			/*!
				@fn		GetSize
				@brief	�ǂݍ��񂾕�����X�g���[���̃T�C�Y���擾
				@return	�T�C�Y
			*/
			inline const int GetSize()const { return m_Size; }
		private:
			/*!
				@var	m_Size
				@brief	�o�b�t�@�T�C�Y
			*/
			int m_Size;

			/*!
				@var	m_Bin
				@brief	�ǂݍ��񂾃o�C�i���f�[�^
			*/
			std::unique_ptr<char>m_Bin;
		};
	}
}