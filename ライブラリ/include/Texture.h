/*

	@file	Texture.h
	@date	2018/11/22
	@author	番場 宥輝
	@brief	テクスチャ
*/
#pragma once
#include <D3D11.h>
#include <string>
#include <DirectXMath.h>
#include <wrl/client.h>
#include "ITexture.h"

/*! APIの名前空間に含める */
namespace API {

	class Texture
		: public ITexture
	{
	public:
		/*!
			@brief	コンストラクタ
		*/
		explicit Texture();

		/*!
			@brief	デストラクタ
		*/
		~Texture()override;

		/*!
			@fn			イニシャライズ
			@brief		初期化
			@detail		画像の読み込みとサンプラーステートの作成を行う
			@param[in]	画像のパス
			@return		成功:S_OK 失敗:E_FAIL

		*/
		HRESULT Initialize(std::string filePath);

		/*!
			@fn			イニシャライズ
			@brief		初期化
			@detail		画像の読み込みと画像サイズの指定を行い、サンプラーステートの作成を行う
			@param[in]	画像のパス
			@param[in]	画像サイズ
			@return		成功:S_OK 失敗:E_FAIL
		*/
		HRESULT Initialize(std::string filePath, const DirectX::XMINT2 size);

		/*!
			@fn			イニシャライズ
			@brief		初期化
			@detail		画像の読み込みとサンプラーステートの作成を行う
			@param[in]	画像のパス
			@param[in]	タイリングモードの設定
			@return		成功:S_OK 失敗:E_FAIL
		*/
		HRESULT Initialize(std::string filePath, const TileMode tileMode);

		/*!
			@fn			イニシャライズ
			@brief		初期化
			@detail		画像の読み込みと画像サイズの指定を行い、サンプラーステートの作成を行う
			@param[in]	画像のパス
			@param[in]	画像サイズ
			@param[in]	タイリングモードの設定
			@return		成功:S_OK 失敗:E_FAIL
		*/
		HRESULT Initialize(std::string filePath, const DirectX::XMINT2 size, const TileMode tileMode);
	
		/*!
			@fn			イニシャライズ
			@brief		初期化
			@detail		画像の読み込みと画像サイズの指定を行い、サンプラーステートの作成を行う
			@param[in]	画像のパス
			@param[in]	フィルタリングモードの設定
			@return		成功:S_OK 失敗:E_FAIL
		*/
		HRESULT Initialize(std::string filePath, const FilteringMode filterMode);
		
		/*!
			@fn			イニシャライズ
			@brief		初期化
			@detail		画像の読み込みと画像サイズの指定を行い、サンプラーステートの作成を行う
			@param[in]	画像のパス
			@param[in]	画像サイズ
			@param[in]	フィルタリングモードの設定
			@return		成功:S_OK 失敗:E_FAIL
		*/
		HRESULT Initialize(std::string filePath, const DirectX::XMINT2 size, const FilteringMode filterMode);

		/*!
			@fn			イニシャライズ
			@brief		初期化
			@detail		画像の読み込みと画像サイズの指定を行い、サンプラーステートの作成を行う
			@param[in]	画像のパス
			@param[in]	タイリングモードの設定
			@param[in]	フィルタリングモードの設定
			@return		成功:S_OK 失敗:E_FAIL
		*/
		HRESULT Initialize(std::string filePath, const TileMode tileMode, const FilteringMode filterMode);

		/*!
			@fn			イニシャライズ
			@brief		初期化
			@detail		画像の読み込みと画像サイズの指定を行い、サンプラーステートの作成を行う
			@param[in]	画像のパス
			@param[in]	画像サイズ
			@param[in]	タイリングモードの設定
			@param[in]	フィルタリングモードの設定
			@return		成功:S_OK 失敗:E_FAIL
		*/
		HRESULT Initialize(std::string filePath, const DirectX::XMINT2 size, const TileMode tileMode, const FilteringMode filterMode);
	
		/*!
			@fn		ファイナライズ
			@brief	破棄処理
			@detail	メンバの明示的な開放
		*/
		void Finalize()override;
	};
};
