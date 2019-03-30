/*
	@file	Keyboard.h
	@brief	Windows�L�[�{�[�h����
	@date	2019/03/29
	@detail	�L�[�R�[�h�m�F�p�����N:http://www.azaelia.net/factory/vk.html
	@author	�ԏ� �G�P
*/
#pragma once
#include<Windows.h>


/*!
	@brief	�L�[�{�[�h���̓N���X
*/
class Keyboard
{
public:
	/*!
		@brief	�f�X�g���N�^
	*/
	~Keyboard();

	/*! �����L�[ */

	/*!
		@var	c_Up
		@brief	�� ����(PgUp)�L�[
	*/
	static constexpr unsigned char c_Up		= 0x26;

	/*!
		@var	c_Down
		@brief	�� �����(PgDn)�L�[
	*/
	static constexpr unsigned char c_Down	= 0x28;
	/*!
		@var	c_Left
		@brief	�� �����(Home)�L�[
	*/
	static constexpr unsigned char c_Left	= 0x25;
	/*!
		@var	c_Right
		@brief	�� �E���(End)�L�[
	*/
	static constexpr unsigned char c_Right	= 0x27;

	/*! ����L�[ */

	/*!
		@var	c_Space
		@brief	�X�y�[�X�L�[
	*/
	static constexpr unsigned char c_Space	= 0x20;
	
	/*!
		@var	c_Enter
		@brief	�G���^�[�L�[
	*/
	static constexpr unsigned char c_Enter	= 0x0D;

	/*!
		@var	c_Shift
		@brief	�V�t�g�L�[
	*/
	static constexpr unsigned char c_Shift	= 0x10;
	/*!
		@var	c_Back
		@brief	�o�b�N�X�y�[�X�L�[
	*/
	static constexpr unsigned char c_Back	= 0x08;
	/*!
		@var	c_Tab
		@brief	�^�u�L�[
	*/
	static constexpr unsigned char c_Tab	= 0x09;
	/*!
		@var	c_Delete
		@brief	�f���[�g�L�[
	*/
	static constexpr unsigned char c_Delete	= 0x2E;

	/*!
		@fn		UpdateBuffer
		@brief	���̓o�b�t�@�̍X�V
		@detail	�����͔�����擾���鉺�L�֐��̌Ăяo���O�ɍs���Ă�������
	*/
	static void UpdateBuffer();

	/*!
		@fn			GetButton
		@brief		�L�[�̉�������
		@param[in]	���肷��L�[
		@return		true:������Ă���  false:������Ă��Ȃ�
	*/
	static bool GetButton(unsigned char key);

	/*!
		@fn			GetButtonDown
		@brief		�L�[�̉������u�Ԃ̉�������
		@param[in]	���肷��L�[
		@return		true:�����ꂽ  false:�����ꂽ�u�Ԃł͂Ȃ�
	*/
	static bool GetButtonDown(unsigned char key);

	/*!
		@fn			GetButtonUp
		@brief		�L�[�̗������u�Ԃ̔���
		@param[in]	���肷��L�[
		@return		true:�����ꂽ  false:�����ꂽ�u�Ԃł͂Ȃ�
	*/
	static bool GetButtonUp(unsigned char key);

	/*!
		@fn		ClearBuffer
		@brief	���̓o�b�t�@���N���A����
		@detail	STL��fill���g���ē��̓o�b�t�@��NULL�ŏ���������
	*/
	static void ClearBuffer();
private:
	/*!
		@brief	�R���X�g���N�^
	*/
	explicit Keyboard();

	/*!
		@fn			ToUpperAlpha
		@brief		�啶���A���t�@�x�b�g�֕ϊ�
		@detail		�A���t�@�x�b�g����Ȃ�����������������̂܂ܕԂ�
		@param[in]	�ϊ�����
		@return		�ϊ��㕶��
	*/
	static unsigned char ToUpperAlpha(unsigned char value);

	/*!
		@var	c_KeyNum
		@brief	�L�[�̐�
	*/
	static constexpr unsigned int c_KeyNum = 256;

	/*!
		@struct	Buffer
		@brief	���̓o�b�t�@��ۑ�����\����
	*/
	struct Buffer
	{
		unsigned char values[c_KeyNum];
	};

	/*!
		@var	m_CurrentBuffer
		@brief	���݂̓��̓o�b�t�@
	*/
	static Buffer m_CurrentBuffer;

	/*!
		@var	m_PrevBuffer
		@brief	1�t���[���O�̓��̓o�b�t�@
	*/
	static Buffer m_PrevBuffer;
};