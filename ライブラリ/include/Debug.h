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
#define DEBUG_CONSOLE

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

	static bool CreateConsoleWindow();
	static void DestroyConsoleWindow();
	static void DisableCloseButton();
private:
	Debug()		= delete;
	~Debug()	= delete;
	static HWND  m_hWndConsole;	/*!< コンソールウィンドウハンドラ */
	static HMENU m_hMenu;		/*!< メニューハンドラ */
};

