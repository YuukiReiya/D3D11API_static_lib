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
std::vector<API::Mesh>vMesh;
/*!
	@fn		Initialize
	@brief	初期化処理
*/
void SampleScene::Initialize() 
{
	cout << "sample init" << endl;

	g_pMesh = make_shared<Mesh>();
	g_pMeshShader = make_shared<D3D11::Graphic::MeshShader>();


	//g_pMesh->Initialize("mesh1.yfm");
	//g_pMesh->Initialize("test.yfm","TEX_00100_anathema_H_D.png");
	//g_pMesh->Initialize("test3.yfm", "hoge.png");
	//g_pMesh->Initialize("test4.yfm","TEX_00100_anathema_H_D.png");
	//g_pMesh->Initialize("test.yfm","pm0025_00_BodyA1.png");
	//g_pMesh->Initialize("test7.yfm", "hoge.png");
	g_pMesh->Initialize("test8.yfm", "hoge.png");
	//g_pMesh->Initialize("test99-2.yfm", "hoge.png");
	//g_pMesh->Initialize("cube.yfm", "hoge.png");

	//g_pMesh->Initialize("描画テスト/test_draw.yfm", "hoge.png");
	//g_pMesh->Initialize("abc.yfm");

	g_pMesh->transform.SetPosition({ -1, 0, 0 });

	g_pMeshShader->Setup();
	g_pMesh->SetupShader(g_pMeshShader.get());

#pragma region 後輩ちゃん
#ifdef use_multiple1

	for (size_t i = 0; i < 14; i++)
	{
		Mesh ptr;
		ptr.Initialize("後輩ちゃん/test99-" + to_string(i) + ".yfm");
		vMesh.push_back(ptr);
}
	for (auto& it : vMesh)
	{
		it.SetupShader(g_pMeshShader.get());
	}

	vMesh[0].CreateTexture("後輩ちゃん/skin.png");
	vMesh[1].CreateTexture("後輩ちゃん/face.png");
	vMesh[2].CreateTexture("後輩ちゃん/eye.png");
	vMesh[3].CreateTexture("後輩ちゃん/face.png");
	vMesh[4].CreateTexture("後輩ちゃん/face.png");
	vMesh[5].CreateTexture("後輩ちゃん/face.png");
	vMesh[6].CreateTexture("後輩ちゃん/face.png");
	vMesh[7].CreateTexture("後輩ちゃん/face.png");
	vMesh[8].CreateTexture("後輩ちゃん/face.png");
	vMesh[9].CreateTexture("後輩ちゃん/hair.png");
	vMesh[10].CreateTexture("後輩ちゃん/eye.png");
	vMesh[11].CreateTexture("後輩ちゃん/clothes.png");
	vMesh[12].CreateTexture("後輩ちゃん/clothes.png");
	vMesh[13].CreateTexture("後輩ちゃん/facial.png");

#endif // use_multiple1
#pragma endregion


}

/*!
	@fn		Finalize
	@brief	破棄処理
*/
void SampleScene::Finalize()
{
	cout << "sample destroy:" << endl;
	for (auto&it : vMesh) {
		it.Finalize();
	}
	vMesh.clear();
}
#define	UseMeshTrans
/*!
	@fn		Update
	@brief	更新処理
*/
void SampleScene::Update()
{
	//if (Keyboard::GetButtonDown('Y')) {
	//	cout << "set next scene" << endl;
	//	SceneRoot::GetInstance().SetupNextScene(new TransitionTestScene());
	//}
	auto val = 0.1f;

	//auto pos = g_pMesh->transform.GetPosition();
	//if (Keyboard::GetButton('a')) {
	//	pos.x -= val;
	//}
	//if (Keyboard::GetButton('d')) {
	//	pos.x += val;
	//}
	//if (Keyboard::GetButton('w')) {
	//	pos.y += val;
	//}
	//if (Keyboard::GetButton('s')) {
	//	pos.y -= val;
	//}
	//if (Keyboard::GetButton('q')) {
	//	pos.z -= val;
	//}
	//if (Keyboard::GetButton('e')) {
	//	pos.z += val;
	//}
	//g_pMesh->transform.SetPosition(pos);

#pragma region メッシュ
#ifdef UseMeshTrans
	auto& m = g_pMesh->transform.m_Matrix;
	
#pragma region x,y,z
	static float x =0, y=0, z=0;

	if (Keyboard::GetButton(Keyboard::c_Left)) {
		x -= val;
	}
	if (Keyboard::GetButton(Keyboard::c_Right)) {
		x += val;
	}
	if (Keyboard::GetButton(Keyboard::c_Up)) {
		y += val;
	}
	if (Keyboard::GetButton(Keyboard::c_Down)) {
		y -= val;
	}
	if (Keyboard::GetButton(Keyboard::c_Back)) {
		z -= val;
	}
	if (Keyboard::GetButton(Keyboard::c_Delete)) {
		z += val;
	}
	g_pMesh->transform.Rotate(x, y, z);
#pragma endregion

#endif // UseMeshTrans


#pragma endregion


#pragma region カメラ操作
#if defined UseCameraAction
	auto& camera = Camera::GetInstance();
	auto eye = camera.GetEyePt();
	auto look = camera.GetLookAtPt();
	
	if (Keyboard::GetButton(Keyboard::c_Left)) {
		//eye.x -= val;
		look.x -= val;
	}
	if (Keyboard::GetButton(Keyboard::c_Right)) {
		//eye.x += val;
		look.x += val;
	}
	if (Keyboard::GetButton(Keyboard::c_Up)) {
		//eye.y += val;
		look.y -= val;
	}
	if (Keyboard::GetButton(Keyboard::c_Down)) {
		//eye.y -= val;
		look.y += val;
	}
	if (Keyboard::GetButton('a')) {
		eye.x -= val;
	}
	if (Keyboard::GetButton('d')) {
		eye.x += val;
	}
	if (Keyboard::GetButton('w')) {
		eye.y += val;
	}
	if (Keyboard::GetButton('s')) {
		eye.y -= val;
	}
	if (Keyboard::GetButton('q')) {
		eye.z -= val;
	}
	if (Keyboard::GetButton('e')) {
		eye.z += val;
	}

	if (Keyboard::GetButton('j')) {
		look.x -= val;
	}
	if (Keyboard::GetButton('l')) {
		look.x += val;
	}
	if (Keyboard::GetButton('i')) {
		look.y += val;
	}
	if (Keyboard::GetButton('k')) {
		look.y -= val;
	}
	if (Keyboard::GetButton('u')) {
		look.z -= val;
	}
	if (Keyboard::GetButton('o')) {
		look.z += val;
	}

	cout << "eye:" << eye.x << "," << eye.y << "," << eye.z << endl;
	cout << "look:" << look.x << "," << look.y << "," << look.z << endl;
	camera.SetViewMatrix(eye,look);
#endif // defined UseCameraAction

#pragma endregion

	
}

/*!
	@fn		Render
	@brief	描画処理
*/
void SampleScene::Render()
{
#pragma region 後輩ちゃん
#ifdef use_multiple1
	for (auto& it : vMesh)
	{
		it.Render();
	}
#endif
#pragma endregion

	g_pMesh->Render();
	//g_pMesh1->Render();
}