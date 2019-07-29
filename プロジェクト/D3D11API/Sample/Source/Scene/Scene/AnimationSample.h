#pragma once
#include "../Interface/IScene.h"
#include <Transform.h>
#include <Mesh.h>

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
	std::shared_ptr<API::Mesh>m_pMesh;
	std::shared_ptr<API::Material>m_pMaterial;
	std::shared_ptr<D3D11::Graphic::AbstractShader>m_pShader;
};
