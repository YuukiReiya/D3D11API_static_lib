/*
	@file	Transform.h
	@date	2019/04/17
	@author	�ԏ� �G�P
	@brief	�g�����X�t�H�[���N���X
*/
#pragma once
#include <DirectXMath.h>
#include <memory>

class Transform
{
public:
	/*!
		@brief	�R���X�g���N�^
	*/
	explicit Transform();

	/*!
		@brief	�f�X�g���N�^
	*/
	~Transform();

	/*!
		@fn			Initialize
		@brief		�s��̏�����
		@detail		�P�ʍs���ݒ�
		@param[in]	����������Transform�̃|�C���^
	*/
	static inline void Initialize(Transform* transform) { transform->m_Matrix = c_IdentityMatrix; }

	/*!
		@fn			Initialize
		@brief		�s��̏�����
		@detail		�P�ʍs���ݒ�
	*/
	inline void Initialize() { Initialize(this); }

	/*!
		@fn			GetPosition
		@brief		���W�̎擾
		@detail		�C�����C���֐�
		@param[in]	�擾����Transform�̃|�C���^
		@return		���W
	*/
	static inline DirectX::XMFLOAT3 GetPosition(Transform* transform) {
		return DirectX::XMFLOAT3{ transform->m_Matrix._41, transform->m_Matrix._42, transform->m_Matrix._43 };
	}

	/*!
		@fn			GetPosition
		@brief		���W�̎擾
		@detail		�C�����C���֐�
		@return		���W
	*/
	inline DirectX::XMFLOAT3 GetPosition() { return GetPosition(this); }

	/*!
		@fn			SetPosition
		@brief		���W�̃Z�b�^�[
		@detail		�C�����C���֐�
		@param[in]	�Z�b�g����Transform�̃|�C���^
		@param[in]	�ݒ肷����W
	*/
	static inline void SetPosition(Transform* transform, DirectX::XMFLOAT3 pos)
	{
		transform->m_Matrix._41 = pos.x;
		transform->m_Matrix._42 = pos.y;
		transform->m_Matrix._43 = pos.z;
	}

	/*!
		@fn			SetPosition
		@brief		���W�̃Z�b�^�[
		@detail		�C�����C���֐�
		@param[in]	�Z�b�g����Transform�̃|�C���^
		@param[in]	�ݒ肷����W
	*/
	inline void SetPosition(DirectX::XMFLOAT3 pos) { SetPosition(this, pos); }

	/*!
		@fn			GetMatrix
		@brief		�s��̎擾
		@detail		�C�����C���֐�
		@param[in]	�擾����Transform�̃|�C���^
		@return		���[���h�s��
	*/
	static inline DirectX::XMMATRIX GetMatrix(Transform* transform) { return DirectX::XMLoadFloat4x4(&transform->m_Matrix); }

	/*!
		@fn			GetMatrix
		@brief		�s��̎擾
		@detail		�C�����C���֐�
		@return		���[���h�s��
	*/
	inline DirectX::XMMATRIX GetMatrix() { return GetMatrix(this); }

	/*!
		@var	c_IdentityMatrix
		@brief	�P�ʍs��
		@NOTE	XMMatrixIdentity()���g���ĕϊ��ł��邪
				�ϊ������ޕ��R�[�h�����G�����邵�A�^���Ⴄ�̂Œ萔�Ƃ��Ď��������B
	*/
	static constexpr DirectX::XMFLOAT4X4 c_IdentityMatrix = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};
private:

	/*!
		@var	m_Matrix
		@brief	�s��
		@NOTE	XMMATRIX�̓����o�ŃC���X�^���X���o���Ȃ��̂�XMFLOAT4X4�ɍs���R�Â�
	*/
	DirectX::XMFLOAT4X4 m_Matrix;
};