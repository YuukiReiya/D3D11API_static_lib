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

/*! APIの名前空間に含める */
namespace API{
	/*!
		@brief スプライトを扱うクラス
	*/
	class Sprite
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
				@fn		イニシャライズ
				@brief	初期化
				@detail	ブレンドステートの作成を行う
			*/
			HRESULT Initialize();

			/*!
				@fn		ファイナライズ
				@brief	破棄処理
				@detail	メンバの明示的な解放とメンバの初期化
			*/
			void Finalize();

			/*!
				@fn		リリース
				@brief	解放処理
				@detail	メンバの明示的な解放
			*/
			void Release();

			/*!
				@fn			テクスチャの描画
				@brief		シンプルテクスチャの描画
				@detail		map,unmapを使ってテクスチャマッピングをする描画
				@param[in]	描画するテクスチャ
				@return		成功:S_OK 失敗:E_FAIL
			*/
			HRESULT Render(Texture* pTexture);

			/*!
				@fn			テクスチャの描画
				@brief		アトラステクスチャの描画
				@detail		SubResourceを使ってUpdateSubResourceをする描画
				@param[in]	描画するテクスチャ
				@return		成功:S_OK 失敗:E_FAIL
			*/
			HRESULT Render(TextureAtlas* pTexture);

			/*!
				@fn			タイリング描画
				@brief		テクスチャをタイリングして描画
				@detail		頂点情報のUVを範囲外にすることで実現
							※比率の維持を優先しているため、別途大きさを変更する必要がある
				@param[in]	描画するテクスチャ
				@param[in]	タイル表示する割合
				@return		成功:S_OK 失敗:E_FAIL
			*/
			HRESULT RenderTile(Texture* pTexture, const DirectX::XMFLOAT2 ratio);

			//----------------------------------------------------------------------------------------
			/*! Transformクラスに書き直す予定 */
			DirectX::XMFLOAT3 GetPos()const { return m_Pos; }
			void SetPos(DirectX::XMFLOAT3 pos);
			void SetPos(DirectX::XMFLOAT2 pos);
			void SetRot(DirectX::XMFLOAT3 rot) { m_Rot.x = rot.x, m_Rot.y = rot.y, m_Rot.z = rot.z; }
			void SetScale(DirectX::XMFLOAT2 scale);
			void SetStencilMask(uint32_t mask) { m_StencilMask = mask; }
			//----------------------------------------------------------------------------------------

			/*!
				@fn			ブレンドステートの作成
				@brief		作成したブレンドステートは描画時にバインドされる
				@detail		メンバのブレンドステートをを変更
				@param[in]	自己定義のブレンドステート
			*/
			void CreateAlphaBlendState(D3D11_BLEND_DESC desc);

			/*!
				@fn			SetupBlendState
				@brief		指定したプリセットのブレンドステートをメンバに設定する
				@param[in]	指定するプリセットの列挙体
			*/
			void SetupBlendState(BlendPreset preset);

		private:
			/*!
				@var	c_VertexCount
				@brief	スプライトの頂点数
				@detail	スプライトの頂点数の定数化
			*/
			static constexpr int c_VertexCount = 4;/*!< スプライトの頂点数 */

			/*!
				@var	c_NormalizeSize
				@brief	基準となるサイズ
				@detail	このピクセルがScaleの1に相当する
				@value	100.0f
			*/
			static const float c_NormalizeSize;

			/*!
				@var	c_ScaleZ
				@brief	板ポリのZスケール
				@detail	生成する頂点のZ方向の大きさ
				@value	0
			*/
			static const float c_ScaleZ;

			/*!
				@var	c_VertexZ
				@brief	板ポリの頂点生成位置
				@detail	CreateVertex関数で生成する頂点のZ位置
				@value	0
			*/
			static const float c_VertexZ;

			/*!
				@fn			頂点生成
				@brief		スプライトの頂点を生成する
				@detail		頂点構造体を使って、情報を頂点バッファにバインド
				@param[in]	画像サイズ
				@return		成功:S_OK 失敗:E_FAIL
			*/
			HRESULT CreateVertex(DirectX::XMINT2 size);

			/*!
				@fn			タイリングテクスチャの頂点生成
				@brief		タイリング用の頂点生成
				@detail		タイリング用のUVを拡張した頂点を生成する
				@param[in]	画像サイズ
				@param[in]	タイリングする割合
				@return		成功:S_OK 失敗:E_FAIL
			*/
			HRESULT CreateTilingVertex(DirectX::XMINT2 size, DirectX::XMFLOAT2 ratio);


			uint32_t									m_StencilMask;
			Microsoft::WRL::ComPtr<ID3D11Buffer>		m_pVertexBuffer;
			Microsoft::WRL::ComPtr<ID3D11BlendState>	m_pBlendState;
			//Microsoft::WRL::ComPtr<ID3D11BlendState>	m_pBlendStateMultiple;

			/****************************************/
			/*		　スプライトのパラメータ		*/
			/****************************************/

			/*! ローカル座標系 */
			DirectX::XMFLOAT3 m_Pos;
			DirectX::XMFLOAT3 m_Rot;
			DirectX::XMFLOAT3 m_Scale;
			/*! スプライトサイズのキャッシュ */
			DirectX::XMINT2 m_Size;
	};

}

/*! Direct3D11関連の名前空間 */
namespace D3D11 {

	/*! 描画関連の名前空間に含める */
	namespace Graphic {

		/****************************************/
		/*		スプライトで扱う構造体			*/
		/****************************************/

		/*!
			@brief	スプライトの頂点構造体
		*/
		struct SpriteVertex
			:public VARTEX_BASE
		{
			SpriteVertex(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT2 uv) {
				m_Pos = pos, m_UV = uv;
			}
			DirectX::XMFLOAT2 m_UV;/*< UV座標 */
		};

#pragma pack(push,16)
		/*!
			@brief	スプライトのコンスタントバッファ構造体
		*/
		struct SpriteShaderBuffer
			:CONSTANT_BUFFER_BASE
		{
			ALIGN16<DirectX::XMFLOAT2>	m_DivNum;
			ALIGN16<DirectX::XMFLOAT2>	m_Index;
			ALIGN16<DirectX::XMFLOAT4>	m_Color;	/*< カラー */
		};
#pragma pack(pop)
	}
}