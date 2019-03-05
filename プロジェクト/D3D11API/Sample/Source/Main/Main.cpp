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
#include <AudioMaster.h>
#include <Camera.h>
#include "Main.h"

/*!
	@brief	名前空間
*/
using namespace D3D11;

/*!
	@brief	コンストラクタ
*/
Main::Main() :m_pWindow(nullptr)
{
}

/*!
	@brief	デストラクタ
*/
Main::~Main()
{
	//delete(wave);
	Release();
	//SAFE_DELETE(m_pWindow);
}

/*!
	@fn			Init
	@brief		初期化
	@param[in]	インスタンスハンドラ
*/

bool Main::Init(HINSTANCE hInstance)
{
	m_pWindow = new Window;

	// UNICODE、マルチバイト両対応用文字列変換
	auto tmp = To_TString(c_AppName.data());
	const auto appName = const_cast<LPTSTR>(tmp.c_str());

	if (!m_pWindow->Initialize(&m_hWnd, hInstance,
		0, 0, c_WindowWidth, c_WindowHeight, appName)) {
		return false;
	}

	// デバイス初期化

	return true;
}

/*!
	@brief	解放
*/
void Main::Release()
{
}

/*!	
	@brief	アプリケーション処理の入り口
*/
void Main::Loop()
{
	// FPSの初期化
	SetUpFPS();

	// カメラの初期化
	Camera::GetInstance().Initialize({ 0,0,-1 });

	HRESULT hr = NULL;

	if (FAILED(hr)) { ErrorLog(""); }

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
	@brief	高解像度タイマーが対応しているか確認
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
	@brief	アプリケーション処理
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
	// FPSの調整方法で分岐
#ifdef Vsync
	// 更新処理
	Update();
	// 描画処理
	Render();
#else
	// 更新処理
	Update();
	// 描画処理
	Render();
	// FPS調整
	SleepApp();
#endif // Vsync
}

/*!
	@brief	更新
*/
void Main::Update()
{

}

/*!
	@brief	描画
*/
void Main::Render()
{
	// 描画ターゲットのクリア
	Direct3D11::GetInstance().Clear();

	// シーンの描画

	// 画面更新 
	Direct3D11::GetInstance().Present();
}

/*!
	@brief	FPS調整の準備
*/
void Main::SetUpFPS()
{
	m_FrameTime = 0;
	// 初回の計算
	QueryPerformanceCounter(&m_TimeStart);
}
/*!
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
