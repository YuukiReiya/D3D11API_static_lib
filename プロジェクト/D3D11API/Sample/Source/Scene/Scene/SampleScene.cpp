/*!
	@file	SampleScene.cpp
	@date	2019/03/28
	@author	�ԏ� �G�P
	@brief	�T���v���V�[��
	@detail	���C�u�����̃e�X�g���s���T���v���p�̃V�[��
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
	@brief	using�f�B���N�e�B�u
	@using	std
*/
using namespace std;

/*!
	@brief	using�f�B���N�e�B�u
	@using	API
*/
using namespace API;

//shared_ptr<SkinMesh>g_pMesh;
//shared_ptr<AnimationClip>g_pAnimClip;

vector<shared_ptr<SkinMesh>>g_pMeshs;
vector<shared_ptr<AnimationClip>>g_pAnimClips;
/*!
	@brief	�R���X�g���N�^
*/
SampleScene::SampleScene()
{
}

/*!
	@fn		Initialize
	@brief	����������
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
			ErrorLog("�A�j���[�V�����ǂݍ��ݎ��s");
		}
		g_pAnimClips.push_back(ac);

		shared_ptr<SkinMesh>sm = make_shared<SkinMesh>();
		if (FAILED(sm->Initialize("newWolf/" + to_string(i) + "/newWolf.yfm")))
		{
			ErrorLog("�ǂݍ��ݎ��s");
		}

		sm->SetupAnimation(ac);
		g_pMeshs.push_back(sm);

	}
}

/*!
	@fn		Finalize
	@brief	�j������
*/
void SampleScene::Finalize()
{
	cout << "sample destroy:" << endl;
}

/*!
	@fn		Update
	@brief	�X�V����
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
	@brief	�`�揈��
*/
void SampleScene::Render()
{
	//g_pMesh->Render();

	for(auto it:g_pMeshs)
	{
		it->Render();
	}
}