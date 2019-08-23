/*
	@file	SkinnedVertex.h
	@date	2019/07/20
	@author	番場 宥輝
	@brief	スキンメッシュ用の頂点構造体
*/
#pragma once
#include <DirectXMath.h>
#include <vector>
#include "Joint.h"

namespace D3D11
{
	namespace Graphic {

		//NOTE:法線は省略
		struct SkinnedVertex
		{
			//SkinnedVertex() :position({ 0,0,0 }), uv({ 0,0 }) {}
			/*!
				@var	position
				@brief	頂点座標
			*/
			DirectX::XMFLOAT3 position;

			/*!
				@var	uv
				@brief	UV座標
			*/
			DirectX::XMFLOAT2 uv;

			/*!
				@var	joints
				@brief	1頂点が関わる関節情報
			*/
			std::vector<Joint> joints;
		};
	}
}
	