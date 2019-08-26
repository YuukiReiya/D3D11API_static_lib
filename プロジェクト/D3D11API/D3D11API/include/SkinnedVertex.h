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

			/*!
				@operator ==
				@brief	�����Z�q
				@detail	���� == �E�� �Ń����o�̊��S��v���ǂ����Ԃ��B
				@return	true:�����o�����S��v false:�����o����ł��Ⴄ
				@note	STL�̃A���S���Y���p�ɒ�`
			*/
			bool operator==(SkinnedVertex self)
			{
				return this->uv.x == self.uv.x&&this->uv.y == self.uv.y&&
					this->position.x == self.position.x&&this->position.y == self.position.y&&this->position.z == self.position.z&&
					std::equal(this->joints.begin(), this->joints.end(), self.joints.begin(), self.joints.end());
			}
		};
	}
}
	