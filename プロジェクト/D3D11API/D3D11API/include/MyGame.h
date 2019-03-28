/*
	@file	MyGame.h
	@date	2018/02/21
	@author	番場 宥輝
	@brief	使えそうな関数まとめ
*/
#pragma once
#include <iostream>
#include <tchar.h>
#include <Windows.h>

/*!
	@fn			GetArraySize
	@brief		静的配列の要素数取得
	@param[in]	静的配列
	@return		静的配列の要素数
*/
template<typename Array,std::size_t Size>
static inline std::size_t GetArraySize(const Array(&)[Size])
{
	return Size;
}

/*!
	@fn			To_TString
	@brief		文字コード変換(マルチバイト・ワイド文字)
	@param[in]	変換したい文字列
	@return		変換文字列
*/
static inline std::basic_string<TCHAR> To_TString(std::string str) {
#ifdef _UNICODE
	std::wstring ret;
	size_t length = 0;
	ret.resize(MAX_PATH - 1);
	mbstowcs_s(&length, &ret.front(), ret.size(), str.c_str(), _TRUNCATE);
	return ret;
#else
	//TCHAR* ret = str;
	return str;
#endif // _UNICODE
}

/*
	@fn			To_WString
	@brief		文字コード変換(ワイド文字列)
	@param[in]	変換したい文字列
	@return		変換文字列
*/
static inline std::wstring To_WString(std::string str) 
{
	std::wstring ret;
	size_t length = 0;
	ret.resize(MAX_PATH - 1);
	mbstowcs_s(&length, &ret.front(), ret.size(), str.c_str(), _TRUNCATE);
	return ret;

}

/*!
	@fn			ErrorLog
	@brief		メッセージボックスのエラー表示
	@param[in]	表示するメッセージ
*/
static inline void ErrorLog(std::string msg) {
#ifdef _UNICODE
	
	size_t length =0;
	WCHAR tmp[MAX_PATH] = { 0 };
	mbstowcs_s(&length, tmp, MAX_PATH, msg.c_str(), _TRUNCATE);
	MessageBoxW(NULL, tmp, _T("Error"), MB_OK);
#else
	MessageBoxA(NULL, msg.c_str(), _T("Error"), MB_OK);
#endif // _UNICODE

		
}

/*!
	@fn		WindowsErrorLog
	@brief	WindowsAPIのエラー出力
*/
static inline void WindowsErrorLog() {

	LPTSTR error;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // デフォルト ユーザー言語 
		(LPTSTR)&error,
		0,
		NULL);
	MessageBox(NULL, error, _T("Error"), MB_OK);
}

/*!
	@fn			SafeDelete
	@brief		安全なdelete
	@param[in]	インスタンス
*/
template <typename T>
static inline void SafeDelete(T* &p)
{
	/*! 不完全な型のポインタをdeleteしようとしたときにコンパイルエラーにする */

	//	[不完全な型の種類] 参考:https://msdn.microsoft.com/ja-jp/library/200xfxh6.aspx
	//	・メンバーがまだ指定されていない構造体型。
	//	・メンバーがまだ指定されていない共用体型。
	//	・次元がまだ指定されていない配列型。

	typedef char type_must_be_complete[sizeof(T) ? 1 : -1];
	(void)sizeof(type_must_be_complete);

	if (p != nullptr) {
		delete(p);
		p = nullptr;
	}
}