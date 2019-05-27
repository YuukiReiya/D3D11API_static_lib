/*
	@file	Mesh.h
	@date	2019/05/24
	@author	番場 宥輝
	@brief	最小限メッシュのクラス
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