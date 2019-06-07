/*
	@file	Material.h
	@date	2019/06/06
	@author	�ԏ� �G�P
	@brief	�}�e���A��
*/
#pragma once
#include "Texture.h"

/*!
	@namespace	API
	@brief		API���O��ԂɊ܂߂�
*/
namespace API {
	class Material
	{
	public:
		/*!
			@brief	�R���X�g���N�^
		*/
		Material();

		/*!
			@brief	���[�u�R���X�g���N�^
		*/
		Material(const Material&ref) { *this = ref; }

		/*!
			@brief	�f�X�g���N�^
		*/
		~Material();

		/*!
			@fn			SetupTexture
			@brief		�e�N�X�`���̐ݒ�
			@detail		Texture��Initialize���s���Ă��邾���̃��b�s���O�֐�
			@param[in]	�e�N�X�`���̃t�@�C���p�X
			@param[in]	�ݒ肷��t�B���^�����O(�A�h���b�V���O���[�h)
			@param[in]	�ݒ肷��^�C�����[�h
			@return		S_OK:����	E_FAIL:���s

		*/
		inline HRESULT SetupTexture(std::string texturePath, Texture::FilteringMode fMode = Texture::FilteringMode::Bilinear, Texture::TileMode tMode = Texture::TileMode::Clamp) { return m_pTexture->Initialize(texturePath, fMode, tMode); }

		/*!
			@fn			SetupTexture
			@brief		�e�N�X�`���̐ݒ�
			@detail		�����Ńe�N�X�`���̓����Texture�̃X�}�[�g�|�C���^���擾
			@param[in]	�e�N�X�`���̃|�C���^
		*/
		inline void SetupTexture(Texture*texture) { m_pTexture = std::make_shared<Texture>(); };
		
		/*!
			@fn		GetSharedPtr
			@brief	�X�}�[�g�|�C���^(�V�F�A�[�h�|�C���^)�̃Q�b�^�[
			@note	�`��I�u�W�F�N�g�ɂ�"weak_ptr"���������邽�߃e�N�X�`���̎Q�Ǝ擾�p
		*/
		inline std::shared_ptr<Material*>GetSharedPtr() { return m_pShared; }
		
		/*!
			@fn		GetSamplerState
			@brief	�T���v���[�X�e�[�g�̎擾
			@detail	Texture�̃Q�b�^�[�̃��b�p�[
			@return	�����o��Texture�̃T���v���[�X�e�[�g�̃|�C���^
			@note	Texture��pulic�ɂ���Ƒ��̕s�v�Ȋ֐����G���Ă��܂����߁A
					�֐��Ń��b�s���O�B
		*/
		inline ID3D11SamplerState**GetSamplerState() { return m_pTexture->GetSamplerState(); }

		/*!
			@fn		GetShaderResourceView
			@brief	�V�F�[�_�[���\�[�X�r���[�̎擾
			@return	�����o�̃V�F�[�_�[���\�[�X�r���[�̃A�h���X
		*/
		inline ID3D11ShaderResourceView**GetShaderResourceView() { return m_pTexture->GetShaderResourceView(); }

	private:
		/*!
			@var	m_pShared
			@brief	���g�̃V�F�A�[�h�|�C���^
		*/
		std::shared_ptr<Material*>m_pShared;

		/*!
			@var	m_pTexture
			@brief	�e�N�X�`��
		*/
		std::shared_ptr<Texture>m_pTexture;
	};
}