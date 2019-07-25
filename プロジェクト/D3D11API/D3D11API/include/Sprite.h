/*
	@file	Sprite.h
	@date	2018/03/04
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

/*! Direct3D11関連の名前空間 */
//namespace D3D11 {
//
//	/*! 描画関連の名前空間に含める */
//	namespace Graphic {
//
//		/****************************************/
//		/*		スプライトで扱う構造体			*/
//		/****************************************/
//
//		/*!
//			@brief	スプライトの頂点構造体
//		*/
//		struct SpriteVertex
//			:public BaseVertex
//		{
//			SpriteVertex(const DirectX::XMFLOAT3 pos, const DirectX::XMFLOAT2 uv) {
//				this->position = pos, m_UV = uv;
//			}
//			/*!
//				@var	m_UV
//				@brief	UV座標
//			*/
//			DirectX::XMFLOAT2 m_UV;
//		};
//
//#pragma pack(push,16)
//		/*!
//			@brief	スプライトのコンスタントバッファ構造体
//		*/
//		struct alignas(16) SpriteShaderBuffer
//			:BaseConstantBuffer
//		{
//			/*!
//				@var	m_DivNum
//				@brief	テクスチャの分割数
//			*/
//			DirectX::XMFLOAT2 m_DivNum;
//
//			/*!
//				@var	m_Index
//				@brief	分割したテクスチャの表示位置
//			*/
//			DirectX::XMFLOAT2 m_Index;
//
//			/*!
//				@var	m_Color
//				@brief	カラー
//			*/
//			DirectX::XMFLOAT4 m_Color;
//		};
//#pragma pack(pop)
//	}
//}

