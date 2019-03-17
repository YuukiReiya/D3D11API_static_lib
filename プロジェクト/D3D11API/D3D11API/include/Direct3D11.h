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
#include "Color.h"

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
			@fn			SetWindowSize
			@brief		ウィンドウサイズの設定
			@param[in]	ウィンドウの横幅
			@param[in]	ウィンドウの縦幅
		*/
		void SetWindowSize(const int width, const int height);

		/*!
			@fn			SetClearColor
			@brief		クリア色の設定
			@param[in]	色
		*/
		void SetClearColor(Color clearColor);

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
			@fn			COMレポートの出力
			@brief		現在のCOMの生存状況を出力ウィンドウに出力する
			@detail		デバッグ専用
			@param[in]	出力ウィンドウに出力するメッセージ(このメッセージを出力後にCOMの生存状況を出力)
		*/
		void ReportCOMs(std::string firstMessage = "");

		/*!
			@fn		GetDevice
			@brief	生成したデバイスを取得
		*/
		ID3D11Device* GetDevice()const { return m_pDevice.Get(); }

		/*!
			@fn		GetImmediateContext
			@brief	生成したイミディエイトコンテキストを取得
		*/
		ID3D11DeviceContext* GetImmediateContext()const { return m_pImmediateContext.Get(); }

		/*!
			@brief	読み取り専用のプロパティ
			@detail	VisualStudio以外では使えないらしい
		*/
		#pragma region Property
		/*!
			@property	Device
			@brief		デバイスのプロパティ(読み取り専用)
			@detail		VisualStudioのみ対応
		*/
		__declspec(property(get = GetDevice))ID3D11Device* Device;

		/*!
			@property	ImmediateContext
			@brief		イミディエイトコンテキストのプロパティ(読み取り専用)
			@detail		VisualStudioのみ対応
		*/
		__declspec(property(get = GetImmediateContext))ID3D11DeviceContext* ImmediateContext;
		#pragma endregion
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
			@brief	メンバー変数の宣言
			@detail	ComPtrを使ったスマートポインタで宣言
		*/
		Microsoft::WRL::ComPtr<ID3D11Device>			m_pDevice;				/*!< デバイス */
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>		m_pImmediateContext;	/*!< イミディエイトコンテキスト */
		Microsoft::WRL::ComPtr<IDXGISwapChain>			m_pSwapChain;			/*!< スワップチェイン */
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	m_pRenderTargetView;	/*!< レンダーターゲットビュー */
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	m_pDepthStencilView;	/*!< デプスステンシルビュー */
		Microsoft::WRL::ComPtr<ID3D11Texture2D>			m_pDepthStencil;		/*!< デプスステンシル */
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	m_pDepthStencilState;	/*!< デプスステンシルステート */
		Microsoft::WRL::ComPtr<ID3D11RasterizerState>	m_pRasterizerState;		/*!< ラスタライザステート */
		Microsoft::WRL::ComPtr<ID3D11Debug>				m_pDebug;				/*!< デバッグ */

		/*!
			@var	m_WindowWidth
			@brief	ウィンドウの横幅
		*/
		int m_WindowWidth;

		/*!
			@var	m_WindowHeight
			@brief	ウィンドウの縦幅
		*/
		int m_WindowHeight;

		/*!
			@var	m_Vsync
			@brief	Vsyncを使ったFPS同期
			@detail true:Vsync false:アプリケーション側でのwait
		*/
		bool m_Vsync;

		/*!
			@var	m_ClearColor
			@brief	描画ターゲットのクリアカラー
		*/
		Color m_ClearColor;
	};
};

