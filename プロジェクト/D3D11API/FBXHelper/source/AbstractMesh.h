/*
	@file	AbstractMesh.h
	@date	2019/04/25
	@author	�ԏ� �G�P
	@brief	���b�V���̒��ۃN���X
	@detail	.fbx�̓ǂݍ��񂾏������郁�b�V���̒��ۃN���X
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


		//TODO:Vertex�ł͂Ȃ����ۃN���X��AbstractVertex��ێ���������]�B
		//	   �����A�|�C���^���ƃC���X�^���X���������邢�̂Ō�����肩�c
		std::vector<Utility::Vertex>vertex;
		//std::shared_ptr<std::vector<AbstractVertex>>vertex;
		std::vector<int>indexBuffer;
		virtual void Setup()abstract;
	protected:
	private:

	};
}