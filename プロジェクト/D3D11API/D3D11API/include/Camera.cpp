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
	@brief	usingディレクティブ
	@using	API
*/
using namespace API;

/*!
	@brief	コンストラクタ
*/
Camera::Camera()
{
	// デフォルト設定スコープ
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
void Camera::Initialize(DirectX::XMINT2 windowSize, DirectX::XMFLOAT3 eyePt, DirectX::XMFLOAT3 lookPt, DirectX::XMFLOAT3 upVec)
{
	// ウィンドウサイズ
	{
		m_WindowSize = windowSize;
	}

	// FLOAT3をVECTOR型に変換
	{
		m_EyePt = DirectX::XMVectorSet(eyePt.x, eyePt.y, eyePt.z, 0.0f);
		m_LookAtPt = DirectX::XMVectorSet(lookPt.x, lookPt.y, lookPt.z, 0.0f);
		m_UpVec = DirectX::XMVectorSet(upVec.x, upVec.y, upVec.z, 0.0f);
	}

	// ビュー行列
	{
		m_ViewMat = DirectX::XMMatrixLookAtLH(
			m_EyePt,							// 視点位置
			m_LookAtPt,							// 注視点
			m_UpVec								// 上向き方向
		);
	}

	// プロジェクション行列
	{
		m_ProjMat = DirectX::XMMatrixPerspectiveFovLH(
			m_FieldOfView,															// 視野角
			static_cast<float>(m_WindowSize.x) / static_cast<float>(m_WindowSize.y),// アスペクト比
			m_NearClip,																// クリッピング距離:近
			m_FarClip																// クリッピング距離:遠
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
	// VECTOR型をFLOAT3に変換
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
	// VECTOR型をFLOAT3に変換
	DirectX::XMFLOAT3 ret;
	DirectX::XMStoreFloat3(&ret, m_LookAtPt);
	return ret;
}

/*!
	@fn			SetWindowSize
	@brief		ウィンドウのサイズ
	@param[in]	設定するサイズ(x:横幅 y:縦幅)
*/
void Camera::SetWindowSize(const DirectX::XMINT2 size)
{
	m_WindowSize = size;
}

/*!
	@fn			SetClipDistance
	@brief		クリップ距離の設定
	@param[in]	クリップ距離:近
	@param[in]	クリップ距離:遠
*/
void Camera::SetClipDistance(const float nearClip, const float farClip)
{
	m_NearClip	= nearClip;
	m_FarClip	= farClip;
}

/*!
	@fn		SetOrthographic
	@brief	Orthographic(正射影法)のプロジェクション行列に切り替える
	@detail	奥行きがない2Dゲーム向けの設定
*/
void Camera::SetOrthographic()
{
	m_ProjMat = DirectX::XMMatrixOrthographicLH(
		static_cast<float>(m_WindowSize.x),	// ウィンドウの横幅
		static_cast<float>(m_WindowSize.y),	// ウィンドウの縦幅
		m_NearClip,							// クリップ距離:近
		m_FarClip							// クリップ距離:遠
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
		m_FieldOfView,															// 視野角
		static_cast<float>(m_WindowSize.x) / static_cast<float>(m_WindowSize.y),// アスペクト比
		m_NearClip,																// クリッピング距離:近
		m_FarClip																// クリッピング距離:遠
	);
}

/*!
	@fn			SetEyePt
	@brief		視点位置のセッター
	@param[in]	設定する視点位置
*/
void API::Camera::SetEyePt(DirectX::XMFLOAT3 eyePt)
{
	m_EyePt = DirectX::XMVectorSet(eyePt.x, eyePt.y, eyePt.z, 0);
}

