#pragma once
#include "../Interface/IScene.h"
#include <Transform.h>
#include <AnimationMesh.h>

class AnimationSample
	:public IScene
{
public:
	explicit AnimationSample();
	~AnimationSample();

	/*!
		@fn		Initialize
		@brief	‰Šú‰»ˆ—
	*/
	void IScene::Initialize();

	/*!
		@fn		Finalize
		@brief	”jŠüˆ—
	*/
	void IScene::Finalize();

	/*!
		@fn		Update
		@brief	XVˆ—
	*/
	void IScene::Update();

	/*!
		@fn		Render
		@brief	•`‰æˆ—
	*/
	void IScene::Render();

private:
	std::shared_ptr<API::AnimationMesh>m_pMesh;
};
