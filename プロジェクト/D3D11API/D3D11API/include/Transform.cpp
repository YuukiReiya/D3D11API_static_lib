/*
	@file	Transform.h
	@date	2019/04/17
	@author	�ԏ� �G�P
	@brief	�g�����X�t�H�[���N���X
*/
#include "stdafx.h"
#include "Transform.h"

/*!
	@brief	using�f�B���N�e�B�u
	@using	DirectX
*/
using namespace DirectX;

/*!
	@brief	using�f�B���N�e�B�u
	@using	std
*/
using namespace std;

/*!
	@brief	�R���X�g���N�^
	@detail	�e�s��ɒP�ʍs��������A������
*/
Transform::Transform()
	:m_ScalingMatrix(c_IdentityMatrix)
	,m_RotationMatrix(c_IdentityMatrix)
	,m_TranslationMatrix(c_IdentityMatrix)
{
}

/*!
	@brief	�f�X�g���N�^
*/
Transform::~Transform()
{
}
