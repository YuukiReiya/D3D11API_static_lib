/*
	@file	GamePad.cpp
	@brief	�Q�[���p�b�h����
	@date	2019/03/29
	@detail	XInput��p��������
			XInput�Q�l:		http://www.geocities.jp/gameprogrammingunit/xinput/index.html
			�񋓑̂̒���:	http://d.hatena.ne.jp/mclh46/20100627/1277603928
	@author	�ԏ� �G�P
*/
#include "stdafx.h"
#include "GamePad.h"
#include <math.h>

/*!
	@brief	using�f�B���N�e�B�u
	@using	KeyCode
*/
using namespace KeyCode;

/*!
	@brief	�R���X�g���N�^
*/
GamePad::GamePad()
	:m_JoyStickInputDead(c_JoyStickInputDead)
	,m_JoyStickInputPrecision(c_JoyStickInputPrecision)
{
	SecureZeroMemory(this, sizeof(this));
	index = Index::One;
}

/*!
	@brief		�����t���R���X�g���N�^
	@param[in]	�ݒ肷��R���g���[���[�ԍ�
*/
GamePad::GamePad(Index && index)
	:m_JoyStickInputDead(c_JoyStickInputDead)
	,m_JoyStickInputPrecision(c_JoyStickInputPrecision)
{
	SecureZeroMemory(this, sizeof(this));
	this->index = index;
}

/*!
	@brief	�f�X�g���N�^
*/
GamePad::~GamePad()
{
	SetVibration(0);
	// XInput�𖳌��ɂ���j(�S�ẴR���g���[���[�Ɉˑ�)
	//XInputEnable(false);
}

/*!
	@fn		Update
	@brief	�X�V����
	@detail	���̓o�b�t�@�̍X�V
*/
void GamePad::Update()
{
	old = now;

	if (XInputGetState(index, &now) == ERROR_SUCCESS) {}//	TODO:���炩����̃G���[����
}

/*!
	@fn			GetButton
	@brief		�{�^���������Ă�Ԃ̎擾
	@param[in]	���肷��{�^���̃L�[�R�[�h
	@return		�{�^���̉������
*/
bool GamePad::GetButton(Button button)
{
	return now.Gamepad.wButtons&ConvertKeyButtonCode(Button(button));
}

/*!
	@fn			GetArrow
	@brief		�\���L�[�������Ă���Ԃ̎擾
	@param[in]	�������
	@return		1:�� -1:�� 0:������Ă��Ȃ�
*/
int GamePad::GetArrow(KeyCode::Arrow arrow)
{
	auto input = ConvertKeyArrowCode(Arrow(arrow));

	// ���̒l
	if ((now.Gamepad.wButtons&input.x)==1) {
		return 1;
	}
	// ���̒l
	if ((now.Gamepad.wButtons&input.y)==1) {
		return -1;
	}

	return 0;
}

/*!
	@fn			GetButtonDown
	@brief		�{�^�����������u�Ԃ̎擾
	@param[in]	���肷��{�^���̃L�[�R�[�h
	@return		�{�^���̉������
*/
bool GamePad::GetButtonDown(KeyCode::Button button)
{
	return ((now.Gamepad.wButtons&ConvertKeyButtonCode(Button(button))) 
		^
		(old.Gamepad.wButtons&ConvertKeyButtonCode(Button(button))))
		&
		(now.Gamepad.wButtons&ConvertKeyButtonCode(Button(button)));
}

/*!
	@fn			GetArrowDwon
	@brief		�\���L�[���������u�Ԃ̎擾
	@param[in]	�������
	@return		1:�� -1:�� 0:������Ă��Ȃ�
*/
int GamePad::GetArrowDown(KeyCode::Arrow arrow)
{
	auto input = ConvertKeyArrowCode(Arrow(arrow));

	// ���̒l
	if ((now.Gamepad.wButtons&input.x)
		^
		(old.Gamepad.wButtons&input.x)
		&
		(now.Gamepad.wButtons&input.x)
		) {
		return 1;
	}

	// ���̒l
	if ((now.Gamepad.wButtons&input.y)
		^
		(old.Gamepad.wButtons&input.y)
		&
		(now.Gamepad.wButtons&input.y)
		) {
		return -1;
	}

	return 0;
}

/*!
	@fn			GetButtonUp
	@brief		�{�^���������ꂽ�u�Ԃ̎擾
	@param[in]	���肷��{�^���̃L�[�R�[�h
	@return		�{�^���̉������
*/
bool GamePad::GetButtonUp(KeyCode::Button button)
{
	return ((now.Gamepad.wButtons&ConvertKeyButtonCode(Button(button)))
		^
		(old.Gamepad.wButtons&ConvertKeyButtonCode(Button(button))))
		&
		(old.Gamepad.wButtons&ConvertKeyButtonCode(Button(button)));
}

