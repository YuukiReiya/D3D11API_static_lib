/*!
	@file	SceneRoot.cpp
	@date	2019/03/28
	@author	番場 宥輝
	@brief	シーンのルート
*/
#include <MyGame.h>
#include <MemoryLeaks.h>
#include "SceneRoot.h"
#include "../Scene/SampleScene.h"

/*!
	@brief	usingディレクティブ
	@using	std
*/
//using namespace std;

/*!
	@brief	コンストラクタ
*/
SceneRoot::SceneRoot()
	:m_pCurrentScene(nullptr)
	,m_pNextScene(nullptr)
{}

/*!
	@brief	デストラクタ
*/
SceneRoot::~SceneRoot()
{
	m_pCurrentScene.reset(nullptr);
	m_pNextScene.reset(nullptr);
}

/*!
	@fn		Initialize
	@brief	初期化処理
*/
void SceneRoot::Initialize()
{
	//	初期シーンの設定
	m_pCurrentScene = std::unique_ptr<IScene>(new SampleScene());
	m_pCurrentScene->Initialize();
}

/*!
	@fn		Finalize
	@brief	破棄処理
*/
void SceneRoot::Finalize()
{
	m_pCurrentScene->Finalize();
}

/*!
	@fn		Update
	@brief	更新処理
*/
void SceneRoot::Update()
{
	//	シーンの更新
	m_pCurrentScene->Update();

	//	シーンの遷移
	OnCallSetupNextScene();
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
	@fn			SetupNextScene
	@brief		遷移先シーンの設定
	@param[in]	遷移先シーン
*/
void SceneRoot::SetupNextScene(IScene * scene)
{
	m_pNextScene = std::unique_ptr<IScene>(scene);
}

/*!
	@fn			SetupNextScene
	@brief		遷移先シーンの設定
	@param[in]	遷移先シーンのユニークポインタ
*/
void SceneRoot::SetupNextScene(std::unique_ptr<IScene> scene)
{
	m_pNextScene = move(scene);
}

/*!
	@fn		OnCallSetupNextScene
	@brief	遷移先シーンの設定後に呼ばれる処理
*/
void SceneRoot::OnCallSetupNextScene()
{
	if (m_pNextScene == nullptr) { return; }

	//	カレントシーンの破棄
	m_pCurrentScene->Finalize();
	m_pCurrentScene.reset(nullptr);

	//	スマートポインタのムーブ
	m_pCurrentScene = std::move(m_pNextScene);

	//	カレントシーンの初期化
	m_pCurrentScene->Initialize();

	//	遷移先シーンの初期化
	m_pNextScene.reset(nullptr);
}