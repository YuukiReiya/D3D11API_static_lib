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
		@brief	����������
	*/
	void IScene::Initialize();

	/*!
		@fn		Finalize
		@brief	�j������
	*/
	void IScene::Finalize();

	/*!
		@fn		Update
		@brief	�X�V����
	*/
	void IScene::Update();

	/*!
		@fn		Render
		@brief	�`�揈��
	*/
	void IScene::Render();

private:
	std::shared_ptr<API::AnimationMesh>m_pMesh;
};
