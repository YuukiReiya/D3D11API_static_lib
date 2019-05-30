/*!
	@file	OriginalFormatMath.h
	@date	2019/05/24
	@author	�ԏ� �G�P
	@brief	�l�ۑ��p�̍\����
	@detail	directXMath.h��ǂݍ��܂Ȃ��̂�"XM"�̑���ƂȂ�\���̂����Ȓ�`
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