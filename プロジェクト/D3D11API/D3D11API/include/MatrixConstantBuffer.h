/*!
	@file	MatrixConstantBuffer.h
	@date	2019/05/18
	@author	番場 宥輝
	@brief	シェーダー(GPU)側に送る行列(w,v,p)の構造体
	@detail	継承するよりバッファ分けて送るか、メンバに持たせて使う
*/
#pragma once
#include <DirectXMath.h>

namespace D3D11 {
	namespace Graphic {

		struct alignas(16) MatrixConstantBuffer
		{
			DirectX::XMMATRIX world;
			DirectX::XMMATRIX view;
			DirectX::XMMATRIX proj;
		};
	}
}