/*
	@file	FrameAnimationMesh.h
	@date	2019/07/22
	@author	番場 宥輝
	@brief	セルアニメーションを構成するメッシュ構造体

	@note	セルアニメーションのとらえ方が人それぞれだったので、
			フレーム毎の頂点を持たせることから"FrameAnimationMesh"としている
*/
#pragma once
#include "MeshVertex.h"
#include <vector>
#include <unordered_map>
#include <DirectXMath.h>
namespace D3D11 {
	namespace Graphic {

		/*!
			@struct	FrameAnimationMesh
			@brief	セルアニメーションを行うための必要情報を格納
		*/
		struct FrameAnimationMesh
		{
			/*!
				@var	indices
				@brief	頂点インデックス
			*/
			std::vector<uint32_t>indices;

			/*!
				@var	vertices
				@brief	頂点情報(座標,UV)
				@sample	vertices[アニメーション番号][フレーム][頂点番号]でアクセスする
			*/
			//std::vector<std::vector<std::vector<MeshVertex>>>vertices;

			//	UVが座標と切り離されるなら↓	無理なら↑

			/*!
				@var	vertices
				@brief	頂点情報(座標)
				@sample	vertices[アニメーション番号][フレーム][頂点番号]でアクセスする
			*/
			//std::vector<std::vector<std::vector<DirectX::XMFLOAT3>>>vertices;
			std::unordered_map<unsigned int, std::unordered_map<unsigned int, std::vector<DirectX::XMFLOAT3>>>vertices;

			/*!
				@var	uv
				@brief	UV
			*/
			std::vector<DirectX::XMFLOAT2>uv;
		};

	}
}