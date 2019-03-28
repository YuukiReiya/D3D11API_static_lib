/*!
	@file	SceneRoot.h
	@date	2019/03/28
	@author	�ԏ� �G�P
	@brief	�V�[���̃��[�g
*/
#pragma once
#include "../Interface/IScene.h"
#include <memory>

/*!
	@brief	���[�g�V�[��
*/
class SceneRoot
	:public IScene
{
	/*!
		@brief	�R���X�g���N�^
	*/
	explicit SceneRoot();

	/*!
		@brief		�����t���R���X�g���N�^
		@param[in]	�����V�[���̃|�C���^
	*/
	explicit SceneRoot(IScene* pInitializeScene);

	/*!
		@brief	�f�X�g���N�^
	*/
	~SceneRoot();


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

	void SetupNextScene(IScene* scene);

private:

	/*!
		@fn		OnCallSetupCurrentScene
		@brief	���ݐݒ肳��Ă���V�[���̑J��	
	*/
	void OnCallSetupCurrentScene();

	/*!
		@fn		OnCallShutdownCurrentScene
		@brief	���ݐݒ肳��Ă���V�[���̔j������
	*/
	void OnCallShutdownCurrentScene();

	/*!
		@var	m_pCurrentScene
		@brief	���ݐݒ肵�Ă���V�[��
	*/
	std::unique_ptr<IScene> m_pCurrentScene;
	
	/*!
		@var	m_pNextScene
		@brief	�J�ڐ�̃V�[��
	*/
	std::unique_ptr<IScene> m_pNextScene;
};