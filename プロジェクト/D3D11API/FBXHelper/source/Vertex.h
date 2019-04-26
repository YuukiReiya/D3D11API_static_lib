/*
	@file	Vertex.h
	@date	2019/04/25
	@author	番場 宥輝
	@brief	最小限頂点のクラス
	@detail	抽象クラスの派生クラス
*/
#pragma once
#include "AbstractVertex.h"

namespace Utility {
	class Vertex
		: public Abstract::AbstractVertex
	{
	public:
		Vertex() :Abstract::AbstractVertex() {};
		~Vertex()override{};

	private:

	};

}