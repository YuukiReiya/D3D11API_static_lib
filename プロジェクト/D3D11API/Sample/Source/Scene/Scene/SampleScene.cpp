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
void SampleScene::Initialize() 
{
	cout << "sample init" << endl;

#pragma region スプライト
#if 1
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

	g_pMeshMaterial->SetupTexture("hoge.png");
	g_pMesh->Initialize("sprite.yfm");
	g_pMeshShader->Setup();
	g_pMesh->SetupShader(g_pMeshShader.get());
	g_pMesh->SetupMaterial(g_pMeshMaterial.get());
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
	//auto& trans = g_pSprite->transform;
	//auto rot->

	/*if (Keyboard::GetButton('a'))
	{
		trans->
	}*/
}

/*!
	@fn		Render
	@brief	描画処理
*/
void SampleScene::Render()
{
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