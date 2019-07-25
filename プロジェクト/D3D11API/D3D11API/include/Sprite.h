/*
	@file	Sprite.h
	@date	2018/03/04
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

/*! Direct3D11�֘A�̖��O��� */
//namespace D3D11 {
//
//	/*! �`��֘A�̖��O��ԂɊ܂߂� */
//	namespace Graphic {
//
//		/****************************************/
//		/*		�X�v���C�g�ň����\����			*/
//		/****************************************/
//
//		/*!
//			@brief	�X�v���C�g�̒��_�\����
//		*/
//		struct SpriteVertex
//			:public BaseVertex
//		{
//			SpriteVertex(const DirectX::XMFLOAT3 pos, const DirectX::XMFLOAT2 uv) {
//				this->position = pos, m_UV = uv;
//			}
//			/*!
//				@var	m_UV
//				@brief	UV���W
//			*/
//			DirectX::XMFLOAT2 m_UV;
//		};
//
//#pragma pack(push,16)
//		/*!
//			@brief	�X�v���C�g�̃R���X�^���g�o�b�t�@�\����
//		*/
//		struct alignas(16) SpriteShaderBuffer
//			:BaseConstantBuffer
//		{
//			/*!
//				@var	m_DivNum
//				@brief	�e�N�X�`���̕�����
//			*/
//			DirectX::XMFLOAT2 m_DivNum;
//
//			/*!
//				@var	m_Index
//				@brief	���������e�N�X�`���̕\���ʒu
//			*/
//			DirectX::XMFLOAT2 m_Index;
//
//			/*!
//				@var	m_Color
//				@brief	�J���[
//			*/
//			DirectX::XMFLOAT4 m_Color;
//		};
//#pragma pack(pop)
//	}
//}

