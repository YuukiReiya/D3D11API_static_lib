/*!
	@file	AnimationMesh.h
	@date	2019/08/04
	@author	番場 宥輝
	@brief	フレームアニメーションメッシュ
*/

#pragma once
#include "AbstractRender.h"
#include "AbstractShader.h"
#include <memory>
#include <DirectXMath.h>
#include "MeshVertex.h"
#include <wrl/client.h>
#include "Transform.h"
#include <unordered_map>

namespace API {
#pragma region 頂点構造体
	/*!
		@struct	VERTEX
		@brief	フレームアニメーションメッシュで使う頂点構造体

		NOTE:	UV未対応
		TODO:	UV対応、法線情報追加、etc
	*/
	struct VERTEX {
		/*!
			@var	pos
			@brief	頂点の座標
		*/
		DirectX::XMFLOAT3 pos;
	};
#pragma endregion

	/*!
		@class	AnimationMesh
		@brief	フレームアニメーションを行うメッシュクラス
	*/
	class AnimationMesh
		:public API::AbstractRender
	{
	public:
		/*!
			@brief	コンストラクタ
		*/
		AnimationMesh() :m_AnimIndex({0,0}) { transform = std::make_shared<Transform>(); }

		/*!
			@brief	デストラクタ
		*/
		~AnimationMesh() { m_VertexList.clear(); };

		/*!
			@fn			Initialize
			@brief		初期化
			@param[in]	読み込みパス
			@return		成功:S_OK 失敗:E_FAIL
		*/
		HRESULT Initialize(std::string path);

		/*!
			@fn		Render
			@brief	描画
		*/
		void Render()override;

		/*!
			@fn		AddAnimIndex
			@brief	アニメーションの切り替え
			@detail	フレーム数はゼロに戻す。
		*/
		void AddAnimIndex() {
			m_AnimIndex.x = (size_t)m_AnimIndex.x + 1 < m_VertexList.size() ? ++m_AnimIndex.x : 0;
			m_AnimIndex.y = 0;
		}

		/*!
			@fn		AddFrameIndex
			@brief	フレームの切り替え
		*/
		void AddFrameIndex() {
			m_AnimIndex.y = (size_t)m_AnimIndex.y + 1 < m_VertexList[m_AnimIndex.x].size() ? ++m_AnimIndex.y : 0;
		}

		/*!
			@var	transform
			@brief	トランスフォーム
		*/
		std::shared_ptr<Transform>transform;
	private:
		/*!
			@var	m_pShader
			@brief	シェーダー
		*/
		std::shared_ptr<D3D11::Graphic::AbstractShader>m_pShader;

		/*!
			@fn			CreateVertexBuffer
			@brief		頂点バッファの作成
			@param[in]	可変長配列に格納した頂点情報
			@return		成功:S_OK 失敗:E_FAIL
		*/
		HRESULT CreateVertexBuffer(std::vector<D3D11::Graphic::MeshVertex>verttices);

		/*!
			@fn			CreateIndexBuffer
			@brief		インデックスバッファの作成
			@param[in]	可変長配列に格納したインデックス情報
			@return		成功:S_OK 失敗:E_FAIL
		*/
		HRESULT CreateIndexBuffer(std::vector<uint32_t>indices);

		/*!
			@fn		SetupTopology
			@brief	トポロジーの設定
		*/
		void SetupTopology();

		/*!
			@fn		SetupInputLayout
			@brief	頂点レイアウトの設定
		*/
		void SetupInputLayout();

		/*!
			@fn		SetupIndexBuffer
			@brief	インデックスバッファの設定
		*/
		void SetupIndexBuffer();

		/*!
			@fn		SetupVertexBuffer
			@brief	頂点バッファの設定
		*/
		void SetupVertexBuffer();

		/*!
			@fn		SetupConstantBuffer
			@brief	コンスタントバッファの設定
			@return	成功:S_OK 失敗:E_FAIL
		*/
		HRESULT SetupConstantBuffer();

		/*!
			@fn		SetupBindShader
			@brief	シェーダーの設定
		*/
		void SetupBindShader();

		/*!
			@var	m_pVertexBuffer
			@brief	頂点バッファ
		*/
		Microsoft::WRL::ComPtr<ID3D11Buffer>m_pVertexBuffer;

		/*!
			@var	m_pIndexBuffer
			@brief	インデックスバッファ
		*/
		Microsoft::WRL::ComPtr<ID3D11Buffer>m_pIndexBuffer;

		/*!
			@var	m_IndexCount
			@brief	インデックス数
		*/
		uint32_t m_IndexCount;

		/*!
			@var	m_VertexCount
			@brief	頂点数
		*/
		uint32_t m_VertexCount;

		/*!
			@var	m_AnimIndex
			@brief	アニメーションの番号とフレームの番号
		*/
		DirectX::XMINT2 m_AnimIndex;

		/*!
			@var	m_VertexList
			@brief	頂点のリスト
			@detail	[アニメーション番号][フレーム番号]
		*/
		std::unordered_map<size_t, std::unordered_map<size_t, std::vector<DirectX::XMFLOAT3>>>m_VertexList;

		/*!
			@var	m_UV
			@brief	UVのテクセル座標

			NOTE:	メッシュ構造体に合わせるために持たせているだけで、
					そもそも頂点情報にUVは含まれていないので中身がなんであろうと関係ない
		*/
		std::vector<DirectX::XMFLOAT2>m_UV;
	};



}
