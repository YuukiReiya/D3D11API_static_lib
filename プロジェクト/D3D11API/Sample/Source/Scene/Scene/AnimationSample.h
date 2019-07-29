#pragma once
#include "../Interface/IScene.h"
#include <Transform.h>
#include <AnimationMesh.h>
#include "BillboardSample.h"

class AnimationSample
	:public IScene
{
public:
	explicit AnimationSample();
	~AnimationSample();

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

	void Reset();
private:
	std::shared_ptr<API::AnimationMesh>m_pMesh;
	DirectX::XMFLOAT3 m_MeshRot;
	int animNo;
	int animFrame;
	int elapsedFrame;
	bool autoAnim;
};
