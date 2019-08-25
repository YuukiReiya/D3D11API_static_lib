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
#include "Mesh.h"
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
			@fn			Output
			@brief		�Ǝ��`���̏o��
			@param[in]	�t�@�C���̃f�B���N�g��
			@param[in]	�t�@�C����(���g���q����)
			@param[in]	���b�V�����
			@TODO		�o�͌`���̌������Ɗ֐��������s��...
		*/
		static void Output(std::string directoryPath, std::string fileName, Mesh mesh);

		/*!
			@fn			OutputSkinMesh
			@brief		�Ǝ��`���̃X�L�����b�V�����o��
			@param[in]	�t�@�C���p�X
			@param[in]	���_�C���f�b�N�X
			@param[in]	���_���
			@param[in]	�W���C���g
		*/
		//static void OutputSkinMesh(std::string filePath, std::vector<uint32_t>indices, std::vector<D3D11::Graphic::SkinnedVertex>vertices, std::vector<D3D11::Graphic::Joint>joints);

		/*!
			@fn			OutputSkinMesh
			@brief		�Ǝ��`���̃X�L�����b�V�����o��
			@param[in]	�t�@�C���p�X
			@param[in]	���_�C���f�b�N�X
			@param[in]	���_���
		*/
		static void OutputSkinMesh(std::string filePath, std::vector<uint32_t>indices, std::vector<D3D11::Graphic::SkinnedVertex>vertices);

		static void OutputAnimation(std::string filePath, API::AnimationClip clips);
		static void Write(std::vector<DirectX::XMMATRIX>mat);
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
	};
}
