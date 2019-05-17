/*
	@file	Mesh.h
	@date	2019/05/17
	@author	番場 宥輝
	@brief	メッシュ
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
			@NOTE	BaseConstantBufferを未継承の場合
					正常に描画される！
					構造体のサイズはどちらも"208"なのになんで？？？
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

/*! APIの名前空間に含める */
namespace API{

	class Mesh
		: public AbstractRender
	{
	public:
		/*!
			@brief	コンストラクタ
		*/
		explicit Mesh();

		/*!
			@brief	デストラクタ	
		*/
		~Mesh();

		/*!
			@fn		Initialize
			@brief	初期化
			@return	S_OK:成功 E_FAIL:失敗
		*/
		HRESULT Initialize();

		/*!
			@fn			Initialize
			@brief		初期化
			@param[in]	外部ファイルのパス
			@return	S_OK:成功 E_FAIL:失敗
		*/
		HRESULT Initialize(std::string path);

		/*!	
			@fn		Finalize
			@brief	破棄処理
			@detail	メンバの明示的な開放
		*/
		void Finalize();

		/*!
			@fn		Render
			@brief	描画処理
			@detail	純粋仮想関数をオーバーライド
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