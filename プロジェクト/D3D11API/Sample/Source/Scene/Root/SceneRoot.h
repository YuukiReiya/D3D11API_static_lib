/*!
	@file	SceneRoot.h
	@date	2019/03/28
	@author	�ԏ� �G�P
	@brief	�V�[���̃��[�g
*/
#pragma once
#include "../Interface/IScene.h"
#include <memory>
#include <Singleton.h>

/*!
	@brief	���[�g�V�[��
*/
class SceneRoot
	:public Singleton<SceneRoot>, public IScene
{
public:
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

	/*!
		@fn			SetupNextScene
		@brief		�J�ڐ�V�[���̐ݒ�
		@param[in]	�J�ڐ�V�[���̃|�C���^
	*/
	void SetupNextScene(IScene* scene);

	/*!
		@fn			SetupNextScene
		@brief		�J�ڐ�V�[���̐ݒ�
		@param[in]	�J�ڐ�V�[���̃��j�[�N�|�C���^
	*/
	void SetupNextScene(std::unique_ptr<IScene> scene);

private:
	/*!
		@brief	�V���O���g���f�U�C���p�^�[���̃e���v���[�g�p��
	*/
	friend class Singleton<SceneRoot>;

	/*!
		@brief	�R���X�g���N�^
	*/
	explicit SceneRoot();

	/*!
		@fn		OnCallSetupNextScene
		@brief	�J�ڐ�V�[���̐ݒ��ɌĂ΂�鏈��
	*/
	void OnCallSetupNextScene();

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