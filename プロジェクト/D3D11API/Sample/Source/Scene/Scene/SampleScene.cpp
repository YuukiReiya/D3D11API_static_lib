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
#include <SkinMesh.h>
#include <Camera.h>
#include <MyGame.h>
#include <Mesh.h>
#include <MeshShader.h>

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

//shared_ptr<SkinMesh>g_pMesh;
//shared_ptr<AnimationClip>g_pAnimClip;

vector<shared_ptr<SkinMesh>>g_pMeshs;
vector<shared_ptr<AnimationClip>>g_pAnimClips;
vector< shared_ptr<Material>>g_pMats;


shared_ptr<Mesh>g_stMesh;
shared_ptr<D3D11::Graphic::MeshShader>g_stShader;
shared_ptr<Material>g_stMat;
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
	Camera::GetInstance().SetViewMatrix({ 0,0,-10 }, { 0,0,0 });
	//g_pMesh = make_shared<SkinMesh>();
	//g_pMesh->Init();
	//g_pMesh->transform->SetScale(0.01f);
	//g_pAnimClip = make_shared<AnimationClip>();
	//if (g_pAnimClip->Load("anim-0.ac")) {
	//	g_pMesh->SetupAnimation(g_pAnimClip);
	//}

	constexpr int c_MeshCount = 5;
	constexpr string_view c_MeshName = "newWolf";
	for (int i = 1; i <= c_MeshCount; ++i)
	{
		shared_ptr<AnimationClip>ac = make_shared<AnimationClip>();
		if (!ac->Load(string(c_MeshName.data()) + "/" + to_string(i) + "/anim-0.ac"))
		{
			ErrorLog("アニメーション読み込み失敗");
		}
		g_pAnimClips.push_back(ac);

		shared_ptr<SkinMesh>sm = make_shared<SkinMesh>();
		if (FAILED(sm->Initialize(string(c_MeshName.data()) + "/" + to_string(i) + "/" + c_MeshName.data() + ".yfm")))
		{
			ErrorLog("読み込み失敗");
		}

		if (i == 1) {
			//マテリアル
			shared_ptr<Material>mat = make_shared<Material>();
			string texPath = "newWolf/Wolf_Body.jpg";
			mat->SetupTexture(texPath);
			sm->SetupMaterial(mat);
			g_pMats.push_back(mat);
		}

		//	設定
		sm->SetupAnimation(ac);
		g_pMeshs.push_back(sm);

	}
	cout << "e" << g_pMeshs.size();

	////////////////////////////////////////////////////
	//	STATIC 
	//g_stMat = make_shared<Material>();
	//g_stMesh = make_shared<Mesh>();
	//g_stShader = make_shared<D3D11::Graphic::MeshShader>();
	//g_stShader->Setup();
	//g_stMesh->Initialize("Miku/Miku.yfm");
	//g_stMat->SetupTexture("hoge.png");

	//g_stMesh->SetupMaterial(g_stMat.get());
	//g_stMesh->SetupShader(g_stShader.get());
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
	using kb = Keyboard;
	//if (kb::GetButton('M'))
	//{
	//	g_pMesh->AddAnimationFrame();
	//}
	float val = 0.1;
	static DirectX::XMFLOAT3 rot = { 0,0,0 };
	if (kb::GetButton('a'))
	{
		rot.x -= val;
	}
	if (kb::GetButton('d'))
	{
		rot.x += val;
	}
	if (kb::GetButton('s'))
	{
		rot.y -= val;
	}
	if (kb::GetButton('w'))
	{
		rot.y += val;
	}

	if (kb::GetButton('m'))
	{
		for (auto it : g_pMeshs)
		{
			it->AddAnimationFrame();
		}
	}

	for (auto it:g_pMeshs)
	{
		it->transform->Rotate(rot);
	}
}

/*!
	@fn		Render
	@brief	描画処理
*/
void SampleScene::Render()
{
	//g_pMesh->Render();

	for(auto it:g_pMeshs)
	{
		it->Render();
	}
}