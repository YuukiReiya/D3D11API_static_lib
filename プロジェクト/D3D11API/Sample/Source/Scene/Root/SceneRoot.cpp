/*!
	@file	SceneRoot.cpp
	@date	2019/03/28
	@author	番場 宥輝
	@brief	シーンのルート
*/
#include <MyGame.h>
#include "SceneRoot.h"

/*!
	@brief	コンストラクタ
*/
SceneRoot::SceneRoot()
	:m_pCurrentScene(nullptr)
	,m_pNextScene(nullptr)
{}

/*!
	@brief		引数付きコンストラクタ
	@param[in]	初期シーンのポインタ
*/
SceneRoot::SceneRoot(IScene * pInitializeScene)
	:m_pCurrentScene(pInitializeScene)
	,m_pNextScene(nullptr)
{}

/*!
	@brief	デストラクタ
*/
SceneRoot::~SceneRoot()
{
	m_pCurrentScene.reset();
	m_pNextScene.reset();
}

/*!
	@fn		Initialize
	@brief	初期化処理
*/
void SceneRoot::Initialize()
{
	m_pCurrentScene->Initialize();
}

/*!
	@fn		Finalize
	@brief	破棄処理
*/
void SceneRoot::Finalize()
{
	m_pCurrentScene->Initialize();
}

/*!
	@fn		Update
	@brief	更新処理
*/
void SceneRoot::Update()
{
	//	シーンの遷移
	OnCallSetupCurrentScene();

	//	シーンの更新
	m_pCurrentScene->Update();

	//	シーンの破棄
	OnCallShutdownCurrentScene();
}

/*!
	@fn		Render
	@brief	描画処理
*/
void SceneRoot::Render()
{
	m_pCurrentScene->Render();
}

/*!
	@fn		OnCallSetupCurrentScene
	@brief	現在設定されているシーンの遷移
*/
void SceneRoot::OnCallSetupCurrentScene()
{
	if (m_pNextScene == nullptr) { return; }

	//	スマートポインタのムーブ
	m_pCurrentScene = std::move(m_pNextScene);

	//	カレントシーンの初期化
	m_pCurrentScene->Initialize();

	//	遷移先シーンの初期化
	m_pNextScene.reset();
	m_pNextScene = nullptr;
}

/*!
	@fn		OnCallShutdownCurrentScene
	@brief	現在設定されているシーンの破棄処理
*/
void SceneRoot::OnCallShutdownCurrentScene()
{
	if (m_pNextScene == nullptr) { return; }

	//	カレントシーンの破棄
	m_pCurrentScene->Finalize();

	m_pCurrentScene.reset();
	m_pCurrentScene = nullptr;
}
