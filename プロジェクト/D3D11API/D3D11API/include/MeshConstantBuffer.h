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
			//:public MatrixConstantBuffer
		{
			/*!
				@var	color
				@brief	色
			*/
			MatrixConstantBuffer m;
			DirectX::XMFLOAT4 color;
			//DirectX::XMMATRIX world;
			//DirectX::XMMATRIX view;
			//DirectX::XMMATRIX proj;
			//MatrixConstantBuffer m;
		};

	}
}