/*
	@file	Mesh.h
	@date	2019/05/24
	@author	番場 宥輝
	@brief	最低限メッシュを表示する情報を保持したクラス
*/
#pragma once
#include <vector>
#include <fbxsdk/scene/geometry/fbxlayer.h>
//#include <map>
#include <unordered_map>
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
			@var	vertexIndices
			@brief	頂点インデックス
		*/
		std::vector<int>vertexIndices;

		/*!
			@struct		UV
			@brief		UV + UVset名
		*/
		struct UV
		{
			/*!
				@var	uvSetName
				@brief	UVset名
			*/
			std::string uvSetName;

			/*!
				@var	uv
				@brief	UV座標
			*/
			Math::FLOAT2 uv;
		};

		/*!
			@var		uv
			@key		uvset名
			@value		UV(UV + UVset名)の可変長配列
			@brief		UVset名をキーにしたUVの可変長配列
		*/
		//std::map<std::string, std::vector<UV>>uv;
		std::unordered_map<std::string, std::vector<UV>>uv;

		/*!
			@var	uvSetNamesList
			@brief	メッシュが保持しているUVset名のリスト
		*/
		std::vector<std::string>uvSetNamesList;
	};
}