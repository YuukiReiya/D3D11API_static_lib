/*!
	@file	SampleScene.h
	@date	2019/03/28
	@author	�ԏ� �G�P
	@brief	�T���v���V�[��
*/
#pragma once
#include "../Interface/IScene.h"

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

};