/*! API�̖��O��ԂɊ܂߂� */
namespace API {
	/*!
		@brief �X�v���C�g�������N���X
	*/
	//	class Sprite
	//		: public AbstractRender
	//	{
	//		public:
	//			/*!
	//				@enum	BlendPreset
	//				@brief	�u�����h���[�h�̃v���Z�b�g�w��p�̗񋓑�
	//				@detail	�Q�l:http://maverickproj.web.fc2.com/d3d11_01.html
	//			*/
	//			enum BlendPreset
	//			{
	//				/*!
	//					@var	Default
	//					@brief	�f�t�H���g(�u�����h����)
	//				*/
	//				Default,
	//
	//				/*!
	//					@var	Linear
	//					@brief	���`����
	//				*/
	//				Linear,
	//
	//				/*!
	//					@var	Add
	//					@brief	���Z����
	//				*/
	//				Add,
	//
	//				/*!
	//					@var	Subtraction
	//					@brief	���Z����
	//				*/
	//				Subtraction,
	//
	//				/*!
	//					@var	Multiple
	//					@brief	��Z����
	//				*/
	//				Multiple
	//			};
	//
	//			/*!
	//				@brief	�R���X�g���N�^
	//			*/
	//			explicit Sprite();
	//
	//			/*!
	//				@brief	�f�X�g���N�^
	//			*/
	//			~Sprite();
	//
	//			/*!
	//				@fn		Initialize
	//				@brief	������
	//				@detail	�u�����h�X�e�[�g�̍쐬���s��
	//			*/
	//			HRESULT Initialize();
	//
	//			/*!
	//				@fn		Finalize
	//				@brief	�j������
	//				@detail	�����o�̖����I�ȉ���ƃ����o�̏�����
	//			*/
	//			void Finalize();
	//
	//			/*!
	//				@fn		Render
	//				@brief	�`��
	//			*/
	//			void Render()override;
	//
	//			/*!
	//				@fn			SetStencilMask
	//				@brief		�[�x�}�X�N�̐ݒ�
	//				@detail		�C�����C���֐�
	//				@param[in]	�ݒ肷��}�X�N�l
	//			*/
	//			inline void SetStencilMask(uint32_t mask) { m_StencilMask = mask; }
	//
	//			/*!
	//				@fn			SetupBlendPreset
	//				@brief		�w�肵���v���Z�b�g�̃u�����h�X�e�[�g�������o�ɐݒ肷��
	//				@param[in]	�w�肷��v���Z�b�g�̗񋓑�
	//			*/
	//			void SetupBlendPreset(BlendPreset preset);
	//
	//			/*!
	//				@fn			SetupTexture
	//				@brief		�e�N�X�`���̐ݒ�
	//				@detail		��Q�ƂŃo�C���h���A���̎��_�Œ��_�������s��
	//				@param[in]	�o�^����e�N�X�`���̃|�C���^
	//			*/
	//			void SetupTexture(Texture* texture);
	//
	//			/*!
	//				@fn			SetupShader
	//				@brief		�V�F�[�_�[�̐ݒ�
	//				@detail		��Q�ƂŃo�C���h����
	//				@param[in]	�o�^����V�F�[�_�[�̃|�C���^
	//			*/
	//			void SetupShader(D3D11::Graphic::AbstractShader* shader);
	//
	//			std::shared_ptr<Transform> transform;
	//
	//		private:
	//			/*!
	//				@var	c_VertexCount
	//				@brief	�X�v���C�g�̒��_��
	//				@detail	�X�v���C�g�̒��_���̒萔��
	//			*/
	//			static constexpr int c_VertexCount = 4;/*!< �X�v���C�g�̒��_�� */
	//
	//			/*!
	//				@var	c_DefaultSize
	//				@brief	��ƂȂ�T�C�Y
	//				@detail	���̃s�N�Z����Scale��1�ɑ�������
	//				@value	100.0f
	//			*/
	//			static constexpr float c_DefaultSize = 100.0f;
	//
	//			/*!
	//				@var	c_ScaleZ
	//				@brief	�|����Z�X�P�[��
	//				@detail	�������钸�_��Z�����̑傫��
	//				@value	0
	//			*/
	//			static constexpr float c_ScaleZ = 0;
	//
	//			/*!
	//				@var	c_VertexZ
	//				@brief	�|���̒��_�����ʒu
	//				@detail	CreateVertexBuffer�֐��Ő������钸�_��Z�ʒu
	//				@value	0
	//			*/
	//			static constexpr float c_VertexZ = 0;
	//
	//			/*!
	//				@fn			���_����
	//				@brief		�X�v���C�g�̒��_�𐶐�����
	//				@detail		���_�\���̂��g���āA���𒸓_�o�b�t�@�Ƀo�C���h
	//				@param[in]	�摜�T�C�Y
	//				@return		����:S_OK ���s:E_FAIL
	//			*/
	//			//HRESULT CreateVertex(DirectX::XMINT2 size);
	//
	//			/*!
	//				@fn			CreateVertexBuffer
	//				@brief		���_�̐ݒ�
	//			*/
	//			//void SetupVertex();
	//
	//			/*!
	//				@fn		SetupTopology
	//				@brief	�g�|���W�[�̐ݒ�
	//				@NOTE	�X�v���C�g�͔|���S�������Ȃ̂ŁA����������TRIANGLESTRIP���w��
	//			*/
	//			void SetupTopology();
	//
	//			/*!
	//				@fn		CreateInputLayout
	//				@brief	���_���C�A�E�g�̐ݒ�
	//			*/
	//			void SetupInputLayout();
	//
	//			/*!
	//				@fn		SetupBindShader
	//				@brief	�V�F�[�_�[�̐ݒ�
	//			*/
	//			void SetupBindShader();
	//
	//			/*!
	//				@fn		SetupSampler
	//				@brief	�T���v���[�X�e�[�g�̐ݒ�
	//			*/
	//			void SetupSampler();
	//
	//			/*!
	//				@fn		SetupSRV
	//				@brief	ShaderResourceView�̐ݒ�
	//			*/
	//			void SetupSRV();
	//
	//			/*!
	//				@fn		SetupConstantBuffer
	//				@brief	�R���X�^���g�o�b�t�@�̐ݒ�
	//			*/
	//			//void SetupConstantBuffer();
	//
	//			/*!
	//				@fn		CreateVertexBuffer
	//				@brief	���_�o�b�t�@�ݒ�
	//			*/
	//			//void SetupVertexBuffer();
	//
	//			/*!
	//				@fn		SetupBlendState
	//				@brief	�u�����h�X�e�[�g��ݒ�
	//			*/
	//			void SetupBlendState();
	//
	//			/*!
	//				@var	m_StencilMask
	//				@brief	�[�x�}�X�N
	//			*/
	//			uint32_t									m_StencilMask;
	//
	//			/*!
	//				@var	m_pVertexBuffer
	//				@brief	���_�o�b�t�@
	//			*/
	//			Microsoft::WRL::ComPtr<ID3D11Buffer>		m_pVertexBuffer;
	//
	//			/*!
	//				@var	m_pBlendState
	//				@brief	�u�����h�X�e�[�g
	//			*/
	//			Microsoft::WRL::ComPtr<ID3D11BlendState>	m_pBlendState;
	//
	//			/*!
	//				@var	m_pTexture
	//				@brief	�e�N�X�`���I�u�W�F�N�g�̎�Q��
	//			*/
	//			std::weak_ptr<Texture*>							m_pTexture;
	//
	//			/*!
	//				@var	m_pShader
	//				@brief	�V�F�[�_�[�I�u�W�F�N�g�̎�Q��
	//			*/
	//			std::weak_ptr<D3D11::Graphic::AbstractShader*>	m_pShader;
	//
	//			/****************************************/
	//			/*		�@�X�v���C�g�̃p�����[�^		*/
	//			/****************************************/
	//			Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>m_pSRV;
	//
	//			struct Vertex
	//			{
	//				DirectX::XMFLOAT3 position;
	//				DirectX::XMFLOAT2 uv;
	//			};
	//
	//			HRESULT SetupConstantBuffer();
	//#pragma region OLD
	//#if 0
	//			/*!
	//				@var	m_Size
	//				@brief	�X�v���C�g�Ő�������摜�̃L���b�V��
	//			*/
	//			DirectX::XMINT2 m_Size;
	//
	//			/*!
	//				@var	m_pIndexVertexBuffer
	//				@brief	���_�o�b�t�@
	//			*/
	//			Microsoft::WRL::ComPtr<ID3D11Buffer>m_pIndexBuffer;
	//
	//			/*!
	//				@fn			���_����
	//				@brief		�X�v���C�g�̒��_�𐶐�����
	//				@detail		���_�\���̂��g���āA���𒸓_�o�b�t�@�Ƀo�C���h
	//				@return		����:S_OK ���s:E_FAIL
	//			*/
	//			//HRESULT CreateIndexBuffer();
	//			//HRESULT CreateVertexBuffer();
	//
	//
	//			static constexpr DirectX::XMFLOAT3 c_Vertices[] = {
	//				{-0.5f * 100,-0.5f * 100,0},//	����
	//				{ 0.5f * 100,-0.5f * 100,0},//	�E��
	//				{ 0.5f * 100, 0.5f * 100,0},//	�E��
	//				{-0.5f * 100, 0.5f * 100,0},//	����
	//			};
	//			static constexpr uint8_t c_Indices[] = {
	//				0,1,2,2,3,0,2 ,
	//			};
	//#endif // 0
	//#pragma endregion
	//
	//	};

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

