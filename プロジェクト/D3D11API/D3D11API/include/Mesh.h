/*
	@file	Mesh.h
	@date	2019/05/17
	@author	�ԏ� �G�P
	@brief	���b�V��
*/
#pragma once
#include <d3d11.h>
#include <string>
#include <wrl/client.h>
#include <DirectXMath.h>
#include "AbstractRender.h"
#include "StructShaderBase.h"

namespace D3D11 {
	namespace Graphic {
		struct MeshShaderBuffer{};

		/*!
			@NOTE	BaseConstantBuffer�𖢌p���̏ꍇ
					����ɕ`�悳���I
					�\���̂̃T�C�Y�͂ǂ����"208"�Ȃ̂ɂȂ�ŁH�H�H
		*/
		struct alignas(16) MeshConstantBuffer
		//	: public BaseConstantBuffer 
		{
			
			DirectX::XMMATRIX world;
			DirectX::XMMATRIX view;
			DirectX::XMMATRIX proj;
			DirectX::XMFLOAT4 color;
		};
		struct MeshV {
			//DirectX::XMFLOAT3 pos;
			//DirectX::XMFLOAT4 color;
			float pos[3];
			float col[4];


		};
	}
}

/*! API�̖��O��ԂɊ܂߂� */
namespace API{

	class Mesh
		: public AbstractRender
	{
	public:
		/*!
			@brief	�R���X�g���N�^
		*/
		explicit Mesh();

		/*!
			@brief	�f�X�g���N�^	
		*/
		~Mesh();

		/*!
			@fn		Initialize
			@brief	������
			@return	S_OK:���� E_FAIL:���s
		*/
		HRESULT Initialize();

		/*!
			@fn			Initialize
			@brief		������
			@param[in]	�O���t�@�C���̃p�X
			@return	S_OK:���� E_FAIL:���s
		*/
		HRESULT Initialize(std::string path);

		/*!	
			@fn		Finalize
			@brief	�j������
			@detail	�����o�̖����I�ȊJ��
		*/
		void Finalize();

		/*!
			@fn		Render
			@brief	�`�揈��
			@detail	�������z�֐����I�[�o�[���C�h
		*/
		void Render()override;

	private:
		static HRESULT CreateInputLayout(Mesh*mesh);
		static HRESULT CreateVertexBuffer(Mesh*mesh);
		static HRESULT CreateIndexBuffer(Mesh*mesh);
		static HRESULT CreateConstantBuffer(Mesh*mesh);
		HRESULT CreateVertexShader();
		HRESULT CreatePixelShader();

		uint32_t indexCount;
		Microsoft::WRL::ComPtr<ID3D11Buffer>m_pVertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer>m_pIndexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer>m_pConstantBuffer;
		Microsoft::WRL::ComPtr<ID3D11InputLayout>m_pInputLayout;
		Microsoft::WRL::ComPtr<ID3D11VertexShader>m_pVertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader>m_pPixelShader;
		Microsoft::WRL::ComPtr<ID3D11SamplerState>m_pSamplerState;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>m_pSRV;
	};

}