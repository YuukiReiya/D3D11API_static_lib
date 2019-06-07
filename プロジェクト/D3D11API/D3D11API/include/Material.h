/*
	@file	Material.h
	@date	2019/06/06
	@author	番場 宥輝
	@brief	マテリアル
*/
#pragma once
#include "Texture.h"

/*!
	@namespace	API
	@brief		API名前空間に含める
*/
namespace API {
	class Material
	{
	public:
		/*!
			@brief	コンストラクタ
		*/
		Material();

		/*!
			@brief	ムーブコンストラクタ
		*/
		Material(const Material&ref) { *this = ref; }

		/*!
			@brief	デストラクタ
		*/
		~Material();

		/*!
			@fn			SetupTexture
			@brief		テクスチャの設定
			@detail		TextureのInitializeを行っているだけのラッピング関数
			@param[in]	テクスチャのファイルパス
			@param[in]	設定するフィルタリング(アドレッシングモード)
			@param[in]	設定するタイルモード
			@return		S_OK:成功	E_FAIL:失敗

		*/
		inline HRESULT SetupTexture(std::string texturePath, Texture::FilteringMode fMode = Texture::FilteringMode::Bilinear, Texture::TileMode tMode = Texture::TileMode::Clamp) { return m_pTexture->Initialize(texturePath, fMode, tMode); }

		/*!
			@fn			SetupTexture
			@brief		テクスチャの設定
			@detail		内部でテクスチャの内包するTextureのスマートポインタを取得
			@param[in]	テクスチャのポインタ
		*/
		inline void SetupTexture(Texture*texture) { m_pTexture = std::make_shared<Texture>(); };
		
		/*!
			@fn		GetSharedPtr
			@brief	スマートポインタ(シェアードポインタ)のゲッター
			@note	描画オブジェクトには"weak_ptr"を持たせるためテクスチャの参照取得用
		*/
		inline std::shared_ptr<Material*>GetSharedPtr() { return m_pShared; }
		
		/*!
			@fn		GetSamplerState
			@brief	サンプラーステートの取得
			@detail	Textureのゲッターのラッパー
			@return	メンバのTextureのサンプラーステートのポインタ
			@note	Textureをpulicにすると他の不要な関数も触られてしまうため、
					関数でラッピング。
		*/
		inline ID3D11SamplerState**GetSamplerState() { return m_pTexture->GetSamplerState(); }

		/*!
			@fn		GetShaderResourceView
			@brief	シェーダーリソースビューの取得
			@return	メンバのシェーダーリソースビューのアドレス
		*/
		inline ID3D11ShaderResourceView**GetShaderResourceView() { return m_pTexture->GetShaderResourceView(); }

	private:
		/*!
			@var	m_pShared
			@brief	自身のシェアードポインタ
		*/
		std::shared_ptr<Material*>m_pShared;

		/*!
			@var	m_pTexture
			@brief	テクスチャ
		*/
		std::shared_ptr<Texture>m_pTexture;
	};
}