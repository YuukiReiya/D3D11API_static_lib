/*
	@file	Texture.h
	@date	2019/06/06
	@author	番場 宥輝
	@brief	テクスチャ
*/
#pragma once
#include <D3D11.h>
#include <string>
#include <DirectXMath.h>
#include <wrl/client.h>
#include "Color.h"

/*!
	@namespace	API
	@brief		API名前空間に含める
*/
namespace API {

	class Texture
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
		explicit Texture();

		/*!
			@brief	ムーブコンストラクタ
		*/
		Texture(const Texture& ref) { *this = ref; }

		/*!
			@brief	デストラクタ
		*/
		~Texture();

		/*!
			@fn			Initialize
			@brief		テクスチャの初期化
			@detail		サンプラーステートとSRVの作成を同時に行う
						デフォルト引数で一番メジャーな設定を渡している
			@param[in]	読み込むテクスチャのパス
			@param[in]	設定するフィルタリング(アドレッシングモード)
			@param[in]	設定するタイルモード
			@return		S_OK:成功	E_FAIL:失敗
		*/
		HRESULT Initialize(std::string filePath, FilteringMode fMode = FilteringMode::Trilinear, TileMode tMode = TileMode::Wrap);

		/*!
			@fn			Initialize
			@brief		テクスチャの初期化
			@detail		サンプラーステートとSRVの作成を同時に行う
						デフォルト引数で一番メジャーな設定を渡している
			@param[in]	読み込むテクスチャのパス
			@param[in]	テクスチャのサイズ
			@param[in]	設定するフィルタリング(アドレッシングモード)
			@param[in]	設定するタイルモード
			@return		S_OK:成功	E_FAIL:失敗
		*/
		HRESULT Initialize(std::string filePath,DirectX::XMINT2 size, FilteringMode fMode = FilteringMode::Trilinear, TileMode tMode = TileMode::Wrap);

		/*!
			@fn		ファイナライズ
			@brief	破棄処理
			@detail	メンバの明示的な開放
		*/
		void Finalize();

		/*!
			@fn		GetSharedPtr
			@brief	スマートポインタ(シェアードポインタ)のゲッター
			@note	描画オブジェクトには"weak_ptr"を持たせるためテクスチャの参照取得用
		*/
		inline std::shared_ptr<Texture*>GetSharedPtr() { return m_pShared; }

		/*!
			@fn		GetSamplerState
			@brief	サンプラーステートの取得
			@return	メンバのサンプラーステートのアドレス
		*/
		inline ID3D11SamplerState**GetSamplerState() { return m_pSamplerState.GetAddressOf(); }

		/*!
			@fn		GetShaderResourceView
			@brief	シェーダーリソースビューの取得
			@return	メンバのシェーダーリソースビューのアドレス
		*/
		inline ID3D11ShaderResourceView**GetShaderResourceView() { return m_pShaderResourceView.GetAddressOf(); }

		/*!
			@fn		GetSize
			@brief	サイズのゲッター
			@return	テクスチャのサイズ
		*/
		inline DirectX::XMINT2 GetSize()const { return m_Size; }

		/*!
			@fn			SetupSamplerState
			@brief		サンプラーステートの作成
			@param[in]	設定するフィルタリング(アドレッシングモード)
			@param[in]	設定するタイルモード
			@return		S_OK:成功	E_FAIL:失敗
		*/
		HRESULT SetupSamplerState(const FilteringMode fMode,const TileMode tMode);

		/*!
			@fn			SetupSRV
			@brief		SRVの作成
			@param[in]	読み込むテクスチャのパス
			@return		読み取り結果 S_OK:成功	E_FAIL:失敗
		*/
		HRESULT SetupShaderResourceView(std::string filePath);

		/*!
			@fn			Load
			@brief		SRVの作成
			@detail		関数ポインタを用いたSetupShaderResourceViewの別名定義
			@param[in]	読み込むテクスチャのパス
			@return		読み取り結果 S_OK:成功	E_FAIL:失敗
			@note		SetupShaderResourceViewだと分かりづらいと思ったのでユーザビリティに配慮した別名定義として実装
		*/
		HRESULT Load(std::string filePath) {
			HRESULT(Texture::*fp)(std::string) = &Texture::SetupShaderResourceView;
			return(this->*fp)(filePath);
		}

		/*!
			@fn			SetupAtlas
			@brief		アトラス情報のセットアップ
			@detail		デフォルト引数を適用すると既に設定した値を使う
			@param[in]	描画するアトラスのインデックス番号。
						(分割したアトラスの横(x)何番目で縦(y)何番目かを指定)
			@param[in]	横に何個分割するか: -1:設定済みの値を使う
			@param[in]	縦に何個分割するか: -1:設定済みの値を使う
			@note		アトラスを使うだけなのにオフセットと分割数でゲッターとセッターを用意すると手順が増え、
						ユーザビリティの低下が心配だったのでアトラスは一つにまとめた。
						分割数は負の値を指定できないように"unsigned"で指定させる
		*/
		void SetupAtlas(DirectX::XMINT2 atlasOffset, unsigned int div_x = c_ConfiguredDevCount, unsigned int div_y = c_ConfiguredDevCount);

		/*!
			@fn		GetDivNum
			@brief	画像の分割数を取得
		*/
		DirectX::XMINT2 GetDivCount()const { return m_DivCount; };

		/*!
			@fn		GetAtlasOffset
			@brief	アトラスのインデックス番号を取得
		*/
		DirectX::XMINT2 GetAtlasOffset()const { return m_AtlasOffset; };

		/*!
			@var	color
			@brief	テクスチャに適用する色
		*/
		Color color;

	private:
		/*!
			@var	c_ConfiguredDevCount
			@brief	設定済みの分割数を扱うための定数
		*/
		static constexpr int c_ConfiguredDevCount = -1;

		/*!
			@var	m_pSamplerState
			@brief	サンプラーステート
		*/
		Microsoft::WRL::ComPtr<ID3D11SamplerState>		m_pSamplerState;

		/*!
			@var	m_pShaderResourceView
			@brief	シェーダーリソースビュー(SRV)
		*/
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>m_pShaderResourceView;	

		/*!
			@var	m_eTileMode
			@brief	タイリングモード
			@detail	サンプラーステートを構成している"タイリングモード"の設定
		*/
		TileMode m_eTileMode;

		/*!
			@var	m_eFilterMode
			@brief	フィルタリングモード
			@detail	サンプラーステートを構成している"フィルタリングモード"の設定
		*/
		FilteringMode m_eFilterMode;

		/*!
			@var	m_Size
			@brief	テクスチャサイズ
		*/
		DirectX::XMINT2 m_Size;

		/*!
			@var	m_DivCount
			@brief	テクスチャの分割数(※アトラス設定のみ)
		*/
		DirectX::XMINT2 m_DivCount;

		/*!
			@var	m_AtlasOffset
			@brief	描画するアトラスのインデックス番号。
		*/
		DirectX::XMINT2 m_AtlasOffset;

		/*!
			@var	m_pShared
			@brief	自身のシェアードポインタ
			@note	描画オブジェクト側に"weak_ptr"を設定するため、取得用の変数。
					クラスのメンバに持たせておくことで"Texture"クラス自体の宣言が
					生とスマートポインタどちらも使えるようになる。
		*/
		std::shared_ptr<Texture*>m_pShared;
	};
};
