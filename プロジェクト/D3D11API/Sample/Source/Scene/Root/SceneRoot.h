/*!
	@file	SceneRoot.h
	@date	2019/03/28
	@author	番場 宥輝
	@brief	シーンのルート
*/
#pragma once
#include "../Interface/IScene.h"
#include <memory>
#include <Singleton.h>

/*!
	@brief	ルートシーン
*/
class SceneRoot
	:public Singleton<SceneRoot>, public IScene
{
public:
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

	/*!
		@fn			SetupNextScene
		@brief		遷移先シーンの設定
		@param[in]	遷移先シーンのポインタ
	*/
	void SetupNextScene(IScene* scene);

	/*!
		@fn			SetupNextScene
		@brief		遷移先シーンの設定
		@param[in]	遷移先シーンのユニークポインタ
	*/
	void SetupNextScene(std::unique_ptr<IScene> scene);

private:
	/*!
		@brief	シングルトンデザインパターンのテンプレート継承
	*/
	friend class Singleton<SceneRoot>;

	/*!
		@brief	コンストラクタ
	*/
	explicit SceneRoot();

	/*!
		@fn		OnCallSetupNextScene
		@brief	遷移先シーンの設定後に呼ばれる処理
	*/
	void OnCallSetupNextScene();

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