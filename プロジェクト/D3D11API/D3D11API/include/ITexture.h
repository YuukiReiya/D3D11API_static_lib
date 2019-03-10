/*
	@file	ITexture.h
	@date	2018/11/20
	@author	番場 宥輝
	@brief	テクスチャのインターフェイスとして制作した抽象クラス
	@detail	通常テクスチャとアトラステクスチャを別クラスで作るため共通な処理を持った基底クラスとして作成
*/
#pragma once
#include <D3D11.h>
#include <DirectXMath.h>
#include <wrl/client.h>
#include <string>
#include "Color.h"

/*!
	@namespace	API
	@brief		API名前空間に含める
*/
namespace API {

	class ITexture abstract
	{
	public:
		/*!
			@enum	TileMode
			@brief	タイリングモードの設定
		*/
		enum TileMode
		{
			// 繰り返し無し
			Clamp = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP,			// デフォルト
			Border = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_BORDER,			// SAMPLER_DESCまたはHLSLで指定した境界色を設定
			MirrorOnce = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_MIRROR_ONCE,
			// タイリング 
			Wrap = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP,				// 繰り返し
			Mirror = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_MIRROR,			// 繰り返し反転 

		};

		/*!
			@enum	FilteringMode
			@brief	フィルタリングモードの設定
		*/
		enum FilteringMode
		{
			Point = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_POINT,			// ポイントフィルタリング:最悪品質でコスパ良
			Bilinear = D3D11_FILTER::D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT,	// バイリニアテクスチャフィルタリング
			Trilinear = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR,		// トリリニアテクスチャフィルタリング:デフォルト設定
			Anisotropic = D3D11_FILTER::D3D11_FILTER_ANISOTROPIC			// 異方性フィルタリング:最高品質でコスパ悪
		};

		/*!
			@brief	コンストラクタ
		*/
		ITexture();

		/*!
			@brief	仮想デストラクタ
		*/
		virtual ~ITexture();

		/*!
			@fn		ファイナライズ
			@brief	破棄処理
			@detail	メンバの明示的な開放
					派生先でメンバが増えることを踏まえ、仮想関数として実装
		*/
		virtual void Finalize();

		/*!
			@fn			画像サイズのセッター
			@brief		読み込む画像のサイズの設定
			@param[in]	画像サイズ
			@return		成功:true 失敗:false
		*/
		bool SetSize(const DirectX::XMINT2 size);

		/*!
			@fn			画像サイズのゲッター
			@brief		画像サイズの取得
			@detail		SetSizeで指定した画像のサイズを取得
		*/
		inline DirectX::XMINT2 GetSize()const { return m_Size; }

		/*!
			@fn			サンプラーステートのゲッター
			@brief		サンプラーステートの取得
			@detail		メンバのサンプラーステートのアドレス取得
						※サンプラーステートはComPtr
		*/
		inline ID3D11SamplerState**GetSamplerState() { return m_pSamplerState.GetAddressOf(); }

		/*!
			@fn		シェーダーリソースビューのゲッター
			@brief	シェーダーリソースビューの取得
			@detail	メンバのシェーダーリソースビューのアドレス取得
					※シェーダーリソースビューはComPtr
		*/
		inline ID3D11ShaderResourceView**GetShaderResourceView() { return m_pShaderResourceView.GetAddressOf(); }

		/*!
			@fn			SetTileAndFiltering
			@brief		タイリングモードとフィルタリングモードの設定
			@param[in]	設定するタイルモード
			@param[in]	設定するフィルタリング(アドレッシングモード)
			@detail		サンプラーステートを作成
		*/
		HRESULT SetTileAndFiltering(const TileMode tileMode, const FilteringMode filterMode);
	public:
		Color			m_Color;		// カラー */
	protected:
		TileMode		m_eTileMode;	// タイリングモード
		FilteringMode	m_eFilterMode;	// フィルタリングモード
		DirectX::XMINT2 m_Size;			// 画像サイズ
		Microsoft::WRL::ComPtr<ID3D11SamplerState>		m_pSamplerState;		// サンプラーステート
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>m_pShaderResourceView;	// シェーダーリソースビュー(SRV)


		/*!
			@fn			Load
			@brief		画像の読み込みを行いSRVを作成
			@detail		WICを使い画像データを読み込み、SRVを作成、メンバ変数にバインド
			@param[in]	読み込むファイルのパス
			@return		読み取り結果 成功:S_OK  失敗:E_FAIL
		*/
		HRESULT Load(std::string filePath);

		/*!
			@var	c_DefaultSize
			@brief	デフォルトの画像サイズ
			@detail	デフォルトで与えられている画像サイズ
		*/
		static const DirectX::XMINT2 c_DefaultSize;
	};
}