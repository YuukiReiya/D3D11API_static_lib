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
#include <../AnimationMesh.h>

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
shared_ptr<AnimationMesh>g_pAnimMesh;
/*!
	@var	g_pShader
	@brief	�V�F�[�_�[
*/
shared_ptr<D3D11::Graphic::AbstractShader>g_pMeshShader;

/*!
	@var	g_pMaterial
	@brief	�}�e���A��
*/
shared_ptr<API::Material>g_pMaterial;

/*!
	@brief	�R���X�g���N�^
*/
SampleScene::SampleScene()
{
}
std::vector<API::Mesh>vMesh;
std::vector<std::shared_ptr<API::Material>>vMaterial;

/*!
	@fn		Initialize
	@brief	����������
*/
void SampleScene::Initialize() 
{
	cout << "sample init" << endl;

	g_pMesh = make_shared<Mesh>();
	g_pMeshShader = make_shared<D3D11::Graphic::MeshShader>();
	g_pMaterial = make_shared<Material>();

	//g_pMesh->Initialize("mesh1.yfm");
	//g_pMesh->Initialize("test.yfm","TEX_00100_anathema_H_D.png");
	//g_pMesh->Initialize("test3.yfm", "hoge.png");
	//g_pMesh->Initialize("test4.yfm","TEX_00100_anathema_H_D.png");
	//g_pMesh->Initialize("test.yfm","pm0025_00_BodyA1.png");
	//g_pMesh->Initialize("test7.yfm", "hoge.png");
	//g_pMaterial->SetupTexture("hoge.png");
	//g_pMesh->Initialize("test8.yfm");
	//g_pMesh->SetupMaterial(g_pMaterial.get());
	//g_pMesh->Initialize("test99-2.yfm", "hoge.png");
	//g_pMesh->Initialize("cube.yfm", "hoge.png");

	//g_pMesh->Initialize("�`��e�X�g/test_draw.yfm", "hoge.png");
	//g_pMesh->Initialize("abc.yfm");


	g_pMeshShader->Setup();
	g_pMesh->SetupShader(g_pMeshShader.get());

#pragma region �s�J�`���E
	//std::string texPathes[4] = { "pm0025_00_BodyA1.png","pm0025_00_BodyB1.png","pm0025_00_Eye1.png","pm0025_00_Mouth1.png", };
	//for (int i = 0; i < 4; ++i)
	//{
	//	Mesh mesh;
	//	shared_ptr<Material> mat = make_shared<Material>();
	//	mesh.Initialize("�|�P����/pokemon-" + to_string(i) + ".yfm");
	//	mat->SetupTexture("�|�P����/" + texPathes[i]);
	//	vMesh.push_back(mesh);
	//	vMaterial.push_back(mat);
	//	vMesh[i].SetupMaterial(vMaterial[i].get());
	//}
#pragma endregion

#pragma region ��y�����
//
//	string texPath[14] =
//	{
//		"��y�����/skin.png",
//		"��y�����/face.png",
//		"��y�����/eye.png",
//		"��y�����/face.png",
//		"��y�����/face.png",
//		"��y�����/face.png",
//		"��y�����/face.png",
//		"��y�����/face.png",
//		"��y�����/face.png",
//		"��y�����/hair.png",
//		"��y�����/eye.png",
//		"��y�����/clothes.png",
//		"��y�����/clothes.png",
//		"��y�����/facial.png",
//	};
//	for (size_t i = 0; i < 14; i++)
//	{
//		Mesh ptr;
//		std::shared_ptr<Material>mat = make_shared<Material>();
//		ptr.Initialize("��y�����/test99-" + to_string(i) + ".yfm");
//		mat->SetupTexture(texPath[i]);
//		vMesh.push_back(ptr);
//		vMaterial.push_back(mat);
//		vMesh[i].SetupMaterial(vMaterial[i].get());
//	}
//	for (auto& it : vMesh)
//	{
//		it.SetupShader(g_pMeshShader.get());
//	}
//
#pragma endregion

#pragma region Sample-Humanoid

	//g_pMesh->Initialize("humanoid.yfm");
	g_pMesh->Initialize("anim.yfm");
	g_pMesh->SetupMaterial(g_pMaterial.get());
	g_pMesh->SetupShader(g_pMeshShader.get());
	g_pMesh->transform->SetScale({ 0.01f });

	g_pAnimMesh = make_shared<AnimationMesh>();
	if (FAILED(g_pAnimMesh->Init("test.yfm"))) {
		ErrorLog("this");
	}
	g_pAnimMesh->transform->SetScale({ 0.01f });

#pragma endregion


#pragma region SD-Unity
	//string texPath = "���j�e�B�����/utc_all2.png";
	//shared_ptr<Material>mat = make_shared<Material>();
	//mat->SetupTexture(texPath);
	//vMaterial.push_back(mat);
	//for (int i = 0; i < 12; ++i)
	//{
	//	Mesh ptr;
	//	ptr.Initialize("���j�e�B�����/sd-unity-" + to_string(i) + ".yfm");
	//	vMesh.push_back(ptr);
	//	vMesh[i].SetupMaterial(vMaterial[0].get());
	//}
#pragma endregion


	if (!vMesh.empty()) {
		for (auto&it : vMesh) { it.SetupShader(g_pMeshShader.get()); }
	}

}

