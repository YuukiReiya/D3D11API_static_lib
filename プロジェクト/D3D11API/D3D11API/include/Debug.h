/*!
	@file	Debug.h
	@date	2018/11/20
	@author	番場 宥輝
	@brief	デバッグ関連のクラス
	@detail	各クラスで使われているシンボルの宣言も含める
			参照は不要なのでメンバは全てstaticで宣言及び定義
*/
#pragma once
#include <Windows.h>

/*! 
	@brief	デバッグ用シンボルの宣言 
	@detail	Releaseビルド時に有効にならないようマクロで囲む
*/
#if defined DEBUG || defined _DEBUG

/*!
	@def	DEBUG_FPS	
	@brief	FPSを表示する
	@detail	Main関数内でSetWindowTextを呼び出しウィンドウ名の部分にFPSの表示をする
*/
#define DEBUG_FPS

/*!
	@def	DEBUG_CONSOLE
	@brief	デバッグコンソールの生成
	@detail	Win32APIの関数を使用している
*/
//#define DEBUG_CONSOLE

/*!
	@def	DEBUG_SPRITE
	@brief	スプライトクラスのデバッグシンボル
	@detail	スプライトクラスのデバッグ処理の有効化
*/
//#define DEBUG_SPRITE

/*!
	@def	DEBUG_TEXTURE
	@brief	テクスチャクラスのデバッグシンボル
	@detail	テクスチャクラスのデバッグ処理の有効化
*/
#define DEBUG_TEXTURE

#endif // _DEBUG

/*!
	@brief	デバッグクラス
*/
class Debug
{
public:

	/*!
		@fn		コンソールウィンドウの生成
		@brief	コンソールウィンドウの作成とハンドラの初期化
		@detail	メンバハンドラにハンドル情報を格納
		@return	true:成功 false:失敗
	*/
	static bool CreateConsoleWindow();

	/*!
		@fn		コンソールウィンドウの破棄
		@brief	生成したコンソールウィンドウの破棄を行う
		@detail	ハンドラがハンドル情報を取得していたらウィンドウが作られているのでFreeConsoleを呼び出す
	*/
	static void DestroyConsoleWindow();

	/*!
		@fn		閉じるボタンの無効化
		@brief	RemoveMenuで閉じるボタンを無効化している
		@detail	CreateConsoleWindowでウィンドウの作成後に呼び出す
	*/
	static void DisableCloseButton();
private:
	/*!
		@brief	コンストラクタ削除
	*/
	Debug()		= delete;

	/*!
		@brief	デストラクタ削除
	*/
	~Debug()	= delete;

	/*!
		@var	m_hWndConsole
		@brief	コンソールウィンドウハンドラ
	*/
	static HWND  m_hWndConsole;	// コンソールウィンドウハンドラ

	/*!
		@var	m_hMenu
		@brief	メニューハンドラ
	*/
	static HMENU m_hMenu;		// メニューハンドラ
};

