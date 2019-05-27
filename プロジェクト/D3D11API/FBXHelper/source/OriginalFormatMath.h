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
	};

	struct FLOAT3
	{
		float x, y, z;
	};

	struct FLOAT2
	{
		float x, y;
		/*bool operator==(FLOAT2 f2)
		{
			return this->x == f2.x&&this->y == f2.y;
		}*/
	};
}