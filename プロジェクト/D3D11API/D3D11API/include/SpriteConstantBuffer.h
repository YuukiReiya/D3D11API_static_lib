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
					@brief	WVP‚ğ‚Âs—ñ‚Ì\‘¢‘Ì
				*/
				MatrixConstantBuffer matrix;

				/*!
					@var	color
					@brief	F
				*/
				DirectX::XMFLOAT4 color;
			};
#pragma pack(pop)
		}
	}
}
