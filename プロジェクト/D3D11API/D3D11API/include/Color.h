/*
	@file	Color.h
	@date	2018/11/26
	@author	番場 宥輝
	@brief	カラークラス
*/
#pragma once
#include <DirectXMath.h>

/*!
	@brief	カラークラス
*/
class Color
{
public:
	/*!
		@brief	コンストラクタ
	*/
	Color();

	/*!
		@brief		引数付きコンストラクタ
		@param[in]	一時変数
	*/
	Color(Color&&color);

	/*!
		@brief		引数付きコンストラクタ
		@param[in]	R( 0.0f ～ 1.0f )
		@param[in]	G( 0.0f ～ 1.0f )
		@param[in]	B( 0.0f ～ 1.0f )
		@param[in]	A( 0.0f ～ 1.0f )
	*/
	Color(float r, float g, float b, float a = 1.0f);

	/*!
		@brief		引数付きコンストラクタ
		@param[in]	FLOAT4型からカラー型へ変換
	*/
	Color(DirectX::XMFLOAT4 color);

	/*!
		@brief	デストラクタ
		@detail	空デストラクタ
	*/
	~Color();

	/*!
		@def	変数の別名定義マクロ
		@detail	参考:https://qiita.com/tyanmahou/items/7fbc5c0638ef9fd4c467
	*/
	#pragma region def
#define PropertyAlias(base,name,...)\
__declspec(property(get = Get_##base, put = Set_##base)) __VA_ARGS__ name;\
__VA_ARGS__ & Get_##base(){return base;}\
void Set_##base(const __VA_ARGS__& value){base = value;}
	#pragma endregion

	/*!
		@fn		カラーのゲッター
		@brief	カラーをXMFLOAT3型で取得
		@detail	カラーのRGBが入ったXMFLOAT3型を取得する
	*/
	DirectX::XMFLOAT3 GetRGB()const;

	/*!
		@fn		カラーのゲッター
		@brief	カラーをXMFLOAT4型で取得
		@detail	カラーのRGBAが入ったXMFLOAT4型を取得する
	*/
	DirectX::XMFLOAT4 GetRGBA()const;

	float x, y, z, w;

	PropertyAlias(x, r, float);
	PropertyAlias(y, g, float);
	PropertyAlias(z, b, float);
	PropertyAlias(w, a, float);

};


