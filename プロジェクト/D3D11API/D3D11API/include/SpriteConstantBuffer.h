#pragma once
#include <DirectXMath.h>
#include "MatrixConstantBuffer.h"

namespace D3D11 {
	namespace Graphic {
		namespace Sprite
		{
#pragma pack(push,16)
			struct alignas(16) ConstantBuffer
			{
				/*!
					@var	matrix
					@brief	WVPを持つ行列の構造体
				*/
				MatrixConstantBuffer matrix;

				/*!
					@var	color
					@brief	色
				*/
				DirectX::XMFLOAT4 color;
			};
#pragma pack(pop)
		}
	}
}
