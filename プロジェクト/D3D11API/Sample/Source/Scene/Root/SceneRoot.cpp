/*!
	@file	SceneRoot.cpp
	@date	2019/03/28
	@author	�ԏ� �G�P
	@brief	�V�[���̃��[�g
*/
#include <MyGame.h>
#include "SceneRoot.h"

/*!
	@brief	�R���X�g���N�^
*/
SceneRoot::SceneRoot()
	:m_pCurrentScene(nullptr)
	,m_pNextScene(nullptr)
{}

/*!
	@brief		�����t���R���X�g���N�^
	@param[in]	�����V�[���̃|�C���^
*/
SceneRoot::SceneRoot(IScene * pInitializeScene)
	:m_pCurrentScene(pInitializeScene)
	,m_pNextScene(nullptr)
{}

/*!
	@brief	�f�X�g���N�^
*/
SceneRoot::~SceneRoot()
{
	m_pCurrentScene.reset();
	m_pNextScene.reset();
}

/*!
	@fn		Initialize
	@brief	����������
*/
void SceneRoot::Initialize()
{
	m_pCurrentScene->Initialize();
}

/*!
	@fn		Finalize
	@brief	�j������
*/
void SceneRoot::Finalize()
{
	m_pCurrentScene->Initialize();
}

/*!
	@fn		Update
	@brief	�X�V����
*/
void SceneRoot::Update()
{
	//	�V�[���̑J��
	OnCallSetupCurrentScene();

	//	�V�[���̍X�V
	m_pCurrentScene->Update();

	//	�V�[���̔j��
	OnCallShutdownCurrentScene();
}

/*!
	@fn		Render
	@brief	�`�揈��
*/
void SceneRoot::Render()
{
	m_pCurrentScene->Render();
}

/*!
	@fn		OnCallSetupCurrentScene
	@brief	���ݐݒ肳��Ă���V�[���̑J��
*/
void SceneRoot::OnCallSetupCurrentScene()
{
	if (m_pNextScene == nullptr) { return; }

	//	�X�}�[�g�|�C���^�̃��[�u
	m_pCurrentScene = std::move(m_pNextScene);

	//	�J�����g�V�[���̏�����
	m_pCurrentScene->Initialize();

	//	�J�ڐ�V�[���̏�����
	m_pNextScene.reset();
	m_pNextScene = nullptr;
}

/*!
	@fn		OnCallShutdownCurrentScene
	@brief	���ݐݒ肳��Ă���V�[���̔j������
*/
void SceneRoot::OnCallShutdownCurrentScene()
{
	if (m_pNextScene == nullptr) { return; }

	//	�J�����g�V�[���̔j��
	m_pCurrentScene->Finalize();

	m_pCurrentScene.reset();
	m_pCurrentScene = nullptr;
}
