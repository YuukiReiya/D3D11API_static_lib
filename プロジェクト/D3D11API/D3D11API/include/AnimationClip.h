/*
	@file	AnimationClip.h
	@date	2019/08/14
	@author	�ԏ� �G�P
	@brief	�A�j���[�V�����̃t���[���p�����i�[�p�̍\����(�s��p���b�g)
*/
#pragma once
#include <vector>
#include <DirectXMath.h>
#include <memory>

namespace API
{
	/*!
		@using	MatrixPalette
		@brief	�ʖ���`
	*/
	using MatrixPalette = std::vector<DirectX::XMMATRIX>;

	/*!
		@struct	AnimationClip
		@brief	�X�L���A�j���[�V�����̃t���[���p�����
	*/
	struct AnimationClip
	{
		/*!
			@var	matrixPalette
			@brief	�t���[���p���̍s��p���b�g
			@detail	matrixPalette[�W���C���g�ԍ�][�t���[��]
		*/
		std::vector<MatrixPalette>matrixPalette;

		/*!
			@fn			Load
			@brief		�A�j���[�V�����f�[�^�̓ǂݍ���
			@param[in]	�o�C���h����A�j���[�V�����N���b�v
			@param[in]	�t�@�C���p�X
			@return		true:���� false:���s
		*/
		static bool Load(AnimationClip*animClip, std::string filePath);

		/*!
			@fn			Load
			@brief		�A�j���[�V�����f�[�^�̓ǂݍ���
			@param[in]	�t�@�C���p�X
			@return		true:���� false:���s
		*/
		inline bool Load(std::string filePath) { return Load(this, filePath); }
	};
}