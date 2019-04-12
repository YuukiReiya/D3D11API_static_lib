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
			DirectX::XMMATRIX m_WorldMatrix;		/*!< ���[���h */
			DirectX::XMMATRIX m_ProjectionMatrix;	/*!< �ˉe�s�� */
			DirectX::XMMATRIX m_ViewMatrix;			/*!< �r���[�s�� */
			DirectX::XMMATRIX m_WVP;
		};
#pragma pack(pop)
	};
};