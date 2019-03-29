/*
	@file	GamePad.cpp
	@brief	ゲームパッド入力
	@date	2019/03/29
	@detail	XInputを用いた入力
			XInput参考:		http://www.geocities.jp/gameprogrammingunit/xinput/index.html
			列挙体の注意:	http://d.hatena.ne.jp/mclh46/20100627/1277603928
	@author	番場 宥輝
*/
#include "stdafx.h"
#include "GamePad.h"
#include <math.h>

/*!
	@brief	usingディレクティブ
	@using	KeyCode
*/
using namespace KeyCode;

/*!
	@brief	コンストラクタ
*/
GamePad::GamePad()
	:m_JoyStickInputDead(c_JoyStickInputDead)
	,m_JoyStickInputPrecision(c_JoyStickInputPrecision)
{
	SecureZeroMemory(this, sizeof(this));
	index = Index::One;
}

/*!
	@brief		引数付きコンストラクタ
	@param[in]	設定するコントローラー番号
*/
GamePad::GamePad(Index && index)
	:m_JoyStickInputDead(c_JoyStickInputDead)
	,m_JoyStickInputPrecision(c_JoyStickInputPrecision)
{
	SecureZeroMemory(this, sizeof(this));
	this->index = index;
}

/*!
	@brief	デストラクタ
*/
GamePad::~GamePad()
{
	SetVibration(0);
	// XInputを無効にするj(全てのコントローラーに依存)
	//XInputEnable(false);
}

/*!
	@fn		Update
	@brief	更新処理
	@detail	入力バッファの更新
*/
void GamePad::Update()
{
	old = now;

	if (XInputGetState(index, &now) == ERROR_SUCCESS) {}//	TODO:何らかしらのエラー処理
}

/*!
	@fn			GetButton
	@brief		ボタンを押してる間の取得
	@param[in]	判定するボタンのキーコード
	@return		ボタンの押下情報
*/
bool GamePad::GetButton(Button button)
{
	return now.Gamepad.wButtons&ConvertKeyButtonCode(Button(button));
}

/*!
	@fn			GetArrow
	@brief		十字キーを押している間の取得
	@param[in]	判定方向
	@return		1:正 -1:負 0:押されていない
*/
int GamePad::GetArrow(KeyCode::Arrow arrow)
{
	auto input = ConvertKeyArrowCode(Arrow(arrow));

	// 正の値
	if ((now.Gamepad.wButtons&input.x)==1) {
		return 1;
	}
	// 負の値
	if ((now.Gamepad.wButtons&input.y)==1) {
		return -1;
	}

	return 0;
}

