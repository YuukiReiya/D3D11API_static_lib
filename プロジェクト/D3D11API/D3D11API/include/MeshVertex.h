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
		struct MeshVertex
			:public BaseVertex
		{
			//int m_Index;
		};

	}
}