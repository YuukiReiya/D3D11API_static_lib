/*
	@file	Direct3D11.cpp
	@date	2018/11/10
	@author	番場 宥輝
	@brief	Direct3D11デバイス
	@detail	シングルトン
*/
#include "stdafx.h"
#include "Direct3D11.h"
#include "MemoryLeaks.h"
#include "MyGame.h"

/*!
	@brief	usingディレクティブ
	@using	D3D11
*/
using namespace D3D11;


/*!
	@brief	コンストラクタ
*/
Direct3D11::Direct3D11()
{
	SecureZeroMemory(this, sizeof(this));
	m_pDevice				= nullptr;
	m_pImmediateContext		= nullptr;
	m_pSwapChain			= nullptr;
	m_pRenderTargetView		= nullptr;
	m_pDepthStencilView		= nullptr;
	m_pDepthStencil			= nullptr;
	m_pDepthStencilState	= nullptr;
	m_pRasterizerState		= nullptr;
	m_pDebug				= nullptr;
	m_WindowHeight = m_WindowWidth = 0;
}

/*!
	@brief	デストラクタ
*/
Direct3D11::~Direct3D11()
{
	Release();
}

/*!
	@fn			SetWindowSize
	@brief		ウィンドウサイズの設定
	@param[in]	ウィンドウの横幅
	@param[in]	ウィンドウの縦幅
*/
void D3D11::Direct3D11::SetWindowSize(const int width, const int height)
{
	m_WindowWidth	= width;
	m_WindowHeight	= height;
}

/*!
	@fn			SetClearColor
	@brief		クリア色の設定
	@param[in]	色
*/
void D3D11::Direct3D11::SetClearColor(Color clearColor)
{
	m_ClearColor = clearColor;
}

/*!
	@fn			イニシャライズ
	@brief		初期化
	@detail		デバイスの作成
	@param[in]	ウィンドウハンドラ
	@return		S_OK:成功 E_FAIL:失敗
*/
HRESULT Direct3D11::Initialize(HWND hWnd)
{
	// デバイスとスワップ・チェイン作成
	DXGI_SWAP_CHAIN_DESC sd;
	SecureZeroMemory(&sd, sizeof(sd));
	sd.BufferCount							= 1;							// バック・バッファ数
	sd.BufferDesc.Width						= m_WindowWidth;				// バック・バッファの幅
	sd.BufferDesc.Height					= m_WindowHeight;				// バック・バッファの高さ
	sd.BufferDesc.Format					= DXGI_FORMAT_R8G8B8A8_UNORM;	// フォーマット
	sd.BufferDesc.RefreshRate.Numerator		= 60;							// リフレッシュ・レート(分子)
	sd.BufferDesc.RefreshRate.Denominator	= 1;							// リフレッシュ・レート(分母)

	// スキャンライン描画モード
	sd.BufferDesc.ScanlineOrdering			= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	// スケーリング・モード
	sd.BufferDesc.Scaling					= DXGI_MODE_SCALING_UNSPECIFIED;

	sd.BufferUsage							= DXGI_USAGE_RENDER_TARGET_OUTPUT;						// バック・バッファの仕様
	sd.OutputWindow							= hWnd;													// 関連付けるウィンドウ

#if defined(MSAA)
	// アンチエイリアス処理有り

	//http://nayuta-solution.hateblo.jp/entry/2015/01/17/142751 こちらを参考に設定を行う	※途中

	// 使用できるMSAAをチェック
	DXGI_SAMPLE_DESC sampleDesc{};

	for (int i = 1; i <= D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT; i <<= 1) {
		UINT quality;
		if (SUCCEEDED(m_pDevice->CheckMultisampleQualityLevels(
			DXGI_FORMAT_D24_UNORM_S8_UINT, i, &quality))) {
			if (0 < quality) {
				sampleDesc.Count = i;
				sampleDesc.Quality = quality - 1;
			}
		}
	}

				
#else
	// アンチエイリアス処理無し
	sd.SampleDesc.Count = 1;							// マルチ・サンプルの数
	sd.SampleDesc.Quality = 0;							// マルチ・サンプルのクオリティ
#endif // アンチエイリアス
	sd.Windowed = true;									// ウィンドウ・モード
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;	// スワップ・チェイン効果指定

	// 機能レベルを指定する配列
	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};
	D3D_FEATURE_LEVEL* pFeatureLevelsSupported = NULL;	// 機能レベルを取得する変数

#if defined(DEBUG)||defined(_DEBUG)
	UINT createDeviceFlags = D3D11_CREATE_DEVICE_DEBUG;
