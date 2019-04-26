/*
	@file	Mesh.h
	@date	2019/04/25
	@author	番場 宥輝
	@brief	最小限メッシュのクラス
	@detail	抽象クラスの派生クラス
*/
#pragma once
#include "AbstractMesh.h"
#include "Vertex.h"

namespace Utility {
	class Mesh 
		: public Abstract::AbstractMesh
	{
	public:
		Mesh() :Abstract::AbstractMesh() {}
		~Mesh()override {}

		void Setup()override;
	};
}