		explicit Sprite();
		~Sprite() {}

		HRESULT Init();
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

		std::shared_ptr<Transform>transform;

	private:
		/*!
			@fn		SetupTopology
			@brief	�g�|���W�[�̐ݒ�
			@NOTE	�X�v���C�g�͔|���S�������Ȃ̂ŁA����������TRIANGLESTRIP���w��
		*/
		void SetupTopology();

		/*!
			@fn		CreateInputLayout
			@brief	���_���C�A�E�g�̐ݒ�
		*/
		void SetupInputLayout();

		/*!
			@fn		SetupBindShader
			@brief	�V�F�[�_�[�̃o�C���h
		*/
		void SetupBindShader();

		/*!
			@fn		SetupSampler
			@brief	�T���v���[�X�e�[�g�̐ݒ�
		*/
		void SetupSampler();

		/*!
			@fn		SetupSRV
			@brief	ShaderResourceView�̐ݒ�
		*/
		void SetupSRV();

		/*!
			@fn		SetupConstantBuffer
			@brief	�R���X�^���g�o�b�t�@�̐ݒ�
		*/
		//void SetupConstantBuffer();

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
		Microsoft::WRL::ComPtr<ID3D11Buffer>m_pVertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11InputLayout>m_pInputLayout;
		Microsoft::WRL::ComPtr<ID3D11VertexShader>m_pVertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader>m_pPixelShader;
		Microsoft::WRL::ComPtr<ID3D11Buffer>m_pConstantBuffer;

		Color color;
		Microsoft::WRL::ComPtr<ID3D11SamplerState>m_pSamplerState;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>m_pShaderResourceView;
	}; 


	struct SpriteVertex
	{
		SpriteVertex(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT2 uv)
		{
			this->pos = pos, this->uv = uv;
		}
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 uv;
	};

}

