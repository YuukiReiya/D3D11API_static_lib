/*!
	@file	SceneRoot.h
	@date	2019/03/28
	@author	番場 宥輝
	@brief	シーンのルート
*/
#pragma once
#include "../Interface/IScene.h"
#include <memory>

/*!
	@brief	ルートシーン
*/
class SceneRoot
	:public IScene
{
	/*!
		@brief	コンストラクタ
	*/
	explicit SceneRoot();

	/*!
		@brief		引数付きコンストラクタ
		@param[in]	初期シーンのポインタ
	*/
	explicit SceneRoot(IScene* pInitializeScene);

	/*!
		@brief	デストラクタ
	*/
	~SceneRoot();


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

	void SetupNextScene(IScene* scene);

private:

	/*!
		@fn		OnCallSetupCurrentScene
		@brief	現在設定されているシーンの遷移	
	*/
	void OnCallSetupCurrentScene();

	/*!
		@fn		OnCallShutdownCurrentScene
		@brief	現在設定されているシーンの破棄処理
	*/
	void OnCallShutdownCurrentScene();

	/*!
		@var	m_pCurrentScene
		@brief	現在設定しているシーン
	*/
	std::unique_ptr<IScene> m_pCurrentScene;
	
	/*!
		@var	m_pNextScene
		@brief	遷移先のシーン
	*/
	std::unique_ptr<IScene> m_pNextScene;
};