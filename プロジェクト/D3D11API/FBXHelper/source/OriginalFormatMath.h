/*!
	@file	OriginalFormatMath.h
	@date	2019/05/24
	@author	番場 宥輝
	@brief	値保存用の構造体
	@detail	directXMath.hを読み込まないので"XM"の代わりとなる構造体を自己定義
*/
#pragma once

namespace Math {
	struct FLOAT4
	{
		float x, y, z, w;
		inline bool operator==(FLOAT4&f4) {
			return this->x == f4.x&&this->y == f4.y&&this->z == f4.z&&this->w == f4.w;
		}
		inline bool operator!=(FLOAT4&f4) {
			return this->x != f4.x || this->y != f4.y || this->z != f4.z || this->w != f4.w;
		}
	};

	struct FLOAT3
	{
		float x, y, z;
		inline bool operator==(FLOAT3&f3) {
			return this->x == f3.x&&this->y == f3.y&&this->z == f3.z;
		}
		inline bool operator!=(FLOAT3&f3) {
			return this->x != f3.x || this->y != f3.y || this->z != f3.z;
		}
	};

	struct FLOAT2
	{
		float x, y;
		inline bool operator==(FLOAT2 f2) {
			return this->x == f2.x&&this->y == f2.y;
		}
		inline bool operator!=(FLOAT2 f2) {
			return this->x != f2.x || this->y != f2.y;
		}
	};
}