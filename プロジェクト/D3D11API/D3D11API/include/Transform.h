/*
	@file	Transform.h
	@date	2019/04/17
	@author	番場 宥輝
	@brief	トランスフォームクラス
*/
#pragma once
#include <DirectXMath.h>
#include <memory>

class Transform
{
public:
	/*!
		@brief	コンストラクタ
	*/
	explicit Transform();

	/*!
		@brief	デストラクタ
	*/
	~Transform();

	/*!
		@fn			Initialize
		@brief		行列の初期化
		@detail		単位行列を設定
		@param[in]	初期化するTransformのポインタ
	*/
	static inline void Initialize(Transform* transform) { transform->m_Matrix = c_IdentityMatrix; }

	/*!
		@fn			Initialize
		@brief		行列の初期化
		@detail		単位行列を設定
	*/
	inline void Initialize() { Initialize(this); }

	/*!
		@fn			GetPosition
		@brief		座標の取得
		@detail		インライン関数
		@param[in]	取得するTransformのポインタ
		@return		座標
	*/
	static inline DirectX::XMFLOAT3 GetPosition(Transform* transform) {
		return DirectX::XMFLOAT3{ transform->m_Matrix._41, transform->m_Matrix._42, transform->m_Matrix._43 };
	}

	/*!
		@fn			GetPosition
		@brief		座標の取得
		@detail		インライン関数
		@return		座標
	*/
	inline DirectX::XMFLOAT3 GetPosition() { return GetPosition(this); }

	/*!
		@fn			SetPosition
		@brief		座標のセッター
		@detail		インライン関数
		@param[in]	セットするTransformのポインタ
		@param[in]	設定する座標
	*/
	static inline void SetPosition(Transform* transform, DirectX::XMFLOAT3 pos)
	{
		transform->m_Matrix._41 = pos.x;
		transform->m_Matrix._42 = pos.y;
		transform->m_Matrix._43 = pos.z;
	}

	/*!
		@fn			SetPosition
		@brief		座標のセッター
		@detail		インライン関数
		@param[in]	セットするTransformのポインタ
		@param[in]	設定する座標
	*/
	inline void SetPosition(DirectX::XMFLOAT3 pos) { SetPosition(this, pos); }

	/*!
		@fn			GetMatrix
		@brief		行列の取得
		@detail		インライン関数
		@param[in]	取得するTransformのポインタ
		@return		ワールド行列
	*/
	static inline DirectX::XMMATRIX GetMatrix(Transform* transform) { return DirectX::XMLoadFloat4x4(&transform->m_Matrix); }

	/*!
		@fn			GetMatrix
		@brief		行列の取得
		@detail		インライン関数
		@return		ワールド行列
	*/
	inline DirectX::XMMATRIX GetMatrix() { return GetMatrix(this); }

	/*!
		@var	c_IdentityMatrix
		@brief	単位行列
		@NOTE	XMMatrixIdentity()を使って変換できるが
				変換を挟む分コードが複雑化するし、型が違うので定数として実装した。
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
		@brief	行列
		@NOTE	XMMATRIXはメンバでインスタンス化出来ないのでXMFLOAT4X4に行列を紐づけ
	*/
	DirectX::XMFLOAT4X4 m_Matrix;
};