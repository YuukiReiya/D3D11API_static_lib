/*
	@file	Transform.h
	@date	2019/04/02
	@author	�ԏ� �G�P
	@brief	�g�����X�t�H�[���N���X
*/
#pragma once
#include <DirectXMath.h>

class Transform
{
public:
	Transform();
	~Transform();

	static inline DirectX::XMFLOAT3 GetPosition(DirectX::XMMATRIX matrix) 
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMStoreFloat3(&pos, matrix.r[3]);
		return pos;
	}

	static inline void SetPosition(DirectX::XMMATRIX*matrix, DirectX::XMFLOAT3 pos) 
	{
		//DirectX::XMVECTOR v; DirectX::XMStoreFloat3(pos)
	}

private:
	DirectX::XMMATRIX m_Matrix;
};