/*
	@file	IScene.h
	@date	2019/03/28
	@author	�ԏ� �G�P
	@brief	�V�[���̃C���^�[�t�F�C�X
*/
#pragma once

/*!
	@brief	�V�[���̃C���^�[�t�F�C�X
*/
__interface IScene
{
	/*!
		@fn		Initialize
		@brief	����������
	*/
	void Initialize();

	/*!
		@fn		Finalize
		@brief	�j������
	*/
	void Finalize();

	/*!
		@fn		Update
		@brief	�X�V����
	*/
	void Update();

	/*!
		@fn		Render
		@brief	�`�揈��
	*/
	void Render();
};