#else
	UINT createDeviceFlags = 0;
#endif//DEBUG|_DEBUG

	// デバイスとスワップ・チェインの作成
	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		NULL,											// 使用するIDXGIAdapter インターフェース
		D3D_DRIVER_TYPE_HARDWARE,						// Direct3D 11 デバイスの種類
		NULL,											
		createDeviceFlags,								// 使用するAPI レイヤーの指定
		&featureLevels[0],								// 機能レベルを指定する配列
		static_cast<UINT>(GetArraySize(featureLevels)),	// 上記配列の要素数
		D3D11_SDK_VERSION,								// 使用しているSDKのバージョン
		&sd,											// スワップ・チェインの設定
		m_pSwapChain.GetAddressOf(),					// IDXGISwapChain インターフェースの変数
		m_pDevice.GetAddressOf(),						// ID3D11Device インターフェースの変数
		pFeatureLevelsSupported,						// 機能レベルを取得する変数
		m_pImmediateContext.GetAddressOf()					// ID3D11DeviceContext インターフェースの変数
	);
	if (FAILED(hr)) { 
		ErrorLog("SwapChain is not create!");
		return E_FAIL;// デバイスの作成に失敗
	}

#if defined DEBUG || _DEBUG

	// デバイスのnullチェック
	if (m_pDevice != nullptr) {

		// デバッグデバイスの生成
		hr = m_pDevice.Get()->QueryInterface(
			__uuidof(ID3D11Debug),
			reinterpret_cast<void**>(m_pDebug.GetAddressOf())
		);
	}

#endif // DEBUG || _DEBUG


	// スワップ・チェインから最初のバックバッファを取得する
	ID3D11Texture2D* pBackBuffer;
	hr = m_pSwapChain->GetBuffer(
		0,								// バック・バッファ番号
		__uuidof(ID3D11Texture2D),		// バッファにアクセスするインターフェース
		(LPVOID*)&pBackBuffer			// バッファを受け取る変数
	);
	if (FAILED(hr)) { 
		ErrorLog("Can't take top back buffer!");
		return E_FAIL; // 取得失敗
	}
	
	// レンダーターゲットビューの作成
	hr = m_pDevice->CreateRenderTargetView(
		pBackBuffer,						// ビューでアクセスするリソース
		NULL,								// レンダーターゲットビューの定義
		m_pRenderTargetView.GetAddressOf()	// レンダーターゲットビューを受け取る変数
	);

	// バック・バッファは以降使わないので解放
	if (pBackBuffer != nullptr) {
		pBackBuffer->Release();
	}
	pBackBuffer = nullptr;

	if (FAILED(hr)) { 
		ErrorLog("RenderTargetView is not create!");
		return E_FAIL;// レンダーターゲットビューの作成に失敗
	}

	// 深度テクスチャ / ステンシルビュー / ステンシルステートの作成
	// 深度 /ステンシル・ステート設定
	D3D11_DEPTH_STENCIL_DESC dc;
	SecureZeroMemory(&dc, sizeof(dc));
	dc.DepthEnable = true;									// 深度テスト有り
	dc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;			// 書き込む
	dc.DepthFunc = D3D11_COMPARISON_LESS;					// 手前の物体を描画
	dc.StencilEnable = true;								// ステンシル・テスト無し
	dc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;	// ステンシル書き込みマスク
	dc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;	// ステンシル読み込みマスク

	// 面が表を向いている場合のステンシル・テストの設定
	dc.FrontFace.StencilFailOp		= D3D11_STENCIL_OP_KEEP;		// ステンシルテストが失敗した時に実行されるステンシル処理
	dc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;		// ステンシルテストが合格で深度テストが不合格の場合に実行される処理
	dc.FrontFace.StencilPassOp		= D3D11_STENCIL_OP_KEEP;		// ステンシルテストと深度テストの両方が合格の場合に実行されるステンシル処理
	dc.FrontFace.StencilFunc		= D3D11_COMPARISON_ALWAYS;		// 既存のデータに対するデータの比較方法
	// 面が裏を向いている場合のステンシル・テストの設定
	dc.BackFace.StencilFailOp		= D3D11_STENCIL_OP_KEEP;		// ステンシルテストが失敗した時に実行されるステンシル処理
	dc.BackFace.StencilDepthFailOp	= D3D11_STENCIL_OP_KEEP;		// ステンシルテストが合格で深度テストが不合格の場合に実行される処理
	dc.BackFace.StencilPassOp		= D3D11_STENCIL_OP_KEEP;		// ステンシルテストと深度テストの両方が合格の場合に実行されるステンシル処理
	dc.BackFace.StencilFunc			= D3D11_COMPARISON_ALWAYS;		// 既存のデータに対するデータの比較方法

	// 深度 /ステンシル・ステート作成
	hr = m_pDevice->CreateDepthStencilState(
		&dc,								// 深度 /ステンシル・ステート設定情報
		m_pDepthStencilState.GetAddressOf()	// 作成した 深度 /ステンシル・ステートを受け取る変数
	);
	if (FAILED(hr)) {
		ErrorLog("StencilState is not create!");
		return E_FAIL; // 深度 /ステンシル・ステート作成失敗
	}

	// 深度 /ステンシル・ステート適応
	m_pImmediateContext->OMSetDepthStencilState(
		m_pDepthStencilState.Get(),		// 深度 /ステンシル・ステート
		1								// ステンシル・テストで参照値
	);


	// 深度 / ステンシル・テクスチャの設定
	D3D11_TEXTURE2D_DESC descDepth;
	SecureZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width					= m_WindowWidth;				// 幅
	descDepth.Height				= m_WindowHeight;				// 高さ
	descDepth.MipLevels				= 1;							// ミップマップ・レベル数
	descDepth.ArraySize				= 1;							// 配列サイズ
	descDepth.Format				= DXGI_FORMAT_D32_FLOAT;		// フォーマット(深度のみ)
