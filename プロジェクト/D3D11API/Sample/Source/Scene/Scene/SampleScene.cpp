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

	constexpr int c_MeshCount = 14;
	constexpr string_view c_MeshName = "kouhai";
	for (int i = 1; i <= c_MeshCount; ++i)
	{
#if 0
		shared_ptr<AnimationClip>ac = make_shared<AnimationClip>();
		if (!ac->Load(string(c_MeshName.data()) + "/" + to_string(i) + "/anim-0.ac"))
		{
			ErrorLog("アニメーション読み込み失敗");
		}
		g_pAnimClips.push_back(ac);
#endif // 0

		shared_ptr<SkinMesh>sm = make_shared<SkinMesh>();
		if (FAILED(sm->Initialize(string(c_MeshName.data()) + "/" + to_string(i) + "/" + c_MeshName.data() + ".yfm")))
		{
			ErrorLog("読み込み失敗");
		}

		//sm->transform->SetScale(0.1f);

		//	設定
		//sm->SetupAnimation(ac);
		g_pMeshs.push_back(sm);
	}

	for (int i = 0; i < c_MeshCount; ++i)
	{
		shared_ptr<Material>mat = make_shared<Material>();
		string path = "kouhai/";
		if (i == 0 || i == 1 || i == 2 || i == 3 || i == 4 || i == 5 || i == 6 || i == 7 || i == 8 || i == 9 || i == 10 || i == 11 || i == 12 || i == 13)
		{
			path += "facial.png";
		}
		mat->SetupTexture(path);
		g_pMats.push_back(mat);
		g_pMeshs[i]->SetupMaterial(mat);
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
	float val = 0.01;
	float tVal = 0.1f;
	if (kb::GetButton('a'))
	{
		for (auto it : g_pMeshs)
		{
			auto pos = it->transform->GetPosition();
			pos.x -= tVal;
			it->transform->SetPosition(pos);
		}
	}
	if (kb::GetButton('d'))
	{
		for (auto it : g_pMeshs)
		{
			auto pos = it->transform->GetPosition();
			pos.x += tVal;
			it->transform->SetPosition(pos);
		}
	}
	if (kb::GetButton('s'))
	{
		for (auto it : g_pMeshs)
		{
			auto pos = it->transform->GetPosition();
			pos.y -= tVal;
			it->transform->SetPosition(pos);
		}
	}
	if (kb::GetButton('w'))
	{
		for (auto it : g_pMeshs)
		{
			auto pos = it->transform->GetPosition();
			pos.y += tVal;
			it->transform->SetPosition(pos);
		}
	}
	if (kb::GetButton('q'))
	{
		for (auto it : g_pMeshs)
		{
			auto pos = it->transform->GetPosition();
			pos.z -= tVal;
			it->transform->SetPosition(pos);
		}
	}
	if (kb::GetButton('e'))
	{
		for (auto it : g_pMeshs)
		{
			auto pos = it->transform->GetPosition();
			pos.z += tVal;
			it->transform->SetPosition(pos);
		}
	}

	static DirectX::XMFLOAT3 rot = { 0,0,0 };
	if (kb::GetButton('j'))
	{
		rot.x -= val;
	}
	if (kb::GetButton('l'))
	{
		rot.x += val;
	}
	if (kb::GetButton('k'))
	{
		rot.y -= val;
	}
	if (kb::GetButton('i'))
	{
		rot.y += val;
	}
	if (kb::GetButton('u'))
	{
		rot.z -= val;
	}
	if (kb::GetButton('o'))
	{
		rot.z += val;
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