/*
	@file	Transform.h
	@date	2019/04/17
	@author	�ԏ� �G�P
	@brief	�g�����X�t�H�[���N���X
	@detail	�Q�l:https://www.fluentcpp.com/2017/10/27/function-aliases-cpp/
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
		@param[in]	�ݒ肷����W
	*/
	inline void SetPosition(DirectX::XMFLOAT3 pos) { SetPosition(this, pos); }

	/*!
		@fn			SetRotation
		@brief		��]�l�̃Z�b�^�[
		@detail		�C�����C���֐�
		@param[in]	�Z�b�g����Transform�̃|�C���^
		@param[in]	�ݒ肷��p�x(�x�N�g��)
		@note		�e(x,y,z)����]
	*/
	static inline void SetRotation(Transform* transform, DirectX::XMVECTOR angle) {
		auto pos = transform->GetPosition();
		DirectX::XMStoreFloat4x4(&transform->m_Matrix, DirectX::XMMatrixRotationRollPitchYawFromVector(angle));
		transform->SetPosition(pos);
	}

	/*!
		@fn			SetRotation
		@brief		��]�l�̃Z�b�^�[
		@detail		�C�����C���֐�
		@param[in]	�ݒ肷��p�x(�x�N�g��)
		@note		�e(x,y,z)����]
	*/
	inline void SetRotation(DirectX::XMVECTOR angle) {
		SetRotation(this, angle);
	}

	/*!
		@fn			SetRotation
		@brief		��]�l�̃Z�b�^�[
		@detail		�C�����C���֐�
		@param[in]	�Z�b�g����Transform�̃|�C���^
		@param[in]	�ݒ肷��roll(x������)
		@param[in]	�ݒ肷��pitch(y������)
		@param[in]	�ݒ肷��yaw(z������)
		@note		����]�ł͂Ȃ��������ɉ�]���邱�Ƃɒ���!!
	*/
	static inline void SetRotation(Transform*transform, float roll, float pitch, float yaw) {
		auto pos = transform->GetPosition();
		DirectX::XMStoreFloat4x4(&transform->m_Matrix, DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll));
		transform->SetPosition(pos);
	}

	/*!
		@fn			SetRotation
		@brief		��]�l�̃Z�b�^�[
		@detail		�C�����C���֐�
		@param[in]	�ݒ肷��roll(x������)
		@param[in]	�ݒ肷��pitch(y������)
		@param[in]	�ݒ肷��yaw(z������)
		@note		����]�ł͂Ȃ��������ɉ�]���邱�Ƃɒ���!!
	*/
	inline void SetRotation(float roll, float pitch, float yaw) {
		SetRotation(this, roll, pitch, yaw);
	}

	/*!
		@fn			SetRotation
		@brief		��]�l�̃Z�b�^�[
		@detail		�C�����C���֐�
		@param[in]	�Z�b�g����Transform�̃|�C���^
		@param[in]	�ݒ肷��R(oll)P(itch)Y(aw)
		@note		R = x
					P = y
					Y = z	�ɑΉ�
	*/
	static inline void SetRotation(Transform*transform, DirectX::XMFLOAT3 rpy) {
		SetRotation(transform, rpy.x, rpy.y, rpy.z);
	}

	/*!
		@fn			SetRotation
		@brief		��]�l�̃Z�b�^�[
		@detail		�C�����C���֐�
		@param[in]	�ݒ肷��R(oll)P(itch)Y(aw)
		@note		R = x
					P = y
					Y = z	�ɑΉ�
	*/
	inline  void SetRotation(DirectX::XMFLOAT3 rpy) {
		SetRotation(this, rpy);
	}


	/*!
		TODO:�֐��|�C���^���g�����֐��G�C���A�X�錾
		NOTE:SetPosition�̊֐����������̂�Unity���[�U�[�ɂȂ��ݐ[��"Rotate"�֐��Ƃ��ĕʖ���錾�����������B
			 �֐��|�C���^���g�����֐��G�C���A�X���ƐÓI�֐��݂̂����Ή����Ă��Ȃ��̂ƁA�֐��̃I�[�o�[���[�h�ɑΉ����Ă��Ȃ�
	*/
	//static inline void(*const Rotate)(Transform*, float,float,float) = SetRotation;
	//inline void Rotate(float roll, float pitch, float yaw) {
	//	void(Transform::*RotateF3)(float, float, float) = &Transform::SetRotation;
	//	(this->*RotateF3)(roll, pitch, yaw);
	//}
	//static inline void(*const Rotate)(Transform*, DirectX::XMFLOAT3) = SetRotation;

	/*!	
		HACK:��L�̖����������邽�߂̃I�[�o�[���[�h(one-liners)
	*/

	/*!
		@fn			Rotate
		@brief		��]
		@detail		SetRotation�̕ʖ�(one-liners)
		@param[in]	�Z�b�g����Transform�̃|�C���^
		@param[in]	�p�x�x�N�g��
	*/
	static inline void Rotate(Transform*transform, DirectX::XMVECTOR angle) {
		SetRotation(transform, angle);
	}

	/*!
		@fn			Rotate
		@brief		��]
		@detail		SetRotation�̕ʖ�(one-liners)
		@param[in]	�p�x�x�N�g��
	*/
	inline void Rotate(DirectX::XMVECTOR angle) {
		SetRotation(this, angle);
	}

	/*!
		@fn			Rotate
		@brief		��]
		@detail		�C�����C���֐�
		@param[in]	�Z�b�g����Transform�̃|�C���^
		@param[in]	�ݒ肷��roll(x������)
		@param[in]	�ݒ肷��pitch(y������)
		@param[in]	�ݒ肷��yaw(z������)
		@note		����]�ł͂Ȃ��������ɉ�]���邱�Ƃɒ���!!
	*/
	static inline void Rotate(Transform*transform, float roll, float pitch, float yaw) {
		SetRotation(transform, roll, pitch, yaw);
	}
	/*!
		@fn			Rotate
		@brief		��]
		@detail		�C�����C���֐�
		@param[in]	�Z�b�g����Transform�̃|�C���^
		@param[in]	�ݒ肷��roll(x������)
		@param[in]	�ݒ肷��pitch(y������)
		@param[in]	�ݒ肷��yaw(z������)
		@note		����]�ł͂Ȃ��������ɉ�]���邱�Ƃɒ���!!
	*/
	inline void Rotate(float roll, float pitch, float yaw) {
		SetRotation(this, roll, pitch, yaw);
	}

	/*!
		@fn			Rotate
		@brief		��]
		@detail		�C�����C���֐�
		@param[in]	�Z�b�g����Transform�̃|�C���^
		@param[in]	�ݒ肷��R(oll)P(itch)Y(aw)
		@note		R = x
					P = y
					Y = z	�ɑΉ�
	*/
	static inline void Rotate(Transform*transform, DirectX::XMFLOAT3 rpy) {
		SetRotation(transform, rpy.x, rpy.y, rpy.z);
	}

	/*!
		@fn			Rotate
		@brief		��]
		@detail		�C�����C���֐�
		@param[in]	�ݒ肷��R(oll)P(itch)Y(aw)
		@note		R = x
					P = y
					Y = z	�ɑΉ�
	*/
	inline  void Rotate(DirectX::XMFLOAT3 rpy) {
		SetRotation(this, rpy);
	}

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
	DirectX::XMFLOAT4X4 m_Matrix;
private:

	/*!
		@var	m_Matrix
		@brief	�s��
		@NOTE	XMMATRIX�̓����o�ŃC���X�^���X���o���Ȃ��̂�XMFLOAT4X4�ɍs���R�Â�
	*/
};