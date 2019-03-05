/*
	@file	Direct3D11.h
	@date	2018/11/10
	@author	番場 宥輝
	@brief	Direct3D11デバイス
	@detail	シングルトン
*/
#pragma once
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"winmm.lib")

#include <D3D11.h>
#include <wrl/client.h>
#include "stdafx.h"
#include <string>
#include <wrl/client.h>
#if defined DEBUG || _DEBUG
#include <dxgidebug.h>
#endif
#include "Singleton.h"

/*!
	@def	Vsync
	@brief	Vsyncを使ったFPS同期(コメントアウトするとアプリケーション側でのwait)
*/
//#define Vsync

/*!
	@def	MSAA
	@brief	マルチサンプル・アンチエイリアス(コメントアウトするとMSAA無し)
*/
//#define MSAA


/*!
	@var	c_WindowWidth
	@brief	ウィンドウの横幅
*/
constexpr INT c_WindowWidth		= 1920;

/*!
	@var	c_WindowHeight
	@brief	ウィンドウの縦幅
*/
constexpr INT c_WindowHeight	= 1080;

/*! Direct3D11関連の名前空間 */
namespace D3D11 {

	/*!
		@brief	Direct3D11デバイスclass
	*/
	class Direct3D11
		:public Singleton<Direct3D11>
	{
	public:
		/*!
			@brief	デストラクタ
		*/
		~Direct3D11();

		/*!
			@fn			イニシャライズ
			@brief		初期化
			@detail		デバイスの作成
			@param[in]	ウィンドウハンドラ
			@return		S_OK:成功 E_FAIL:失敗
		*/
		HRESULT Initialize(HWND hWnd);

		/*!
			@fn		リリース
			@brief	開放処理
			@detail	メンバの明示的な開放
		*/
		void Release();

		/*!
			@fn		クリア
			@brief	描画のクリア
			@detail	レンダーターゲットとデプスステンシルビューのクリアを行う
		*/
		void Clear();

		/*!
			@fn		画面更新
			@brief	描画の更新
			@detail	バックバッファをフロントバッファに切り替える
		*/
		void Present();

		/*!
			@fn		COMレポートの出力
			@brief	現在のCOMの生存状況を出力ウィンドウに出力する
			@detail	デバッグ専用
			@param[in]	出力ウィンドウに出力するメッセージ(このメッセージを出力後にCOMの生存状況を出力)
		*/
		void ReportCOMs(std::string firstMessage = "");

		/*!
			@fn		デバイスのゲッター
			@brief	生成したデバイスを取得
		*/
		ID3D11Device* GetDevice()const { return m_pDevice.Get(); }

		/*!
			@fn		コンテキストのゲッター
			@brief	生成したコンテキストを取得
		*/
		ID3D11DeviceContext* GetDeviceContext()const { return m_pDeviceContext.Get(); }
	private:
		/*!
			@brief	シングルトンデザインパターンのテンプレート継承
		*/
		friend class Singleton<Direct3D11>;

		/*!
			@brief	コンストラクタ
		*/
		Direct3D11();

		/*!
			@var	c_ClearColor
			@brief	描画ターゲットのクリアカラー
		*/
		static constexpr float m_ClearColor[4]{ 0.0f,0.0f,0.0f,0.0f };		/*!< 描画ターゲットクリアカラー */


		/*!
			@brief	メンバー変数の宣言
			@detail	ComPtrを使ったスマートポインタで宣言
		*/
		Microsoft::WRL::ComPtr<ID3D11Device>			m_pDevice;				/*!< デバイス */
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>		m_pDeviceContext;		/*!< コンテキスト */
		Microsoft::WRL::ComPtr<IDXGISwapChain>			m_pSwapChain;			/*!< スワップチェイン */
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	m_pRenderTargetView;	/*!< レンダーターゲットビュー */
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	m_pDepthStencilView;	/*!< デプスステンシルビュー */
		Microsoft::WRL::ComPtr<ID3D11Texture2D>			m_pDepthStencil;		/*!< デプスステンシル */
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	m_pDepthStencilState;	/*!< デプスステンシルステート */
		Microsoft::WRL::ComPtr<ID3D11RasterizerState>	m_pRasterizerState;		/*!< ラスタライザステート */
		Microsoft::WRL::ComPtr<ID3D11Debug>				m_pDebug;				/*!< デバッグ */
	};
};

