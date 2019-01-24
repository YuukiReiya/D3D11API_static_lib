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
		@fn			イニシャライズ
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

	static const double		c_Pi;				/*!< 円周率π */
	static const float		c_FieldOfView;		/*!< 画角 */
	static const float		c_NearClip;			/*!< デフォルトクリッピング距離:近 */
	static const float		c_FarClip;			/*!< デフォルトクリッピング距離:遠 */
	static const DirectX::XMFLOAT3 c_UpVector;	/*!< デフォルトの上向きベクトル */

	/*!
		@fn		ビュー行列のゲッター
		@brief	ビュー行列の取得
		@return	ビュー行列
	*/
	DirectX::XMMATRIX GetViewMatrix()const { return m_ViewMat; }

	/*!
		@fn		プロジェクション行列のゲッター
		@brief	プロジェクション行列の取得
		@return	プロジェクション行列
	*/
	DirectX::XMMATRIX GetProjMatrix()const { return m_ProjMat; }

	/*!
		@fn		視点位置のゲッター
		@brief	視点位置の取得
		@return	視点位置
	*/
	DirectX::XMFLOAT3 GetEyePt()const;

	/*!
		@fn		注視点のゲッター
		@brief	注視点の取得
		@return	注視点
	*/
	DirectX::XMFLOAT3 GetLookAtPt()const;

	/*!
		@fn			FOVのセット
		@brief		視野角の設定
		@param[in]	FOVの設定値
	*/
	void SetFieldOfView(float fov)	{ m_FieldOfView = fov; }

	/*!
		@fn			近距離クリップのセット
		@brief		クリップ距離:近の設定
		@param[in]	近距離クリップの設定値
	*/
	void SetNearClip(float nearClip){ m_NearClip = nearClip; }

	/*!
		@fn			遠距離クリップのセット
		@brief		クリップ距離:遠の設定
		@param[in]	遠距離クリップの設定値
	*/
	void SetFarClip(float farClip)	{ m_FarClip = farClip; }

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
	float m_FieldOfView;			/*!< 視野角 */
	float m_NearClip;				/*!< デフォルトのクリッピング距離:近 */
	float m_FarClip;				/*!< デフォルトのクリッピング距離:遠 */
	DirectX::XMVECTOR m_EyePt;		/*!< 視点位置 */
	DirectX::XMVECTOR m_LookAtPt;	/*!< 注視点 */
	DirectX::XMVECTOR m_UpVec;		/*!< 上向きベクトル */
	DirectX::XMMATRIX m_ViewMat;	/*!< ビュー行列 */
	DirectX::XMMATRIX m_ProjMat;	/*!< プロジェクション行列 */
};

