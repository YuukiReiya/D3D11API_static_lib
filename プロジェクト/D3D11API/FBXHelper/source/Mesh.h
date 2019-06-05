/*
	@file	Mesh.h
	@date	2019/05/24
	@author	�ԏ� �G�P
	@brief	�Œ�����b�V����\���������ێ������N���X
*/
#pragma once
#include <vector>
#include <fbxsdk/scene/geometry/fbxlayer.h>
//#include <map>
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
			@brief		UV + UVset��
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
				@brief	UV���W
			*/
			Math::FLOAT2 uv;
		};

		/*!
			@var		uv
			@key		uvset��
			@value		UV(UV + UVset��)�̉ϒ��z��
			@brief		UVset�����L�[�ɂ���UV�̉ϒ��z��
		*/
		//std::map<std::string, std::vector<UV>>uv;
		std::unordered_map<std::string, std::vector<UV>>uv;

		/*!
			@var	uvSetNamesList
			@brief	���b�V�����ێ����Ă���UVset���̃��X�g
		*/
		std::vector<std::string>uvSetNamesList;
	};
}