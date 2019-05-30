/*
	@file	Mesh.h
	@date	2019/05/24
	@author	�ԏ� �G�P
	@brief	�Œ�����b�V����\���������ێ������N���X
*/
#pragma once
#include <vector>
#include "OriginalFormatMath.h"

namespace Utility {
	class Mesh 
	{
	public:
		/*!
			@brief	�R���X�g���N�^
		*/
		Mesh();

		/*!
			@brief	�f�X�g���N�^
		*/
		~Mesh();

		/*!
			@var	vertices
			@brief	���_���i�[����ϒ��z��
		*/
		std::vector<Math::FLOAT4>vertices;

		/*!
			@var	uv
			@brief	UV���i�[����ϒ��z��
		*/
		std::vector<Math::FLOAT2>uv;

		/*!
			@var	vertexIndices
			@brief	���_�C���f�b�N�X
		*/
		std::vector<int>vertexIndices;

		/*!
			@var	uvIndices
			@brief	UV�C���f�b�N�X
		*/
		std::vector<int>uvIndices;
	};
}