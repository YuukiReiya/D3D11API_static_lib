/*
	@file	Color.cpp
	@date	2018/11/26
	@author	番場 宥輝
	@brief	カラークラス
*/
#include "stdafx.h"
#include "Color.h"

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
Color::Color(Color && color) :x(color.x), y(color.y), z(color.z), w(color.w)
{
}

/*!
	@brief		引数付きコンストラクタ
	@param[in]	R( 0.0f ～ 1.0f )
	@param[in]	G( 0.0f ～ 1.0f )
	@param[in]	B( 0.0f ～ 1.0f )
	@param[in]	A( 0.0f ～ 1.0f )
*/
Color::Color(float r, float g, float b, float a)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

/*!
	@brief		引数付きコンストラクタ
	@param[in]	FLOAT4型からカラー型へ変換
*/
Color::Color(DirectX::XMFLOAT4 color)
{
	r = color.x;
	g = color.y;
	b = color.z;
	a = color.w;
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