/*!
	@fn			GetArrowUp
	@brief		�\���L�[�𗣂����u�Ԃ̎擾
	@param[in]	�������
	@return		1:�� -1:�� 0:������Ă��Ȃ�
*/
int GamePad::GetArrowUp(KeyCode::Arrow arrow)
{
	auto input = ConvertKeyArrowCode(Arrow(arrow));

	// ���̒l
	if ((now.Gamepad.wButtons&input.x)
		^
		(old.Gamepad.wButtons&input.x)
		&
		(old.Gamepad.wButtons&input.x)
		) {
		return 1;
	}

	// ���̒l
	if ((now.Gamepad.wButtons&input.y)
		^
		(old.Gamepad.wButtons&input.y)
		&
		(old.Gamepad.wButtons&input.y)
		) {
		return -1;
	}

	return 0;
}

/*!
	@fn			GetJoyStick
	@brief		�W���C�X�e�B�b�N�̓��͎擾
	@param[in]	�W���C�X�e�B�b�N�̎��
	@return		x:�����Ay:�c���Az:��������(0��1)
*/
DirectX::XMFLOAT3 GamePad::GetJoyStick(KeyCode::JoyStick joyStick)
{
	DirectX::XMFLOAT3 input = { 0,0,0 };

	switch (joyStick)
	{
	case KeyCode::JoyStick::LEFT:
		input.x = now.Gamepad.sThumbLX;
		input.y = now.Gamepad.sThumbLY;
		// ���X�e�B�b�N�̉�������
		input.z = now.Gamepad.wButtons & 0x0040 ? 1.0f : 0.0f;
		break;
	case KeyCode::JoyStick::RIGHT:
		input.x = now.Gamepad.sThumbRX;
		input.y = now.Gamepad.sThumbRY;
		// �E�X�e�B�b�N�̉�������
		input.z = now.Gamepad.wButtons & 0x0080 ? 1.0f : 0.0f;
		break;
	default:
		input = DirectX::XMFLOAT3(0, 0, 0);
		break;
	}

	// �l�̐��K��(-1�`1)
	input.x = InputNormalize(input.x);
	input.y = InputNormalize(input.y);

	// �l���ۂ߂�
	input.x = InputRound(input.x);
	input.y = InputRound(input.y);

	return input;
}

/*!
	@fn			GetTrigger
	@brief		�g���K�[�̓��͎擾
	@param[in]	���肷��g���K�[
	@return		0�`255�͈̔͂̋�����Ԃ�
*/
BYTE GamePad::GetTrigger(KeyCode::Trigger trigger)
{
	BYTE input = 0;
	
	switch (trigger)
	{
	case KeyCode::LT:
		input = now.Gamepad.bLeftTrigger;
		break;
	case KeyCode::RT:
		input = now.Gamepad.bRightTrigger;
		break;
	default:
		break;
	}

	return input;
}

/*!
	@fn			SetVibration
	@brief		�o�C�u���[�V�����̐ݒ�
	@detail		�f�t�H���g������rightPower = -1���w�肷�邱�Ƃō��E�̏o�͂𓯂��ɂ���
	@param[in]	�����̃��[�^�[�̋���
	@param[in]	�E���̃��[�^�[�̋���
*/
void GamePad::SetVibration(int leftPower, int rightPower)
{
	// ���E�ŏo�͂������Ȃ�
	if (rightPower == -1) {
		rightPower = leftPower;
	}
	XINPUT_VIBRATION vibration;
	SecureZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
	vibration.wLeftMotorSpeed = VibrationConvertPower(leftPower);
	vibration.wRightMotorSpeed = VibrationConvertPower(rightPower);
	XInputSetState(index, &vibration);
}

/*!
	@fn			SetJoyStickDead
	@brief		�W���C�X�e�B�b�N�̓��͐����l�̐ݒ�
	@detail		���̒l�ȉ��̒l�� 0.0f �Ƃ��Ĉ����� ���W���C�X�e�B�b�N�̂ݑΉ�
	@param[in]	�ݒ�l
*/
void GamePad::SetJoyStickDead(float dead)
{
	m_JoyStickInputDead = dead;
}

/*!
	@fn			SetJoyStickPrecision
	@brief		�W���C�X�e�B�b�N�̓��͐��x�l�̐ݒ�
	@detail		���x ex)10:�������ʁA100:��������
	@param[in]	�ݒ�l
*/
void GamePad::SetJoyStickPrecision(unsigned int precision)
{
	m_JoyStickInputPrecision = precision;
}

