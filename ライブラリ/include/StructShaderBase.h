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
		struct VARTEX_BASE
		{
			DirectX::XMFLOAT3 m_Pos;/*< 座標 */
		};

		/*!
			@brief	シェーダー側に渡すコンスタントバッファの基底構造体
		*/
#pragma pack(push,16)

		/*!
			@brief	packマクロに変更
		*/
		struct CONSTANT_BUFFER_BASE
		{
			/*!
				@brief	エイリアステンプレートを用いた別名定義
			*/
			
			template<typename alignType>
			//using ALIGN16 = __declspec(align(16)) alignType;/*!< 16バイト境界に型を設定するための別名 */
			using ALIGN16 = alignType;/*!< 16バイト境界に型を設定するための別名 */
			/*! メンバの行列 */
			//ALIGN16<DirectX::XMMATRIX>m_WorldMatrix;		/*!< ワールド */
			//ALIGN16<DirectX::XMMATRIX>m_ProjectionMatrix;	/*!< 射影行列 */
			//ALIGN16<DirectX::XMMATRIX>m_ViewMatrix;			/*!< ビュー行列 */
			ALIGN16<DirectX::XMMATRIX>m_WVP;
		};
#pragma pack(pop)
	};
};