/*! APIの名前空間に含める */
namespace API {
	/*!
		@brief スプライトを扱うクラス
	*/
	//	class Sprite
	//		: public AbstractRender
	//	{
	//		public:
	//			/*!
	//				@enum	BlendPreset
	//				@brief	ブレンドモードのプリセット指定用の列挙体
	//				@detail	参考:http://maverickproj.web.fc2.com/d3d11_01.html
	//			*/
	//			enum BlendPreset
	//			{
	//				/*!
	//					@var	Default
	//					@brief	デフォルト(ブレンド無し)
	//				*/
	//				Default,
	//
	//				/*!
	//					@var	Linear
	//					@brief	線形合成
	//				*/
	//				Linear,
	//
	//				/*!
	//					@var	Add
	//					@brief	加算合成
	//				*/
	//				Add,
	//
	//				/*!
	//					@var	Subtraction
	//					@brief	減算合成
	//				*/
	//				Subtraction,
	//
	//				/*!
	//					@var	Multiple
	//					@brief	乗算合成
	//				*/
	//				Multiple
	//			};
	//
	//			/*!
	//				@brief	コンストラクタ
	//			*/
	//			explicit Sprite();
	//
	//			/*!
	//				@brief	デストラクタ
	//			*/
	//			~Sprite();
	//
	//			/*!
	//				@fn		Initialize
	//				@brief	初期化
	//				@detail	ブレンドステートの作成を行う
	//			*/
	//			HRESULT Initialize();
	//
	//			/*!
	//				@fn		Finalize
	//				@brief	破棄処理
	//				@detail	メンバの明示的な解放とメンバの初期化
	//			*/
	//			void Finalize();
	//
	//			/*!
	//				@fn		Render
	//				@brief	描画
	//			*/
	//			void Render()override;
	//
	//			/*!
	//				@fn			SetStencilMask
	//				@brief		深度マスクの設定
	//				@detail		インライン関数
	//				@param[in]	設定するマスク値
	//			*/
	//			inline void SetStencilMask(uint32_t mask) { m_StencilMask = mask; }
	//
	//			/*!
	//				@fn			SetupBlendPreset
	//				@brief		指定したプリセットのブレンドステートをメンバに設定する
	//				@param[in]	指定するプリセットの列挙体
	//			*/
	//			void SetupBlendPreset(BlendPreset preset);
	//
	//			/*!
	//				@fn			SetupTexture
	//				@brief		テクスチャの設定
	//				@detail		弱参照でバインドし、この時点で頂点生成を行う
	//				@param[in]	登録するテクスチャのポインタ
	//			*/
	//			void SetupTexture(Texture* texture);
	//
	//			/*!
	//				@fn			SetupShader
	//				@brief		シェーダーの設定
	//				@detail		弱参照でバインドする
	//				@param[in]	登録するシェーダーのポインタ
	//			*/
	//			void SetupShader(D3D11::Graphic::AbstractShader* shader);
	//
	//			std::shared_ptr<Transform> transform;
	//
	//		private:
	//			/*!
	//				@var	c_VertexCount
	//				@brief	スプライトの頂点数
	//				@detail	スプライトの頂点数の定数化
	//			*/
	//			static constexpr int c_VertexCount = 4;/*!< スプライトの頂点数 */
	//
	//			/*!
	//				@var	c_DefaultSize
	//				@brief	基準となるサイズ
	//				@detail	このピクセルがScaleの1に相当する
	//				@value	100.0f
	//			*/
	//			static constexpr float c_DefaultSize = 100.0f;
	//
	//			/*!
	//				@var	c_ScaleZ
	//				@brief	板ポリのZスケール
	//				@detail	生成する頂点のZ方向の大きさ
	//				@value	0
	//			*/
	//			static constexpr float c_ScaleZ = 0;
	//
	//			/*!
	//				@var	c_VertexZ
	//				@brief	板ポリの頂点生成位置
	//				@detail	CreateVertexBuffer関数で生成する頂点のZ位置
	//				@value	0
	//			*/
	//			static constexpr float c_VertexZ = 0;
	//
	//			/*!
	//				@fn			頂点生成
	//				@brief		スプライトの頂点を生成する
	//				@detail		頂点構造体を使って、情報を頂点バッファにバインド
	//				@param[in]	画像サイズ
	//				@return		成功:S_OK 失敗:E_FAIL
	//			*/
	//			//HRESULT CreateVertex(DirectX::XMINT2 size);
	//
	//			/*!
	//				@fn			CreateVertexBuffer
	//				@brief		頂点の設定
	//			*/
	//			//void SetupVertex();
	//
	//			/*!
	//				@fn		SetupTopology
	//				@brief	トポロジーの設定
	//				@NOTE	スプライトは板ポリゴン実装なので、効率がいいTRIANGLESTRIPを指定
	//			*/
	//			void SetupTopology();
	//
	//			/*!
	//				@fn		CreateInputLayout
	//				@brief	頂点レイアウトの設定
	//			*/
	//			void SetupInputLayout();
	//
	//			/*!
	//				@fn		SetupBindShader
	//				@brief	シェーダーの設定
	//			*/
	//			void SetupBindShader();
	//
	//			/*!
	//				@fn		SetupSampler
	//				@brief	サンプラーステートの設定
	//			*/
	//			void SetupSampler();
	//
	//			/*!
	//				@fn		SetupSRV
	//				@brief	ShaderResourceViewの設定
	//			*/
	//			void SetupSRV();
	//
	//			/*!
	//				@fn		SetupConstantBuffer
	//				@brief	コンスタントバッファの設定
	//			*/
	//			//void SetupConstantBuffer();
	//
	//			/*!
	//				@fn		CreateVertexBuffer
	//				@brief	頂点バッファ設定
	//			*/
	//			//void SetupVertexBuffer();
	//
	//			/*!
	//				@fn		SetupBlendState
	//				@brief	ブレンドステートを設定
	//			*/
	//			void SetupBlendState();
	//
	//			/*!
	//				@var	m_StencilMask
	//				@brief	深度マスク
	//			*/
	//			uint32_t									m_StencilMask;
	//
	//			/*!
	//				@var	m_pVertexBuffer
	//				@brief	頂点バッファ
	//			*/
	//			Microsoft::WRL::ComPtr<ID3D11Buffer>		m_pVertexBuffer;
	//
	//			/*!
	//				@var	m_pBlendState
	//				@brief	ブレンドステート
	//			*/
	//			Microsoft::WRL::ComPtr<ID3D11BlendState>	m_pBlendState;
	//
	//			/*!
	//				@var	m_pTexture
	//				@brief	テクスチャオブジェクトの弱参照
	//			*/
	//			std::weak_ptr<Texture*>							m_pTexture;
	//
	//			/*!
	//				@var	m_pShader
	//				@brief	シェーダーオブジェクトの弱参照
	//			*/
	//			std::weak_ptr<D3D11::Graphic::AbstractShader*>	m_pShader;
	//
	//			/****************************************/
	//			/*		　スプライトのパラメータ		*/
	//			/****************************************/
	//			Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>m_pSRV;
	//
	//			struct Vertex
	//			{
	//				DirectX::XMFLOAT3 position;
	//				DirectX::XMFLOAT2 uv;
	//			};
	//
	//			HRESULT SetupConstantBuffer();
	//#pragma region OLD
	//#if 0
	//			/*!
	//				@var	m_Size
	//				@brief	スプライトで生成する画像のキャッシュ
	//			*/
	//			DirectX::XMINT2 m_Size;
	//
	//			/*!
	//				@var	m_pIndexVertexBuffer
	//				@brief	頂点バッファ
	//			*/
	//			Microsoft::WRL::ComPtr<ID3D11Buffer>m_pIndexBuffer;
	//
	//			/*!
	//				@fn			頂点生成
	//				@brief		スプライトの頂点を生成する
	//				@detail		頂点構造体を使って、情報を頂点バッファにバインド
	//				@return		成功:S_OK 失敗:E_FAIL
	//			*/
	//			//HRESULT CreateIndexBuffer();
	//			//HRESULT CreateVertexBuffer();
	//
	//
	//			static constexpr DirectX::XMFLOAT3 c_Vertices[] = {
	//				{-0.5f * 100,-0.5f * 100,0},//	左上
	//				{ 0.5f * 100,-0.5f * 100,0},//	右上
	//				{ 0.5f * 100, 0.5f * 100,0},//	右下
	//				{-0.5f * 100, 0.5f * 100,0},//	左下
	//			};
	//			static constexpr uint8_t c_Indices[] = {
	//				0,1,2,2,3,0,2 ,
	//			};
	//#endif // 0
	//#pragma endregion
	//
	//	};

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

