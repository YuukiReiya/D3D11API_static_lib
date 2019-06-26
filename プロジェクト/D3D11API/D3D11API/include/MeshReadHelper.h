/*
	@file	MeshReadHelper.h
	@date	2019/05/05
	@author	番場 宥輝
	@brief	外部ファイルのメッシュ読み込み
	@detail	シングルトン
*/
#pragma once
#include <DirectXMath.h>
#include <string>
#include <vector>
#include <fstream>
#include "MeshVertex.h"


#include <unordered_map>

namespace D3D11 {
	namespace Helper {
		class MeshReadHelper
		{
		public:
			/*!
				@struct	ReadBuffer
				@brief	読み込んだバッファ
			*/
			struct ReadBuffer
			{
				/*!
					@var	vertices
					@brief	頂点
					@TODO	外部ファイルにはdouble型で書き込まれているため、float型にキャストする
				*/
				std::vector<Graphic::MeshVertex>vertices;

				/*!
					@var	indices
					@brief	頂点インデックス
				*/
				std::vector<uint32_t>indices;
			};

			struct AnimReadBuffer {
				unsigned int animCount;
				//n番目のアニメーションのフレーム数
				std::unordered_map<unsigned int, unsigned int>frameCount;

				//n番目のアニメーションの頂点
				std::unordered_map<unsigned int, std::unordered_map<unsigned int, std::vector<Graphic::MeshVertex>>>vertices;
				std::vector<uint32_t>indices;
			};

			static ReadBuffer Read(std::string path);
			static AnimReadBuffer ReadAnim(std::string path);
		private:
			MeshReadHelper() = delete;
			~MeshReadHelper() = delete;

		};
	}
}