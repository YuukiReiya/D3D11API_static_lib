/*
	@file	Mesh.h
	@date	2019/05/24
	@author	�ԏ� �G�P
	@brief	�Œ�����b�V����\���������ێ������N���X
*/
#pragma once
#include <vector>
#include <fbxsdk/scene/geometry/fbxlayer.h>
#include <unordered_map>
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
			@var	vertexIndices
			@brief	���_�C���f�b�N�X
		*/
		std::vector<int>vertexIndices;

		/*!
			@struct		UV
			@brief		UVset�� + �e�N�X�`�����W
		*/
		struct UV
		{
			/*!
				@var	uvSetName
				@brief	UVset��
			*/
			std::string uvSetName;

			/*!
				@var	uv
				@brief	�e�N�X�`�����W
			*/
			Math::FLOAT2 uv;
		};

		/*!
			@var		uv
			@key		uvset��
			@value		UV(UVset�� + �e�N�X�`�����W)�̉ϒ��z��
			@brief		UVset�����L�[�ɂ����e�N�X�`�����W�̉ϒ��z��
		*/
		std::unordered_map<std::string, std::vector<UV>>uv;

		/*!
			@var	uvSetNamesList
			@brief	���b�V�����ێ����Ă���UVset���̃��X�g
		*/
		std::vector<std::string>uvSetNamesList;
	};
}