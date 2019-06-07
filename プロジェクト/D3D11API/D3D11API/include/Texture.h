/*
	@file	Texture.h
	@date	2019/06/06
	@author	�ԏ� �G�P
	@brief	�e�N�X�`��
*/
#pragma once
#include <D3D11.h>
#include <string>
#include <DirectXMath.h>
#include <wrl/client.h>
#include "Color.h"

/*!
	@namespace	API
	@brief		API���O��ԂɊ܂߂�
*/
namespace API {

	class Texture
	{
	public:
		/*!
			@enum	TileMode
			@brief	�^�C�����O���[�h�̐ݒ�
		*/
		enum TileMode
		{
			// �J��Ԃ�����
			Clamp = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP,			// �f�t�H���g
			Border = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_BORDER,			// SAMPLER_DESC�܂���HLSL�Ŏw�肵�����E�F��ݒ�
			MirrorOnce = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_MIRROR_ONCE,
			// �^�C�����O 
			Wrap = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP,				// �J��Ԃ�
			Mirror = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_MIRROR,			// �J��Ԃ����] 
		};

		/*!
			@enum	FilteringMode
			@brief	�t�B���^�����O���[�h�̐ݒ�
		*/
		enum FilteringMode
		{
			Point = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_POINT,			// �|�C���g�t�B���^�����O:�ň��i���ŃR�X�p��
			Bilinear = D3D11_FILTER::D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT,	// �o�C���j�A�e�N�X�`���t�B���^�����O
			Trilinear = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR,		// �g�����j�A�e�N�X�`���t�B���^�����O:�f�t�H���g�ݒ�
			Anisotropic = D3D11_FILTER::D3D11_FILTER_ANISOTROPIC			// �ٕ����t�B���^�����O:�ō��i���ŃR�X�p��
		};

		/*!
			@brief	�R���X�g���N�^
		*/
		explicit Texture();

		/*!
			@brief	���[�u�R���X�g���N�^
		*/
		Texture(const Texture& ref) { *this = ref; }

		/*!
			@brief	�f�X�g���N�^
		*/
		~Texture();

		/*!
			@fn			Initialize
			@brief		�e�N�X�`���̏�����
			@detail		�T���v���[�X�e�[�g��SRV�̍쐬�𓯎��ɍs��
						�f�t�H���g�����ň�ԃ��W���[�Ȑݒ��n���Ă���
			@param[in]	�ǂݍ��ރe�N�X�`���̃p�X
			@param[in]	�ݒ肷��t�B���^�����O(�A�h���b�V���O���[�h)
			@param[in]	�ݒ肷��^�C�����[�h
			@return		S_OK:����	E_FAIL:���s
		*/
		HRESULT Initialize(std::string filePath, FilteringMode fMode = FilteringMode::Trilinear, TileMode tMode = TileMode::Wrap);

		/*!
			@fn			Initialize
			@brief		�e�N�X�`���̏�����
			@detail		�T���v���[�X�e�[�g��SRV�̍쐬�𓯎��ɍs��
						�f�t�H���g�����ň�ԃ��W���[�Ȑݒ��n���Ă���
			@param[in]	�ǂݍ��ރe�N�X�`���̃p�X
			@param[in]	�e�N�X�`���̃T�C�Y
			@param[in]	�ݒ肷��t�B���^�����O(�A�h���b�V���O���[�h)
			@param[in]	�ݒ肷��^�C�����[�h
			@return		S_OK:����	E_FAIL:���s
		*/
		HRESULT Initialize(std::string filePath,DirectX::XMINT2 size, FilteringMode fMode = FilteringMode::Trilinear, TileMode tMode = TileMode::Wrap);

		/*!
			@fn		�t�@�C�i���C�Y
			@brief	�j������
			@detail	�����o�̖����I�ȊJ��
		*/
		void Finalize();

		/*!
			@fn		GetSharedPtr
			@brief	�X�}�[�g�|�C���^(�V�F�A�[�h�|�C���^)�̃Q�b�^�[
			@note	�`��I�u�W�F�N�g�ɂ�"weak_ptr"���������邽�߃e�N�X�`���̎Q�Ǝ擾�p
		*/
		inline std::shared_ptr<Texture*>GetSharedPtr() { return m_pShared; }

		/*!
			@fn		GetSamplerState
			@brief	�T���v���[�X�e�[�g�̎擾
			@return	�����o�̃T���v���[�X�e�[�g�̃A�h���X
		*/
		inline ID3D11SamplerState**GetSamplerState() { return m_pSamplerState.GetAddressOf(); }

		/*!
			@fn		GetShaderResourceView
			@brief	�V�F�[�_�[���\�[�X�r���[�̎擾
			@return	�����o�̃V�F�[�_�[���\�[�X�r���[�̃A�h���X
		*/
		inline ID3D11ShaderResourceView**GetShaderResourceView() { return m_pShaderResourceView.GetAddressOf(); }

		/*!
			@fn		GetSize
			@brief	�T�C�Y�̃Q�b�^�[
			@return	�e�N�X�`���̃T�C�Y
		*/
		inline DirectX::XMINT2 GetSize()const { return m_Size; }

