/*
	@file	Transform.h
	@date	2019/04/17
	@author	番場 宥輝
	@brief	トランスフォームクラス
*/
#include "stdafx.h"
#include "Transform.h"

/*!
	@brief	usingディレクティブ
	@using	DirectX
*/
using namespace DirectX;

/*!
	@brief	usingディレクティブ
	@using	std
*/
using namespace std;

/*!
	@brief	コンストラクタ
	@detail	各行列に単位行列を代入し、初期化
*/
Transform::Transform()
	:m_ScalingMatrix(c_IdentityMatrix)
	,m_RotationMatrix(c_IdentityMatrix)
	,m_TranslationMatrix(c_IdentityMatrix)
{
}

/*!
	@brief	デストラクタ
*/
Transform::~Transform()
{
}
