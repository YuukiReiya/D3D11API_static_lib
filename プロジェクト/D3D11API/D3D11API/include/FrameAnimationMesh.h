/*
	@file	FrameAnimationMesh.h
	@date	2019/07/22
	@author	�ԏ� �G�P
	@brief	�Z���A�j���[�V�������\�����郁�b�V���\����

	@note	�Z���A�j���[�V�����̂Ƃ炦�����l���ꂼ�ꂾ�����̂ŁA
			�t���[�����̒��_���������邱�Ƃ���"FrameAnimationMesh"�Ƃ��Ă���
*/
#pragma once
#include "MeshVertex.h"
#include <vector>
#include <unordered_map>
#include <DirectXMath.h>
namespace D3D11 {
	namespace Graphic {

		/*!
			@struct	FrameAnimationMesh
			@brief	�Z���A�j���[�V�������s�����߂̕K�v�����i�[
		*/
		struct FrameAnimationMesh
		{
			/*!
				@var	indices
				@brief	���_�C���f�b�N�X
			*/
			std::vector<uint32_t>indices;

			/*!
				@var	vertices
				@brief	���_���(���W,UV)
				@sample	vertices[�A�j���[�V�����ԍ�][�t���[��][���_�ԍ�]�ŃA�N�Z�X����
			*/
			//std::vector<std::vector<std::vector<MeshVertex>>>vertices;

			//	UV�����W�Ɛ؂藣�����Ȃ火	�����Ȃ灪

			/*!
				@var	vertices
				@brief	���_���(���W)
				@sample	vertices[�A�j���[�V�����ԍ�][�t���[��][���_�ԍ�]�ŃA�N�Z�X����
			*/
			//std::vector<std::vector<std::vector<DirectX::XMFLOAT3>>>vertices;
			std::unordered_map<unsigned int, std::unordered_map<unsigned int, std::vector<DirectX::XMFLOAT3>>>vertices;

			/*!
				@var	uv
				@brief	UV
			*/
			std::vector<DirectX::XMFLOAT2>uv;
		};

	}
}