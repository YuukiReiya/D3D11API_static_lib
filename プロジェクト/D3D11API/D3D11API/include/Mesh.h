/*
	@file	Mesh.h
	@date	2019/04/29
	@author	番場 宥輝
	@brief	メッシュ用のクラス
*/
#pragma once
#include <d3d11.h>
#include <string>
#include <memory>
#include <vector>
#include "AbstractRender.h"
#include "AbstractShader.h"
#include "StructShaderBase.h"
#include "MeshVertex.h"
#include "Transform.h"
#include "Color.h"

namespace D3D11 {
	namespace Graphic {
		/*!
			@struct	MeshShaderBuffer
			@brief	メッシュのコンスタントバッファ
		*/
		struct alignas(16) MeshShaderBuffer
			:public BaseConstantBuffer
		{
			/*!
				@var	m_Color
				@brief	色
			*/
			DirectX::XMFLOAT4 m_Color;
		};
	}
}

namespace API{
	class Mesh
		:public AbstractRender
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
			@fn			Initialize
			@brief		初期化
			@param[in]	読み込むファイルのパス
		*/
		HRESULT Initialize(std::string path);

		/*!
			@fn		Render
			@brief	描画
		*/
		void Render()override final;

		/*!
			@fn			SetupShader
			@brief		シェーダーの設定
			@detail		弱参照でバインドする
			@param[in]	登録するシェーダーのポインタ
		*/
		void SetupShader(D3D11::Graphic::AbstractShader* shader);

		/*!
			@var	transform
			@brief	トランスフォーム
		*/
		std::shared_ptr<Transform> transform;

		/*!
			@var	color
			@brief	色
		*/
		Color color;
	private:
		/*!
			@fn		SetupTopology
			@brief	トポロジーの設定
		*/
		void SetupTopology();

		/*!
			@fn		SetupConstantBuffer
			@brief	コンスタントバッファの設定
		*/
		void SetupConstantBuffer();

		/*!
			@fn		SetupBindShader
			@brief	シェーダーの設定
		*/
		void SetupBindShader();

		/*!
			@fn			CreateVertexBuffer
			@brief		メッシュの頂点を生成
			@detail		静的関数
			@param[in]	設定するメッシュのポインタ
			@param[in]	構成する頂点情報
			@return		成功:S_OK 失敗:E_FAIL
		*/
		static HRESULT CreateVertexBuffer(Mesh*mesh, std::vector<D3D11::Graphic::MeshVertex>vertex);

		/*!
			@fn		SetupVertexBuffer
			@brief	頂点バッファ設定
		*/
		void SetupVertexBuffer();

		/*!
			@fn			CreateIndexBuffer
			@brief		インデックスバッファ作成
			@detail		静的関数
			@param[in]	設定するメッシュのポインタ
			@param[in]	頂点のインデックス
			@return		成功:S_OK 失敗:E_FAIL
		*/
		static HRESULT CreateIndexBuffer(Mesh*mesh, std::vector<uint32_t>index);

		/*!
			@fn		SetupVertexBuffer
			@brief	インデックスバッファ設定
		*/
		void SetupIndexBuffer();

		/*!
			@var	m_VertexIndex
			@brief	頂点インデックス
			@NOTE	頂点インデックスの数≠頂点数
					∴ MeshVertexに"uint"型で含むことが出来ない
		*/
		std::vector<uint32_t>m_VertexIndex;

		/*!
			@var	m_pVertexBuffer
			@brief	頂点バッファ
		*/
		Microsoft::WRL::ComPtr<ID3D11Buffer>		m_pVertexBuffer;

		/*!
			@var	m_pIndexBuffer
			@brief	インデックスバッファ
		*/
		Microsoft::WRL::ComPtr<ID3D11Buffer>		m_pIndexBuffer;

		/*!
			@var	m_pShader
			@brief	シェーダーオブジェクトの弱参照
		*/
		std::weak_ptr<D3D11::Graphic::AbstractShader*>m_pShader;
	};
}