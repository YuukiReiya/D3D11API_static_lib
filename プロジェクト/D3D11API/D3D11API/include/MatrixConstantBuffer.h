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

		/*!
			@struct	MatrixConstantBuffer
			@brief	�s������܂Ƃ߂��R���X�^���g�o�b�t�@�p�̍\����
			@detail	�萔�o�b�t�@�̂���16�o�C�g���E�ɃA���C�����g���Ă����B
		*/
		struct alignas(16) MatrixConstantBuffer
		{
			/*!
				@var	world
				@brief	���[���h�s��
			*/
			DirectX::XMMATRIX world;

			/*!
				@var	view
				@brief	�r���[�s��
			*/
			DirectX::XMMATRIX view;

			/*!
				@var	proj
				@brief	�v���W�F�N�V�����s��
			*/
			DirectX::XMMATRIX proj;
		};
	}
}