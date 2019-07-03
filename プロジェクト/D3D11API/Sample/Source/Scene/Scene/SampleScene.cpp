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


using VMesh = vector<shared_ptr<Mesh>>;
VMesh g_vpMesh;
using VMaterial = vector<shared_ptr<Material>>;
VMaterial vMat;


/*!
	@var	g_pShader
	@brief	シェーダー
*/
shared_ptr<D3D11::Graphic::AbstractShader>g_pMeshShader;

/*!
	@var	g_pMaterial
	@brief	マテリアル
*/
shared_ptr<API::Material>g_pMaterial;

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
	g_pMaterial = make_shared<Material>();

	//複数メッシュ
	string texPath[14] =
	{
		"後輩ちゃん/skin.png",
		"後輩ちゃん/face.png",
		"後輩ちゃん/eye.png",
		"後輩ちゃん/face.png",
		"後輩ちゃん/face.png",
		"後輩ちゃん/face.png",
		"後輩ちゃん/face.png",
		"後輩ちゃん/face.png",
		"後輩ちゃん/face.png",
		"後輩ちゃん/hair.png",
		"後輩ちゃん/eye.png",
		"後輩ちゃん/clothes.png",
		"後輩ちゃん/clothes.png",
		"後輩ちゃん/facial.png",
	};
	for (int i = 0; i < 14; ++i) {

		auto mat = make_shared<Material>();
		mat->SetupTexture(texPath[i]);
		vMat.push_back(mat);

		auto inst = make_shared<Mesh>();
		inst->Initialize("後輩ちゃん/test00-" + to_string(i) + ".yfm");
		inst->SetupMaterial(mat.get());
		g_vpMesh.push_back(inst);
	}
	for (auto&it : g_vpMesh) { it->SetupShader(g_pMeshShader.get()); }
	g_pMaterial->SetupTexture("hoge.png");
	g_pMeshShader->Setup();

#pragma region Sample-Humanoid
	//g_pMesh->Initialize("humanoid.yfm");
	//g_pMesh->Initialize("humanoid.yfm");
	g_pMesh->Initialize("twin.yfm");
	g_pMesh->SetupMaterial(g_pMaterial.get());
	g_pMesh->SetupShader(g_pMeshShader.get());
#pragma endregion

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
	auto val = 0.1f;

#pragma region メッシュ
	auto& t = g_pMesh->transform;
#pragma region 回転

	//static float x = 0, y = 0, z = 0;

	//if (Keyboard::GetButton(Keyboard::c_Left)) {
	//	x -= val;
	//}
	//if (Keyboard::GetButton(Keyboard::c_Right)) {
	//	x += val;
	//}
	//if (Keyboard::GetButton(Keyboard::c_Up)) {
	//	y += val;
	//}
	//if (Keyboard::GetButton(Keyboard::c_Down)) {
	//	y -= val;
	//}
	//if (Keyboard::GetButton(Keyboard::c_Back)) {
	//	z -= val;
	//}
	//if (Keyboard::GetButton(Keyboard::c_Delete)) {
	//	z += val;
	//}
	//t->Rotate(x,y,z);
#pragma endregion

#pragma region 移動
	auto pos = g_pMesh->transform->GetPosition();

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
	g_pMesh->transform->SetPosition(pos);

#pragma endregion

#pragma region 拡縮
	static auto scale = t->GetScale();
	if (Keyboard::GetButton('Z')) {
		scale.x -= val * 0.01f;
		scale.y -= val * 0.01f;
		scale.z -= val * 0.01f;
	}
	if (Keyboard::GetButton('X')) {
		scale.x += val * 0.01f;
		scale.y += val * 0.01f;
		scale.z += val * 0.01f;
	}
	t->SetScale(scale);
#pragma endregion
	if (Keyboard::GetButton(Keyboard::c_Back)) {
		auto cp = Camera::GetInstance().GetEyePt();
		auto cl = Camera::GetInstance().GetLookAtPt();
		system("pause");
	}

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
	//for (auto it : g_vpMesh) { it->Render(); }

	g_pMesh->Render();
}