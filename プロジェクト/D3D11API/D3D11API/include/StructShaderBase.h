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
		struct BaseVertex abstract
		{
			/*!
				@brief	�f�t�H���g�R���X�g���N�^
			*/
			BaseVertex() = default;

			/*!
				@brief	�����t���R���X�g���N�^
			*/
			inline BaseVertex(DirectX::XMFLOAT3 position)
			{
				this->position = position;
			}

			/*!
				@var	position
				@brief	3D��ԏ�̍��W
			*/
			DirectX::XMFLOAT3 position;
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
		};
#pragma pack(pop)
	};
};