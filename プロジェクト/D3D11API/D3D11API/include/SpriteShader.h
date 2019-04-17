/*
	@file	SpriteShader
	@date	2019/04/13
	@author	番場 宥輝
	@brief	スプライトのデフォルトシェーダー
*/
#pragma once
#include "AbstractShader.h"

namespace D3D11
{
	namespace Graphic {

		class SpriteShader
			: public AbstractShader
		{
		public:
			/*!
				@brief	コンストラクタ
			*/
			SpriteShader();

			/*!
				@brief	デストラクタ
			*/
			~SpriteShader();

			/*!
				@fn		Setup
				@brief	プリコンパイル済みシェーダーファイルを利用してセットアップを行う
				@detail	オーバーライド
				@TODO	読み込めない.csoファイルが存在し、動作が安定しない。
			*/
			HRESULT Setup()override final;

			/*!
				@fn			Setup
				@brief		ファイルの階層を行い、プリコンパイル済みシェーダーファイルを利用したセットアップを行う
				@detail		オーバーロード
				@param[in]	ファイルの階層パス
				@NOTE		シェーダーファイルの配置位置が人によって変わってしまうため、ディレクトリだけでも設定出来るようにする
			*/
			HRESULT Setup(std::string& directory);

			/*!
				@fn		DynamicSetup
				@brief	動的コンパイルを利用したセットアップを行う
				@detail	オーバーライド
			*/
			HRESULT DynamicSetup()override final;

			/*!
				@fn			DynamicSetup
				@brief		ファイルの階層を行い、動的コンパイルを利用したセットアップを行う
				@detail		オーバーロード
				@param[in]	ファイルの階層パス
				@NOTE		シェーダーファイルの配置位置が人によって変わってしまうため、ディレクトリだけでも設定出来るようにする
			*/
			HRESULT DynamicSetup(std::string& directory);
		private:
			/*!
				@fn			SetupDirectory
				@brief		ファイルを参照するディレクトリを設定
				@param[in]	参照するディレクトリ
			*/
			void SetupDirectory(std::string& path);

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
				@fn			CreateInputLayout	
				@brief		頂点レイアウトの作成
				@param[in]	コンパイル済みシェーダーファイル(バイナリデータ)
				@return		S_OK:成功 E_FAIL:失敗
			*/
			HRESULT CreateInputLayout(CustomShaderBin*bin);


			/*!
				@var	m_Directory
				@brief	シェーダーファイルの階層文字列
			*/
			std::string m_Directory;

			/*!
				@var	c_HLSL
				@brief	HLSLのファイル名
			*/
			std::string_view c_HLSL			= "SpriteShader.hlsl";

			/*!
				@var	c_CSO
				@brief	CSOのファイル名
			*/
			std::string_view c_CSO			= "SpriteShader.cso";

			/*!
				@var	c_VSEntryName
				@brief	頂点シェーダーのエントリーポイントの名前
			*/
			std::string_view c_VSEntryName	= "VS";
			
			/*!
				@var	c_PSEntryName
				@brief	ピクセルシェーダーのエントリーポイントの名前
			*/
			std::string_view c_PSEntryName	= "PS";
			
			/*!
				@var	c_VSProfile
				@brief	頂点シェーダーのプロファイル
			*/
			std::string_view c_VSProfile	= "vs_5_0";

			/*!
				@var	c_PSProfile
				@brief	ピクセルシェーダーのプロファイル
			*/
			std::string_view c_PSProfile	= "ps_5_0";
		};
	}
}