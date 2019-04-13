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
		struct BaseVertex
		{
			DirectX::XMFLOAT3 m_Pos;	/*< 座標 */
		};

#pragma pack(push,16)
		/*!
			@brief	シェーダー側に渡すコンスタントバッファの基底構造体
		*/
		struct alignas(16) BaseConstantBuffer
		{
//------------------------------------------------------------------------------------
//	@NOTE	コンスタントバッファのメンバ変更に伴う、
//			シェーダーの切り替えを行う
//			(※シェーダーの切り替えは参照するディレクトリを変更することで実現する！)
//
//			StractShaderBase内の"mat"マクロで切り替え
//			"mat"定義済み→行列をワールド、ビュー、プロジェクションに分割
//			"mat"未定義  →3つを掛けた変換済みの行列を渡す
//			
//			"mat"マクロを参照しているファイル
//			※依存関係
//			・Sprite.cpp/h
//			・StractShaderBase.h
//			・main.cpp
//
//			以下で宣言している"mat"をコメントアウト等で消せば"mat"未定義動作が実行される。
//------------------------------------------------------------------------------------

#define mat
#if defined mat
			DirectX::XMMATRIX m_WorldMatrix;		/*!< ワールド */
			DirectX::XMMATRIX m_ProjectionMatrix;	/*!< 射影行列 */
			DirectX::XMMATRIX m_ViewMatrix;			/*!< ビュー行列 */
#else
			DirectX::XMMATRIX m_WVP;
#endif
		};
#pragma pack(pop)
	};
};