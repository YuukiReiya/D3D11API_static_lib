/*
	@file	Quaternion.h
	@date	2019/04/02
	@author	番場 宥輝
	@brief	クォータニオンクラス
*/
#pragma once
#include <DirectXMath.h>

class Quaternion
{
public:
	Quaternion();
	~Quaternion();


	static inline DirectX::XMMATRIX GetRotationMatrix(DirectX::XMVECTOR );
	static inline DirectX::XMVECTOR GetQuaternion(Quaternion quaternion) { return quaternion.m_Quaternion; }

private:
	DirectX::XMVECTOR m_Quaternion;

	float x;
	float y;
	float z;
	float w;

#pragma region def
#define PropertyAlias(base,name,...)\
__declspec(property(get = Get##base, put = Set##base)) __VA_ARGS__ name;\
__VA_ARGS__ & Get##base(){return base;}\
void Set##base(const __VA_ARGS__& value){base = value;}
#pragma endregion

};

/*!--memo--
https://kumadasu.com/2017/09/20/%E7%B5%84%E8%BE%BC%E3%81%BF%E6%8A%80%E8%A1%93%E8%80%85%E3%81%AE%E3%81%9F%E3%82%81%E3%81%AE%E5%9B%9B%E5%85%83%E6%95%B0%EF%BC%88%E3%82%AF%E3%82%A9%E3%83%BC%E3%82%BF%E3%83%8B%E3%82%AA%E3%83%B3%EF%BC%89/
	q = w*v = [w,x,y,z]

	q = Quaternion
	w = スカラー
	v = ベクトル : [x,y,z]

*/