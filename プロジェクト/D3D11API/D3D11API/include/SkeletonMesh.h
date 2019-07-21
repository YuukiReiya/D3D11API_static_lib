/*
	@file	SkeletonMesh.h
	@date	2019/07/20
	@author	番場 宥輝
	@brief	スキンメッシュ用の構造体

	@note	スケルトンに頂点の情報を加えたもの
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