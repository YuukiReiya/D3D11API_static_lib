/*!
	@file	Entry.cpp
	@date	2019/03/06
	@author	番場 宥輝
	@brief	Windowsのエントリーポイント
*/
#pragma once
#include "Main/Main.h"
#include <MemoryLeaks.h>
#include <stdio.h>
#include <tchar.h>
#include <locale>
#include <memory>


#include <Direct3D11.h>
/*!
	@var	g_pMain
	@brief	変数宣言
	@detail	ユニークポインタを使いメモリリークを抑制
*/
std::unique_ptr<Main>g_pMain;

/*!
	@fn			_tWinMain
	@brief		エントリーポイント
	@param[in]	インスタンスハンドラ
*/
INT WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, INT)
{
	//	メモリリーク箇所特定
#if defined DEBUG||_DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // DEBUG||_DEBUG

#ifdef _UNICODE
	// 文字コード設定
	setlocale(LC_ALL, "JPN");
#endif // _UNICODE

	g_pMain = std::make_unique<Main>();

	//	高解像度タイマーへの対応を調べ、未対応ならプログラムを終了させる
	if (!g_pMain->HighQualityTimer())
	{
		g_pMain.reset(nullptr);
		return 0;
	}

	//	アプリケーションを初期化し、メインループへ
	if (g_pMain->Init(hInstance)) {
		//g_pMain->Loop();
	}


	//	明示的開放
	g_pMain.reset(nullptr);

	D3D11::Direct3D11::GetInstance().ReportCOMs("開放はじめ");
	return 0;
}