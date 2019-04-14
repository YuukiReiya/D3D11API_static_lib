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
#include <SpriteShader.h>
#include <Sprite.h>
#include <MyGame.h>

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
shared_ptr<Sprite>g_pSprites[3];

/*!
	@var	g_pTexture
	@brief	テクスチャ
*/
shared_ptr<Texture>g_pTexture;
shared_ptr<Texture>g_pTextures[3];

/*!
	@var	g_pShader
	@brief	シェーダー
*/
shared_ptr<D3D11::Graphic::AbstractShader>g_pShader;

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
	
	g_pSprite	= make_shared<Sprite>();
	for (auto& it : g_pSprites)
	{
		it= make_shared<Sprite>();
	}

	g_pTexture	= make_shared<Texture>();
	for (auto& it : g_pTextures)
	{
		it = make_shared<Texture>();
	}

	g_pShader	= make_shared<D3D11::Graphic::SpriteShader>();
	if (FAILED(g_pShader->DynamicSetup())) {
		ErrorLog("失敗");
	}
	g_pTexture->Initialize("Resources/red.png");
	g_pTextures[0]->Initialize("Resources/blue.png");



	g_pSprite->SetupShader(g_pShader.get());
	g_pSprite->SetupTexture(g_pTexture.get());

	g_pSprites[0]->SetupShader(g_pShader.get());
	g_pSprites[0]->SetupTexture(g_pTextures[0].get());
	g_pSprites[0]->SetPos({ -0.1f,0 });
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
	if (Keyboard::GetButtonDown('Y')) {
		cout << "set next scene" << endl;
		SceneRoot::GetInstance().SetupNextScene(new TransitionTestScene());
	}

	auto pos = g_pSprite->GetPos();


	if (Keyboard::GetButtonDown('a')) {
		pos.z--;
	}
	if (Keyboard::GetButtonDown('d')) {
		pos.z++;
	}
	if (Keyboard::GetButtonDown('w')) {
		pos.y--;
	}
	if (Keyboard::GetButtonDown('s')) {
		pos.y++;
	}
}

/*!
	@fn		Render
	@brief	描画処理
*/
void SampleScene::Render()
{
//	g_pSprite->Render(g_pTexture.get());
	g_pSprite->Render();
	g_pSprites[0]->Render();
}