/*
	@file	StructShaderBase.h
	@date	2018/03/04
	@author	�ԏ� �G�P
	@brief	�V�F�[�_�[�֘A�̍\���̐錾
*/
#pragma once
#include <DirectXMath.h>


/*! Direct3D11�֘A�̖��O��� */
namespace D3D11 {

	/*! �`��֘A�̖��O��� */
	namespace Graphic {

		/*!
			@brief	��꒸�_�\����
		*/
		struct BaseVertex
		{
			DirectX::XMFLOAT3 m_Pos;	/*< ���W */
		};

#pragma pack(push,16)
		/*!
			@brief	�V�F�[�_�[���ɓn���R���X�^���g�o�b�t�@�̊��\����
		*/
		struct alignas(16) BaseConstantBuffer
		{
//------------------------------------------------------------------------------------
//	@NOTE	�R���X�^���g�o�b�t�@�̃����o�ύX�ɔ����A
//			�V�F�[�_�[�̐؂�ւ����s��
//			(���V�F�[�_�[�̐؂�ւ��͎Q�Ƃ���f�B���N�g����ύX���邱�ƂŎ�������I)
//
//			StractShaderBase����"mat"�}�N���Ő؂�ւ�
//			"mat"��`�ς݁��s������[���h�A�r���[�A�v���W�F�N�V�����ɕ���
//			"mat"����`  ��3���|�����ϊ��ς݂̍s���n��
//			
//			"mat"�}�N�����Q�Ƃ��Ă���t�@�C��
//			���ˑ��֌W
//			�ESprite.cpp/h
//			�EStractShaderBase.h
//			�Emain.cpp
//
//			�ȉ��Ő錾���Ă���"mat"���R�����g�A�E�g���ŏ�����"mat"����`���삪���s�����B
//------------------------------------------------------------------------------------

#define mat
#if defined mat
			DirectX::XMMATRIX m_WorldMatrix;		/*!< ���[���h */
			DirectX::XMMATRIX m_ProjectionMatrix;	/*!< �ˉe�s�� */
			DirectX::XMMATRIX m_ViewMatrix;			/*!< �r���[�s�� */
#else
			DirectX::XMMATRIX m_WVP;
#endif
		};
#pragma pack(pop)
	};
};