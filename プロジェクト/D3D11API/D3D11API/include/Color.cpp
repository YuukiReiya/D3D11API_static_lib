/*
	@file	Color.cpp
	@date	2018/11/26
	@author	�ԏ� �G�P
	@brief	�J���[�N���X
*/
#include "stdafx.h"
#include "Color.h"
#include <algorithm>

/*!
	@brief	using�f�B���N�e�B�u
	@using	std
*/
using namespace std;

/*!
	@brief	��R���X�g���N�^
*/
Color::Color()
{
}

/*!
	@brief	�����t���R���X�g���N�^
	@param[in]	�ꎞ�ϐ�
*/
Color::Color(Color && color) :
	x(clamp(color.x, c_MinValue, c_MaxValue)),
	y(clamp(color.y, c_MinValue, c_MaxValue)),
	z(clamp(color.z, c_MinValue, c_MaxValue)),
	w(clamp(color.w, c_MinValue, c_MaxValue))
{
}

/*!
	@brief		�����t���R���X�g���N�^
	@param[in]	R( 0.0f �` 1.0f )
	@param[in]	G( 0.0f �` 1.0f )
	@param[in]	B( 0.0f �` 1.0f )
	@param[in]	A( 0.0f �` 1.0f )
*/
Color::Color(float r, float g, float b, float a):
	x(clamp(r, c_MinValue, c_MaxValue)),
	y(clamp(g, c_MinValue, c_MaxValue)),
	z(clamp(b, c_MinValue, c_MaxValue)),
	w(clamp(a, c_MinValue, c_MaxValue))
{
}

/*!
	@brief		�����t���R���X�g���N�^
	@param[in]	FLOAT4�^����J���[�^�֕ϊ�
*/
Color::Color(DirectX::XMFLOAT4 color):
	x(clamp(color.x, c_MinValue, c_MaxValue)),
	y(clamp(color.y, c_MinValue, c_MaxValue)),
	z(clamp(color.z, c_MinValue, c_MaxValue)),
	w(clamp(color.w, c_MinValue, c_MaxValue))

{
}

/*!
	@brief	�f�X�g���N�^
	@detail	��f�X�g���N�^
*/
Color::~Color()
{
}

/*!
	@fn		�J���[�̃Q�b�^�[
	@brief	�J���[��XMFLOAT3�^�Ŏ擾
	@detail	�J���[��RGB��������XMFLOAT3�^���擾����
*/
DirectX::XMFLOAT3 Color::GetRGB() const
{
	return DirectX::XMFLOAT3(x, y, z);
}

/*!
	@fn		�J���[�̃Q�b�^�[
	@brief	�J���[��XMFLOAT4�^�Ŏ擾
	@detail	�J���[��RGBA��������XMFLOAT4�^���擾����
*/
DirectX::XMFLOAT4 Color::GetRGBA() const
{
	return DirectX::XMFLOAT4(x, y, z, w);
}
