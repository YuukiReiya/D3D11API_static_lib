/*
	@file	IRender.h
	@date	2019/03/13
	@author	�ԏ� �G�P
	@brief	�`��̒��ۃN���X
	@detail	�X�v���C�g�⃂�f���Ȃǂ̕`��I�u�W�F�N�g�̊��N���X
*/
#pragma once

/*!
	@namespace	API
	@brief		API���O��ԂɊ܂߂�
*/
namespace API
{
	class IRender abstract 
	{
	public :
		/*!
			@brief	�R���X�g���N�^
		*/
		IRender();

		/*!
			@brief	�f�X�g���N�^
		*/
		~IRender();

		/*!
			@fn		Render
			@brief	���z�`��֐�
			@detail	�������z�֐��Ƃ��Ď���
		*/
		virtual void Render()abstract;
	};
}