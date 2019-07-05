#pragma once
#include "Vertex.h"
/*!
	�Ƃ��"�A�_�v�^�["�Ŏ������Ƃ��āA�I�������C���̕��j�B
*/

namespace FBX {
	namespace Utility {
		class SkeltonMesh
		{
		public:
			SkeltonMesh() :vertices(),indices(0),maxBonesElementsCount(0) {};
			~SkeltonMesh() = default;

			std::vector<Vertex> vertices;
			std::vector<unsigned int>indices;

			//	�{�[�����H
			std::vector<Math::FLOAT4X4> initialMatrix;
			std::vector<std::vector<Math::FLOAT4X4>> frameMatrix;
			unsigned int maxBonesElementsCount;//�ێ����钸�_�̍ő�"�֘A�{�[��"��
		private:

		};
	}
}