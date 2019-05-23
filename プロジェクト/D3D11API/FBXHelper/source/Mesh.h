#include "..\..\D3D11API\include\Mesh.h"
#include "..\..\D3D11API\include\Mesh.h"
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

struct FLOAT4
{
	float x, y, z, w;
};

struct FLOAT3
{
	float x, y, z;
};

struct FLOAT2
{
	float x, y;
	bool operator==(FLOAT2 f2)
	{
		return this->x == f2.x&&this->y == f2.y;
	}
};


namespace Utility {
	class Mesh 
		: public Abstract::AbstractMesh
	{
	public:
		Mesh() :Abstract::AbstractMesh() {}
		~Mesh()override {}

		void Setup()override;
	};

	struct OutputMesh
	{
		std::vector<FLOAT4>vertexPos;
		std::vector<FLOAT2>uv;
		std::vector<int>index;
	};
}