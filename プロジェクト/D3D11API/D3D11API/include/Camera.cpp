/*
	@file	Camera.cpp
	@date	2018/04/22
	@author	番場 宥輝
	@brief	カメラ
	@detail	シングルトン
*/
#include "stdafx.h"
#include "Camera.h"
#include "Direct3D11.h"

/*!
	@var	定数宣言
*/
const double Camera::			c_Pi			= 3.14159265358979323846;					/*!< 円周率π ※D3DX_PIと同値 */
const float Camera::			c_FieldOfView	= static_cast<float>(Camera::c_Pi) / 4.0f;	/*!< デフォルトの視野角(45度) */
const float Camera::			c_NearClip		= 0.1f;										/*!< デフォルトのクリッピング距離:近 */
const float Camera::			c_FarClip		= 100.0f;									/*!< デフォルトのクリッピング距離:遠 */
const DirectX::XMFLOAT3 Camera::c_UpVector		= { 0.0f,1.0f,0.0f };						/*!< デフォルトの上向きベクトル */

/*!
	@brief	コンストラクタ
*/
Camera::Camera()
{
	/*! デフォルト設定スコープ */
	{
		m_FieldOfView	= c_FieldOfView;
		m_NearClip		= c_NearClip;
		m_FarClip		= c_FarClip;
		m_EyePt			= DirectX::XMVectorSet(0.0f, 0.0f, -10.0f, 0.0f);
		m_LookAtPt		= DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		m_UpVec			= DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	}

	/*! 行列の生成 */
	Initialize(GetEyePt());
}

/*!
	@brief	デストラクタ
*/
Camera::~Camera()
{
}

/*!
	@brief		初期化
	@detail		引数からビュー行列とプロジェクション行列を生成
	@param[in]	視点位置	
	@param[in]	注視点
	@param[in]	上向きベクトル
*/
void Camera::Initialize(DirectX::XMFLOAT3 eyePt, DirectX::XMFLOAT3 lookPt, DirectX::XMFLOAT3 upVec) 
{
	/*! FLOAT3をVECTOR型に変換 */
	{
		m_EyePt = DirectX::XMVectorSet(eyePt.x, eyePt.y, eyePt.z, 0.0f);
		m_LookAtPt = DirectX::XMVectorSet(lookPt.x, lookPt.y, lookPt.z, 0.0f);
		m_UpVec = DirectX::XMVectorSet(upVec.x, upVec.y, upVec.z, 0.0f);
	}

	/*! ビュー行列 */
	{
		m_ViewMat = DirectX::XMMatrixLookAtLH(
			m_EyePt,							/*!< 視点位置 */
			m_LookAtPt,							/*!< 注視点 */
			m_UpVec								/*!< 上向き方向 */
		);
	}

	/*! プロジェクション行列 */
	{
		m_ProjMat = DirectX::XMMatrixPerspectiveFovLH(
			m_FieldOfView,															/*!< 視野角 */
			static_cast<float>(c_WindowWidth) / static_cast<float>(c_WindowHeight),	/*!< アスペクト比 */
			m_NearClip,																/*!< クリッピング距離:近 */
			m_FarClip																/*!< クリッピング距離:遠 */
		);
	}
}

/*!
	@fn		視点位置のゲッター
	@brief	視点位置の取得
	@return	視点位置
*/
DirectX::XMFLOAT3 Camera::GetEyePt() const
{
	/*! VECTOR型をFLOAT3に変換 */
	DirectX::XMFLOAT3 ret;
	DirectX::XMStoreFloat3(&ret, m_EyePt);
	return ret;
}

/*!
	@fn		注視点のゲッター
	@brief	注視点の取得
	@return	注視点
*/
DirectX::XMFLOAT3 Camera::GetLookAtPt() const
{
	/*! VECTOR型をFLOAT3に変換 */
	DirectX::XMFLOAT3 ret;
	DirectX::XMStoreFloat3(&ret, m_LookAtPt);
	return ret;
}

/*!
	@fn		SetOrthographic
	@brief	Orthographic(正射影法)のプロジェクション行列に切り替える
	@detail	奥行きがない2Dゲーム向けの設定
*/
void Camera::SetOrthographic()
{
	m_ProjMat = DirectX::XMMatrixOrthographicLH(
		static_cast<float>(c_WindowWidth),
		static_cast<float>(c_WindowHeight),
		m_NearClip,
		m_FarClip
	);
}

/*!
	@fn		SetPerspective
	@brief	Perspective(透視図法)のプロジェクション行列に切り替える
	@detail	奥行きのある3Dゲーム向けの設定
*/
void Camera::SetPerspective()
{
	m_ProjMat = DirectX::XMMatrixPerspectiveFovLH(
		m_FieldOfView,															/*!< 視野角 */
		static_cast<float>(c_WindowWidth) / static_cast<float>(c_WindowHeight),	/*!< アスペクト比 */
		m_NearClip,																/*!< クリッピング距離:近 */
		m_FarClip																/*!< クリッピング距離:遠 */
	);
}

