/*!
	@file	MeshConstantBuffer.h
	@date	2019/05/18
	@author	�ԏ� �G�P
	@brief	���b�V���ŃV�F�[�_�[(GPU)���Ƀf�[�^�𑗂�R���X�^���g�o�b�t�@�̍\����
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
				@brief	�F
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