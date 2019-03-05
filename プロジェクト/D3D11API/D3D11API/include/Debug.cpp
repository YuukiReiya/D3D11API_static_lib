/*!
	@file	Debug.cpp
	@date	2018/11/20
	@author	番場 宥輝
	@brief	デバッグ関連のクラス
	@detail	各クラスで使われているシンボルの宣言も含める
			参照は不要なのでメンバは全てstaticで宣言及び定義
*/
#include "stdafx.h"
#include "Debug.h"
#include "MyGame.h"

/*!
	静的メンバ変数の初期化と宣言
*/
HWND	Debug::m_hWndConsole	= nullptr;
HMENU	Debug::m_hMenu			= nullptr;


/*!
	@brief	空コンストラクタ
*/
//Debug::Debug()
//{
//}

/*!	
	@brief	空デストラクタ
*/
//Debug::~Debug()
//{
//}
//
/*!
	@fn		コンソールウィンドウの生成
	@brief	コンソールウィンドウの作成とハンドラの初期化
	@detail	メンバハンドラにハンドル情報を格納
*/
bool Debug::CreateConsoleWindow()
{
	/*! コンソール作成 */
	if (!AllocConsole()) {
		ErrorLog("Debug Console Window is not create!");
		return false;
	}

	/*! 標準入出力に割り当てる */
	FILE*fp;

	/*! 新たなファイルを開きストリームと結びつける */
	freopen_s(&fp, "CONOUT$", "w", stdout);
	freopen_s(&fp, "CONIN$", "r", stdin);

	/*! ウィンドウハンドラ取得 */
	m_hWndConsole = GetConsoleWindow();
	if (m_hWndConsole == nullptr) {
		ErrorLog("console window handle is \"nullptr\"!");
		return false;
	}

	/*! メニューハンドラ取得 */
	m_hMenu = GetSystemMenu(m_hWndConsole, false);
	if (m_hMenu == nullptr) {
		ErrorLog("console menu handle is \"nullptr\"!");
		return false;
	}
	return true;
}

/*!
	@fn		コンソールウィンドウの破棄
	@brief	生成したコンソールウィンドウの破棄を行う
	@detail	ハンドラがハンドル情報を取得していたらウィンドウが作られているのでFreeConsoleを呼び出す
*/
void Debug::DestroyConsoleWindow()
{
	if (m_hWndConsole == nullptr) { return; }
	FreeConsole();
}

/*!
	@fn		閉じるボタンの無効化
	@brief	閉じるボタンを押せなくする
	@detail	RemoveMenuで閉じるボタンを無効化している
*/
void Debug::DisableCloseButton()
{
	/*! メニューの閉じるボタンを無効化 */
	RemoveMenu(m_hMenu, SC_CLOSE, MF_BYCOMMAND);
}
