/*
	@file	AbstractShader.h
	@date	2019/04/05
	@author	番場 宥輝
	@brief	シェーダーの抽象クラス
	@note	描画クラス設計見直しに際し、更新していく
*/
#pragma once
#include <d3d11.h>
#include <memory>
#include <wrl/client.h>
#include <string>
#include "CustomShaderBin.h"

/*! Direct3D11関連 */
namespace D3D11 {

	/*! 描画関連の名前空間に含める */
	namespace Graphic {

		/*!
			@brief	シェーダーの抽象クラス
		*/
		class AbstractShader abstract {
		public:
			/*!
				@brief	コンストラクタ
				@detail	インスタンス生成時にインスタンスのアドレスをシェアード・ポインタに保持させる
			*/
			explicit AbstractShader();

			/*!
				@fn		Setup
				@brief	プリコンパイル済みシェーダーファイルを利用してセットアップを行う
				@detail	純粋仮想関数
			*/
			virtual HRESULT Setup()abstract;

			/*!
				@fn		DynamicSetup
				@brief	動的コンパイルを利用したセットアップを行う
				@detail	純粋仮想関数
			*/
			virtual HRESULT DynamicSetup()abstract;

			/*!
				@fn		GetInputLayout
				@brief	頂点レイアウトの取得
				@detail	インライン関数
			*/
			inline ID3D11InputLayout**GetInputLayout() { return m_pVertexLayout.GetAddressOf(); }

			/*!
				@fn		GetVertexShader
				@brief	頂点シェーダーの取得
				@detail	インライン関数
			*/
			inline ID3D11VertexShader**GetVertexShader(){ return m_pVertexShader.GetAddressOf(); }

			/*!
				@fn		GetPixelShader
				@brief	ピクセルシェーダーの取得
				@detail	インライン関数

			*/
			inline ID3D11PixelShader**GetPixelShader() { return m_pPixelShader.GetAddressOf(); }
			
			/*!
				@fn		GetConstantBuffer
				@brief	コンスタントバッファの取得
				@detail	インライン関数
			*/
			inline ID3D11Buffer**GetConstantBuffer() { return m_pConstantBuffer.GetAddressOf(); }
			
			/*!
				@fn		GetSharedPtr
				@brief	AbstractShader型シェアード・ポインタの取得
				@detail	インライン関数
						インスタンス生成時に自信のアドレスをスマートポインタに保持させる
				@note	Renderクラスからは弱参照の方が安全なため、設計を工夫	
			*/
			inline std::shared_ptr<AbstractShader*>GetSharedPtr() { return m_pShared; }

		protected:
			/*!
				@fn			DynamicCompile
				@brief		シェーダーファイルの動的コンパイル
				@param[in]	シェーダーファイルのパス
				@param[in]	エントリーポイント名
				@param[in]	シェーダーのプロファイル名 ex)vs_5_0
				@param[in]	コンパイル用のブロブ
				@return		S_OK:成功 E_FAIL:失敗
			*/
			static HRESULT DynamicCompile(std::string path, std::string funcName, std::string profileName, ID3DBlob**ppBlob);

			/*!
				@var	m_pVertexLayout
				@brief	頂点レイアウト
			*/
			Microsoft::WRL::ComPtr<ID3D11InputLayout>	m_pVertexLayout;

			/*!
				@var	m_pVertexShader
				@brief	頂点シェーダー
			*/
			Microsoft::WRL::ComPtr<ID3D11VertexShader>	m_pVertexShader;

			/*!
				@var	m_pPixelShader
				@brief	ピクセルシェーダー
			*/
			Microsoft::WRL::ComPtr<ID3D11PixelShader>	m_pPixelShader;

			/*!
				@var	m_pConstantBuffer
				@brief	コンスタントバッファ
			*/
			Microsoft::WRL::ComPtr<ID3D11Buffer>		m_pConstantBuffer;

			/*!
				@var	m_pShared
				@brief	インスタンスのアドレスを保持したシェアード・ポインタ
			*/
			std::shared_ptr<AbstractShader*>			m_pShared;
			
			/*!
				@fn			CreateVertexShader
				@brief		頂点シェーダーの作成
				@detail		事前にブロブをコンパイルしておくこと！
				@param[in]	コンパイル済みブロブ
				@param[in]	頂点シェーダー
				@return		S_OK:成功 E_FAIL:失敗
			*/
			static HRESULT CreateVertexShader(ID3DBlob*pBlob, ID3D11VertexShader**pVertexShader);

			/*!
				@fn			CreateVertexShader
				@brief		頂点シェーダーの作成
				@detail		プリコンパイル済みシェーダーファイルを使用する
				@param[in]	コンパイル済みシェーダーファイル(バイナリデータ)
				@param[in]	頂点シェーダー
				@return		S_OK:成功 E_FAIL:失敗
			*/
			static HRESULT CreateVertexShader(CustomShaderBin* bin, ID3D11VertexShader**pVertexShader);

			/*!
				@fn			CreateInputLayout
				@brief		頂点レイアウト作成
				@detail		プリコンパイル済みシェーダーファイルを使用する
				@param[in]	コンパイル済みシェーダーファイル(バイナリデータ)
				@param[in]	頂点レイアウトの定義
				@param[in]	頂点レイアウト
				@return		S_OK:成功 E_FAIL:失敗
			*/
			static HRESULT CreateInputLayout(CustomShaderBin* bin, D3D11_INPUT_ELEMENT_DESC desc[], ID3D11InputLayout**pInputLayout);

			/*!
				@fn			CreatePixelShader
				@brief		ピクセルシェーダーの作成
				@detail		事前にブロブをコンパイルしておくこと！
				@param[in]	コンパイル済みブロブ
				@param[in]	ピクセルシェーダー
				@return		S_OK:成功 E_FAIL:失敗
			*/
			static HRESULT CreatePixelShader(ID3DBlob*pBlob, ID3D11PixelShader**pPixelShader);

			/*!
				@fn			CreatePixelShader
				@brief		ピクセルシェーダーの作成
				@detail		プリコンパイル済みシェーダーファイルを使用する
				@param[in]	コンパイル済みシェーダーファイル(バイナリデータ)
				@param[in]	ピクセルシェーダー
				@return		S_OK:成功 E_FAIL:失敗
			*/
			static HRESULT CreatePixelShader(CustomShaderBin* bin, ID3D11PixelShader**pPixelShader);
		};
	}
}