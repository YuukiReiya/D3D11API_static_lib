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