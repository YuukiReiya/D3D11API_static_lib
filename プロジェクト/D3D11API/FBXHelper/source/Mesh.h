/*
	@file	Mesh.h
	@date	2019/04/25
	@author	�ԏ� �G�P
	@brief	�ŏ������b�V���̃N���X
	@detail	���ۃN���X�̔h���N���X
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