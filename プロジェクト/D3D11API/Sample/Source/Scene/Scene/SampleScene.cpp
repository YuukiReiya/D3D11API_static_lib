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
#include <MyGame.h>
#include <Camera.h>
#include "BillboardSample.h"

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

#pragma region アニメーションメッシュ
#ifdef ANIMATION_MESH
	m_pRender = make_shared<AnimationMesh>();

	m_pRender->Initialize("frameHumanoid.yfm");
	m_pRender->transform->SetScale({ 0.01f });
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
	Transform* tmp = new Transform;
	SceneRoot::GetInstance().SetupNextScene(make_unique<BillboardSample>());
#ifdef ANIMATION_MESH
	auto& trans =  m_pRender->transform;
#else
	auto& trans = tmp;// g_pMesh->transform;
#endif // ANIMATION_MESH
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

#ifdef ANIMATION_MESH
	if (Keyboard::GetButton('m')) {
		m_pRender->AddFrameIndex();
	}
	if (Keyboard::GetButton('n')) {
		m_pRender->AddAnimIndex();
	}
#endif // ANIMAITON_MESH

	trans->SetPosition(pos);
	trans->SetRotation(rot);
	delete tmp;
}

/*!
	@fn		Render
	@brief	描画処理
*/
void SampleScene::Render()
{
#ifdef ANIMATION_MESH
	m_pRender->Render();
#endif // ANIMATION_MESH



}