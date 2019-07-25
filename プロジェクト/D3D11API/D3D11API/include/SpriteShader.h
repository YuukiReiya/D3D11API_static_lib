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
			explicit SpriteShader();

			/*!
				@brief	�f�X�g���N�^
			*/
			~SpriteShader();

			/*!
				@fn		Setup
				@brief	�v���R���p�C���ς݃V�F�[�_�[�t�@�C���𗘗p���ăZ�b�g�A�b�v���s��
				@detail	�I�[�o�[���C�h
			*/
			HRESULT Setup()override final;

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
				@return		S_OK:���� E_FAIL:���s
			*/
			HRESULT CreateInputLayout();

			/*!
				@var	m_Directory
				@brief	�V�F�[�_�[�t�@�C���̊K�w������
			*/
			std::string m_Directory;

			/*!
				@var	c_HLSL
				@brief	HLSL�̃t�@�C����
			*/
			static constexpr std::string_view c_HLSL = "SpriteShader.hlsl";

			/*!
				@var	c_VSEntryName
				@brief	���_�V�F�[�_�[�̃G���g���[�|�C���g�̖��O
			*/
			static constexpr std::string_view c_VSEntryName	= "VS";
			
			/*!
				@var	c_PSEntryName
				@brief	�s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g�̖��O
			*/
			static constexpr std::string_view c_PSEntryName	= "PS";
			
			/*!
				@var	c_VSProfile
				@brief	���_�V�F�[�_�[�̃v���t�@�C��
			*/
			static constexpr std::string_view c_VSProfile	= "vs_5_0";

			/*!
				@var	c_PSProfile
				@brief	�s�N�Z���V�F�[�_�[�̃v���t�@�C��
			*/
			static constexpr std::string_view c_PSProfile	= "ps_5_0";

			/*!
				@var	c_InputLayout
				@brief	�C���v�b�g���C�A�E�g
			*/
			//static constexpr D3D11_INPUT_ELEMENT_DESC c_InputLayout[]=
			//{
			//	{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			//	{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
			//};
		};
	}
}