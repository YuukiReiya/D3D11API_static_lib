/*!
	@file	SceneRoot.cpp
	@date	2019/03/28
	@author	�ԏ� �G�P
	@brief	�V�[���̃��[�g
*/
#include <MyGame.h>
#include <MemoryLeaks.h>
#include "SceneRoot.h"
#include "../Scene/SampleScene.h"

/*!
	@brief	using�f�B���N�e�B�u
	@using	std
*/
//using namespace std;

/*!
	@brief	�R���X�g���N�^
*/
SceneRoot::SceneRoot()
	:m_pCurrentScene(nullptr)
	,m_pNextScene(nullptr)
{}

/*!
	@brief	�f�X�g���N�^
*/
SceneRoot::~SceneRoot()
{
	m_pCurrentScene.reset(nullptr);
	m_pNextScene.reset(nullptr);
}

/*!
	@fn		Initialize
	@brief	����������
*/
void SceneRoot::Initialize()
{
	//	�����V�[���̐ݒ�
	m_pCurrentScene = std::unique_ptr<IScene>(new SampleScene());
	m_pCurrentScene->Initialize();
}

/*!
	@fn		Finalize
	@brief	�j������
*/
void SceneRoot::Finalize()
{
	m_pCurrentScene->Finalize();
}

/*!
	@fn		Update
	@brief	�X�V����
*/
void SceneRoot::Update()
{
	//	�V�[���̍X�V
	m_pCurrentScene->Update();

	//	�V�[���̑J��
	OnCallSetupNextScene();
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
	@fn			SetupNextScene
	@brief		�J�ڐ�V�[���̐ݒ�
	@param[in]	�J�ڐ�V�[��
*/
void SceneRoot::SetupNextScene(IScene * scene)
{
	m_pNextScene = std::unique_ptr<IScene>(scene);
}

/*!
	@fn			SetupNextScene
	@brief		�J�ڐ�V�[���̐ݒ�
	@param[in]	�J�ڐ�V�[���̃��j�[�N�|�C���^
*/
void SceneRoot::SetupNextScene(std::unique_ptr<IScene> scene)
{
	m_pNextScene = move(scene);
}

/*!
	@fn		OnCallSetupNextScene
	@brief	�J�ڐ�V�[���̐ݒ��ɌĂ΂�鏈��
*/
void SceneRoot::OnCallSetupNextScene()
{
	if (m_pNextScene == nullptr) { return; }

	//	�J�����g�V�[���̔j��
	m_pCurrentScene->Finalize();
	m_pCurrentScene.reset(nullptr);

	//	�X�}�[�g�|�C���^�̃��[�u
	m_pCurrentScene = std::move(m_pNextScene);

	//	�J�����g�V�[���̏�����
	m_pCurrentScene->Initialize();

	//	�J�ڐ�V�[���̏�����
	m_pNextScene.reset(nullptr);
}