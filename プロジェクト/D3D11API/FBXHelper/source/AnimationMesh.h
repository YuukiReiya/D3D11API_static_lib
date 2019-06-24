/*!
	@brief	アニメーション込みのメッシュ用のクラス
*/

#pragma once
#include <vector>
#include <map>
//#include <unordered_map>
#include "OriginalFormatMath.h"


namespace Utility {

	struct AnimationInfo {

		unsigned int fps;//	フレーム数

		//nフレーム目の頂点情報
		//std::unordered_map<unsigned int,Math::FLOAT3>vertices;//頂点
		std::map<unsigned int, std::vector<Math::FLOAT3>>vertices;//頂点
	};

	struct AnimationMesh {
		AnimationMesh() :animCount(0) {}
		~AnimationMesh() {}

		std::vector<unsigned int>indices;
		std::vector<Math::FLOAT2> uvPosition;

		unsigned int animCount;

		//n番目のアニメーションの情報
		//std::unordered_map<int, AnimationInfo>info;
		std::map<int, AnimationInfo>info;

		//	UVセットとUVのセットのリストは省略
	};
}