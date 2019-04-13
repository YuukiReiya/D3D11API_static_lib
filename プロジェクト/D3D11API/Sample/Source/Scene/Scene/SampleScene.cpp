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

/*!
	@var	g_pTexture
	@brief	テクスチャ
*/
shared_ptr<Texture>g_pTexture;

/*!
	@brief	コンストラクタ
*/
SampleScene::SampleScene()
{
}

shared_ptr<Sprite> sprite;
shared_ptr<D3D11::Graphic::AbstractShader>shader;
shared_ptr<Texture> tex;
/*!
	@fn		Initialize
	@brief	初期化処理
*/
void SampleScene::Initialize() 
{
	cout << "sample init" << endl;
	
	g_pSprite = make_shared<Sprite>();
	g_pTexture = make_shared<Texture>();

	g_pTexture->Initialize("Resources/red.png");
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
	g_pSprite->Render(g_pTexture.get());
}