/*
	@file	Mesh.h
	@date	2019/05/17
	@author	番場 宥輝
	@brief	メッシュ
*/
#pragma once
#include <d3d11.h>
#include <string>
#include <vector>
#include <memory>
#include <wrl/client.h>
#include <DirectXMath.h>
#include "AbstractRender.h"
#include "Transform.h"
#include "AbstractShader.h"

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

		void SetupShader(D3D11::Graphic::AbstractShader*s) {
			m_pShader = s->GetSharedPtr();
		}

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

		/*!
			@var	transform
			@brief	トランスフォーム
		*/
		Transform transform;
	private:
		static HRESULT CreateInputLayout(Mesh*mesh);
		static HRESULT CreateVertexBuffer(Mesh*mesh, std::vector<DirectX::XMFLOAT3>verttices);
		static HRESULT CreateIndexBuffer(Mesh*mesh,std::vector<uint32_t>indices);
		static HRESULT CreateConstantBuffer(Mesh*mesh);
		static void SetupIndexBuffer(Mesh*mesh);
		std::weak_ptr<D3D11::Graphic::AbstractShader*>m_pShader;
		template <class Vertex>
		static HRESULT CreateVertexBuffer(Mesh*mesh, std::vector<Vertex>verttices);


		HRESULT CreateVertexShader();
		HRESULT CreatePixelShader();

		

		uint32_t indexCount;
		Microsoft::WRL::ComPtr<ID3D11Buffer>m_pVertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer>m_pIndexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer>m_pConstantBuffer;

		//Microsoft::WRL::ComPtr<ID3D11InputLayout>m_pInputLayout;
		//Microsoft::WRL::ComPtr<ID3D11VertexShader>m_pVertexShader;
		//Microsoft::WRL::ComPtr<ID3D11PixelShader>m_pPixelShader;
		Microsoft::WRL::ComPtr<ID3D11SamplerState>m_pSamplerState;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>m_pSRV;
	};
}