		/*!
			@fn			SetupSamplerState
			@brief		�T���v���[�X�e�[�g�̍쐬
			@param[in]	�ݒ肷��t�B���^�����O(�A�h���b�V���O���[�h)
			@param[in]	�ݒ肷��^�C�����[�h
			@return		S_OK:����	E_FAIL:���s
		*/
		HRESULT SetupSamplerState(const FilteringMode fMode,const TileMode tMode);

		/*!
			@fn			SetupSRV
			@brief		SRV�̍쐬
			@param[in]	�ǂݍ��ރe�N�X�`���̃p�X
			@return		�ǂݎ�茋�� S_OK:����	E_FAIL:���s
		*/
		HRESULT SetupShaderResourceView(std::string filePath);

		/*!
			@fn			Load
			@brief		SRV�̍쐬
			@detail		�֐��|�C���^��p����SetupShaderResourceView�̕ʖ���`
			@param[in]	�ǂݍ��ރe�N�X�`���̃p�X
			@return		�ǂݎ�茋�� S_OK:����	E_FAIL:���s
			@note		SetupShaderResourceView���ƕ�����Â炢�Ǝv�����̂Ń��[�U�r���e�B�ɔz�������ʖ���`�Ƃ��Ď���
		*/
		HRESULT Load(std::string filePath) {
			HRESULT(Texture::*fp)(std::string) = &Texture::SetupShaderResourceView;
			return(this->*fp)(filePath);
		}

		/*!
			@fn			SetupAtlas
			@brief		�A�g���X���̃Z�b�g�A�b�v
			@detail		�f�t�H���g������K�p����Ɗ��ɐݒ肵���l���g��
			@param[in]	�`�悷��A�g���X�̃C���f�b�N�X�ԍ��B
						(���������A�g���X�̉�(x)���Ԗڂŏc(y)���Ԗڂ����w��)
			@param[in]	���ɉ��������邩: -1:�ݒ�ς݂̒l���g��
			@param[in]	�c�ɉ��������邩: -1:�ݒ�ς݂̒l���g��
			@note		�A�g���X���g�������Ȃ̂ɃI�t�Z�b�g�ƕ������ŃQ�b�^�[�ƃZ�b�^�[��p�ӂ���Ǝ菇�������A
						���[�U�r���e�B�̒ቺ���S�z�������̂ŃA�g���X�͈�ɂ܂Ƃ߂��B
						�������͕��̒l���w��ł��Ȃ��悤��"unsigned"�Ŏw�肳����
		*/
		void SetupAtlas(DirectX::XMINT2 atlasOffset, unsigned int div_x = c_ConfiguredDevCount, unsigned int div_y = c_ConfiguredDevCount);

		/*!
			@fn		GetDivNum
			@brief	�摜�̕��������擾
		*/
		DirectX::XMINT2 GetDivCount()const { return m_DivCount; };

		/*!
			@fn		GetAtlasOffset
			@brief	�A�g���X�̃C���f�b�N�X�ԍ����擾
		*/
		DirectX::XMINT2 GetAtlasOffset()const { return m_AtlasOffset; };

		/*!
			@var	color
			@brief	�e�N�X�`���ɓK�p����F
		*/
		Color color;

	private:
		/*!
			@var	c_ConfiguredDevCount
			@brief	�ݒ�ς݂̕��������������߂̒萔
		*/
		static constexpr int c_ConfiguredDevCount = -1;

		/*!
			@var	m_pSamplerState
			@brief	�T���v���[�X�e�[�g
		*/
		Microsoft::WRL::ComPtr<ID3D11SamplerState>		m_pSamplerState;

		/*!
			@var	m_pShaderResourceView
			@brief	�V�F�[�_�[���\�[�X�r���[(SRV)
		*/
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>m_pShaderResourceView;	

		/*!
			@var	m_eTileMode
			@brief	�^�C�����O���[�h
			@detail	�T���v���[�X�e�[�g���\�����Ă���"�^�C�����O���[�h"�̐ݒ�
		*/
		TileMode m_eTileMode;

		/*!
			@var	m_eFilterMode
			@brief	�t�B���^�����O���[�h
			@detail	�T���v���[�X�e�[�g���\�����Ă���"�t�B���^�����O���[�h"�̐ݒ�
		*/
		FilteringMode m_eFilterMode;

		/*!
			@var	m_Size
			@brief	�e�N�X�`���T�C�Y
		*/
		DirectX::XMINT2 m_Size;

		/*!
			@var	m_DivCount
			@brief	�e�N�X�`���̕�����(���A�g���X�ݒ�̂�)
		*/
		DirectX::XMINT2 m_DivCount;

		/*!
			@var	m_AtlasOffset
			@brief	�`�悷��A�g���X�̃C���f�b�N�X�ԍ��B
		*/
		DirectX::XMINT2 m_AtlasOffset;

		/*!
			@var	m_pShared
			@brief	���g�̃V�F�A�[�h�|�C���^
			@note	�`��I�u�W�F�N�g����"weak_ptr"��ݒ肷�邽�߁A�擾�p�̕ϐ��B
					�N���X�̃����o�Ɏ������Ă������Ƃ�"Texture"�N���X���̂̐錾��
					���ƃX�}�[�g�|�C���^�ǂ�����g����悤�ɂȂ�B
		*/
		std::shared_ptr<Texture*>m_pShared;
	};
};
