/*
	@file	StructShaderBase.h
	@date	2018/03/04
	@author	番場 宥輝
	@brief	シェーダー関連の構造体宣言
*/
#pragma once
#include <DirectXMath.h>


/*! Direct3D11関連の名前空間 */
namespace D3D11 {

	/*! 描画関連の名前空間 */
	namespace Graphic {

		/*!
			@brief	基底頂点構造体
		*/
		struct BaseVertex abstract
		{
			/*!
				@brief	デフォルトコンストラクタ
			*/
			BaseVertex() = default;

			/*!
				@brief	引数付きコンストラクタ
			*/
			inline BaseVertex(DirectX::XMFLOAT3 position)
			{
				this->position = position;
			}

			/*!
				@var	position
				@brief	3D空間上の座標
			*/
			DirectX::XMFLOAT3 position;
		};

#pragma pack(push,16)
		/*!
			@brief	シェーダー側に渡すコンスタントバッファの基底構造体
		*/
		struct alignas(16) BaseConstantBuffer
		{
			DirectX::XMMATRIX m_WorldMatrix;		/*!< ワールド */
			DirectX::XMMATRIX m_ProjectionMatrix;	/*!< 射影行列 */
			DirectX::XMMATRIX m_ViewMatrix;			/*!< ビュー行列 */
		};
#pragma pack(pop)
	};
};