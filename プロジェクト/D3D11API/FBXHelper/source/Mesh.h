/*
	@file	Mesh.h
	@date	2019/05/24
	@author	番場 宥輝
	@brief	最低限メッシュを表示する情報を保持したクラス
*/
#pragma once
#include <vector>
#include "OriginalFormatMath.h"

namespace Utility {
	class Mesh 
	{
	public:
		/*!
			@brief	コンストラクタ
		*/
		Mesh();

		/*!
			@brief	デストラクタ
		*/
		~Mesh();

		/*!
			@var	vertices
			@brief	頂点を格納する可変長配列
		*/
		std::vector<Math::FLOAT4>vertices;

		/*!
			@var	uv
			@brief	UVを格納する可変長配列
		*/
		std::vector<Math::FLOAT2>uv;

		/*!
			@var	vertexIndices
			@brief	頂点インデックス
		*/
		std::vector<int>vertexIndices;

		/*!
			@var	uvIndices
			@brief	UVインデックス
		*/
		std::vector<int>uvIndices;
	};
}