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
#include <unordered_map>
#include <fstream>
#include "MeshVertex.h"


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
			static ReadBuffer Read(std::string path);

			struct SkeltonMesh
			{
				SkeltonMesh() :vertices(), indices(0), maxBonesElementsCount(0), frame(0) {};
				~SkeltonMesh() = default;

				int frame;
				struct Vertex
				{
				public:
					inline Vertex() :position({ 0,0,0 }), uv({ -1,-1 }) {};
					~Vertex() {};

					DirectX::XMFLOAT3 position;
					DirectX::XMFLOAT2 uv;

					/*!
						NOTE:固定長配列でも良かったのだが、ボーン数の多いモデルの読み込みや動作保証を持たせたかった。
						また、ボーン数が少ない場合は描画コストを削減できる見込み。

						※別途".ini"等に配列サイズを吐き出した方がいい。
					*/
					std::vector<unsigned int>indexOfBonesAffested;

					//	"indexOfBonesAffested"をキーに対応する重みを格納
					std::unordered_map<unsigned int, float>weight;
				};


				std::vector<Vertex> vertices;
				std::vector<unsigned int>indices;

				//	ボーン毎？
				std::vector<DirectX::XMFLOAT4X4> initialMatrix;
				std::vector<std::vector<DirectX::XMFLOAT4X4>> frameMatrix;
				unsigned int maxBonesElementsCount;//保持する頂点の最大"関連ボーン"数
			};

			static SkeltonMesh ReadAnim(std::string path);
		private:
			MeshReadHelper() = delete;
			~MeshReadHelper() = delete;

		};
	}
}