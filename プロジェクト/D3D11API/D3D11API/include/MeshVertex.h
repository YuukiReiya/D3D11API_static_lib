/*
	@file	MeshVertex.h
	@date	2019/05/06
	@author	番場 宥輝
	@brief	メッシュを構成する頂点クラス
	@NOTE	スプライトと違ってメッシュは独自形式で読み込むため、ヘルパクラスを実装する。
			その際に相互参照を避けるためファイル分けを行う。
*/

#pragma once
#include "StructShaderBase.h"
namespace D3D11 {
	namespace Graphic {
		/*!
			@struct	MeshVertex
			@brief	メッシュクラスで扱う頂点の構造体
		*/
		struct alignas(16) MeshVertex
			:public BaseVertex
		{
			/*!
				@brief	コンストラクタ
			*/
			MeshVertex() :BaseVertex(), uv({ -1,-1 }) {}

			/*!
				@brief		引数付きコンストラクタ
				@param[in]	座標
			*/
			MeshVertex(DirectX::XMFLOAT3 position) :BaseVertex(position), uv({ -1,-1 }) {}

			/*!
				@var	uv
				@brief	テクセル座標
			*/
			DirectX::XMFLOAT2 uv;
		};

	}
}