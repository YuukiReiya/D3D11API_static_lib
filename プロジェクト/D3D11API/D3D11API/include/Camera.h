/*
	@file	Camera.h
	@date	2018/04/22
	@author	番場 宥輝
	@brief	カメラ
	@detail	シングルトン
*/
#pragma once
#include <DirectXMath.h>
#include "Singleton.h"

/****************************************/
/*			カメラの描画域(v)			*/
/*		NearClip <= v < FarClip			*/
/*	Ex)									*/
/*	NearClip = 0.1f, FarClip = 100.0f	*/
/*	描画されるオブジェクトはカメラの視	*/
/*	点+NearClipからカメラの視点+FarClip */
/*	の範囲に収まっているものを描画する	*/
/****************************************/

class Camera
	:public Singleton<Camera>
{
public:

	/*!
		@brief	デストラクタ
	*/
	~Camera();

	/*!
		@fn			Initialize
		@brief		初期化
		@param[in]	視点
		@param[in]	注視点
		@param[in]	上向きベクトル
	*/
	void Initialize(
		DirectX::XMFLOAT3 eyePt,
		DirectX::XMFLOAT3 lookPt = { 0.0f,0.0f,0.0f },
		DirectX::XMFLOAT3 upVec = c_UpVector
	);

	/*!
		@var	c_Pi
		@brief	円周率π ※D3DX_PIと同値
	*/
	static constexpr double	c_Pi = 3.14159265358979323846;

	/*!
		@var	c_FieldOfView
		@brief	画角
		@value	static_cast<float>(Camera::c_Pi) / 4.0f 
				デフォルトの視野角(45度)
	*/
	static constexpr float	c_FieldOfView = static_cast<float>(Camera::c_Pi) / 4.0f;

	/*!
		@var	c_NearClip
		@brief	デフォルトクリッピング距離:近
	*/
	static constexpr float	c_NearClip = 0.1f;

	/*!
		@var	c_FarClip
		@brief	デフォルトクリッピング距離:遠
		@value	0.1f
	*/
	static constexpr float	c_FarClip = 0.1f;

	/*!
		@var	c_UpVector
		@brief	デフォルトの上向きベクトル
	*/
	static constexpr DirectX::XMFLOAT3 c_UpVector = { 0,1,0 };

	/*!
		@fn		GetViewMatrix
		@brief	ビュー行列の取得
		@return	ビュー行列
	*/
	DirectX::XMMATRIX GetViewMatrix()const { return m_ViewMat; }

	/*!
		@fn		GetProjMatrix
		@brief	プロジェクション行列の取得
		@return	プロジェクション行列
	*/
	DirectX::XMMATRIX GetProjMatrix()const { return m_ProjMat; }

	/*!
		@fn		GetEyePt
		@brief	視点位置の取得
		@return	視点位置
	*/
	DirectX::XMFLOAT3 GetEyePt()const;

	/*!
		@fn		GetLookAtPt
		@brief	注視点の取得
		@return	注視点
	*/
	DirectX::XMFLOAT3 GetLookAtPt()const;

	/*!
		@fn			SetWindowSize
		@brief		ウィンドウのサイズ
		@param[in]	設定するサイズ(x:横幅 y:縦幅)
	*/
	void SetWindowSize(const DirectX::XMINT2 size);

	/*!
		@fn			SetFieldOfView
		@brief		視野角の設定
		@param[in]	FOVの設定値
	*/
	void SetFieldOfView(float fov)	{ m_FieldOfView = fov; }

	/*!
		@fn			SetNearClip
		@brief		クリップ距離:近の設定
		@param[in]	近距離クリップの設定値
	*/
	void SetNearClip(float nearClip){ m_NearClip = nearClip; }

	/*!
		@fn			SetFarClip
		@brief		クリップ距離:遠の設定
		@param[in]	遠距離クリップの設定値
	*/
	void SetFarClip(float farClip)	{ m_FarClip = farClip; }
	
	/*!
		@fn		SetOrthographic
		@brief	Orthographic(正射影法)のプロジェクション行列に切り替える
		@detail	奥行きがない2Dゲーム向けの設定
	*/
	void SetOrthographic();

	/*!
		@fn		SetPerspective
		@brief	Perspective(透視図法)のプロジェクション行列に切り替える
		@detail	奥行きのある3Dゲーム向けの設定
	*/
	void SetPerspective();

private:
	/*!
		@brief	コンストラクタ
	*/
	Camera();

	/*!
		@brief	シングルトンデザインパターンのテンプレート継承
	*/
	friend class Singleton<Camera>;

	/*! 変数 */
	float m_FieldOfView;			// 視野角
	float m_NearClip;				// デフォルトのクリッピング距離:近
	float m_FarClip;				// デフォルトのクリッピング距離:遠
	DirectX::XMINT2 m_WindowSize;	// ウィンドウのサイズ
	DirectX::XMVECTOR m_EyePt;		// 視点位置
	DirectX::XMVECTOR m_LookAtPt;	// 注視点
	DirectX::XMVECTOR m_UpVec;		// 上向きベクトル
	DirectX::XMMATRIX m_ViewMat;	// ビュー行列
	DirectX::XMMATRIX m_ProjMat;	// プロジェクション行列
};