#if defined(MSAA)
	descDepth.SampleDesc = sampleDesc;								// MSAAの設定と同じもの
#else
	descDepth.SampleDesc.Count		= 1;							// マルチサンプリングの設定
	descDepth.SampleDesc.Quality	= 0;							// マルチサンプリングの品質
#endif// アンチエイリアス
	descDepth.Usage					= D3D11_USAGE_DEFAULT;			// デフォルトの仕様
	descDepth.BindFlags				= D3D11_BIND_DEPTH_STENCIL;		// 深度 / ステンシルとして使用
	descDepth.CPUAccessFlags		= 0;							// CPUからはアクセスしない
	descDepth.MiscFlags				= 0;							// その他 設定なし

	// 深度 / ステンシル・テクスチャの作成
	hr = m_pDevice->CreateTexture2D(
		&descDepth,						// 作成する2Dテクスチャの設定
		NULL,							// MSAAを利用する場合 NULL にする
		m_pDepthStencil.GetAddressOf()	// 作成したテクスチャを受けとる変数
	);
	if (FAILED(hr)) {
		ErrorLog("StencilTexture is not create!");
		return E_FAIL;// ステンシルテクスチャの作成に失敗
	}

	// 深度 / ステンシル・ビュー設定
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	SecureZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format				= DXGI_FORMAT_D32_FLOAT;			// ビューのフォーマット
	descDSV.ViewDimension		= D3D11_DSV_DIMENSION_TEXTURE2D;	// リソースの種類
	//descDSV.Flags				= 0;								// テクスチャのアクセス権 0:RW(読み込み書き込み)
	descDSV.Texture2D.MipSlice	= 0;								

	// 深度 / ステンシル・テクスチャに対しビューを作成
	hr = m_pDevice->CreateDepthStencilView(
		m_pDepthStencil.Get(),				// 深度 / ステンシル・ビューを作るテクスチャ
		&descDSV,							// 深度 / ステンシル・ビューの設定
		m_pDepthStencilView.GetAddressOf()	// 作成したビューを受け取る変数
	);
	if (FAILED(hr)) {
		ErrorLog("StencilView is not create!");
		return E_FAIL;
	}// 深度 / ステンシル・ビュー作成に失敗

	// 描画ターゲット・ビューを出力マネージャーのターゲットとして設定
	m_pImmediateContext->OMSetRenderTargets(
		1,									// 描画ターゲット数
		m_pRenderTargetView.GetAddressOf(),	// 描画ターゲットビュー

		//	NOTE: 無効化しておかないと、カメラからみて奥のほうにある半透明オブジェクトが描画されないことがある。
		//	TODO: 場合によってはNULLを入れる↓
		m_pDepthStencilView.Get()			// 深度 / ステンシルビュー
	);

	// ビューポートの設定
	D3D11_VIEWPORT vp;
	SecureZeroMemory(&vp, sizeof(vp));
	vp.Width	= static_cast<float>(m_WindowWidth);		// ビューポート領域の幅
	vp.Height	= static_cast<float>(m_WindowHeight);		// ビューポート領域の高さ
	//vp.MinDepth = 0.0f;									// ビューポート領域の深度最小値(ニア・クリッピング距離)
	vp.MaxDepth = 1.0f;										// ビューポート領域の深度最大値(ファー・クリッピング距離)
	//vp.TopLeftX = 0;										// ビューポート領域の左上x座標
	//vp.TopLeftY = 0;										// ビューポート領域の左上y座標
	m_pImmediateContext->RSSetViewports(
		1,			// ビューポートの数
		&vp			// 設定するビューポート配列
	);

	// ラスタライズ設定
	D3D11_RASTERIZER_DESC rd;
	SecureZeroMemory(&rd, sizeof(rd));
	rd.FillMode					= D3D11_FILL_SOLID;			// 普通に描画
	//rd.FillMode					= D3D11_FILL_WIREFRAME;		// ワイヤーフレーム描画
	rd.CullMode					= D3D11_CULL_NONE;	// 両面を描画
	rd.FrontCounterClockwise	= false;			// 深度バイアス「0」
	rd.DepthBias				= 0;
	rd.DepthBiasClamp			= 0;
	rd.SlopeScaledDepthBias		= 0;
	rd.DepthClipEnable			= false;			// 深度クリッピング無し
	rd.ScissorEnable			= false;			// シザー矩形無し