/*!
	@fn			InputNormalize
	@brief		���͒l�̐��K��
	@param[in]	���K������l
	@return		-1�`1�͈̔͂ɒl�𐳋K�������l��Ԃ�
*/
float GamePad::InputNormalize(float input)
{
	input = (input - c_JoyStickInputMin) / (c_JoyStickInputMax - c_JoyStickInputMin)*(1 - (-1)) + (-1);
	return input;
}

/*!
	@fn			InputRound
	@brief		���͒l���ۂ߂�
	@detail		�ݒ肵�����x�Ő��l���ۂ߂�
	@param[in]	���͒l
	@return		�ۂ߂��l
*/
float GamePad::InputRound(float input)
{
	int tmp = static_cast<int>(input) * m_JoyStickInputPrecision;
	input = static_cast<float>(tmp) / static_cast<float>(m_JoyStickInputPrecision);

	// �ݒ肵�����͒l�ȉ��Ȃ� ���͒l��0�ɂ���
	if (fabs(input) < m_JoyStickInputDead) {
		input = 0.0f;
	}

	return input;
}

/*!
	@fn			VibrationConvertPower
	@brief		���K�������l���o�C�u���[�V�����l�ɕϊ�
	@detail		0�`100�͈̔͂ɐ��K�������l���o�C�u���[�V�����Ŏg���l�ɕϊ�
	@param[in]	���K�������l(0�`100)
	@return		�o�C�u���[�V�����̒l�ɕϊ������l
*/
int GamePad::VibrationConvertPower(int vibration)
{
	// �͈͊O�␳
	{
		if (vibration < 0) { vibration = 0; }
		else if (100 < vibration) { vibration = 100; }
	}
	vibration *= c_VibrationRange / 100;
	return vibration;
}

/*!
	@fn			ConvertKeyButtonCode
	@brief		�{�^���̃L�[�R�[�h�ϊ�
	@detail		�{�^���̗񋓑̂���XInput�̔���p16�i���֕ϊ�
	@param[in]	�ϊ�����{�^��
	@return		�ϊ���̃L�[�R�[�h
*/
int GamePad::ConvertKeyButtonCode(Button && button)
{
	switch (button)
	{
	case Button::A:			return 0x1000;	// A�{�^�� �}�N���y XINPUT_GAMEPAD_A�z
	case Button::B:			return 0x2000;	// B�{�^�� �}�N���y XINPUT_GAMEPAD_B�z
	case Button::X:			return 0x4000;	// X�{�^�� �}�N���y XINPUT_GAMEPAD_X�z
	case Button::Y:			return 0x8000;	// Y�{�^�� �}�N���y XINPUT_GAMEPAD_Y�z
	case Button::START:		return 0x0010;	// START�{�^�� �}�N���y XINPUT_GAMEPAD_START�z
	case Button::BACK:		return 0x0020;	// BACK�{�^�� �}�N���y XINPUT_GAMEPAD_BACK�z
	case Button::LB:		return 0x0100;	// LB�{�^�� �}�N���y XINPUT_GAMEPAD_LEFT_SHOULDER�z
	case Button::RB:		return 0x0200;	// RB�{�^�� �}�N���y XINPUT_GAMEPAD_RIGHT_SHOULDER�z
	case Button::L_STICK:	return 0x0040;	// ���X�e�B�b�N�̉������� �}�N���y XINPUT_GAMEPAD_LEFT_THUMB�z
	case Button::R_STICK:	return 0x0080;	// �E�X�e�B�b�N�̉������� �}�N���y XINPUT_GAMEPAD_RIGHT_THUMB�z
	default:
		break;
	}
	return 0;
}

/*!
	@fn			ConvertKeyArrowCode
	@brief		�\���L�[�̃L�[�R�[�h�ϊ�
	@detail		�\���L�[�̗񋓑̂���XInput�̔���p16�i���֕ϊ�
	@param[in]	�ϊ�����\���L�[
	@return		�ϊ���̏�񂪓�����XMINT2�^
*/
DirectX::XMINT2 GamePad::ConvertKeyArrowCode(KeyCode::Arrow && arrow)
{
	DirectX::XMINT2 code = { 0,0 };

	switch (arrow)
	{
	case KeyCode::VERTICAL:
		code.x = 0x0001;	// �\���L�[�̏�
		code.y = 0x0002;	// �\���L�[�̉�
		break;
	case KeyCode::HORIZONTAL:
		code.x = 0x0008;	// �\���L�[�̉E
		code.y = 0x0004;	// �\���L�[�̍�
		break;
	default:
		break;
	}
	return code;
}
