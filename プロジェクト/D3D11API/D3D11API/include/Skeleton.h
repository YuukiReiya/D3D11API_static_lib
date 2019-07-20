/*
	@file	Skeleton.h
	@date	2019/07/20
	@author	�ԏ� �G�P
	@brief	�֐߂ƃX�P���g���̍\���̒�`

	@note	�Q�[���G���W���A�[�L�e�N�`���Q�l
*/
#pragma once
#include <string>
#include <DirectXMath.h>
#include <memory>

/*!
	@struct	Joint
	@brief	�֐�
*/
struct Joint
{
	DirectX::XMFLOAT4X3 invBindPose;
	std::string name;
	uint8_t parent;
};

/*!
	@struct	Skeleton
	@brief	�X�P���g��

	@note	���b�V���ɑΉ������֐߂�ێ�����\����
*/
struct Skeleton
{
	uint32_t jointCount;
	std::shared_ptr<Joint>joints;
};