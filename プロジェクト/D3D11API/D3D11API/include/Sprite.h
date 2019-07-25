/*
	@file	Sprite.h
	@date	2019/07/25
	@author	番場 宥輝
	@brief	スプライト関連
	@detail	αブレンドについて参考サイト:http://neareal.net/index.php?ComputerGraphics%2FXNA%2FBlendState
*/
#pragma once
#include <string>
#include <D3D11.h>
#include "StructShaderBase.h"
#include <DirectXMath.h>
#include "Texture.h"
#include "TextureAtlas.h"
#include "AbstractRender.h"
#include "AbstractShader.h"
#include "Transform.h"

/*! APIの名前空間に含める */
namespace API {
	/*!
		@brief スプライトを扱うクラス
	*/
	class Sprite
		:public AbstractRender
	{
	public:
		/*!
			@enum	BlendPreset
			@brief	ブレンドモードのプリセット指定用の列挙体
			@detail	参考:http://maverickproj.web.fc2.com/d3d11_01.html
		*/
		enum BlendPreset
		{
			/*!
				@var	Default
				@brief	デフォルト(ブレンド無し)
			*/
			Default,

			/*!
				@var	Linear
				@brief	線形合成
			*/
			Linear,

			/*!
				@var	Add
				@brief	加算合成
			*/
			Add,

			/*!
				@var	Subtraction
				@brief	減算合成
			*/
			Subtraction,

			/*!
				@var	Multiple
				@brief	乗算合成
			*/
			Multiple
		};

		/*!
			@brief	コンストラクタ
		*/
		explicit Sprite();

		/*!
			@brief	デストラクタ
		*/
		~Sprite();

		/*!
			@fn			Initialize
			@brief		初期化
			@param[in]	テクスチャ
			@return		成功:S_OK 失敗:E_FAIL
		*/
		HRESULT Initialize(Texture*texture);

		/*!
			@fn		ファイナライズ
			@brief	破棄処理
			@detail	メンバの明示的な解放とメンバの初期化
		*/
		void Finalize();

		/*!
			@fn		Render
			@brief	描画
		*/
		void Render()override;

		/*!
			@fn			SetStencilMask
			@brief		深度マスクの設定
			@detail		インライン関数
			@param[in]	設定するマスク値
		*/
		inline void SetStencilMask(uint32_t mask) { m_StencilMask = mask; }

		/*!
			@fn			SetupBlendPreset
			@brief		指定したプリセットのブレンドステートをメンバに設定する
			@param[in]	指定するプリセットの列挙体
		*/
		void SetupBlendPreset(BlendPreset preset);

		/*!
			@fn			SetupTexture
			@brief		テクスチャの設定
			@detail		弱参照でバインドし、この時点で頂点生成を行う
			@param[in]	登録するテクスチャのポインタ
		*/
		void SetupTexture(Texture* texture);

		/*!
			@fn			SetupShader
			@brief		シェーダーの設定
			@detail		弱参照でバインドする
			@param[in]	登録するシェーダーのポインタ
		*/
		void SetupShader(D3D11::Graphic::AbstractShader* shader);

		/*!
			@var	transform
			@brief	トランスフォーム(行列クラス)
		*/
		std::shared_ptr<Transform>transform;

		/*!
			@var	color
			@brief	色の乗算値(※)
			@detail	計算方法はシェーダーに依存
		*/
		Color color;
	private:
		/*!
			@fn		SetupTopology
			@brief	トポロジーの設定
			@note	スプライトは板ポリゴン実装なので、効率がいいTRIANGLESTRIPを指定
		*/
		void SetupTopology();

		/*!
			@fn			SetupInputLayout
			@brief		頂点レイアウトの設定
			@param[in]	シェーダー
			@note		弱参照で取得したポインタから取得
		*/
		void SetupInputLayout(D3D11::Graphic::AbstractShader*shader);

		/*!
			@fn			SetupBindShader
			@brief		シェーダーのバインド
			@param[in]	シェーダー
			@note		弱参照で取得したポインタから取得
		*/
		void SetupBindShader(D3D11::Graphic::AbstractShader*shader);
		
		/*!
			@fn			SetupConstantBuffer
			@brief		コンスタントバッファの設定
			@param[in]	シェーダー
			@note		弱参照で取得したポインタから取得
		*/
		void SetupConstantBuffer(D3D11::Graphic::AbstractShader*shader);

		/*!
			@fn		SetupVertexBuffer
			@brief	頂点バッファ設定
		*/
		void SetupVertexBuffer();

		/*!
			@fn			SetupSampler
			@brief		サンプラーステートの設定
			@param[in]	テクスチャ
			@note		弱参照で取得したポインタから取得
		*/
		void SetupSampler(Texture*texture);

		/*!
			@fn			SetupSRV
			@brief		ShaderResourceViewの設定
			@param[in]	テクスチャ
			@note		弱参照で取得したポインタから取得
		*/
		void SetupSRV(Texture*texture);

		/*!
			@fn		SetupBlendState
			@brief	ブレンドステートを設定
		*/
		void SetupBlendState();

		/*!
			@fn			CreateVertexBuffer
			@brief		頂点の生成
			@detail		画像のサイズに合わせて板ポリゴンを生成する
			@param[in]	画像のサイズ
			@return		成功:S_OK 失敗:E_FAIL
*/
		HRESULT CreateVertexBuffer(const DirectX::XMFLOAT2 size);

		/*!
			@var	m_pBlendState
			@brief	ブレンドステート
		*/
		Microsoft::WRL::ComPtr<ID3D11BlendState>m_pBlendState;

		/*!
			@var	m_StencilMask
			@brief	深度マスク
		*/
		uint32_t m_StencilMask;

		/*!
			@var	m_pVertexBuffer
			@brief	頂点バッファ
		*/
		Microsoft::WRL::ComPtr<ID3D11Buffer>m_pVertexBuffer;

		/*!
			@var	m_pShader
			@brief	シェーダーオブジェクトの弱参照
		*/
		std::weak_ptr<D3D11::Graphic::AbstractShader*>m_pShader;

		/*!
			@var	m_pTexture
			@brief	テクスチャオブジェクトの弱参照
		*/
		std::weak_ptr<Texture*>m_pTexture;
		
		/*!
			@var	c_NormalizeSize
			@brief	板ポリゴンの基準となるサイズ
			@detail	このピクセルがScaleの1に相当する
		*/
		static constexpr float c_NormalizeSize = 5.0f;
	}; 
}

