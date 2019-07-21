#pragma once
#include <vector>
#include <DirectXMath.h>
#include <memory>
#include "Skeleton.h"

//NOTE:メンバが増えないなら別名定義でいい
//using AnimationJoints = std::vector<DirectX::XMMATRIX>;
//struct FrameMatrix
//{
//	std::vector<DirectX::XMMATRIX>FrameMatrix;
//
//};
using FrameMatrix = std::vector<DirectX::XMMATRIX>;

struct AnimationClip
{
	Skeleton*pSkeleton;//関連スケルトン
	uint32_t frameCount;//	フレーム数
	//std::vector<DirectX::XMMATRIX>jointsMatrix;//フレーム行列
	std::vector<FrameMatrix>bonesMatrix;//ボーン毎のフレーム行列が入っている

};