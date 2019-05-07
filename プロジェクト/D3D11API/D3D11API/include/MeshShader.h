/*
	@file	MeshShader
	@date	2019/05/04
	@author	�ԏ� �G�P
	@brief	���b�V���̃f�t�H���g�V�F�[�_�[
*/
#pragma once
#include "AbstractShader.h"

namespace D3D11 {
	namespace Graphic {
		class MeshShader
			: public AbstractShader
		{
		public:
			/*!
				@brief	�R���X�g���N�^
			*/
			MeshShader();

			/*!
				@brief	�f�X�g���N�^
			*/
			~MeshShader();

			/*!
				@fn		Setup
				@brief	�v���R���p�C���ς݃V�F�[�_�[�t�@�C���𗘗p���ăZ�b�g�A�b�v���s��
				@detail	�I�[�o�[���C�h
				@TODO	�ǂݍ��߂Ȃ�.cso�t�@�C�������݂��A���삪���肵�Ȃ��B
			*/
			HRESULT Setup()override final;

			/*!
				@fn		DynamicSetup
				@brief	���I�R���p�C���𗘗p�����Z�b�g�A�b�v���s��
				@detail	�I�[�o�[���C�h
			*/
			HRESULT DynamicSetup()override final;

		private:
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
				@var	m_Directory
				@brief	�V�F�[�_�[�t�@�C���̊K�w������
			*/
			std::string m_Directory;

			/*!
				@var	c_HLSL
				@brief	HLSL�̃t�@�C����
			*/
			static constexpr std::string_view c_HLSL = "MeshShader.hlsl";

			/*!
				@var	c_CSO
				@brief	CSO�̃t�@�C����
			*/
			static constexpr std::string_view c_CSO = "SpriteShader.cso";

			/*!
				@var	c_VSEntryName
				@brief	���_�V�F�[�_�[�̃G���g���[�|�C���g�̖��O
			*/
			static constexpr std::string_view c_VSEntryName = "VS";

			/*!
				@var	c_PSEntryName
				@brief	�s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g�̖��O
			*/
			static constexpr std::string_view c_PSEntryName = "PS";

			/*!
				@var	c_VSProfile
				@brief	���_�V�F�[�_�[�̃v���t�@�C��
			*/
			static constexpr std::string_view c_VSProfile = "vs_5_0";

			/*!
				@var	c_PSProfile
				@brief	�s�N�Z���V�F�[�_�[�̃v���t�@�C��
			*/
			static constexpr std::string_view c_PSProfile = "ps_5_0";

		};

	}
}
