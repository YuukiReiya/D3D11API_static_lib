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
#include <Sprite.h>

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

/*!
	@var	g_pTexture
	@brief	�e�N�X�`��
*/
shared_ptr<Texture>g_pTexture;

/*!
	@brief	�R���X�g���N�^
*/
SampleScene::SampleScene()
{
}

shared_ptr<Sprite> sprite;
shared_ptr<D3D11::Graphic::AbstractShader>shader;
shared_ptr<Texture> tex;
/*!
	@fn		Initialize
	@brief	����������
*/
void SampleScene::Initialize() 
{
	cout << "sample init" << endl;
	
	g_pSprite = make_shared<Sprite>();
	g_pTexture = make_shared<Texture>();

	g_pTexture->Initialize("Resources/red.png");
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
	g_pSprite->Render(g_pTexture.get());
}