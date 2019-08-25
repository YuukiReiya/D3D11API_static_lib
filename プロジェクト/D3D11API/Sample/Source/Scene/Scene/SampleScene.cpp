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

	for (int i = 1; i <= 5; ++i)
	{
		shared_ptr<AnimationClip>ac = make_shared<AnimationClip>();
		if (!ac->Load("newWolf/" + to_string(i) + "/anim-0.ac"))
		{
			ErrorLog("アニメーション読み込み失敗");
		}
		g_pAnimClips.push_back(ac);

		shared_ptr<SkinMesh>sm = make_shared<SkinMesh>();
		if (FAILED(sm->Initialize("newWolf/" + to_string(i) + "/newWolf.yfm")))
		{
			ErrorLog("読み込み失敗");
		}

		sm->SetupAnimation(ac);
		g_pMeshs.push_back(sm);

	}
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