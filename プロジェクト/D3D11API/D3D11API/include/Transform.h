/*
	@file	Transform.h
	@date	2019/04/17
	@author	番場 宥輝
	@brief	トランスフォームクラス
	@detail	参考:https://www.fluentcpp.com/2017/10/27/function-aliases-cpp/
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
		@param[in]	設定する座標
	*/
	inline void SetPosition(DirectX::XMFLOAT3 pos) { SetPosition(this, pos); }

	/*!
		@fn			SetRotation
		@brief		回転値のセッター
		@detail		インライン関数
		@param[in]	セットするTransformのポインタ
		@param[in]	設定する角度(ベクトル)
		@note		各(x,y,z)軸回転
	*/
	static inline void SetRotation(Transform* transform, DirectX::XMVECTOR angle) {
		auto pos = transform->GetPosition();
		DirectX::XMStoreFloat4x4(&transform->m_Matrix, DirectX::XMMatrixRotationRollPitchYawFromVector(angle));
		transform->SetPosition(pos);
	}

	/*!
		@fn			SetRotation
		@brief		回転値のセッター
		@detail		インライン関数
		@param[in]	設定する角度(ベクトル)
		@note		各(x,y,z)軸回転
	*/
	inline void SetRotation(DirectX::XMVECTOR angle) {
		SetRotation(this, angle);
	}

	/*!
		@fn			SetRotation
		@brief		回転値のセッター
		@detail		インライン関数
		@param[in]	セットするTransformのポインタ
		@param[in]	設定するroll(x軸方向)
		@param[in]	設定するpitch(y軸方向)
		@param[in]	設定するyaw(z軸方向)
		@note		軸回転ではなく軸方向に回転することに注意!!
	*/
	static inline void SetRotation(Transform*transform, float roll, float pitch, float yaw) {
		auto pos = transform->GetPosition();
		DirectX::XMStoreFloat4x4(&transform->m_Matrix, DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll));
		transform->SetPosition(pos);
	}

	/*!
		@fn			SetRotation
		@brief		回転値のセッター
		@detail		インライン関数
		@param[in]	設定するroll(x軸方向)
		@param[in]	設定するpitch(y軸方向)
		@param[in]	設定するyaw(z軸方向)
		@note		軸回転ではなく軸方向に回転することに注意!!
	*/
	inline void SetRotation(float roll, float pitch, float yaw) {
		SetRotation(this, roll, pitch, yaw);
	}

	/*!
		@fn			SetRotation
		@brief		回転値のセッター
		@detail		インライン関数
		@param[in]	セットするTransformのポインタ
		@param[in]	設定するR(oll)P(itch)Y(aw)
		@note		R = x
					P = y
					Y = z	に対応
	*/
	static inline void SetRotation(Transform*transform, DirectX::XMFLOAT3 rpy) {
		SetRotation(transform, rpy.x, rpy.y, rpy.z);
	}

	/*!
		@fn			SetRotation
		@brief		回転値のセッター
		@detail		インライン関数
		@param[in]	設定するR(oll)P(itch)Y(aw)
		@note		R = x
					P = y
					Y = z	に対応
	*/
	inline  void SetRotation(DirectX::XMFLOAT3 rpy) {
		SetRotation(this, rpy);
	}


	/*!
		TODO:関数ポインタを使った関数エイリアス宣言
		NOTE:SetPositionの関数名が長いのとUnityユーザーになじみ深い"Rotate"関数として別名を宣言したかった。
			 関数ポインタを使った関数エイリアスだと静的関数のみしか対応していないのと、関数のオーバーロードに対応していない
	*/
	//static inline void(*const Rotate)(Transform*, float,float,float) = SetRotation;
	//inline void Rotate(float roll, float pitch, float yaw) {
	//	void(Transform::*RotateF3)(float, float, float) = &Transform::SetRotation;
	//	(this->*RotateF3)(roll, pitch, yaw);
	//}
	//static inline void(*const Rotate)(Transform*, DirectX::XMFLOAT3) = SetRotation;

	/*!	
		HACK:上記の問題を解決するためのオーバーロード(one-liners)
	*/

	/*!
		@fn			Rotate
		@brief		回転
		@detail		SetRotationの別名(one-liners)
		@param[in]	セットするTransformのポインタ
		@param[in]	角度ベクトル
	*/
	static inline void Rotate(Transform*transform, DirectX::XMVECTOR angle) {
		SetRotation(transform, angle);
	}

	/*!
		@fn			Rotate
		@brief		回転
		@detail		SetRotationの別名(one-liners)
		@param[in]	角度ベクトル
	*/
	inline void Rotate(DirectX::XMVECTOR angle) {
		SetRotation(this, angle);
	}

	/*!
		@fn			Rotate
		@brief		回転
		@detail		インライン関数
		@param[in]	セットするTransformのポインタ
		@param[in]	設定するroll(x軸方向)
		@param[in]	設定するpitch(y軸方向)
		@param[in]	設定するyaw(z軸方向)
		@note		軸回転ではなく軸方向に回転することに注意!!
	*/
	static inline void Rotate(Transform*transform, float roll, float pitch, float yaw) {
		SetRotation(transform, roll, pitch, yaw);
	}
	/*!
		@fn			Rotate
		@brief		回転
		@detail		インライン関数
		@param[in]	セットするTransformのポインタ
		@param[in]	設定するroll(x軸方向)
		@param[in]	設定するpitch(y軸方向)
		@param[in]	設定するyaw(z軸方向)
		@note		軸回転ではなく軸方向に回転することに注意!!
	*/
	inline void Rotate(float roll, float pitch, float yaw) {
		SetRotation(this, roll, pitch, yaw);
	}

	/*!
		@fn			Rotate
		@brief		回転
		@detail		インライン関数
		@param[in]	セットするTransformのポインタ
		@param[in]	設定するR(oll)P(itch)Y(aw)
		@note		R = x
					P = y
					Y = z	に対応
	*/
	static inline void Rotate(Transform*transform, DirectX::XMFLOAT3 rpy) {
		SetRotation(transform, rpy.x, rpy.y, rpy.z);
	}

	/*!
		@fn			Rotate
		@brief		回転
		@detail		インライン関数
		@param[in]	設定するR(oll)P(itch)Y(aw)
		@note		R = x
					P = y
					Y = z	に対応
	*/
	inline  void Rotate(DirectX::XMFLOAT3 rpy) {
		SetRotation(this, rpy);
	}

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
	DirectX::XMFLOAT4X4 m_Matrix;
private:

	/*!
		@var	m_Matrix
		@brief	行列
		@NOTE	XMMATRIXはメンバでインスタンス化出来ないのでXMFLOAT4X4に行列を紐づけ
	*/
};