#if defined(MSAA)
	rd.MultisampleEnable		= true;
#else
	rd.MultisampleEnable		= false;			// マルチサンプリング無し
#endif
	rd.AntialiasedLineEnable	= false;			// ライン・アンチエイリアシング無し

	// ラスタライズ設定の作成
	hr = m_pDevice->CreateRasterizerState(
		&rd,								// ラスタライズ設定
		m_pRasterizerState.GetAddressOf()	// 設定を受け取る変数
	);
	if (FAILED(hr)) { 
		ErrorLog("Rasterizer is not create!");
		return E_FAIL;// ラスタライザーステート作成失敗
	}
	
	// ラスタライズを設定
	m_pImmediateContext->RSSetState(m_pRasterizerState.Get());

	// 初期化終了
	return S_OK;
}

/*!
	@fn		リリース
	@brief	開放処理
	@detail	メンバの明示的な開放
*/
void Direct3D11::Release()
{
	m_pRasterizerState.Reset();
	m_pSwapChain.Reset();
	m_pRenderTargetView.Reset();
	m_pDepthStencil.Reset();
	m_pDepthStencilView.Reset();
	m_pDepthStencilState.Reset();
	m_pDevice.Reset();
	m_pImmediateContext.Reset();
}

/*!
	@fn		クリア
	@brief	描画のクリア
	@detail	レンダーターゲットとデプスステンシルビューのクリアを行う
*/
void Direct3D11::Clear()
{
	//	クリアカラーのキャスト
	float clearColor[] = 
	{
		m_ClearColor.x ,
		m_ClearColor.y ,
		m_ClearColor.z ,
		m_ClearColor.w ,
	};

	// レンダーターゲットビューのクリア
	m_pImmediateContext->ClearRenderTargetView(
		m_pRenderTargetView.Get(),	// クリアするレンダーターゲットビュー
		clearColor					// クリアカラー
	);

	// デプス・ステンシルビューのクリア
	m_pImmediateContext->ClearDepthStencilView(
		m_pDepthStencilView.Get(),					// クリアするデプス・ステンシルビュー
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,	// クリアするデータの型
		1.0f,										// 深度バッファのクリア値
		0											// ステンシルバッファのクリア値
	);
}

/*!
	@fn		画面更新
	@brief	描画の更新
	@detail	バックバッファをフロントバッファに切り替える
*/
void Direct3D11::Present()
{
	m_pSwapChain->Present(
		isVsync ? 1 : 0,	// 画面更新タイミング(垂直回帰の同期設定) true:Vsync false:アプリケーション側のsleep
		0					// 画面更新(DXGI_PRESENT_TESTだと更新は行わない)
	);
}

/*!
	@fn		COMレポートの出力
	@brief	現在のCOMの生存状況を出力ウィンドウに出力する
	@detail	デバッグ専用
	@param[in]	出力ウィンドウに出力するメッセージ(このメッセージを出力後にCOMの生存状況を出力)
*/
void Direct3D11::ReportCOMs(std::string firstMessage)
{
#if defined DEBUG || _DEBUG
	// デバッグインターフェイスのチェック
	if (m_pDebug.Get() != nullptr) {

		auto str = firstMessage + "\n";
		auto tmp = To_TString(str.c_str());
		const auto message = const_cast<LPTSTR>(tmp.c_str());

		// ウィンドウに任意の文字列を表示
		OutputDebugString(message);

		// COMの生存状況を出力ウィンドウに出力
		m_pDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	}

#endif // DEBUG || _DEBUG
}
