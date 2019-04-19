/*
	@file	Camera.h
	@date	2018/04/22
	@author	�ԏ� �G�P
	@brief	�J����
	@detail	�V���O���g��
*/
#pragma once
#include <DirectXMath.h>
#include "Singleton.h"

/****************************************/
/*			�J�����̕`���(v)			*/
/*		NearClip <= v < FarClip			*/
/*	Ex)									*/
/*	NearClip = 0.1f, FarClip = 100.0f	*/
/*	�`�悳���I�u�W�F�N�g�̓J�����̎�	*/
/*	�_+NearClip����J�����̎��_+FarClip */
/*	�͈̔͂Ɏ��܂��Ă�����̂�`�悷��	*/
/****************************************/

/*! API�̖��O��ԂɊ܂߂� */
namespace API {

	/*!
		@brief	�J�����������N���X
	*/
	class Camera
		:public Singleton<Camera>
	{
	public:

		/*!
			@brief	�f�X�g���N�^
		*/
		~Camera();

		/*!
			@var	c_Pi
			@brief	�~������ ��D3DX_PI�Ɠ��l
		*/
		static constexpr double	c_Pi = 3.14159265358979323846;

		/*!
			@var	c_FieldOfView
			@brief	��p
			@value	static_cast<float>(Camera::c_Pi) / 4.0f
					�f�t�H���g�̎���p(45�x)
		*/
		static constexpr float	c_FieldOfView = static_cast<float>(Camera::c_Pi) / 4.0f;

		/*!
			@var	c_NearClip
			@brief	�f�t�H���g�N���b�s���O����:��
		*/
		static constexpr float	c_NearClip = 0.1f;

		/*!
			@var	c_FarClip
			@brief	�f�t�H���g�N���b�s���O����:��
			@value	0.1f
		*/
		static constexpr float	c_FarClip = 100.0f;

		/*!
			@var	c_UpVector
			@brief	�f�t�H���g�̏�����x�N�g��
		*/
		static constexpr DirectX::XMFLOAT3 c_UpVector = { 0,1,0 };

		/*!
			@fn			Initialize
			@brief		������
			@param[in]	���_
			@param[in]	�����_
			@param[in]	������x�N�g��
		*/
		void Initialize(
			DirectX::XMINT2 windowSize,
			DirectX::XMFLOAT3 eyePt,
			DirectX::XMFLOAT3 lookPt = { 0.0f,0.0f,0.0f },
			DirectX::XMFLOAT3 upVec = c_UpVector
		);

		/*!
			@fn		GetViewMatrix
			@brief	�r���[�s��̎擾
			@return	�r���[�s��
		*/
		DirectX::XMMATRIX GetViewMatrix()const { return m_ViewMat; }

		/*!
			@fn		GetProjMatrix
			@brief	�v���W�F�N�V�����s��̎擾
			@return	�v���W�F�N�V�����s��
		*/
		DirectX::XMMATRIX GetProjMatrix()const { return m_ProjMat; }

		/*!
			@fn		GetEyePt
			@brief	���_�ʒu�̎擾
			@return	���_�ʒu
		*/
		DirectX::XMFLOAT3 GetEyePt()const;

		/*!
			@fn		GetLookAtPt
			@brief	�����_�̎擾
			@return	�����_
		*/
		DirectX::XMFLOAT3 GetLookAtPt()const;

		/*!
			@fn			SetWindowSize
			@brief		�E�B���h�E�̃T�C�Y
			@param[in]	�ݒ肷��T�C�Y(x:���� y:�c��)
		*/
		void SetWindowSize(const DirectX::XMINT2 size);

		/*!
			@fn			SetClipDistance
			@brief		�N���b�v�����̐ݒ�
			@param[in]	�N���b�v����:��
			@param[in]	�N���b�v����:��
		*/
		void SetClipDistance(const float nearClip, const float farClip);

		/*!
			@fn			SetFieldOfView
			@brief		����p�̐ݒ�
			@param[in]	FOV�̐ݒ�l
		*/
		void SetFieldOfView(float fov) { m_FieldOfView = fov; }

		/*!
			@fn			SetNearClip
			@brief		�N���b�v����:�߂̐ݒ�
			@param[in]	�ߋ����N���b�v�̐ݒ�l
		*/
		void SetNearClip(float nearClip) { m_NearClip = nearClip; }

		/*!
			@fn			SetFarClip
			@brief		�N���b�v����:���̐ݒ�
			@param[in]	�������N���b�v�̐ݒ�l
		*/
		void SetFarClip(float farClip) { m_FarClip = farClip; }

		/*!
			@fn		SetOrthographic
			@brief	Orthographic(���ˉe�@)�̃v���W�F�N�V�����s��ɐ؂�ւ���
			@detail	���s�����Ȃ�2D�Q�[�������̐ݒ�
		*/
		void SetOrthographic();

		/*!
			@fn		SetPerspective
			@brief	Perspective(�����}�@)�̃v���W�F�N�V�����s��ɐ؂�ւ���
			@detail	���s���̂���3D�Q�[�������̐ݒ�
		*/
		void SetPerspective();

		/*!
			@fn			SetEyePt
			@brief		���_�ʒu�̃Z�b�^�[
			@param[in]	�ݒ肷�鎋�_�ʒu
		*/
		void SetEyePt(DirectX::XMFLOAT3 eyePt);

	private:
		/*!
			@brief	�R���X�g���N�^
		*/
		Camera();

		/*!
			@brief	�V���O���g���f�U�C���p�^�[���̃e���v���[�g�p��
		*/
		friend class Singleton<Camera>;

		//	�����o�ϐ�
		float m_FieldOfView;			// ����p
		float m_NearClip;				// �f�t�H���g�̃N���b�s���O����:��
		float m_FarClip;				// �f�t�H���g�̃N���b�s���O����:��
		DirectX::XMINT2 m_WindowSize;	// �E�B���h�E�̃T�C�Y
		DirectX::XMVECTOR m_EyePt;		// ���_�ʒu
		DirectX::XMVECTOR m_LookAtPt;	// �����_
		DirectX::XMVECTOR m_UpVec;		// ������x�N�g��
		DirectX::XMMATRIX m_ViewMat;	// �r���[�s��
		DirectX::XMMATRIX m_ProjMat;	// �v���W�F�N�V�����s��
	};
}
