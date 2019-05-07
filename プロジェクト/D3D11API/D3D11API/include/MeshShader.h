/*
	@file	MeshShader
	@date	2019/05/04
	@author	番場 宥輝
	@brief	メッシュのデフォルトシェーダー
*/
#pragma once
#include "AbstractShader.h"

namespace D3D11 {
	namespace Graphic {
		class MeshShader
			: public AbstractShader
		{
		public:
			/*!
				@brief	コンストラクタ
			*/
			MeshShader();

			/*!
				@brief	デストラクタ
			*/
			~MeshShader();

			/*!
				@fn		Setup
				@brief	プリコンパイル済みシェーダーファイルを利用してセットアップを行う
				@detail	オーバーライド
				@TODO	読み込めない.csoファイルが存在し、動作が安定しない。
			*/
			HRESULT Setup()override final;

			/*!
				@fn		DynamicSetup
				@brief	動的コンパイルを利用したセットアップを行う
				@detail	オーバーライド
			*/
			HRESULT DynamicSetup()override final;

		private:
			/*!
				@fn		CreateConstantBuffer
				@brief	コンスタントバッファの作成
				@return	S_OK:成功 E_FAIL:失敗
			*/
			HRESULT CreateConstantBuffer();
			
			/*!
				@fn			CreateInputLayout
				@brief		頂点レイアウトの作成
				@param[in]	コンパイル済みブロブ
				@return		S_OK:成功 E_FAIL:失敗
			*/
			HRESULT CreateInputLayout(ID3DBlob*pBlob);

			/*!
				@var	m_Directory
				@brief	シェーダーファイルの階層文字列
			*/
			std::string m_Directory;

			/*!
				@var	c_HLSL
				@brief	HLSLのファイル名
			*/
			static constexpr std::string_view c_HLSL = "MeshShader.hlsl";

			/*!
				@var	c_CSO
				@brief	CSOのファイル名
			*/
			static constexpr std::string_view c_CSO = "SpriteShader.cso";

			/*!
				@var	c_VSEntryName
				@brief	頂点シェーダーのエントリーポイントの名前
			*/
			static constexpr std::string_view c_VSEntryName = "VS";

			/*!
				@var	c_PSEntryName
				@brief	ピクセルシェーダーのエントリーポイントの名前
			*/
			static constexpr std::string_view c_PSEntryName = "PS";

			/*!
				@var	c_VSProfile
				@brief	頂点シェーダーのプロファイル
			*/
			static constexpr std::string_view c_VSProfile = "vs_5_0";

			/*!
				@var	c_PSProfile
				@brief	ピクセルシェーダーのプロファイル
			*/
			static constexpr std::string_view c_PSProfile = "ps_5_0";

		};

	}
}
