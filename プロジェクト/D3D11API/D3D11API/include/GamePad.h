/*
	@file	GamePad.h
	@brief	ゲームパッド入力
	@date	2018/04/24
	@detail	XInputを用いた入力
			XInput参考:		http://www.geocities.jp/gameprogrammingunit/xinput/index.html
			列挙体の注意:	http://d.hatena.ne.jp/mclh46/20100627/1277603928
	@author	番場 宥輝
*/#pragma once
#include <windows.h>
#include <Xinput.h>
#include <DirectXMath.h>

#pragma comment(lib, "XInput.lib")

/*!
	@brief	名前空間
*/
namespace KeyCode {
	/*! 列挙体キーコード */

	/*!
		@enum	Button
		@brief	ボタンのキーコード列挙体
	*/
	enum Button		{ A, B, X, Y, START, BACK, LB, RB, L_STICK, R_STICK, };	/*!< ボタン */

	/*!
		@enum	Trigger
		@brief	トリガーのキーコード列挙体
	*/
	enum Trigger	{ LT, RT };												/*!< トリガー */

	/*!
		@enum	JoyStick
		@brief	ジョイスティックの種類列挙体
	*/
	enum JoyStick	{ LEFT, RIGHT };										/*!< ジョイスティック */

	/*!
		@enum	Arrow
		@brief	方向キーのキーコード列挙体
	*/
	enum Arrow		{ VERTICAL, HORIZONTAL };								/*!< V:縦 H:横 */
}

/*!
	@brief	ゲームパッドクラス
	@detail	XBOXコントローラーの入力制御クラス
*/
class GamePad {
public:
	/*! コントローラー番号 */
	enum Index { One, Two, Three, Four, };

	/*!
		@brief	コンストラクタ
	*/
	explicit GamePad();

	/*!
		@brief	引数付きコンストラクタ
	*/
	explicit GamePad(Index&& index);

	/*!
		@brief	デストラクタ
	*/
	~GamePad();


	/*!
		@fn		Update
		@brief	更新処理
		@detail	入力バッファの更新
	*/
	void Update();

	/*!
		@fn			GetButton
		@brief		ボタンを押してる間の取得
		@param[in]	判定するボタンのキーコード
		@return		ボタンの押下情報
	*/
	bool GetButton(KeyCode::Button button);

	/*!
		@fn			GetButtonDown
		@brief		ボタンを押した瞬間の取得
		@param[in]	判定するボタンのキーコード
		@return		ボタンの押下情報
	*/
	bool GetButtonDown(KeyCode::Button button);

	/*!
		@fn			GetButtonUp
		@brief		ボタンが離された瞬間の取得
		@param[in]	判定するボタンのキーコード
		@return		ボタンの押下情報
	*/
	bool GetButtonUp(KeyCode::Button button);

	/*!
		@fn			GetArrow
		@brief		十字キーを押している間の取得
		@param[in]	判定方向
		@return		1: -1: 0:押されていない
	*/
	int GetArrow(KeyCode::Arrow arrow);

	/*!
		@fn			GetArrow
		@brief		十字キーを押した瞬間の取得
		@param[in]	判定方向
		@return		1: -1: 0:押されていない
	*/
	int GetArrowDown(KeyCode::Arrow arrow);

	/*!
		@fn			GetArrowUp
		@brief		十字キーを離した瞬間の取得
		@param[in]	判定方向
		@return		1: -1: 0:押されていない
	*/
	int GetArrowUp(KeyCode::Arrow arrow);

	/*!
		@fn			GetJoyStick
		@brief		ジョイスティックの入力取得
		@param[in]	ジョイスティックの種類
		@return		x:横軸、y:縦軸、z:押し込み(0か1)
	*/
	DirectX::XMFLOAT3 GetJoyStick(KeyCode::JoyStick joyStick);

	/*!
		@fn			GetTrigger
		@brief		トリガーの入力取得
		@param[in]	判定するトリガー
		@return		0～255の範囲の強さを返す
	*/
	BYTE GetTrigger(KeyCode::Trigger trigger);

	/*!
		@fn			GetBTrigger
		@brief		トリガーの入力取得
		@param[in]	判定するトリガー
		@return		押されている/いないのどちらかを返す
	*/
	bool GetBTrigger(KeyCode::Trigger trigger);

	/*!
		@fn			SetVibration
		@brief		バイブレーションの設定
		@detail		デフォルト引数のrightPower = -1を指定することで左右の出力を同じにする
		@param[in]	左側のモーターの強さ
		@param[in]	右側のモーターの強さ
	*/
	void SetVibration(int leftPower, int rightPower = -1);
private:

	/*!
		@fn			InputNormalize
		@brief		入力値の正規化
		@param[in]	正規化する値
		@return		-1～1の範囲に値を正規化した値を返す
	*/
	float InputNormalize(float input);

	/*!
		@fn			InputRound
		@brief		入力値を丸める
		@detail		設定した精度で数値を丸める
		@param[in]	入力値
		@return		丸めた値
	*/
	float InputRound(float input);

	/*!
		@fn			VibrationConvertPower
		@brief		正規化した値をバイブレーション値に変換
		@detail		0～100の範囲に正規化した値をバイブレーションで使う値に変換
		@param[in]	正規化した値(0～100)
		@return		バイブレーションの値に変換した値
	*/
	int VibrationConvertPower(int vibration);

	/*!
		@fn			ConvertKeyButtonCode
		@brief		ボタンのキーコード変換
		@detail		ボタンの列挙体からXInputの判定用16進数へ変換
		@param[in]	変換するボタン
		@return		変換後のキーコード
	*/
	int ConvertKeyButtonCode(KeyCode::Button&& button);

	/*!
		@fn			ConvertKeyArrowCode
		@brief		十字キーのキーコード変換
		@detail		十字キーの列挙体からXInputの判定用16進数へ変換
		@param[in]	変換する十字キー
		@return		変換後の情報が入ったXMINT2型
	*/
	DirectX::XMINT2 ConvertKeyArrowCode(KeyCode::Arrow&& arrow);

	/*!
		@var	c_JoyStickInputMin
		@brief	ジョイスティック入力の最小値
	*/
	static const int c_JoyStickInputMin;

	/*!
		@var	c_JoyStickInputMax
		@brief	ジョイスティック入力の最大値
	*/
	static const int c_JoyStickInputMax;

	/*!
		@var	c_VibrationRange
		@brief	バイブレーションの値域の範囲
	*/
	static const int c_VibrationRange;

	/*!
		@var	c_JoyStickInputPrecision
		@brief	ジョイスティックの入力精度
		@detail	精度 ex)10:少数第一位、100:少数第二位
	*/
	static constexpr int c_JoyStickInputPrecision = 10;

	/*!
		@var	c_JoyStickInputDead
		@brief	ジョイスティックの入力制限
		@detail	この値以下の値は 0.0f として扱われる ※ジョイスティックのみ対応
	*/
	static constexpr float c_JoyStickInputDead = 0.0f;

	/*! パラメーター */
	Index index;			/*!< コントローラー番号 */
	XINPUT_STATE now,old;	/*!< 入力バッファ */

};