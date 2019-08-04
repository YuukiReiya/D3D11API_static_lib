/*!
	@file	SampleScene.h
	@date	2019/03/28
	@author	�ԏ� �G�P
	@brief	�T���v���V�[��
	@detail	���C�u�����̃e�X�g���s���T���v���p�̃V�[��
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
		@brief	�R���X�g���N�^
	*/
	explicit SampleScene();

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