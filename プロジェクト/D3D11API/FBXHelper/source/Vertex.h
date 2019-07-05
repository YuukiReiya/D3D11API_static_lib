#pragma once
#include "OriginalFormatMath.h"
#include <vector>
#include <unordered_map>

namespace FBX {
	namespace Utility{

		struct Vertex
		{
		public:
			inline Vertex() :position({ 0,0,0 }), uv({ -1,-1 }) {};
			~Vertex() {};

			Math::FLOAT3 position;
			Math::FLOAT2 uv;

			/*!
				NOTE:固定長配列でも良かったのだが、ボーン数の多いモデルの読み込みや動作保証を持たせたかった。
				また、ボーン数が少ない場合は描画コストを削減できる見込み。
				
				※別途".ini"等に配列サイズを吐き出した方がいい。
			*/
			std::vector<unsigned int>indexOfBonesAffested;

			//	"indexOfBonesAffested"をキーに対応する重みを格納
			std::unordered_map<unsigned int,float>weight;
		private:

		};
	}
}