/*
	@file	Keyboard.cpp
	@brief	Windows�L�[�{�[�h����
	@date	2019/03/29
	@detail	�L�[�R�[�h�m�F�p�����N:http://www.azaelia.net/factory/vk.html
	@author	�ԏ� �G�P
*/
#include "stdafx.h"
#include "Keyboard.h"
#include <algorithm>
#include <cctype>

/*!
	@var	m_CurrentBuffer
	@brief	���݂̓��̓o�b�t�@
			�ÓI�����o�̐錾
*/
Keyboard::Buffer Keyboard::m_CurrentBuffer;

/*!
	@var	m_PrevBuffer
	@brief	1�t���[���O�̓��̓o�b�t�@
			�ÓI�����o�̐錾
*/

Keyboard::Buffer Keyboard::m_PrevBuffer;

/*!
	@brief	�R���X�g���N�^
*/
Keyboard::Keyboard()
{
	ClearBuffer();
}

/*!
	@brief	�f�X�g���N�^
*/
Keyboard::~Keyboard()
{
	ClearBuffer();
}

/*!
	@fn		UpdateBuffer
	@brief	���̓o�b�t�@�̍X�V
	@detail	�����͔�����擾���鉺�L�֐��̌Ăяo���O�ɍs���Ă�������
*/
void Keyboard::UpdateBuffer()
{
	//	�Â��o�b�t�@�̍X�V
	m_PrevBuffer = m_CurrentBuffer;

	//	�V�����o�b�t�@�̍X�V
	GetKeyboardState(m_CurrentBuffer.values);
}

/*!
	@fn			GetButton
	@brief		�L�[�̉�������
	@param[in]	���肷��L�[
	@return		true:������Ă���  false:������Ă��Ȃ�
*/
bool Keyboard::GetButton(unsigned char key)
{
	//	�啶���Ə������Œl���Ⴄ�̂�
	//	�A���t�@�x�b�g�͑啶���ɕϊ����Ă�����
	auto value = ToUpperAlpha(key);
	return m_CurrentBuffer.values[value] & 0x80;
}

/*!
	@fn			GetButtonDown
	@brief		�L�[�̉������u�Ԃ̉�������
	@param[in]	���肷��L�[
	@return		true:�����ꂽ  false:�����ꂽ�u�Ԃł͂Ȃ�
*/
bool Keyboard::GetButtonDown(unsigned char key)
{
	//	�啶���Ə������Œl���Ⴄ�̂�
	//	�A���t�@�x�b�g�͑啶���ɕϊ����Ă�����
	auto value = ToUpperAlpha(key);
	return (m_CurrentBuffer.values[value] ^ m_PrevBuffer.values[value]) & m_CurrentBuffer.values[value];
}

/*!
	@fn			GetButtonUp
	@brief		�L�[�̗������u�Ԃ̔���
	@param[in]	���肷��L�[
	@return		true:�����ꂽ  false:�����ꂽ�u�Ԃł͂Ȃ�
*/
bool Keyboard::GetButtonUp(unsigned char key)
{
	//	�啶���Ə������Œl���Ⴄ�̂�
	//	�A���t�@�x�b�g�͑啶���ɕϊ����Ă�����
	auto value = ToUpperAlpha(key);
	return (m_CurrentBuffer.values[value] ^ m_PrevBuffer.values[value]) & m_PrevBuffer.values[value];
}

/*!
	@fn		ClearBuffer
	@brief	���̓o�b�t�@���N���A����
	@detail	STL��fill���g���ē��̓o�b�t�@��NULL�ŏ���������
*/
void Keyboard::ClearBuffer()
{
	std::fill(m_CurrentBuffer.values, m_CurrentBuffer.values + c_KeyNum, NULL);
	std::fill(m_PrevBuffer.values, m_PrevBuffer.values + c_KeyNum, NULL);
}

/*!
	@fn			ToUpperAlpha
	@brief		�啶���A���t�@�x�b�g�֕ϊ�
	@detail		�A���t�@�x�b�g����Ȃ�����������������̂܂ܕԂ�
	@param[in]	�ϊ�����
	@return		�ϊ��㕶��
*/
unsigned char Keyboard::ToUpperAlpha(unsigned char value)
{
	//	�A���t�@�x�b�g�łȂ���Α��X�ɕԋp
	if (!std::isalpha(value)) { return value; }

	//	�啶���ɂ��ĕԋp
	return toupper(value);
}