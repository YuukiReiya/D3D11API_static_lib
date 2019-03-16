/*
	@file	Color.cpp
	@date	2018/11/26
	@author	番場 宥輝
	@brief	カラークラス
*/
#include "stdafx.h"
#include "Color.h"
#include <algorithm>

/*!
	@brief	usingディレクティブ
	@using	std
*/
using namespace std;

/*!
	@brief	空コンストラクタ
*/
Color::Color()
{
}

/*!
	@brief	引数付きコンストラクタ
	@param[in]	一時変数
*/
Color::Color(Color && color) :
	x(clamp(color.x, c_MinValue, c_MaxValue)),
	y(clamp(color.y, c_MinValue, c_MaxValue)),
	z(clamp(color.z, c_MinValue, c_MaxValue)),
	w(clamp(color.w, c_MinValue, c_MaxValue))
{
}

/*!
	@brief		引数付きコンストラクタ
	@param[in]	R( 0.0f ～ 1.0f )
	@param[in]	G( 0.0f ～ 1.0f )
	@param[in]	B( 0.0f ～ 1.0f )
	@param[in]	A( 0.0f ～ 1.0f )
*/
Color::Color(float r, float g, float b, float a):
	x(clamp(r, c_MinValue, c_MaxValue)),
	y(clamp(g, c_MinValue, c_MaxValue)),
	z(clamp(b, c_MinValue, c_MaxValue)),
	w(clamp(a, c_MinValue, c_MaxValue))
{
}

/*!
	@brief		引数付きコンストラクタ
	@param[in]	FLOAT4型からカラー型へ変換
*/
Color::Color(DirectX::XMFLOAT4 color):
	x(clamp(color.x, c_MinValue, c_MaxValue)),
	y(clamp(color.y, c_MinValue, c_MaxValue)),
	z(clamp(color.z, c_MinValue, c_MaxValue)),
	w(clamp(color.w, c_MinValue, c_MaxValue))

{
}

/*!
	@brief	デストラクタ
	@detail	空デストラクタ
*/
Color::~Color()
{
}

/*!
	@fn		カラーのゲッター
	@brief	カラーをXMFLOAT3型で取得
	@detail	カラーのRGBが入ったXMFLOAT3型を取得する
*/
DirectX::XMFLOAT3 Color::GetRGB() const
{
	return DirectX::XMFLOAT3(x, y, z);
}

/*!
	@fn		カラーのゲッター
	@brief	カラーをXMFLOAT4型で取得
	@detail	カラーのRGBAが入ったXMFLOAT4型を取得する
*/
DirectX::XMFLOAT4 Color::GetRGBA() const
{
	return DirectX::XMFLOAT4(x, y, z, w);
}
