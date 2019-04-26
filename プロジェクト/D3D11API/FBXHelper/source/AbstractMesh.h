/*
	@file	AbstractMesh.h
	@date	2019/04/25
	@author	番場 宥輝
	@brief	メッシュの抽象クラス
	@detail	.fbxの読み込んだ情報を入れるメッシュの抽象クラス
*/
#pragma once
#include <fbxsdk.h>
#include "Vertex.h"
#include <vector>
#include <memory>

namespace Abstract{
	class AbstractMesh abstract
	{
	public:
		AbstractMesh() {};
		virtual ~AbstractMesh() {};


		//TODO:Vertexではなく抽象クラスのAbstractVertexを保持したい願望。
		//	   ただ、ポインタだとインスタンス生成がだるいので現状安定か…
		std::vector<Utility::Vertex>vertex;
		//std::shared_ptr<std::vector<AbstractVertex>>vertex;
		std::vector<int>indexBuffer;
		virtual void Setup()abstract;
	protected:
	private:

	};
}