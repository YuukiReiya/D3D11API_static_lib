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
				//MatrixConstantBuffer matrix;
				DirectX::XMMATRIX world;
				DirectX::XMMATRIX view;
				DirectX::XMMATRIX proj;
				DirectX::XMFLOAT4 color;
			};
#pragma pack(pop)
		}
	}
}
