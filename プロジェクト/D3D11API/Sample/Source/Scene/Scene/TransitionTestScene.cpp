/*!
	@file	TransitionTestScene.cpp
	@date	2019/03/29
	@author	番場 宥輝
	@brief	シーン遷移のテスト用シーン
*/
#include "TransitionTestScene.h"
#include <iostream>

using namespace std;

/*!
	@brief	コンストラクタ
*/
TransitionTestScene::TransitionTestScene()
{
}

/*!
	@fn		Initialize
	@brief	初期化処理
*/
void TransitionTestScene::Initialize()
{
	cout << "test init" << endl;
}

/*!
	@fn		Finalize
	@brief	破棄処理
*/
void TransitionTestScene::Finalize()
{
	cout << "test destroy" << endl;
}

/*!
	@fn		Update
	@brief	更新処理
*/
bool s = false;
void TransitionTestScene::Update()
{
	if (!s) {
		s = true;
		cout << "test update" << endl;
	}
}

/*!
	@fn		Render
	@brief	描画処理
*/
void TransitionTestScene::Render()
{

}