/*!
	@fn		Finalize
	@brief	�j������
*/
void SampleScene::Finalize()
{
	cout << "sample destroy:" << endl;
	for (auto&it : vMesh) {
		it.Finalize();
	}
	vMesh.clear();
}
//#define UseCameraAction
/*!
	@fn		Update
	@brief	�X�V����
*/
void SampleScene::Update()
{
	auto val = 0.1f;

#pragma region ���b�V��
#ifndef UseCameraAction
	auto& t = g_pMesh->transform;
	//auto& t = g_pAnimMesh->transform;
#pragma region ��]

	static float x = 0, y = 0, z = 0;

	if (Keyboard::GetButton(Keyboard::c_Left)) {
		x -= val;
	}
	if (Keyboard::GetButton(Keyboard::c_Right)) {
		x += val;
	}
	if (Keyboard::GetButton(Keyboard::c_Up)) {
		y += val;
	}
	if (Keyboard::GetButton(Keyboard::c_Down)) {
		y -= val;
	}
	if (Keyboard::GetButton(Keyboard::c_Back)) {
		z -= val;
	}
	if (Keyboard::GetButton(Keyboard::c_Delete)) {
		z += val;
	}
	t->Rotate(x,y,z);

	if (!vMesh.empty()) { for (auto it : vMesh) { it.transform->Rotate(x, y, z); } }
#pragma endregion

#pragma region �ړ�
	auto pos = t->GetPosition();

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
	t->SetPosition(pos);

	if (!vMesh.empty()) {
		for (auto it : vMesh) {
			auto pos = it.transform->GetPosition();
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
			it.transform->SetPosition(pos);
		}
	}
#pragma endregion

#pragma region �g�k
	static auto scale = t->GetScale();
	if (Keyboard::GetButton('Z')) {
		scale.x -= val * 0.01f;
		scale.y -= val * 0.01f;
		scale.z -= val * 0.01f;
	}
	if (Keyboard::GetButton('X')) {
		scale.x += val * 0.01f;
		scale.y += val * 0.01f;
		scale.z += val * 0.01f;
	}
	t->SetScale(scale);
#pragma endregion

#endif // !UseCameraAction
#pragma endregion

#pragma region �J��������
#if defined UseCameraAction
	auto& camera = Camera::GetInstance();
	auto eye = camera.GetEyePt();
	auto look = camera.GetLookAtPt();
	
	if (Keyboard::GetButton(Keyboard::c_Left)) {
		//eye.x -= val;
		look.x -= val;
	}
	if (Keyboard::GetButton(Keyboard::c_Right)) {
		//eye.x += val;
		look.x += val;
	}
	if (Keyboard::GetButton(Keyboard::c_Up)) {
		//eye.y += val;
		look.y -= val;
	}
	if (Keyboard::GetButton(Keyboard::c_Down)) {
		//eye.y -= val;
		look.y += val;
	}
	if (Keyboard::GetButton('a')) {
		eye.x -= val;
	}
	if (Keyboard::GetButton('d')) {
		eye.x += val;
	}
	if (Keyboard::GetButton('w')) {
		eye.y += val;
	}
	if (Keyboard::GetButton('s')) {
		eye.y -= val;
	}
	if (Keyboard::GetButton('q')) {
		eye.z -= val;
	}
	if (Keyboard::GetButton('e')) {
		eye.z += val;
	}

	if (Keyboard::GetButton('j')) {
		look.x -= val;
	}
	if (Keyboard::GetButton('l')) {
		look.x += val;
	}
	if (Keyboard::GetButton('i')) {
		look.y += val;
	}
	if (Keyboard::GetButton('k')) {
		look.y -= val;
	}
	if (Keyboard::GetButton('u')) {
		look.z -= val;
	}
	if (Keyboard::GetButton('o')) {
		look.z += val;
	}

	cout << "eye:" << eye.x << "," << eye.y << "," << eye.z << endl;
	cout << "look:" << look.x << "," << look.y << "," << look.z << endl;
	camera.SetViewMatrix(eye,look);
#endif // defined UseCameraAction

#pragma endregion

	
#pragma region �A�j���[�V����
	auto& animMesh = g_pAnimMesh;

	if (Keyboard::GetButtonDown('N')) {
		animMesh->AddAnimIndex();
	}
	if (Keyboard::GetButtonDown('M')) {
		animMesh->AddFrameIndex();
	}

#pragma endregion
}

/*!
	@fn		Render
	@brief	�`�揈��
*/
void SampleScene::Render()
{
#pragma region ��y�����
#ifdef use_multiple1
	for (auto& it : vMesh)
	{
		it.Render();
	}
#endif
#pragma endregion



	if (!vMesh.empty())
	{
		for (auto it : vMesh) { it.Render(); }
#pragma region Unity-Chan
		//vMesh[1].Render();
		//vMesh[2].Render();
#pragma endregion
#pragma region �s�J�`���E
		//vMesh[0].Render();	//	����
		//vMesh[1].Render();		//	
		//vMesh[2].Render();
		//vMesh[3].Render();
		//for (int i = 0; i < 1;++i) { vMesh[i].Render(); }
#pragma endregion
	}

	//g_pMesh->Render();
	g_pAnimMesh->Render();
	//g_pMesh1->Render();
}