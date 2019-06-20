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
		@fn			GetPosition
		@brief		座標の取得
		@detail		インライン関数
		@param[in]	取得するTransformのポインタ
		@return		座標
	*/
	static inline DirectX::XMFLOAT3 GetPosition(Transform* transform) {
		return DirectX::XMFLOAT3{ transform->m_TranslationMatrix._41, transform->m_TranslationMatrix._42, transform->m_TranslationMatrix._43 };
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
		@brief		座標の設定
		@detail		インライン関数
		@param[in]	セットするTransformのポインタ
		@param[in]	設定する座標
	*/
	static inline void SetPosition(Transform* transform, DirectX::XMFLOAT3 pos)
	{
		transform->m_TranslationMatrix._41 = pos.x;
		transform->m_TranslationMatrix._42 = pos.y;
		transform->m_TranslationMatrix._43 = pos.z;
	}

	/*!
		@fn			SetPosition
		@brief		座標の設定
		@detail		インライン関数
		@param[in]	設定する座標
	*/
	inline void SetPosition(DirectX::XMFLOAT3 pos) { SetPosition(this, pos); }

	/*!
		@fn			SetRotation
		@brief		回転値の設定
		@detail		インライン関数
		@param[in]	セットするTransformのポインタ
		@param[in]	設定する角度(ベクトル)
		@note		各(x,y,z)軸回転
	*/
	static inline void SetRotation(Transform* transform, DirectX::XMVECTOR angle) {
		DirectX::XMStoreFloat4x4(&transform->m_RotationMatrix, DirectX::XMMatrixRotationRollPitchYawFromVector(angle));
	}

	/*!
		@fn			SetRotation
		@brief		回転値の設定
		@detail		インライン関数
		@param[in]	設定する角度(ベクトル)
		@note		各(x,y,z)軸回転
	*/
	inline void SetRotation(DirectX::XMVECTOR angle) {
		SetRotation(this, angle);
	}

	/*!
		@fn			SetRotation
		@brief		回転値の設定
		@detail		インライン関数
		@param[in]	セットするTransformのポインタ
		@param[in]	設定するroll(x軸方向)
		@param[in]	設定するpitch(y軸方向)
		@param[in]	設定するyaw(z軸方向)
		@note		軸回転ではなく軸方向に回転することに注意!!
	*/
	static inline void SetRotation(Transform*transform, float roll, float pitch, float yaw) {
		DirectX::XMStoreFloat4x4(&transform->m_RotationMatrix, DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll));
	}

	/*!
		@fn			SetRotation
		@brief		回転値の設定
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
		@brief		回転値の設定
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
		@brief		回転値の設定
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
		@fn			GetScale
		@brief		大きさの取得
		@detail		インライン関数
		@param[in]	取得するTransformのポインタ
		@return		大きさ
	*/
	static inline DirectX::XMFLOAT3 GetScale(Transform*transform) {

		//	匿名関数:スケールの求め方が大きさ
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
		@brief		大きさの取得
		@detail		インライン関数
		@param[in]	取得するTransformのポインタ
		@return		大きさ
	*/
	inline DirectX::XMFLOAT3 GetScale() { return GetScale(this); }

	/*!
		@fn			SetScale
		@brief		拡縮の設定
		@detail		インライン関数
		@param[in]	セットするTransformのポインタ
		@param[in]	拡縮
	*/
	static inline void SetScale(Transform*transform, DirectX::XMFLOAT3 scale)
	{
		transform->m_ScalingMatrix._11 = scale.x;
		transform->m_ScalingMatrix._22 = scale.y;
		transform->m_ScalingMatrix._33 = scale.z;
	}

	/*!
		@fn			SetScale
		@brief		拡縮の設定
		@detail		インライン関数
		@param[in]	拡縮
	*/
	inline void SetScale(DirectX::XMFLOAT3 scale) { SetScale(this, scale); }

	/*!
		@fn			SetScale
		@brief		拡縮の設定
		@detail		"x,y,z"全てに同じ値を設定し、比率を変更
		@param[in]	拡縮
	*/
	inline void SetScale(float scale) { SetScale(this, { scale,scale ,scale }); }

	/*!
		@fn			GetWorldMatrix
		@brief		行列の取得
		@detail		インライン関数
		@param[in]	取得するTransformのポインタ
		@return		ワールド行列

		@note		ワールド行列 = 拡縮行列 * 回転行列 * 平行移動行列
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
		@brief		ワールド行列の取得
		@detail		インライン関数
		@return		ワールド行列

		@note		ワールド行列 = 拡縮行列 * 回転行列 * 平行移動行列
	*/
	inline DirectX::XMMATRIX GetWorldMatrix() { return GetWorldMatrix(this); }

	/*!
		@fn			SetTranslationMatrix
		@brief		平行移動行列の設定
		@detail		インライン関数
		@param[in]	設定する平行移動行列
	*/
	inline void SetTranslationMatrix(DirectX::XMFLOAT4X4 translationMatrix) { m_TranslationMatrix = translationMatrix; }

	/*!
		@fn			SetRotationMatrix
		@brief		回転行列の設定
		@detail		インライン関数
		@param[in]	設定する拡縮行列
	*/
	inline void SetRotationMatrix(DirectX::XMFLOAT4X4 rotationMatrix) { m_RotationMatrix = rotationMatrix; }

	/*!
		@fn			SetScalingMatrix
		@brief		拡縮行列の設定
		@detail		インライン関数
		@param[in]	設定する拡縮行列
	*/
	inline void SetScalingMatrix(DirectX::XMFLOAT4X4 scalingMatrix) { m_ScalingMatrix = scalingMatrix; }

	/*!
		@fn			GetTranslationMatrix
		@brief		平行移動行列の取得
		@detail		インライン関数
		@param[in]	取得するTransformのポインタ
		@return		平行移動行列
	*/
	static inline DirectX::XMMATRIX GetTranslationMatrix(Transform*transform) { return DirectX::XMLoadFloat4x4(&transform->m_TranslationMatrix); }

	/*!
		@fn			GetTranslationMatrix
		@brief		平行移動行列の取得
		@detail		インライン関数
		@return		平行移動行列
	*/
	inline DirectX::XMMATRIX GetTranslationMatrix() { return GetTranslationMatrix(this); }

	/*!
		@fn			GetRotationMatrix
		@brief		回転行列の取得
		@detail		インライン関数
		@param[in]	取得するTransformのポインタ
		@return		回転行列
	*/
	static inline DirectX::XMMATRIX GetRotationMatrix(Transform*transform) { return DirectX::XMLoadFloat4x4(&transform->m_RotationMatrix); }

	/*!
		@fn			GetRotationMatrix
		@brief		回転行列の取得
		@detail		インライン関数
		@return		回転行列
	*/
	inline DirectX::XMMATRIX GetRotationMatrix() { return GetRotationMatrix(this); }

	/*!
		@fn			GetScalingMatrix
		@brief		拡縮行列の取得
		@detail		インライン関数
		@param[in]	取得するTransformのポインタ
		@return		拡縮行列
	*/
	static inline DirectX::XMMATRIX GetScalingMatrix(Transform*transform) { return  DirectX::XMLoadFloat4x4(&transform->m_ScalingMatrix); }

	/*!
		@fn			GetScalingMatrix
		@brief		拡縮行列の取得
		@detail		インライン関数
		@param[in]	取得するTransformのポインタ
		@return		拡縮行列
	*/
	inline DirectX::XMMATRIX GetScalingMatrix() { return GetScalingMatrix(this); }

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
	//DirectX::XMFLOAT4X4 m_Matrix;

	/*!
		@var	m_TranslationMatrix
		@brief	平行移動行列
		@note	XMMATRIXはメンバでインスタンス化出来ないのでXMFLOAT4X4に行列を紐づけ
	*/
	DirectX::XMFLOAT4X4 m_TranslationMatrix;

	/*!
		@var	m_RotationMatrix
		@brief	回転行列
		@note	XMMATRIXはメンバでインスタンス化出来ないのでXMFLOAT4X4に行列を紐づけ
	*/
	DirectX::XMFLOAT4X4 m_RotationMatrix;

	/*!
		@var	m_ScalingMatrix
		@brief	拡縮行列
		@note	XMMATRIXはメンバでインスタンス化出来ないのでXMFLOAT4X4に行列を紐づけ
	*/
	DirectX::XMFLOAT4X4 m_ScalingMatrix;
};