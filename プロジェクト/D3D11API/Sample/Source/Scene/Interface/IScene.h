/*
	@file	IScene.h
	@date	2019/03/28
	@author	番場 宥輝
	@brief	シーンのインターフェイス
*/
#pragma once

/*!
	@brief	シーンのインターフェイス
*/
__interface IScene
{
	/*!
		@fn		Initialize
		@brief	初期化処理
	*/
	void Initialize();

	/*!
		@fn		Finalize
		@brief	破棄処理
	*/
	void Finalize();

	/*!
		@fn		Update
		@brief	更新処理
	*/
	void Update();

	/*!
		@fn		Render
		@brief	描画処理
	*/
	void Render();
};