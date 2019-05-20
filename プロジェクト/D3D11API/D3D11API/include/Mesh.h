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

		/*!
			@fn			SetupShader
			@brief		シェーダーの参照のセット
			@detail		静的関数で作っておけばループ文で回せる
			@param[in]	設定先のメッシュ
			@param[in]	設定元のシェーダー
		*/
		static inline void SetupShader(Mesh*mesh, D3D11::Graphic::AbstractShader*shader)
		{
			mesh->m_pShader = shader->GetSharedPtr();
		}

		/*!
			@fn			SetupShader
			@brief		シェーダーの参照のセット
			@param[in]	設定元のシェーダー
		*/
		inline void SetupShader(D3D11::Graphic::AbstractShader*shader)
		{
			SetupShader(this, shader);
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
		/*!
			@fn			CreateIndexBuffer
			@brief		インデックスバッファ作成
			@detail		生成関数は静的でいい
			@param[in]	バインドするメッシュ
			@param[in]	インデックス情報
			@return		S_OK:成功 E_FAIL:失敗
		*/
		static HRESULT CreateIndexBuffer(Mesh*mesh,std::vector<uint32_t>indices);

		/*!
			@fn			SetupIndexBuffer
			@brief		生成したインデックスバッファのセット
			@param[in]	バインド先のメッシュ
		*/
		static void SetupIndexBuffer(Mesh*mesh);

		/*!
			@template	Vertex
			@brief		CreateVertexBuffer関数で使う頂点構造体のためのテンプレート
		*/
		template <class Vertex>

		/*!
			@fn			CreateVertexBuffer
			@brief		頂点バッファ生成
			@param[in]	生成先のメッシュ
			@param[in]	頂点バッファを構成する頂点情報
			@return		S_OK:成功 E_FAIL:失敗
		*/
		static HRESULT CreateVertexBuffer(Mesh*mesh, std::vector<Vertex>verttices);

		/*!
			@var	m_IndexCount
			@brief	頂点インデックスのインデックス数
		*/
		uint32_t m_IndexCount;

		/*!
			@var	m_pShader
			@brief	Meshクラスで使用するシェーダーの弱参照
			@detail	weak_ptr
		*/
		std::weak_ptr<D3D11::Graphic::AbstractShader*>m_pShader;

		/*!
			@var	m_pVertexBuffer
			@brief	頂点バッファ
			@detail	ComPtr
		*/
		Microsoft::WRL::ComPtr<ID3D11Buffer>m_pVertexBuffer;

		/*!
			@var	m_pIndexBuffer
			@brief	インデックスバッファ
			@detail	ComPtr
		*/
		Microsoft::WRL::ComPtr<ID3D11Buffer>m_pIndexBuffer;

		/*!
			@var	m_pSamplerState
			@brief	サンプラーステート
			@detail	ComPtr
		*/
		Microsoft::WRL::ComPtr<ID3D11SamplerState>m_pSamplerState;

		/*!
			@var	m_pSRV
			@brief	シェーダーリソースビュー
			@detail	ComPtr
		*/
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>m_pSRV;
	};
}