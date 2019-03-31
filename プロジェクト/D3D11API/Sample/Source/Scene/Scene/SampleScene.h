/*!
	@file	SampleScene.h
	@date	2019/03/28
	@author	番場 宥輝
	@brief	サンプルシーン
*/
#pragma once
#include "../Interface/IScene.h"

class SampleScene 
	:public IScene
{
public:
	/*!
		@brief	コンストラクタ
	*/
	explicit SampleScene();

	/*!
		@fn		Initialize
		@brief	初期化処理
*/
	void IScene::Initialize();

	/*!
		@fn		Finalize
		@brief	破棄処理
	*/
	void IScene::Finalize();

	/*!
		@fn		Update
		@brief	更新処理
	*/
	void IScene::Update();

	/*!
		@fn		Render
		@brief	描画処理
	*/
	void IScene::Render();

};