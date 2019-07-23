/*
	@file	IOMesh.h
	@date	2019/05/30
	@author	�ԏ� �G�P
	@brief	���b�V���f�[�^�̂̓��o�͊֌W
	@detail	�V���O���g��
*/
#pragma once
#include <fstream>
#include <string_view>
#include <iostream>
#include "Mesh.h"
#include <SkeletonMesh.h>
#include <AnimationClip.h>

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

		//static void Output(std::string directoryPath, std::string fileName, Mesh mesh);

		static void Output(std::string fileName,SkeletonMesh mesh,AnimationClip clip);
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
	};
}
