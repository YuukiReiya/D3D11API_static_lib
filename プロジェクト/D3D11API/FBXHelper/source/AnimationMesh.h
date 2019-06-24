/*!
	@brief	�A�j���[�V�������݂̃��b�V���p�̃N���X
*/

#pragma once
#include <vector>
#include <map>
//#include <unordered_map>
#include "OriginalFormatMath.h"


namespace Utility {

	struct AnimationInfo {

		unsigned int fps;//	�t���[����

		//n�t���[���ڂ̒��_���
		//std::unordered_map<unsigned int,Math::FLOAT3>vertices;//���_
		std::map<unsigned int, std::vector<Math::FLOAT3>>vertices;//���_
	};

	struct AnimationMesh {
		AnimationMesh() :animCount(0) {}
		~AnimationMesh() {}

		std::vector<unsigned int>indices;
		std::vector<Math::FLOAT2> uvPosition;

		unsigned int animCount;

		//n�Ԗڂ̃A�j���[�V�����̏��
		//std::unordered_map<int, AnimationInfo>info;
		std::map<int, AnimationInfo>info;

		//	UV�Z�b�g��UV�̃Z�b�g�̃��X�g�͏ȗ�
	};
}