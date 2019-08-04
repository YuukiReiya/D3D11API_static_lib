/*!
	@file	MeshConstantBuffer.h
	@date	2019/05/18
	@author	番場 宥輝
	@brief	メッシュでシェーダー(GPU)側にデータを送るコンスタントバッファの構造体
*/
#pragma once
#include <DirectXMath.h>
#include "MatrixConstantBuffer.h"

namespace D3D11 {
	namespace Graphic {
		struct alignas(16) MeshConstantBuffer
		{

			/*!
				@var	matrix
				@brief	WVPの行列
			*/
			MatrixConstantBuffer matrix;

			/*!
				@var	color
				@brief	色
			*/
			DirectX::XMFLOAT4 color;
		};

	}
}