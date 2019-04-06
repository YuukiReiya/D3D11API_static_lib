/*!
	@file	SampleScene.cpp
	@date	2019/03/28
	@author	番場 宥輝
	@brief	サンプルシーン
	@detail	ライブラリのテストを行うサンプル用のシーン
*/
#include "SampleScene.h"
#include <iostream>
#include <Keyboard.h>
#include "TransitionTestScene.h"
#include "../Root/SceneRoot.h"
#include <Sprite.h>

/*!
	@brief	usingディレクティブ
	@using	std
*/
using namespace std;

/*!
	@brief	usingディレクティブ
	@using	API
*/
using namespace API;

/*!
	@var	g_pSprite
	@brief	スプライト
*/
shared_ptr<Sprite>g_pSprite;
shared_ptr<Sprite>g_pS1[1];

/*!
	@var	g_pTexture
	@brief	テクスチャ
*/
shared_ptr<Texture>g_pTexture;
shared_ptr<Texture>g_pT1[1];

/*!
	@brief	コンストラクタ
*/
SampleScene::SampleScene()
{
}

/*!
	@fn		Initialize
	@brief	初期化処理
*/
void SampleScene::Initialize() 
{
	cout << "sample init" << endl;
	
	g_pSprite = make_shared<Sprite>();
	g_pTexture = make_shared<Texture>();

	for (auto itr : g_pS1) {
		itr = make_shared<Sprite>();
	}

	for (auto itr : g_pT1) {
		itr = make_shared<Texture>();
	}


	g_pTexture->Initialize("Resources/Smoke.png");
	g_pTexture->SetSize({ 256,256 });
}

/*!
	@fn		Finalize
	@brief	破棄処理
*/
void SampleScene::Finalize()
{
	cout << "sample destroy:" << endl;
	
}

/*!
	@fn		Update
	@brief	更新処理
*/
void SampleScene::Update()
{
	if (Keyboard::GetButtonDown('a')) {
		cout << "set next scene" << endl;
		SceneRoot::GetInstance().SetupNextScene(new TransitionTestScene());
	}
}

/*!
	@fn		Render
	@brief	描画処理
*/
void SampleScene::Render()
{
	g_pSprite->Render(g_pTexture.get());
}