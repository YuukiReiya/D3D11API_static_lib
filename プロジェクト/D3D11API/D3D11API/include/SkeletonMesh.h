/*
	@file	SkeletonMesh.h
	@date	2019/07/20
	@author	�ԏ� �G�P
	@brief	�X�L�����b�V���p�̍\����

	@note	�X�P���g���ɒ��_�̏�������������
*/
#pragma once
#include "Skeleton.h"
#include "SkinnedVertex.h"
#include <vector>

struct SkeletonMesh
{
	std::vector<SkinnedVertex>vertices;
	std::vector<uint32_t>indices;
	Skeleton skeleton;
};