		explicit Sprite();
		~Sprite() {}

		HRESULT Init();
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

		std::shared_ptr<Transform>transform;

	private:
		/*!
			@fn		SetupTopology
			@brief	トポロジーの設定
			@NOTE	スプライトは板ポリゴン実装なので、効率がいいTRIANGLESTRIPを指定
		*/
		void SetupTopology();

		/*!
			@fn		CreateInputLayout
			@brief	頂点レイアウトの設定
		*/
		void SetupInputLayout();

		/*!
			@fn		SetupBindShader
			@brief	シェーダーのバインド
		*/
		void SetupBindShader();

		/*!
			@fn		SetupSampler
			@brief	サンプラーステートの設定
		*/
		void SetupSampler();

		/*!
			@fn		SetupSRV
			@brief	ShaderResourceViewの設定
		*/
		void SetupSRV();

		/*!
			@fn		SetupConstantBuffer
			@brief	コンスタントバッファの設定
		*/
		//void SetupConstantBuffer();

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
		Microsoft::WRL::ComPtr<ID3D11Buffer>m_pVertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11InputLayout>m_pInputLayout;
		Microsoft::WRL::ComPtr<ID3D11VertexShader>m_pVertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader>m_pPixelShader;
		Microsoft::WRL::ComPtr<ID3D11Buffer>m_pConstantBuffer;

		Color color;
		Microsoft::WRL::ComPtr<ID3D11SamplerState>m_pSamplerState;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>m_pShaderResourceView;
	}; 


	struct SpriteVertex
	{
		SpriteVertex(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT2 uv)
		{
			this->pos = pos, this->uv = uv;
		}
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 uv;
	};

}

