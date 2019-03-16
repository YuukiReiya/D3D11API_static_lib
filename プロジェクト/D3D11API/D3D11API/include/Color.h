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
	@detail	メンバは( 0.0f ～ 1.0f )までの範囲に収める
*/
class Color
{
public:
	/*!
		@brief	空コンストラクタ
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

	/*!
		@var	c_MinValue
		@brief	取りうる値の最小値
	*/
	static constexpr float c_MinValue = 0.0f;

	/*!
		@var	c_MinValue
		@brief	取りうる値の最大値
	*/
	static constexpr float c_MaxValue = 1.0f;

	/*!
		@var	メンバ変数
		@brief	x = r , y = g , z = b ,w = a
	*/
	float x, y, z, w;

	/*!
		@brief	別名定義(赤)
		@detail	xをrと関連付ける
	*/
	PropertyAlias(x, r, float);

	/*!
		@brief	別名定義(緑)
		@detail	yをgと関連付ける
	*/
	PropertyAlias(y, g, float);

	/*!
		@brief	別名定義(青)
		@detail	zをbと関連付ける
	*/
	PropertyAlias(z, b, float);

	/*!
		@brief	別名定義(透過)
		@detail	wをaと関連付ける
	*/
	PropertyAlias(w, a, float);

	/*!
		@brief	代入演算子
	*/
	Color &operator=(const Color &color) {
		this->x = color.x;
		this->y = color.y;
		this->z = color.z;
		this->w = color.w;
		return (*this);
	}
};