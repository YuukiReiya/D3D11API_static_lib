/*
	@file	Mesh.h
	@date	2019/05/24
	@author	�ԏ� �G�P
	@brief	�ŏ������b�V���̃N���X
*/
#pragma once
#include <vector>
#include "OriginalFormatMath.h"

namespace Utility {
	class Mesh 
	{
	public:
		Mesh();
		~Mesh();

		std::vector<Math::FLOAT4>vertices;
		std::vector<Math::FLOAT2>uv;
		std::vector<int>index;
	};
}