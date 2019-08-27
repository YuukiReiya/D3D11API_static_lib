/*
	@file	IOMesh.h
	@date	2019/05/30
	@author	�ԏ� �G�P
	@brief	���b�V���f�[�^�̂̓��o�͊֌W
	@detail	�V���O���g��

	TODO:�o�͊֐��ɒ����K�{(�ϐ����A�֐�����etc.)
*/
#pragma once
#include <fstream>
#include <string_view>
#include <iostream>
#include <AnimationClip.h>
#include <SkinnedVertex.h>
#include <Joint.h>

namespace Utility{
	class IOMesh
	{
	public:
		/*!
			@fn			Delete
			@brief		yfm�t�@�C���폜
			@param[in]	�t�@�C���̃f�B���N�g��
			@param[in]	�t�@�C����(���g���q����)
		*/
		static void Delete(std::string directoryPath, std::string fileName);

		/*!
			@fn			OutputMesh
			@brief		�Ǝ��`���̃X�L�����b�V�����o��
			@param[in]	�o�̓t�@�C���p�X
			@param[in]	���_�C���f�b�N�X
			@param[in]	���_���
		*/
		static void OutputMesh(std::string filePath, std::vector<uint32_t>indices, std::vector<D3D11::Graphic::SkinnedVertex>vertices);

		/*!
			@fn			OutputAnimation
			@brief		�Ǝ��`���̃A�j���[�V�������o��
			@param[in]	�o�̓t�@�C���p�X
			@param[in]	�A�j���[�V�����N���b�v
		*/
		static void OutputAnimation(std::string filePath, API::AnimationClip clips);

		/*!
			@fn			OutputTextureName
			@brief		�g�p����Ă���e�N�X�`������.ini�t�@�C���Ƃ��ď����o��
			@param[in]	�o�̓t�@�C���p�X
			@param[in]	�g�p����Ă���e�N�X�`���̉ϒ��z��
		*/
		static void OutputTextureName(std::string filePath, std::vector<std::string> textureNames);
	private:
		/*!
			@brief	�R���X�g���N�^�폜
		*/
		IOMesh() = delete;

		/*!
			@brief	�f�X�g���N�^�폜
		*/
		~IOMesh() = delete;

		/*!
			@var	c_Delimiter
			@brief	�g���q�̋�؂蕶��
		*/
		static constexpr std::string_view c_Delimiter = ".";

		/*!
			@brief	�Ǝ��`���̊g���q
			@detail	y(uuki)f(ormat)m(esh)
		*/
		static constexpr std::string_view c_Extension = "yfm";

		/*!
			@brief	�A�j���[�V�����f�[�^�̓Ǝ��g���q
			@detail	a(nimation)c(lip)
		*/
		static constexpr std::string_view c_AnimExtension = "ac";

		/*!
			@brief	�e�N�X�`���̃p�X��������g���q(.ini)
			@detail	�ʓr�ǂݍ��݂ł���悤�ɐݒ�t�@�C��(.ini)�Ƃ��ď����o���B
		*/
		static constexpr std::string_view c_IniExtension = "ini";
	};
}
