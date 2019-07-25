/*
	@file	Sprite.h
	@date	2019/07/25
	@author	�ԏ� �G�P
	@brief	�X�v���C�g�֘A
	@detail	���u�����h�ɂ��ĎQ�l�T�C�g:http://neareal.net/index.php?ComputerGraphics%2FXNA%2FBlendState
*/
#pragma once
#include <string>
#include <D3D11.h>
#include "StructShaderBase.h"
#include <DirectXMath.h>
#include "Texture.h"
#include "TextureAtlas.h"
#include "AbstractRender.h"
#include "AbstractShader.h"
#include "Transform.h"

/*! API�̖��O��ԂɊ܂߂� */
namespace API {
	/*!
		@brief �X�v���C�g�������N���X
	*/
	class Sprite
		:public AbstractRender
	{
	public:
		/*!
			@enum	BlendPreset
			@brief	�u�����h���[�h�̃v���Z�b�g�w��p�̗񋓑�
			@detail	�Q�l:http://maverickproj.web.fc2.com/d3d11_01.html
		*/
		enum BlendPreset
		{
			/*!
				@var	Default
				@brief	�f�t�H���g(�u�����h����)
			*/
			Default,

			/*!
				@var	Linear
				@brief	���`����
			*/
			Linear,

			/*!
				@var	Add
				@brief	���Z����
			*/
			Add,

			/*!
				@var	Subtraction
				@brief	���Z����
			*/
			Subtraction,

			/*!
				@var	Multiple
				@brief	��Z����
			*/
			Multiple
		};

		/*!
			@brief	�R���X�g���N�^
		*/
		explicit Sprite();

		/*!
			@brief	�f�X�g���N�^
		*/
		~Sprite();

		/*!
			@fn			Initialize
			@brief		������
			@param[in]	�e�N�X�`��
			@return		����:S_OK ���s:E_FAIL
		*/
		HRESULT Initialize(Texture*texture);

		/*!
			@fn		�t�@�C�i���C�Y
			@brief	�j������
			@detail	�����o�̖����I�ȉ���ƃ����o�̏�����
		*/
		void Finalize();

		/*!
			@fn		Render
			@brief	�`��
		*/
		void Render()override;

		/*!
			@fn			SetStencilMask
			@brief		�[�x�}�X�N�̐ݒ�
			@detail		�C�����C���֐�
			@param[in]	�ݒ肷��}�X�N�l
		*/
		inline void SetStencilMask(uint32_t mask) { m_StencilMask = mask; }

		/*!
			@fn			SetupBlendPreset
			@brief		�w�肵���v���Z�b�g�̃u�����h�X�e�[�g�������o�ɐݒ肷��
			@param[in]	�w�肷��v���Z�b�g�̗񋓑�
		*/
		void SetupBlendPreset(BlendPreset preset);

		/*!
			@fn			SetupTexture
			@brief		�e�N�X�`���̐ݒ�
			@detail		��Q�ƂŃo�C���h���A���̎��_�Œ��_�������s��
			@param[in]	�o�^����e�N�X�`���̃|�C���^
		*/
		void SetupTexture(Texture* texture);

		/*!
			@fn			SetupShader
			@brief		�V�F�[�_�[�̐ݒ�
			@detail		��Q�ƂŃo�C���h����
			@param[in]	�o�^����V�F�[�_�[�̃|�C���^
		*/
		void SetupShader(D3D11::Graphic::AbstractShader* shader);

		/*!
			@var	transform
			@brief	�g�����X�t�H�[��(�s��N���X)
		*/
		std::shared_ptr<Transform>transform;

		/*!
			@var	color
			@brief	�F�̏�Z�l(��)
			@detail	�v�Z���@�̓V�F�[�_�[�Ɉˑ�
		*/
		Color color;
	private:
		/*!
			@fn		SetupTopology
			@brief	�g�|���W�[�̐ݒ�
			@note	�X�v���C�g�͔|���S�������Ȃ̂ŁA����������TRIANGLESTRIP���w��
		*/
		void SetupTopology();

		/*!
			@fn			SetupInputLayout
			@brief		���_���C�A�E�g�̐ݒ�
			@param[in]	�V�F�[�_�[
			@note		��Q�ƂŎ擾�����|�C���^����擾
		*/
		void SetupInputLayout(D3D11::Graphic::AbstractShader*shader);

		/*!
			@fn			SetupBindShader
			@brief		�V�F�[�_�[�̃o�C���h
			@param[in]	�V�F�[�_�[
			@note		��Q�ƂŎ擾�����|�C���^����擾
		*/
		void SetupBindShader(D3D11::Graphic::AbstractShader*shader);
		
		/*!
			@fn			SetupConstantBuffer
			@brief		�R���X�^���g�o�b�t�@�̐ݒ�
			@param[in]	�V�F�[�_�[
			@note		��Q�ƂŎ擾�����|�C���^����擾
		*/
		void SetupConstantBuffer(D3D11::Graphic::AbstractShader*shader);

		/*!
			@fn		SetupVertexBuffer
			@brief	���_�o�b�t�@�ݒ�
		*/
		void SetupVertexBuffer();

		/*!
			@fn			SetupSampler
			@brief		�T���v���[�X�e�[�g�̐ݒ�
			@param[in]	�e�N�X�`��
			@note		��Q�ƂŎ擾�����|�C���^����擾
		*/
		void SetupSampler(Texture*texture);

		/*!
			@fn			SetupSRV
			@brief		ShaderResourceView�̐ݒ�
			@param[in]	�e�N�X�`��
			@note		��Q�ƂŎ擾�����|�C���^����擾
		*/
		void SetupSRV(Texture*texture);

		/*!
			@fn		SetupBlendState
			@brief	�u�����h�X�e�[�g��ݒ�
		*/
		void SetupBlendState();

		/*!
			@fn			CreateVertexBuffer
			@brief		���_�̐���
			@detail		�摜�̃T�C�Y�ɍ��킹�Ĕ|���S���𐶐�����
			@param[in]	�摜�̃T�C�Y
			@return		����:S_OK ���s:E_FAIL
*/
		HRESULT CreateVertexBuffer(const DirectX::XMFLOAT2 size);

		/*!
			@var	m_pBlendState
			@brief	�u�����h�X�e�[�g
		*/
		Microsoft::WRL::ComPtr<ID3D11BlendState>m_pBlendState;

		/*!
			@var	m_StencilMask
			@brief	�[�x�}�X�N
		*/
		uint32_t m_StencilMask;

		/*!
			@var	m_pVertexBuffer
			@brief	���_�o�b�t�@
		*/
		Microsoft::WRL::ComPtr<ID3D11Buffer>m_pVertexBuffer;

		/*!
			@var	m_pShader
			@brief	�V�F�[�_�[�I�u�W�F�N�g�̎�Q��
		*/
		std::weak_ptr<D3D11::Graphic::AbstractShader*>m_pShader;

		/*!
			@var	m_pTexture
			@brief	�e�N�X�`���I�u�W�F�N�g�̎�Q��
		*/
		std::weak_ptr<Texture*>m_pTexture;
		
		/*!
			@var	c_NormalizeSize
			@brief	�|���S���̊�ƂȂ�T�C�Y
			@detail	���̃s�N�Z����Scale��1�ɑ�������
		*/
		static constexpr float c_NormalizeSize = 5.0f;
	}; 
}

