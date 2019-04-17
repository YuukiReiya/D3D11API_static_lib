/*
	@file	SpriteShader
	@date	2019/04/13
	@author	�ԏ� �G�P
	@brief	�X�v���C�g�̃f�t�H���g�V�F�[�_�[
*/
#pragma once
#include "AbstractShader.h"

namespace D3D11
{
	namespace Graphic {

		class SpriteShader
			: public AbstractShader
		{
		public:
			/*!
				@brief	�R���X�g���N�^
			*/
			SpriteShader();

			/*!
				@brief	�f�X�g���N�^
			*/
			~SpriteShader();

			/*!
				@fn		Setup
				@brief	�v���R���p�C���ς݃V�F�[�_�[�t�@�C���𗘗p���ăZ�b�g�A�b�v���s��
				@detail	�I�[�o�[���C�h
				@TODO	�ǂݍ��߂Ȃ�.cso�t�@�C�������݂��A���삪���肵�Ȃ��B
			*/
			HRESULT Setup()override final;

			/*!
				@fn			Setup
				@brief		�t�@�C���̊K�w���s���A�v���R���p�C���ς݃V�F�[�_�[�t�@�C���𗘗p�����Z�b�g�A�b�v���s��
				@detail		�I�[�o�[���[�h
				@param[in]	�t�@�C���̊K�w�p�X
				@NOTE		�V�F�[�_�[�t�@�C���̔z�u�ʒu���l�ɂ���ĕς���Ă��܂����߁A�f�B���N�g�������ł��ݒ�o����悤�ɂ���
			*/
			HRESULT Setup(std::string& directory);

			/*!
				@fn		DynamicSetup
				@brief	���I�R���p�C���𗘗p�����Z�b�g�A�b�v���s��
				@detail	�I�[�o�[���C�h
			*/
			HRESULT DynamicSetup()override final;

			/*!
				@fn			DynamicSetup
				@brief		�t�@�C���̊K�w���s���A���I�R���p�C���𗘗p�����Z�b�g�A�b�v���s��
				@detail		�I�[�o�[���[�h
				@param[in]	�t�@�C���̊K�w�p�X
				@NOTE		�V�F�[�_�[�t�@�C���̔z�u�ʒu���l�ɂ���ĕς���Ă��܂����߁A�f�B���N�g�������ł��ݒ�o����悤�ɂ���
			*/
			HRESULT DynamicSetup(std::string& directory);
		private:
			/*!
				@fn			SetupDirectory
				@brief		�t�@�C�����Q�Ƃ���f�B���N�g����ݒ�
				@param[in]	�Q�Ƃ���f�B���N�g��
			*/
			void SetupDirectory(std::string& path);

			/*!
				@fn		CreateConstantBuffer
				@brief	�R���X�^���g�o�b�t�@�̍쐬
				@return	S_OK:���� E_FAIL:���s
			*/
			HRESULT CreateConstantBuffer();

			/*!
				@fn			CreateInputLayout
				@brief		���_���C�A�E�g�̍쐬
				@param[in]	�R���p�C���ς݃u���u
				@return		S_OK:���� E_FAIL:���s
			*/
			HRESULT CreateInputLayout(ID3DBlob*pBlob);

			/*!
				@fn			CreateInputLayout	
				@brief		���_���C�A�E�g�̍쐬
				@param[in]	�R���p�C���ς݃V�F�[�_�[�t�@�C��(�o�C�i���f�[�^)
				@return		S_OK:���� E_FAIL:���s
			*/
			HRESULT CreateInputLayout(CustomShaderBin*bin);


			/*!
				@var	m_Directory
				@brief	�V�F�[�_�[�t�@�C���̊K�w������
			*/
			std::string m_Directory;

			/*!
				@var	c_HLSL
				@brief	HLSL�̃t�@�C����
			*/
			std::string_view c_HLSL			= "SpriteShader.hlsl";

			/*!
				@var	c_CSO
				@brief	CSO�̃t�@�C����
			*/
			std::string_view c_CSO			= "SpriteShader.cso";

			/*!
				@var	c_VSEntryName
				@brief	���_�V�F�[�_�[�̃G���g���[�|�C���g�̖��O
			*/
			std::string_view c_VSEntryName	= "VS";
			
			/*!
				@var	c_PSEntryName
				@brief	�s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g�̖��O
			*/
			std::string_view c_PSEntryName	= "PS";
			
			/*!
				@var	c_VSProfile
				@brief	���_�V�F�[�_�[�̃v���t�@�C��
			*/
			std::string_view c_VSProfile	= "vs_5_0";

			/*!
				@var	c_PSProfile
				@brief	�s�N�Z���V�F�[�_�[�̃v���t�@�C��
			*/
			std::string_view c_PSProfile	= "ps_5_0";
		};
	}
}