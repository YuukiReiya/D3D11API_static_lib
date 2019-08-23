/*
	@file	SkinnedVertex.h
	@date	2019/07/20
	@author	�ԏ� �G�P
	@brief	�X�L�����b�V���p�̒��_�\����
*/
#pragma once
#include <DirectXMath.h>
#include <vector>
#include "Joint.h"

namespace D3D11
{
	namespace Graphic {

		//NOTE:�@���͏ȗ�
		struct SkinnedVertex
		{
			//SkinnedVertex() :position({ 0,0,0 }), uv({ 0,0 }) {}
			/*!
				@var	position
				@brief	���_���W
			*/
			DirectX::XMFLOAT3 position;

			/*!
				@var	uv
				@brief	UV���W
			*/
			DirectX::XMFLOAT2 uv;

			/*!
				@var	joints
				@brief	1���_���ւ��֐ߏ��
			*/
			std::vector<Joint> joints;
		};
	}
}
	