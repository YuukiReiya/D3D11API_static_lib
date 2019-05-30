/*
	@file	Mesh.cpp
	@date	2019/05/24
	@author	�ԏ� �G�P
	@brief	�Œ�����b�V����\���������ێ������N���X
*/
#include "Common.h"
#include "Mesh.h"

/*!
	@brief	using�f�B���N�e�B�u
	@using	std
*/
using namespace std;

/*!
	@brief	using�f�B���N�e�B�u
	@using	Utility
*/
using namespace Utility;

/*!
	@brief	�R���X�g���N�^
*/
Mesh::Mesh()
{
	vertices.clear();
	uv.clear();
	vertexIndices.clear();
	uvIndices.clear();
}

/*!
	@brief	�f�X�g���N�^
*/
Mesh::~Mesh()
{

}