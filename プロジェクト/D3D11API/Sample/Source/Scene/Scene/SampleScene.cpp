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
#include <SpriteShader.h>
#include <Sprite.h>
#include <MyGame.h>
#include <MeshShader.h>
#include <Mesh.h>
#include <Hoge.h>
#include <Camera.h>

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

/*!
	@var	g_pMesh
	@brief	���b�V��
*/
shared_ptr<Mesh>g_pMesh;

/*!
	@var	g_pShader
	@brief	�V�F�[�_�[
*/
shared_ptr<D3D11::Graphic::AbstractShader>g_pMeshShader;

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

	g_pMesh = make_shared<Mesh>();
	g_pMeshShader = make_shared<D3D11::Graphic::MeshShader>();

	g_pMesh->Initialize("test.yfm", "hoge.png");
	//g_pMesh->Initialize("cube.yfm", "hoge.png");

	//g_pMesh->Initialize("�`��e�X�g/test_draw.yfm", "hoge.png");
	//g_pMesh->Initialize("abc.yfm");
	g_pMeshShader->Setup();
	g_pMesh->SetupShader(g_pMeshShader.get());
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
	@brief	�`�揈��
*/
void SampleScene::Render()
{
	g_pMesh->Render();
}