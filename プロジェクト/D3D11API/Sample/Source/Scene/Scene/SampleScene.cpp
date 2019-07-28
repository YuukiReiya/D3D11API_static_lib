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
	@var	g_pTexture
	@brief	テクスチャ
*/
shared_ptr<Texture>g_pTexture;

/*!
	@var	g_pSprite
	@brief	スプライト
*/
shared_ptr<Sprite>g_pSprite;

/*!
	@var	g_pSpriteShader
	@brief	スプライトのシェーダー
*/
shared_ptr< D3D11::Graphic::AbstractShader>g_pSpriteShader;

/*!
	@var	g_pMesh
	@brief	メッシュ
*/
shared_ptr<Mesh>g_pMesh;
shared_ptr<Transform>g_pT;

/*!
	@var	g_pMeshShader
	@brief	シェーダー
*/
shared_ptr<D3D11::Graphic::AbstractShader>g_pMeshShader;

/*!
	@var	g_pMaterial
	@brief	マテリアル
*/
shared_ptr<Material>g_pMeshMaterial;

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
#include "BillboardSample.h"
void SampleScene::Initialize() 
{
	cout << "sample init" << endl;

	BillboardSample*ns=new BillboardSample;
	SceneRoot::GetInstance().SetupNextScene(ns);
	return;

#pragma region 作り直しSprite
#if 1
#define SPRITE_REMAKE
	g_pSprite = make_shared<Sprite>();
	g_pTexture = make_shared<Texture>();
	g_pSpriteShader = make_shared<D3D11::Graphic::SpriteShader>();

	g_pTexture->Initialize("ncc.png", { 400,400 });
	//g_pSprite->SetupTexture(g_pTexture.get());
	g_pSpriteShader->Setup();
	g_pSprite->SetupShader(g_pSpriteShader.get());

	g_pT = make_shared<Transform>();
	g_pSprite->SetupBil(g_pT);


	HRESULT hr = g_pSprite->Initialize(g_pTexture.get());
	if (FAILED(hr)) { ErrorLog("スプライトの初期化失敗"); }

	Camera::GetInstance().SetViewMatrix({-3,5,-10});
#endif // 0
#pragma endregion

#pragma region スプライト
#if 0
#define SPRITE_EXECUTE
	g_pSpriteShader = make_shared<D3D11::Graphic::SpriteShader>();
	g_pSprite		= make_shared<Sprite>();
	g_pTexture		= make_shared<Texture>();

	try
	{
		if (FAILED(g_pSpriteShader->Setup())) { throw runtime_error("shader init"); }
		g_pSprite->Initialize();
		g_pSprite->SetupShader(g_pSpriteShader.get());
		g_pSprite->SetupTexture(g_pTexture.get());
	}
	catch (const std::exception&e)
	{
		ErrorLog(e.what());
	}
#endif // 1
#pragma endregion

#pragma region メッシュ
#if 0
#define MESH_EXECUTE
	g_pMesh = make_shared<Mesh>();
	g_pMeshShader = make_shared<D3D11::Graphic::MeshShader>();
	g_pMeshMaterial = make_shared<Material>();

	//g_pMeshMaterial->SetupTexture("hoge.png");
	g_pMesh->Initialize("anim.yfm");
	g_pMeshShader->Setup();
	g_pMesh->SetupShader(g_pMeshShader.get());
	g_pMesh->SetupMaterial(g_pMeshMaterial.get());
	g_pMesh->transform->SetScale({ 0.01f });
#endif // 1
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
	return;
	//auto& trans = g_pMesh->transform;
	auto& trans = g_pSprite->transform;
	auto pos = trans->GetPosition();
	static DirectX::XMFLOAT3 rot = { 0,0,0 };
	auto scale = trans->GetScale();
	float val = 0.1f;

	if (Keyboard::GetButton(Keyboard::c_Shift)) {

		if (Keyboard::GetButton('a') || Keyboard::GetButton(Keyboard::c_Left))
		{
			rot.x -= val;
		}
		if (Keyboard::GetButton('d') || Keyboard::GetButton(Keyboard::c_Right))
		{
			rot.x += val;
		}
		if (Keyboard::GetButton('w') || Keyboard::GetButton(Keyboard::c_Up))
		{
			rot.y += val;
		}
		if (Keyboard::GetButton('s') || Keyboard::GetButton(Keyboard::c_Down))
		{
			rot.y -= val;
		}
		if (Keyboard::GetButton('q'))
		{
			rot.z -= val;
		}
		if (Keyboard::GetButton('e'))
		{
			rot.z += val;
		}
	}
	else if (Keyboard::GetButton(Keyboard::c_Tab))
	{
		auto& cam = Camera::GetInstance().transform;
		auto camPos = cam->GetPosition();
		if (Keyboard::GetButton('a') || Keyboard::GetButton(Keyboard::c_Left))
		{
			camPos.x -= val;
		}
		if (Keyboard::GetButton('d') || Keyboard::GetButton(Keyboard::c_Right))
		{
			camPos.x += val;
		}
		if (Keyboard::GetButton('w') || Keyboard::GetButton(Keyboard::c_Up))
		{
			camPos.y += val;
		}
		if (Keyboard::GetButton('s') || Keyboard::GetButton(Keyboard::c_Down))
		{
			camPos.y -= val;
		}
		if (Keyboard::GetButton('q'))
		{
			camPos.z -= val;
		}
		if (Keyboard::GetButton('e'))
		{
			camPos.z += val;
		}
		//cam->SetPosition(camPos);
		Camera::GetInstance().transform->SetPosition(camPos);
	}
	else {
		if (Keyboard::GetButton('a') || Keyboard::GetButton(Keyboard::c_Left))
		{
			pos.x -= val;
		}
		if (Keyboard::GetButton('d') || Keyboard::GetButton(Keyboard::c_Right))
		{
			pos.x += val;
		}
		if (Keyboard::GetButton('w') || Keyboard::GetButton(Keyboard::c_Up))
		{
			pos.y += val;
		}
		if (Keyboard::GetButton('s') || Keyboard::GetButton(Keyboard::c_Down))
		{
			pos.y -= val;
		}
		if (Keyboard::GetButton('q'))
		{
			pos.z -= val;
		}
		if (Keyboard::GetButton('e'))
		{
			pos.z += val;
		}
	}

	trans->SetPosition(pos);
	trans->SetRotation(rot);
}

/*!
	@fn		Render
	@brief	描画処理
*/
void SampleScene::Render()
{

#pragma region スプライトの作り直し
#ifdef SPRITE_REMAKE
	//g_pSprite->Render();
	g_pSprite->RenderBillboard();
#endif // SPRITE_REMAKE

#pragma endregion

#pragma region スプライト
#ifdef SPRITE_EXECUTE
	g_pSprite->Render();
#endif // 
#pragma endregion

#pragma region メッシュ
#ifdef MESH_EXECUTE
	g_pMesh->Render();
#endif
#pragma endregion


}