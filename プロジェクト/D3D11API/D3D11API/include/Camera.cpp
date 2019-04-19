/*
	@file	Camera.cpp
	@date	2018/04/22
	@author	�ԏ� �G�P
	@brief	�J����
	@detail	�V���O���g��
*/
#include "stdafx.h"
#include "Camera.h"
#include "Direct3D11.h"

/*!
	@brief	using�f�B���N�e�B�u
	@using	API
*/
using namespace API;

/*!
	@brief	�R���X�g���N�^
*/
Camera::Camera()
{
	// �f�t�H���g�ݒ�X�R�[�v
	{
		m_FieldOfView	= c_FieldOfView;
		m_NearClip		= c_NearClip;
		m_FarClip		= c_FarClip;
		m_EyePt			= DirectX::XMVectorSet(0.0f, 0.0f, -10.0f, 0.0f);
		m_LookAtPt		= DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		m_UpVec			= DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		m_WindowSize	= { 0,0 };
	}
}

/*!
	@brief	�f�X�g���N�^
*/
Camera::~Camera()
{
}

/*!
	@brief		������
	@detail		��������r���[�s��ƃv���W�F�N�V�����s��𐶐�
	@param[in]	���_�ʒu	
	@param[in]	�����_
	@param[in]	������x�N�g��
*/
void Camera::Initialize(DirectX::XMINT2 windowSize, DirectX::XMFLOAT3 eyePt, DirectX::XMFLOAT3 lookPt, DirectX::XMFLOAT3 upVec)
{
	// �E�B���h�E�T�C�Y
	{
		m_WindowSize = windowSize;
	}

	// FLOAT3��VECTOR�^�ɕϊ�
	{
		m_EyePt = DirectX::XMVectorSet(eyePt.x, eyePt.y, eyePt.z, 0.0f);
		m_LookAtPt = DirectX::XMVectorSet(lookPt.x, lookPt.y, lookPt.z, 0.0f);
		m_UpVec = DirectX::XMVectorSet(upVec.x, upVec.y, upVec.z, 0.0f);
	}

	// �r���[�s��
	{
		m_ViewMat = DirectX::XMMatrixLookAtLH(
			m_EyePt,							// ���_�ʒu
			m_LookAtPt,							// �����_
			m_UpVec								// ���������
		);
	}

	// �v���W�F�N�V�����s��
	{
		m_ProjMat = DirectX::XMMatrixPerspectiveFovLH(
			m_FieldOfView,															// ����p
			static_cast<float>(m_WindowSize.x) / static_cast<float>(m_WindowSize.y),// �A�X�y�N�g��
			m_NearClip,																// �N���b�s���O����:��
			m_FarClip																// �N���b�s���O����:��
		);
	}
}

/*!
	@fn		���_�ʒu�̃Q�b�^�[
	@brief	���_�ʒu�̎擾
	@return	���_�ʒu
*/
DirectX::XMFLOAT3 Camera::GetEyePt() const
{
	// VECTOR�^��FLOAT3�ɕϊ�
	DirectX::XMFLOAT3 ret;
	DirectX::XMStoreFloat3(&ret, m_EyePt);
	return ret;
}

/*!
	@fn		�����_�̃Q�b�^�[
	@brief	�����_�̎擾
	@return	�����_
*/
DirectX::XMFLOAT3 Camera::GetLookAtPt() const
{
	// VECTOR�^��FLOAT3�ɕϊ�
	DirectX::XMFLOAT3 ret;
	DirectX::XMStoreFloat3(&ret, m_LookAtPt);
	return ret;
}

/*!
	@fn			SetWindowSize
	@brief		�E�B���h�E�̃T�C�Y
	@param[in]	�ݒ肷��T�C�Y(x:���� y:�c��)
*/
void Camera::SetWindowSize(const DirectX::XMINT2 size)
{
	m_WindowSize = size;
}

/*!
	@fn			SetClipDistance
	@brief		�N���b�v�����̐ݒ�
	@param[in]	�N���b�v����:��
	@param[in]	�N���b�v����:��
*/
void Camera::SetClipDistance(const float nearClip, const float farClip)
{
	m_NearClip	= nearClip;
	m_FarClip	= farClip;
}

/*!
	@fn		SetOrthographic
	@brief	Orthographic(���ˉe�@)�̃v���W�F�N�V�����s��ɐ؂�ւ���
	@detail	���s�����Ȃ�2D�Q�[�������̐ݒ�
*/
void Camera::SetOrthographic()
{
	m_ProjMat = DirectX::XMMatrixOrthographicLH(
		static_cast<float>(m_WindowSize.x),	// �E�B���h�E�̉���
		static_cast<float>(m_WindowSize.y),	// �E�B���h�E�̏c��
		m_NearClip,							// �N���b�v����:��
		m_FarClip							// �N���b�v����:��
	);
}

/*!
	@fn		SetPerspective
	@brief	Perspective(�����}�@)�̃v���W�F�N�V�����s��ɐ؂�ւ���
	@detail	���s���̂���3D�Q�[�������̐ݒ�
*/
void Camera::SetPerspective()
{
	m_ProjMat = DirectX::XMMatrixPerspectiveFovLH(
		m_FieldOfView,															// ����p
		static_cast<float>(m_WindowSize.x) / static_cast<float>(m_WindowSize.y),// �A�X�y�N�g��
		m_NearClip,																// �N���b�s���O����:��
		m_FarClip																// �N���b�s���O����:��
	);
}

/*!
	@fn			SetEyePt
	@brief		���_�ʒu�̃Z�b�^�[
	@param[in]	�ݒ肷�鎋�_�ʒu
*/
void API::Camera::SetEyePt(DirectX::XMFLOAT3 eyePt)
{
	m_EyePt = DirectX::XMVectorSet(eyePt.x, eyePt.y, eyePt.z, 0);
}

