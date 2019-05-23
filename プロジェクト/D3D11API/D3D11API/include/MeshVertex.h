/*
	@file	MeshVertex.h
	@date	2019/05/06
	@author	�ԏ� �G�P
	@brief	���b�V�����\�����钸�_�N���X
	@NOTE	�X�v���C�g�ƈ���ă��b�V���͓Ǝ��`���œǂݍ��ނ��߁A�w���p�N���X����������B
			���̍ۂɑ��ݎQ�Ƃ�����邽�߃t�@�C���������s���B
*/

#pragma once
#include "StructShaderBase.h"
namespace D3D11 {
	namespace Graphic {
		/*!
			@struct	MeshVertex
			@brief	���b�V���N���X�ň������_�̍\����
		*/
		struct alignas(16) MeshVertex
			:public BaseVertex
		{
			/*!
				@brief	�f�t�H���g�R���X�g���N�^
			*/
			//MeshVertex() = default;

			MeshVertex() :BaseVertex(), uv({ -1,-1 }) {}

			/*!
				@brief		�����t���R���X�g���N�^
				@param[in]	���W
			*/
			MeshVertex(DirectX::XMFLOAT3 position) :BaseVertex(position), uv({ -1,-1 }) {}


			//TODO:�R�����g���������菑��
			DirectX::XMFLOAT2 uv;
		};

	}
}