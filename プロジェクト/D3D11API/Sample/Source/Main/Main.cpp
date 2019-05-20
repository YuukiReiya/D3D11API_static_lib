/*!
	@file	Main.cpp
	@date	2018/02/08
	@author	番場 宥輝
	@brief	アプリケーションメインエントリーポイント
*/
#include <tchar.h>
#include <Direct3D11.h>
#include <MyGame.h>
#include <MemoryLeaks.h>
#include <Debug.h>
#include <AudioMaster.h>
#include <Camera.h>
#include <Keyboard.h>
#include "Main.h"
#include <StructShaderBase.h> 
#include "../Scene/Root/SceneRoot.h"

/*!
	@brief	usingディレクティブ
	@using	D3D11
*/
using namespace D3D11;

/*!
	@brief	usingディレクティブ
	@using	API
*/
using namespace API;

/*!
	@brief	コンストラクタ
*/
Main::Main()
	:m_hWnd(nullptr),
	m_hInstance(nullptr),
	m_pWindow(nullptr)
{
}

/*!
	@brief	デストラクタ
*/
Main::~Main()
{
	Release();
}

/*!
	@fn			Init
	@brief		初期化
	@param[in]	インスタンスハンドラ
	@return		true:成功 false:失敗
*/
bool Main::Init(HINSTANCE hInstance)
{
	m_pWindow = std::make_unique<Window>();
	m_hInstance = hInstance;

	//	ウィンドウ生成
	if (!m_pWindow->Create(&m_hWnd, m_hInstance, 0, 0,
		c_WindowWidth, c_WindowHeight, c_AppName.data())) {
		ErrorLog("Window generation failed!");
		return false;
	}

	//	ウィンドウサイズの指定
	Direct3D11::GetInstance().SetWindowSize(c_WindowWidth, c_WindowHeight);

	//	レンダーのクリアカラー
	Direct3D11::GetInstance().SetClearColor(Color(0.5f, 0.5f, 0.5f, 1));

	//	デバイス初期化
	HRESULT hr = NULL;
	hr = Direct3D11::GetInstance().Initialize(m_hWnd);
	if (FAILED(hr)) {
		ErrorLog("Device initialization failed!");
		return false;
	}

	//	デバッグコンソール表示
#if defined DEBUG_CONSOLE

	//	コンソールウィンドウの表示
	if (Debug::CreateConsoleWindow())
	{
		//	デバッグコンソールウィンドウの閉じるボタン無効化
		Debug::DisableCloseButton();
		return true;
	}
	//	コンソールの表示失敗
	else 
	{
		ErrorLog("Debug console generation failed!");
		return false;
	}
	
#endif // DEBUG

	return true;
}

/*!
	@fn		Release
	@brief	明示的な解放処理
*/
void Main::Release()
{
	SceneRoot::GetInstance().Finalize();
	Direct3D11::GetInstance().Release();
	m_pWindow.reset(nullptr);
}

/*!	
	@fn		Loop
	@brief	アプリケーション処理の入り口
*/
void Main::Loop()
{
	// FPSの初期化
	SetUpFPS();

	// カメラの初期化
	Camera::GetInstance().Initialize({ c_WindowWidth,c_WindowHeight }, { 0,0,-10 });

	//	シーンの初期化
	SceneRoot::GetInstance().Initialize();

	HRESULT hr = NULL;

	// メッセージループ
	MSG msg{ 0 };
	ZeroMemory(&msg, sizeof(&msg));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
		//	App処理
			App(); 
		}
	}

}

/*
	@fn		HighQualityTimer
	@brief	高解像度タイマーの対応を確認
	@return	true:対応 false:未対応
*/
bool Main::HighQualityTimer()
{
	// QueryPerformanceFrequencyの対応を見る
	if (!QueryPerformanceFrequency(&m_TimeFreq)) {
		ErrorLog("This PC is not fit QueryPerformanceFrequency!\n"
			"Sorry this project is exit!");
		return false;
	}

	return true;
}

/*!
	@fn		App
	@brief	アプリケーションの処理
*/
void Main::App()
{
	// FPS確認
#ifdef _DEBUG
	static DWORD time = 0;
	static int frame = 0;
	frame++;
	char str[50];
	sprintf_s(str, "fps = %d", frame);
	if (timeGetTime() - time > 1000) {
		time = timeGetTime();
		frame = 0;

		auto tmp = To_TString(str);
		const auto fps = const_cast<LPTSTR>(tmp.c_str());
		if (!SetWindowText(m_hWnd, fps)) {
			WindowsErrorLog();
		}
	}
#endif
	//	FPSの調整方法で分岐(Vsync)
	if (Direct3D11::GetInstance().isVsync) {

		// 更新処理
		Update();
		// 描画処理
		Render();
	}
	//	アプリケーション側で
	else {

		// 更新処理
		Update();
		// 描画処理
		Render();
		// FPS調整
		SleepApp();
	}
}

/*!
	@fn		Update
	@brief	更新処理
*/
void Main::Update()
{
	//	キーボードの入力バッファの更新
	Keyboard::UpdateBuffer();

	//	シーンの更新
	SceneRoot::GetInstance().Update();
}

/*!
	@fn		Render
	@brief	描画処理
*/
void Main::Render()
{
	// 描画ターゲットのクリア
	Direct3D11::GetInstance().Clear();

	// シーンの描画
	SceneRoot::GetInstance().Render();

	// 画面更新 
	Direct3D11::GetInstance().Present();
}

/*!
	@fn		SetUpFPS
	@brief	FPS調整の準備
*/
void Main::SetUpFPS()
{
	m_FrameTime = 0;
	// 初回の計算
	QueryPerformanceCounter(&m_TimeStart);
}
/*!
	@fn		SleepApp
	@brief	アプリケーション処理のお休み
*/
void Main::SleepApp()
{
	//	今の時間を取得
	QueryPerformanceCounter(&m_TimeEnd);
	
	// 経過時間
	m_FrameTime = static_cast<float>(m_TimeEnd.QuadPart - m_TimeStart.QuadPart) / static_cast<float>(m_TimeFreq.QuadPart);
	if (m_FrameTime < c_MinFrameTime)
	{
		DWORD SleepTime = static_cast<DWORD>((c_MinFrameTime - m_FrameTime) * 1000);// ズレ分
		timeBeginPeriod(1);	// 分解性能を上げる
		Sleep(SleepTime);	// お休み
		timeEndPeriod(1);	// 戻す
	}
	QueryPerformanceCounter(&m_TimeStart);
}
