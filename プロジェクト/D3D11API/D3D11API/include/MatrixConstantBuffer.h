/*!
	@file	MatrixConstantBuffer.h
	@date	2019/05/18
	@author	�ԏ� �G�P
	@brief	�V�F�[�_�[(GPU)���ɑ���s��(w,v,p)�̍\����
	@detail	�p��������o�b�t�@�����đ��邩�A�����o�Ɏ������Ďg��
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