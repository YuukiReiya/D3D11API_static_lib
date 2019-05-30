/*
	@file	Mesh.cpp
	@date	2019/05/24
	@author	番場 宥輝
	@brief	最低限メッシュを表示する情報を保持したクラス
*/
#include "Common.h"
#include "Mesh.h"

/*!
	@brief	usingディレクティブ
	@using	std
*/
using namespace std;

/*!
	@brief	usingディレクティブ
	@using	Utility
*/
using namespace Utility;

/*!
	@brief	コンストラクタ
*/
Mesh::Mesh()
{
	vertices.clear();
	uv.clear();
	vertexIndices.clear();
	uvIndices.clear();
}

/*!
	@brief	デストラクタ
*/
Mesh::~Mesh()
{

}