/*!
	@file	TransitionTestScene.h
	@date	2019/03/29
	@author	�ԏ� �G�P
	@brief	�V�[���J�ڂ̃e�X�g�p�V�[��
*/
#pragma once
#include "../Interface/IScene.h"

class TransitionTestScene
	:public IScene
{
public:
	/*!
		@brief	�R���X�g���N�^
	*/
	explicit TransitionTestScene();

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

};