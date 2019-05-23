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
#include <MeshShader.h>
#include <Mesh.h>
#include <Hoge.h>
#include <Camera.h>

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
	@var	g_pMesh
	@brief	メッシュ
*/
shared_ptr<Mesh>g_pMesh;

/*!
	@var	g_pShader
	@brief	シェーダー
*/
shared_ptr<D3D11::Graphic::AbstractShader>g_pMeshShader;

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

	g_pMesh = make_shared<Mesh>();
	g_pMeshShader = make_shared<D3D11::Graphic::MeshShader>();

	g_pMesh->Initialize("test.yfm", "hoge.png");
	//g_pMesh->Initialize("cube.yfm", "hoge.png");

	//g_pMesh->Initialize("描画テスト/test_draw.yfm", "hoge.png");
	//g_pMesh->Initialize("abc.yfm");
	g_pMeshShader->Setup();
	g_pMesh->SetupShader(g_pMeshShader.get());
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

	auto pos = g_pMesh->transform.GetPosition();
	auto val = 0.1f;

	if (Keyboard::GetButton('a')) {
		pos.x -= val;
	}
	if (Keyboard::GetButton('d')) {
		pos.x += val;
	}
	if (Keyboard::GetButton('w')) {
		pos.y += val;
	}
	if (Keyboard::GetButton('s')) {
		pos.y -= val;
	}
	if (Keyboard::GetButton('q')) {
		pos.z -= val;
	}
	if (Keyboard::GetButton('e')) {
		pos.z += val;
	}

	auto& camera = Camera::GetInstance();
	auto eye = camera.GetEyePt();
	auto look = camera.GetLookAtPt();
	
	if (Keyboard::GetButton(Keyboard::c_Left)) {
		eye.x -= val;
	}
	if (Keyboard::GetButton(Keyboard::c_Right)) {
		eye.x += val;
	}
	if (Keyboard::GetButton(Keyboard::c_Up)) {
		eye.y += val;
	}
	if (Keyboard::GetButton(Keyboard::c_Down)) {
		eye.y -= val;
	}

	camera.SetViewMatrix(eye, look);
	g_pMesh->transform.SetPosition(pos);
}

/*!
	@fn		Render
	@brief	描画処理
*/
void SampleScene::Render()
{
	g_pMesh->Render();
}