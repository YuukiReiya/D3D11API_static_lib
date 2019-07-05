#pragma once
#include "Vertex.h"
/*!
	とりま"アダプター"で実装しといて、終わったら修正の方針。
*/

namespace FBX {
	namespace Utility {
		class SkeltonMesh
		{
		public:
			SkeltonMesh() :vertices(),indices(0),maxBonesElementsCount(0) {};
			~SkeltonMesh() = default;

			std::vector<Vertex> vertices;
			std::vector<unsigned int>indices;

			//	ボーン毎？
			std::vector<Math::FLOAT4X4> initialMatrix;
			std::vector<std::vector<Math::FLOAT4X4>> frameMatrix;
			unsigned int maxBonesElementsCount;//保持する頂点の最大"関連ボーン"数
		private:

		};
	}
}