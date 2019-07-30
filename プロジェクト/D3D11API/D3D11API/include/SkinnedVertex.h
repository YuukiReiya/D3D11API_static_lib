/*
	@file	SkinnedVertex.h
	@date	2019/07/20
	@author	番場 宥輝
	@brief	スキンメッシュ用の頂点構造体

	@note	ゲームエンジンアーキテクチャ参考
*/
#pragma once
#include <DirectXMath.h>
#include <vector>
//NOTE:法線は省略
struct SkinnedVertex
{
	//SkinnedVertex() :position({ 0,0,0 }), uv({ 0,0 }), jointWeights() { for (auto&it : joijntIndex) { it = 0; } }
	DirectX::XMFLOAT3 position;
	//DirectX::XMFLOAT2 uv;

	//static constexpr uint8_t c_MaxBindJointCount = 4;
	//uint8_t joijntIndex[c_MaxBindJointCount];
	//float jointWeights[c_MaxBindJointCount - 1];

	//std::vector<uint8_t>jointsIndex;
	//std::vector<float>jointsWeight;

};
