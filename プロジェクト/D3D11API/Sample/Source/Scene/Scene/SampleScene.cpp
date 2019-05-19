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

Hoge*gh;

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
	@var	g_pSprite
	@brief	�X�v���C�g
*/
shared_ptr<Sprite>g_pSprite;
shared_ptr<Sprite>g_pSprites[3];

/*!
	@var	g_pTexture
	@brief	�e�N�X�`��
*/
shared_ptr<Texture>g_pTexture;
shared_ptr<Texture>g_pTextures[3];

/*!
	@var	g_pMesh
	@brief	���b�V��
*/
shared_ptr<Mesh>g_pMesh;

/*!
	@var	g_pShader
	@brief	�V�F�[�_�[
*/
shared_ptr<D3D11::Graphic::AbstractShader>g_pShader;
shared_ptr<D3D11::Graphic::AbstractShader>g_pMeshShader;

API::Mesh* gpMesh;

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
	
#pragma region Sprite
	g_pSprite	= make_shared<Sprite>();
	for (auto& it : g_pSprites)
	{
		it= make_shared<Sprite>();
	}

	g_pTexture	= make_shared<Texture>();
	for (auto& it : g_pTextures)
	{
		it = make_shared<Texture>();
	}

	g_pShader	= make_shared<D3D11::Graphic::SpriteShader>();

	//	�v���R���p�C���ς݃V�F�[�_�[�͎g�p�������I�R���p�C�����s��
	if (FAILED(g_pShader->DynamicSetup())) {
		ErrorLog("���s");
	}

	g_pTexture->Initialize("Resources/red.png");
	g_pTextures[0]->Initialize("Resources/blue.png");

	g_pSprite->SetupShader(g_pShader.get());
	g_pSprite->SetupTexture(g_pTexture.get());

	g_pSprites[0]->SetupShader(g_pShader.get());
	g_pSprites[0]->SetupTexture(g_pTextures[0].get());
#pragma endregion

#pragma region Mesh
	g_pMesh = make_shared<Mesh>();
	//if (FAILED(g_pMesh->Initialize("test.yfm"))) {
	//	exit(0);
	//}

#pragma endregion
	HRESULT hr = g_pMesh->Initialize();
	//g_pMeshShader = make_shared<D3D11::Graphic::MeshShader>();
	//if (FAILED(g_pMeshShader->Setup())) {
	//@	exit(0);
	//}

	//g_pMesh->SetupShader(g_pMeshShader.get());


	gpMesh = new API::Mesh();
	gpMesh->Initialize();

	gh = new Hoge();
	gh->Init();
}

/*!
	@fn		Finalize
	@brief	�j������
*/
void SampleScene::Finalize()
{
	cout << "sample destroy:" << endl;
	delete gh;
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

	auto pos = g_pSprite->transform->GetPosition();


	if (Keyboard::GetButtonDown('a')) {
		pos.x--;
	}
	if (Keyboard::GetButtonDown('d')) {
		pos.x++;
	}
	if (Keyboard::GetButtonDown('w')) {
		pos.y++;
	}
	if (Keyboard::GetButtonDown('s')) {
		pos.y--;
	}
	if (Keyboard::GetButtonDown('q')) {
		pos.z--;
	}
	if (Keyboard::GetButtonDown('e')) {
		pos.z++;
	}


	g_pSprite->transform->SetPosition(pos);
}

/*!
	@fn		Render
	@brief	�`�揈��
*/
void SampleScene::Render()
{
	//g_pSprite->Render(g_pTexture.get());
	//g_pSprite->Render();
	//g_pSprites[0]->Render();

	//	���b�V��
	{
		//g_pMesh->Render();
		//gpMesh->Render();
	}

	//	���_�o�b�t�@���C���f�b�N�X�o�b�t�@
	{
		gh->Draw();
	}
}