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
		@fn			GetPosition
		@brief		���W�̎擾
		@detail		�C�����C���֐�
		@param[in]	�擾����Transform�̃|�C���^
		@return		���W
	*/
	static inline DirectX::XMFLOAT3 GetPosition(Transform* transform) {
		return DirectX::XMFLOAT3{ transform->m_TranslationMatrix._41, transform->m_TranslationMatrix._42, transform->m_TranslationMatrix._43 };
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
		@brief		���W�̐ݒ�
		@detail		�C�����C���֐�
		@param[in]	�Z�b�g����Transform�̃|�C���^
		@param[in]	�ݒ肷����W
	*/
	static inline void SetPosition(Transform* transform, DirectX::XMFLOAT3 pos)
	{
		transform->m_TranslationMatrix._41 = pos.x;
		transform->m_TranslationMatrix._42 = pos.y;
		transform->m_TranslationMatrix._43 = pos.z;
	}

	/*!
		@fn			SetPosition
		@brief		���W�̐ݒ�
		@detail		�C�����C���֐�
		@param[in]	�ݒ肷����W
	*/
	inline void SetPosition(DirectX::XMFLOAT3 pos) { SetPosition(this, pos); }

	/*!
		@fn			SetRotation
		@brief		��]�l�̐ݒ�
		@detail		�C�����C���֐�
		@param[in]	�Z�b�g����Transform�̃|�C���^
		@param[in]	�ݒ肷��p�x(�x�N�g��)
		@note		�e(x,y,z)����]
	*/
	static inline void SetRotation(Transform* transform, DirectX::XMVECTOR angle) {
		DirectX::XMStoreFloat4x4(&transform->m_RotationMatrix, DirectX::XMMatrixRotationRollPitchYawFromVector(angle));
	}

	/*!
		@fn			SetRotation
		@brief		��]�l�̐ݒ�
		@detail		�C�����C���֐�
		@param[in]	�ݒ肷��p�x(�x�N�g��)
		@note		�e(x,y,z)����]
	*/
	inline void SetRotation(DirectX::XMVECTOR angle) {
		SetRotation(this, angle);
	}

	/*!
		@fn			SetRotation
		@brief		��]�l�̐ݒ�
		@detail		�C�����C���֐�
		@param[in]	�Z�b�g����Transform�̃|�C���^
		@param[in]	�ݒ肷��roll(x������)
		@param[in]	�ݒ肷��pitch(y������)
		@param[in]	�ݒ肷��yaw(z������)
		@note		����]�ł͂Ȃ��������ɉ�]���邱�Ƃɒ���!!
	*/
	static inline void SetRotation(Transform*transform, float roll, float pitch, float yaw) {
		DirectX::XMStoreFloat4x4(&transform->m_RotationMatrix, DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll));
	}

	/*!
		@fn			SetRotation
		@brief		��]�l�̐ݒ�
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
		@brief		��]�l�̐ݒ�
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
		@brief		��]�l�̐ݒ�
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
		@fn			GetScale
		@brief		�傫���̎擾
		@detail		�C�����C���֐�
		@param[in]	�擾����Transform�̃|�C���^
		@return		�傫��
	*/
	static inline DirectX::XMFLOAT3 GetScale(Transform*transform) {

		//	�����֐�:�X�P�[���̋��ߕ����傫��
		auto scale = [](DirectX::XMFLOAT3 scale) ->float {
			float ret;
			auto length = DirectX::XMVector3Length(
				DirectX::XMLoadFloat3(&scale)
			);
			DirectX::XMStoreFloat(&ret, length);
			return ret;
		};

		auto& t = transform;
		DirectX::XMFLOAT3 ret{
			scale({t->m_ScalingMatrix._11,t->m_ScalingMatrix._12,t->m_ScalingMatrix._13}),
			scale({t->m_ScalingMatrix._21,t->m_ScalingMatrix._22,t->m_ScalingMatrix._23}),
			scale({t->m_ScalingMatrix._31,t->m_ScalingMatrix._32,t->m_ScalingMatrix._33}),
		};
		return ret;
	}

	/*!
		@fn			GetScale
		@brief		�傫���̎擾
		@detail		�C�����C���֐�
		@param[in]	�擾����Transform�̃|�C���^
		@return		�傫��
	*/
	inline DirectX::XMFLOAT3 GetScale() { return GetScale(this); }

	/*!
		@fn			SetScale
		@brief		�g�k�̐ݒ�
		@detail		�C�����C���֐�
		@param[in]	�Z�b�g����Transform�̃|�C���^
		@param[in]	�g�k
	*/
	static inline void SetScale(Transform*transform, DirectX::XMFLOAT3 scale)
	{
		transform->m_ScalingMatrix._11 = scale.x;
		transform->m_ScalingMatrix._22 = scale.y;
		transform->m_ScalingMatrix._33 = scale.z;
	}

	/*!
		@fn			SetScale
		@brief		�g�k�̐ݒ�
		@detail		�C�����C���֐�
		@param[in]	�g�k
	*/
	inline void SetScale(DirectX::XMFLOAT3 scale) { SetScale(this, scale); }

	/*!
		@fn			SetScale
		@brief		�g�k�̐ݒ�
		@detail		"x,y,z"�S�Ăɓ����l��ݒ肵�A�䗦��ύX
		@param[in]	�g�k
	*/
	inline void SetScale(float scale) { SetScale(this, { scale,scale ,scale }); }

	/*!
		@fn			GetWorldMatrix
		@brief		�s��̎擾
		@detail		�C�����C���֐�
		@param[in]	�擾����Transform�̃|�C���^
		@return		���[���h�s��

		@note		���[���h�s�� = �g�k�s�� * ��]�s�� * ���s�ړ��s��
	*/
	static inline DirectX::XMMATRIX GetWorldMatrix(Transform* transform) {
		return 
		DirectX::XMLoadFloat4x4(&transform->m_ScalingMatrix)
		*
		DirectX::XMLoadFloat4x4(&transform->m_RotationMatrix)
		*
		DirectX::XMLoadFloat4x4(&transform->m_TranslationMatrix);
	}

	/*!
		@fn			GetWorldMatrix
		@brief		���[���h�s��̎擾
		@detail		�C�����C���֐�
		@return		���[���h�s��

		@note		���[���h�s�� = �g�k�s�� * ��]�s�� * ���s�ړ��s��
	*/
	inline DirectX::XMMATRIX GetWorldMatrix() { return GetWorldMatrix(this); }

	/*!
		@fn			SetTranslationMatrix
		@brief		���s�ړ��s��̐ݒ�
		@detail		�C�����C���֐�
		@param[in]	�ݒ肷�镽�s�ړ��s��
	*/
	inline void SetTranslationMatrix(DirectX::XMFLOAT4X4 translationMatrix) { m_TranslationMatrix = translationMatrix; }

	/*!
		@fn			SetRotationMatrix
		@brief		��]�s��̐ݒ�
		@detail		�C�����C���֐�
		@param[in]	�ݒ肷��g�k�s��
	*/
	inline void SetRotationMatrix(DirectX::XMFLOAT4X4 rotationMatrix) { m_RotationMatrix = rotationMatrix; }

	/*!
		@fn			SetScalingMatrix
		@brief		�g�k�s��̐ݒ�
		@detail		�C�����C���֐�
		@param[in]	�ݒ肷��g�k�s��
	*/
	inline void SetScalingMatrix(DirectX::XMFLOAT4X4 scalingMatrix) { m_ScalingMatrix = scalingMatrix; }

	/*!
		@fn			GetTranslationMatrix
		@brief		���s�ړ��s��̎擾
		@detail		�C�����C���֐�
		@param[in]	�擾����Transform�̃|�C���^
		@return		���s�ړ��s��
	*/
	static inline DirectX::XMMATRIX GetTranslationMatrix(Transform*transform) { return DirectX::XMLoadFloat4x4(&transform->m_TranslationMatrix); }

	/*!
		@fn			GetTranslationMatrix
		@brief		���s�ړ��s��̎擾
		@detail		�C�����C���֐�
		@return		���s�ړ��s��
	*/
	inline DirectX::XMMATRIX GetTranslationMatrix() { return GetTranslationMatrix(this); }

	/*!
		@fn			GetRotationMatrix
		@brief		��]�s��̎擾
		@detail		�C�����C���֐�
		@param[in]	�擾����Transform�̃|�C���^
		@return		��]�s��
	*/
	static inline DirectX::XMMATRIX GetRotationMatrix(Transform*transform) { return DirectX::XMLoadFloat4x4(&transform->m_RotationMatrix); }

	/*!
		@fn			GetRotationMatrix
		@brief		��]�s��̎擾
		@detail		�C�����C���֐�
		@return		��]�s��
	*/
	inline DirectX::XMMATRIX GetRotationMatrix() { return GetRotationMatrix(this); }

	/*!
		@fn			GetScalingMatrix
		@brief		�g�k�s��̎擾
		@detail		�C�����C���֐�
		@param[in]	�擾����Transform�̃|�C���^
		@return		�g�k�s��
	*/
	static inline DirectX::XMMATRIX GetScalingMatrix(Transform*transform) { return  DirectX::XMLoadFloat4x4(&transform->m_ScalingMatrix); }

	/*!
		@fn			GetScalingMatrix
		@brief		�g�k�s��̎擾
		@detail		�C�����C���֐�
		@param[in]	�擾����Transform�̃|�C���^
		@return		�g�k�s��
	*/
	inline DirectX::XMMATRIX GetScalingMatrix() { return GetScalingMatrix(this); }

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
	//DirectX::XMFLOAT4X4 m_Matrix;

	/*!
		@var	m_TranslationMatrix
		@brief	���s�ړ��s��
		@note	XMMATRIX�̓����o�ŃC���X�^���X���o���Ȃ��̂�XMFLOAT4X4�ɍs���R�Â�
	*/
	DirectX::XMFLOAT4X4 m_TranslationMatrix;

	/*!
		@var	m_RotationMatrix
		@brief	��]�s��
		@note	XMMATRIX�̓����o�ŃC���X�^���X���o���Ȃ��̂�XMFLOAT4X4�ɍs���R�Â�
	*/
	DirectX::XMFLOAT4X4 m_RotationMatrix;

	/*!
		@var	m_ScalingMatrix
		@brief	�g�k�s��
		@note	XMMATRIX�̓����o�ŃC���X�^���X���o���Ȃ��̂�XMFLOAT4X4�ɍs���R�Â�
	*/
	DirectX::XMFLOAT4X4 m_ScalingMatrix;
};