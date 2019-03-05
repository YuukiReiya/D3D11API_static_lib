/*!
	@file	Main.h
	@date	2018/02/08
	@author	番場 宥輝
	@brief	アプリケーションメインエントリーポイント
*/
#pragma once

#include <Windows.h>
#include <mmsystem.h>
#include <string_view>
#include <Window.h>

class Main
{
public:
	Main();
	~Main();

	/*!
		@fn			Init
		@brief		初期化
		@param[in]	インスタンスハンドラ
		@return		true:成功 false:失敗
	*/
	bool Init(HINSTANCE hInstance);

	/*!
		@fn		Release
		@brief	解放処理
	*/
	void Release();

	/*!
		@fn		Loop
		@brief	メインループ処理
	*/
	void Loop();

	/*!
		@fn		HighQualityTimer
		@brief	高解像度タイマーの対応を確認
		@return	true:対応 false:未対応
	*/
	bool HighQualityTimer();

	/*!
		@fn		SetUpFPS
		@brief	FPS調整の準備
	*/
	void SetUpFPS();
private:
	/*!
		@fn		App
		@brief	アプリケーションの処理
	*/
	void App();

	/*!
		@fn		Update
		@brief	更新処理
	*/
	void Update();

	/*!
		@fn		Render
		@brief	描画処理
	*/
	void Render();

	/*!
		@fn		SleepApp
		@brief	アプリケーション処理のお休み
	*/
	void SleepApp();

	/*!
		@var	c_AppName
		@brief	アプリケーションの名前
		@value	Sample
	*/
	static constexpr std::string_view c_AppName = "Sample";

	/*!
		@var	c_FPS
		@brief	ゲームのターゲットFPS
		@value	60
	*/
	static constexpr int	c_FPS = 60;

	/*!
		@var	c_MinFrameTime
		@brief	1フレーム辺りの経過時間
		@value	1.0f / Main::c_FPS
	*/
	static constexpr float	c_MinFrameTime = 1.0f / Main::c_FPS;

	/*!
		@var	c_WindowWidth
		@brief	ウィンドウの横幅
		@value	1920
	*/
	static constexpr int c_WindowWidth	= 1920;

	/*!
		@var	c_WindowHeight
		@brief	ウィンドウの縦幅
		@value	1080
	*/
	static constexpr int c_WindowHeight = 1080;

	/*!
		@var	m_hWnd
		@brief	ウィンドウズハンドラ
	*/
	HWND m_hWnd;

	/*!
		@var	m_FrameTime
		@brief	経過時間
	*/
	float m_FrameTime;

	/*!
		@var	m_TimeStart
		@brief	開始時間のカウンタ
	*/
	LARGE_INTEGER m_TimeStart;

	/*!
		@var	m_TimeEnd
		@brief	終了時間のカウンタ
	*/
	LARGE_INTEGER m_TimeEnd;

	/*!
		@var	m_TimeFreq
		@brief	高解像度タイマーの周波数
	*/
	LARGE_INTEGER m_TimeFreq;

	Window* m_pWindow;
};