/*!
	@fn			GetButtonDown
	@brief		ボタンを押した瞬間の取得
	@param[in]	判定するボタンのキーコード
	@return		ボタンの押下情報
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
	@brief		十字キーを押した瞬間の取得
	@param[in]	判定方向
	@return		1:正 -1:負 0:押されていない
*/
int GamePad::GetArrowDown(KeyCode::Arrow arrow)
{
	auto input = ConvertKeyArrowCode(Arrow(arrow));

	// 正の値
	if ((now.Gamepad.wButtons&input.x)
		^
		(old.Gamepad.wButtons&input.x)
		&
		(now.Gamepad.wButtons&input.x)
		) {
		return 1;
	}

	// 負の値
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
	@brief		ボタンが離された瞬間の取得
	@param[in]	判定するボタンのキーコード
	@return		ボタンの押下情報
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
	@brief		十字キーを離した瞬間の取得
	@param[in]	判定方向
	@return		1:正 -1:負 0:押されていない
*/
int GamePad::GetArrowUp(KeyCode::Arrow arrow)
{
	auto input = ConvertKeyArrowCode(Arrow(arrow));

	// 正の値
	if ((now.Gamepad.wButtons&input.x)
		^
		(old.Gamepad.wButtons&input.x)
		&
		(old.Gamepad.wButtons&input.x)
		) {
		return 1;
	}

	// 負の値
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
	@brief		ジョイスティックの入力取得
	@param[in]	ジョイスティックの種類
	@return		x:横軸、y:縦軸、z:押し込み(0か1)
*/
DirectX::XMFLOAT3 GamePad::GetJoyStick(KeyCode::JoyStick joyStick)
{
	DirectX::XMFLOAT3 input = { 0,0,0 };

	switch (joyStick)
	{
	case KeyCode::JoyStick::LEFT:
		input.x = now.Gamepad.sThumbLX;
		input.y = now.Gamepad.sThumbLY;
		// 左スティックの押し込み
		input.z = now.Gamepad.wButtons & 0x0040 ? 1.0f : 0.0f;
		break;
	case KeyCode::JoyStick::RIGHT:
		input.x = now.Gamepad.sThumbRX;
		input.y = now.Gamepad.sThumbRY;
		// 右スティックの押し込み
		input.z = now.Gamepad.wButtons & 0x0080 ? 1.0f : 0.0f;
		break;
	default:
		input = DirectX::XMFLOAT3(0, 0, 0);
		break;
	}

	// 値の正規化(-1～1)
	input.x = InputNormalize(input.x);
	input.y = InputNormalize(input.y);

	// 値を丸める
	input.x = InputRound(input.x);
	input.y = InputRound(input.y);

	return input;
}

/*!
	@fn			GetTrigger
	@brief		トリガーの入力取得
	@param[in]	判定するトリガー
	@return		0～255の範囲の強さを返す
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
	@brief		バイブレーションの設定
	@detail		デフォルト引数のrightPower = -1を指定することで左右の出力を同じにする
	@param[in]	左側のモーターの強さ
	@param[in]	右側のモーターの強さ
*/
void GamePad::SetVibration(int leftPower, int rightPower)
{
	// 左右で出力が同じなら
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
	@brief		ジョイスティックの入力制限値の設定
	@detail		この値以下の値は 0.0f として扱われる ※ジョイスティックのみ対応
	@param[in]	設定値
*/
void GamePad::SetJoyStickDead(float dead)
{
	m_JoyStickInputDead = dead;
}

/*!
	@fn			SetJoyStickPrecision
	@brief		ジョイスティックの入力精度値の設定
	@detail		精度 ex)10:少数第一位、100:少数第二位
	@param[in]	設定値
*/
void GamePad::SetJoyStickPrecision(unsigned int precision)
{
	m_JoyStickInputPrecision = precision;
}

/*!
	@fn			InputNormalize
	@brief		入力値の正規化
	@param[in]	正規化する値
	@return		-1～1の範囲に値を正規化した値を返す
*/
float GamePad::InputNormalize(float input)
{
	input = (input - c_JoyStickInputMin) / (c_JoyStickInputMax - c_JoyStickInputMin)*(1 - (-1)) + (-1);
	return input;
}

/*!
	@fn			InputRound
	@brief		入力値を丸める
	@detail		設定した精度で数値を丸める
	@param[in]	入力値
	@return		丸めた値
*/
float GamePad::InputRound(float input)
{
	int tmp = static_cast<int>(input) * m_JoyStickInputPrecision;
	input = static_cast<float>(tmp) / static_cast<float>(m_JoyStickInputPrecision);

	// 設定した入力値以下なら 入力値を0にする
	if (fabs(input) < m_JoyStickInputDead) {
		input = 0.0f;
	}

	return input;
}

/*!
	@fn			VibrationConvertPower
	@brief		正規化した値をバイブレーション値に変換
	@detail		0～100の範囲に正規化した値をバイブレーションで使う値に変換
	@param[in]	正規化した値(0～100)
	@return		バイブレーションの値に変換した値
*/
int GamePad::VibrationConvertPower(int vibration)
{
	// 範囲外補正
	{
		if (vibration < 0) { vibration = 0; }
		else if (100 < vibration) { vibration = 100; }
	}
	vibration *= c_VibrationRange / 100;
	return vibration;
}

/*!
	@fn			ConvertKeyButtonCode
	@brief		ボタンのキーコード変換
	@detail		ボタンの列挙体からXInputの判定用16進数へ変換
	@param[in]	変換するボタン
	@return		変換後のキーコード
*/
int GamePad::ConvertKeyButtonCode(Button && button)
{
	switch (button)
	{
	case Button::A:			return 0x1000;	// Aボタン マクロ【 XINPUT_GAMEPAD_A】
	case Button::B:			return 0x2000;	// Bボタン マクロ【 XINPUT_GAMEPAD_B】
	case Button::X:			return 0x4000;	// Xボタン マクロ【 XINPUT_GAMEPAD_X】
	case Button::Y:			return 0x8000;	// Yボタン マクロ【 XINPUT_GAMEPAD_Y】
	case Button::START:		return 0x0010;	// STARTボタン マクロ【 XINPUT_GAMEPAD_START】
	case Button::BACK:		return 0x0020;	// BACKボタン マクロ【 XINPUT_GAMEPAD_BACK】
	case Button::LB:		return 0x0100;	// LBボタン マクロ【 XINPUT_GAMEPAD_LEFT_SHOULDER】
	case Button::RB:		return 0x0200;	// RBボタン マクロ【 XINPUT_GAMEPAD_RIGHT_SHOULDER】
	case Button::L_STICK:	return 0x0040;	// 左スティックの押し込み マクロ【 XINPUT_GAMEPAD_LEFT_THUMB】
	case Button::R_STICK:	return 0x0080;	// 右スティックの押し込み マクロ【 XINPUT_GAMEPAD_RIGHT_THUMB】
	default:
		break;
	}
	return 0;
}

/*!
	@fn			ConvertKeyArrowCode
	@brief		十字キーのキーコード変換
	@detail		十字キーの列挙体からXInputの判定用16進数へ変換
	@param[in]	変換する十字キー
	@return		変換後の情報が入ったXMINT2型
*/
DirectX::XMINT2 GamePad::ConvertKeyArrowCode(KeyCode::Arrow && arrow)
{
	DirectX::XMINT2 code = { 0,0 };

	switch (arrow)
	{
	case KeyCode::VERTICAL:
		code.x = 0x0001;	// 十字キーの上
		code.y = 0x0002;	// 十字キーの下
		break;
	case KeyCode::HORIZONTAL:
		code.x = 0x0008;	// 十字キーの右
		code.y = 0x0004;	// 十字キーの左
		break;
	default:
		break;
	}
	return code;
}
