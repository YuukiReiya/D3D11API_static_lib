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
	@var	g_pTexture
	@brief	�e�N�X�`��
*/
shared_ptr<Texture>g_pTexture;

/*!
	@var	g_pSprite
	@brief	�X�v���C�g
*/
shared_ptr<Sprite>g_pSprite;

/*!
	@var	g_pSpriteShader
	@brief	�X�v���C�g�̃V�F�[�_�[
*/
shared_ptr< D3D11::Graphic::AbstractShader>g_pSpriteShader;

/*!
	@var	g_pMesh
	@brief	���b�V��
*/
shared_ptr<Mesh>g_pMesh;

/*!
	@var	g_pMeshShader
	@brief	�V�F�[�_�[
*/
shared_ptr<D3D11::Graphic::AbstractShader>g_pMeshShader;

/*!
	@var	g_pMaterial
	@brief	�}�e���A��
*/
shared_ptr<Material>g_pMeshMaterial;

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

#pragma region ��蒼��Sprite
#define SPRITE_REMAKE
	g_pSprite = make_shared<Sprite>();

	HRESULT hr = g_pSprite->Init();
	if (FAILED(hr)) { ErrorLog("�X�v���C�g�̏��������s"); }

	auto p = Camera::GetInstance().GetEyePt();
#pragma endregion


#pragma region �X�v���C�g
#if 0
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

#pragma region ���b�V��
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
	auto& trans = g_pSprite->transform;
	auto pos = trans->GetPosition();
	static DirectX::XMFLOAT3 rot = { 0,0,0 };
	auto scale = trans->GetScale();
	float val = 0.1f;
	if (Keyboard::GetButton('a'))
	{
		pos.x -= val;
	}
	if (Keyboard::GetButton('d'))
	{
		pos.x += val;
	}
	if (Keyboard::GetButton('w'))
	{
		pos.y += val;
	}
	if (Keyboard::GetButton('s'))
	{
		pos.y -= val;
	}
	trans->SetPosition(pos);
	if (Keyboard::GetButton(Keyboard::c_Left))
	{
		rot.x -= val;
	}
	if (Keyboard::GetButton(Keyboard::c_Right))
	{
		rot.x += val;
	}
	if (Keyboard::GetButton(Keyboard::c_Up))
	{
		rot.y += val;
	}
	if (Keyboard::GetButton(Keyboard::c_Down))
	{
		rot.y -= val;
	}
	trans->SetRotation(rot);
}

/*!
	@fn		Render
	@brief	�`�揈��
*/
void SampleScene::Render()
{

#pragma region �X�v���C�g�̍�蒼��
#ifdef SPRITE_REMAKE
	g_pSprite->Render();
#endif // SPRITE_REMAKE

#pragma endregion

#pragma region �X�v���C�g
#ifdef SPRITE_EXECUTE
	g_pSprite->Render();
#endif // 
#pragma endregion

#pragma region ���b�V��
#ifdef MESH_EXECUTE
	g_pMesh->Render();
#endif
#pragma endregion


}