/*!
	@file	SampleScene.h
	@date	2019/03/28
	@author	番場 宥輝
	@brief	サンプルシーン
	@detail	ライブラリのテストを行うサンプル用のシーン
*/
#pragma once
#include "../Interface/IScene.h"
#include <AnimationMesh.h>
#include <Mesh.h>
#include <AbstractShader.h>
#include <Material.h>

class SampleScene 
	:public IScene
{
public:
	/*!
		@brief	コンストラクタ
	*/
	explicit SampleScene();

	/*!
		@fn		Initialize
		@brief	初期化処理
*/
	void IScene::Initialize();

	/*!
		@fn		Finalize
		@brief	破棄処理
	*/
	void IScene::Finalize();

	/*!
		@fn		Update
		@brief	更新処理
	*/
	void IScene::Update();

	/*!
		@fn		Render
		@brief	描画処理
	*/
	void IScene::Render();

private:
#if 1
#define ANIMATION_MESH
	std::shared_ptr<API::AnimationMesh>m_pRender;
#elif 0
#define STATIC_MESH
	std::shared_ptr<API::Mesh>m_pRender;
	std::shared_ptr<API::Material>m_pMaterial;
	std::shared_ptr<D3D11::Graphic::AbstractShader>m_pShader;
#endif // 1

};