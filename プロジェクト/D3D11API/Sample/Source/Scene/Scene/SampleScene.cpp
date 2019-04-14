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
	@var	g_pShader
	@brief	�V�F�[�_�[
*/
shared_ptr<D3D11::Graphic::AbstractShader>g_pShader;

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
	if (FAILED(g_pShader->DynamicSetup())) {
		ErrorLog("���s");
	}
	g_pTexture->Initialize("Resources/red.png");
	g_pTextures[0]->Initialize("Resources/blue.png");



	g_pSprite->SetupShader(g_pShader.get());
	g_pSprite->SetupTexture(g_pTexture.get());

	g_pSprites[0]->SetupShader(g_pShader.get());
	g_pSprites[0]->SetupTexture(g_pTextures[0].get());
	g_pSprites[0]->SetPos({ -0.1f,0 });
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

	auto pos = g_pSprite->GetPos();


	if (Keyboard::GetButtonDown('a')) {
		pos.z--;
	}
	if (Keyboard::GetButtonDown('d')) {
		pos.z++;
	}
	if (Keyboard::GetButtonDown('w')) {
		pos.y--;
	}
	if (Keyboard::GetButtonDown('s')) {
		pos.y++;
	}
}

/*!
	@fn		Render
	@brief	�`�揈��
*/
void SampleScene::Render()
{
//	g_pSprite->Render(g_pTexture.get());
	g_pSprite->Render();
	g_pSprites[0]->Render();
}