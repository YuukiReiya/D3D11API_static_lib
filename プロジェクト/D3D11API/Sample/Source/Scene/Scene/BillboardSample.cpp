#include "BillboardSample.h"
#include <SpriteShader.h>
#include <MeshShader.h>
#include <Camera.h>
#include <Keyboard.h>
#include <GeometryFactor.h>
using namespace std;
using namespace API;
using namespace D3D11::Graphic;

BillboardSample::BillboardSample()
{
}

BillboardSample::~BillboardSample()
{
}

void BillboardSample::Initialize()
{
	m_pBTTexture = make_shared<Texture>();
	m_pBTSprite = make_shared<Sprite>();
	m_pTexture = make_shared<Texture>();
	m_pSprite = make_shared<Sprite>();
	m_pShader = make_shared<SpriteShader>();

#pragma region ビルボードターゲット
#if 0
#define SPRITE_TARGET
	m_pBTTexture->Initialize("hoge.png", {640,640});
	m_pBillboradTarget = m_pBTSprite->transform;
	m_pBTSprite->SetupShader(m_pShader.get());
	m_pBTSprite->Initialize(m_pBTTexture.get());
	m_pBTSprite->transform->SetPosition({ -3,0,0 });
	m_pBTSprite->transform->SetScale({ 0.5f });

	m_pSprite->transform->SetPosition({ 3,0,0 });
#endif // 1
#if 1
#define MESH_TARGET
	m_pMesh = GeometryFactor::GetInstance().CreateCube();
	m_pMeshMaterial = make_shared<Material>();
	m_pMeshShader = make_shared<D3D11::Graphic::MeshShader>();
	m_pMeshShader->Setup();
	m_pMesh->SetupShader(m_pMeshShader.get());
	m_pMeshMaterial->SetupTexture("white.png");
	m_pMesh->SetupMaterial(m_pMeshMaterial.get());

	m_pMesh->transform->SetPosition({ -1,0,0 });

	m_pBillboradTarget = m_pMesh->transform;
#endif // 1

#pragma endregion

	Camera::GetInstance().transform->SetPosition({ 0,0,-10 });

	m_pSprite->Initialize(m_pTexture.get());
	m_pShader->Setup();
	m_pTexture->Initialize("ncc.png", { 400,400 });
	m_pSprite->SetupShader(m_pShader.get());
	m_pSprite->SetupBil(m_pBillboradTarget);
	m_pSprite->transform->SetPosition({ 1,0,0 });
}

void BillboardSample::Finalize()
{
	
}

void BillboardSample::Update()
{
	using kb = Keyboard;
#pragma region カメラ
#if 0
	auto&camera = Camera::GetInstance();
	auto& camTrans = Camera::GetInstance().transform;
	auto camPos = camTrans->GetPosition();
	float camValue = 1.1f;

	if (kb::GetButton(kb::c_Shift)){
		if (kb::GetButton('a') || kb::GetButton(kb::c_Left)) {

		}
		if (kb::GetButton('d') || kb::GetButton(kb::c_Right)) {

		}
		if (kb::GetButton('w') || kb::GetButton(kb::c_Up)) {

		}
		if (kb::GetButton('s') || kb::GetButton(kb::c_Down)) {

		}
		if (kb::GetButton('q')) {

		}
		if (kb::GetButton('e')) {

		}

	}
	else if (kb::GetButton(kb::c_Tab)){
		if (kb::GetButton('a') || kb::GetButton(kb::c_Left)) {

		}
		if (kb::GetButton('d') || kb::GetButton(kb::c_Right)) {

		}
		if (kb::GetButton('w') || kb::GetButton(kb::c_Up)) {

		}
		if (kb::GetButton('s') || kb::GetButton(kb::c_Down)) {

		}
		if (kb::GetButton('q')) {

		}
		if (kb::GetButton('e')) {

		}
	}
	else {
		if (kb::GetButton('a') || kb::GetButton(kb::c_Left)) {
			camPos.x -= camValue;
		}
		if (kb::GetButton('d') || kb::GetButton(kb::c_Right)) {
			camPos.x += camValue;
		}
		if (kb::GetButton('w') || kb::GetButton(kb::c_Up)) {
			camPos.y += camValue;
		}
		if (kb::GetButton('s') || kb::GetButton(kb::c_Down)) {
			camPos.y -= camValue;
		}
		if (kb::GetButton('q')) {
			camPos.z -= camValue;
		}
		if (kb::GetButton('e')) {
			camPos.z += camValue;
		}
	}
	camTrans->SetPosition(camPos);
	camera.SetViewMatrix();
#endif // 1
#pragma endregion

#pragma region スプライト
#if 0
	//auto&spriteTrans = m_pSprite->transform;
	auto&spriteTrans = m_pBTSprite->transform;
	auto spPos = spriteTrans->GetPosition();
	float spValue = 0.1f;
	if (kb::GetButton(kb::c_Shift)) {
		if (kb::GetButton('a') || kb::GetButton(kb::c_Left)) {
	
		}
		if (kb::GetButton('d') || kb::GetButton(kb::c_Right)) {
		
		}
		if (kb::GetButton('w') || kb::GetButton(kb::c_Up)) {

		}
		if (kb::GetButton('s') || kb::GetButton(kb::c_Down)) {

		}
		if (kb::GetButton('q')) {

		}
		if (kb::GetButton('e')) {

		}

	}
	else if (kb::GetButton(kb::c_Tab)) {
		if (kb::GetButton('a') || kb::GetButton(kb::c_Left)) {

		}
		if (kb::GetButton('d') || kb::GetButton(kb::c_Right)) {

		}
		if (kb::GetButton('w') || kb::GetButton(kb::c_Up)) {

		}
		if (kb::GetButton('s') || kb::GetButton(kb::c_Down)) {

		}
		if (kb::GetButton('q')) {

		}
		if (kb::GetButton('e')) {

		}
	}
	else {
		if (kb::GetButton('a') || kb::GetButton(kb::c_Left)) {
			spPos.x -= spValue;
		}
		if (kb::GetButton('d') || kb::GetButton(kb::c_Right)) {
			spPos.x += spValue;
		}
		if (kb::GetButton('w') || kb::GetButton(kb::c_Up)) {
			spPos.y += spValue;
		}
		if (kb::GetButton('s') || kb::GetButton(kb::c_Down)) {
			spPos.y -= spValue;
		}
		if (kb::GetButton('q')) {
			spPos.z -= spValue;
		}
		if (kb::GetButton('e')) {
			spPos.z += spValue;
		}
	}
	spriteTrans->SetPosition(spPos);
#endif // 1
#pragma endregion

#pragma region メッシュ
#if 1
	auto&meshTrans = m_pMesh->transform;
	auto meshPos = meshTrans->GetPosition();
	static DirectX::XMFLOAT3 meshRot = { 0,0,0 };
	float meshValue = 0.1f;
	if (kb::GetButton(kb::c_Shift)) {
		if (kb::GetButton('a') || kb::GetButton(kb::c_Left)) {
			meshRot.x -= meshValue;
		}
		if (kb::GetButton('d') || kb::GetButton(kb::c_Right)) {
			meshRot.x += meshValue;
		}
		if (kb::GetButton('w') || kb::GetButton(kb::c_Up)) {
			meshRot.y += meshValue;
		}
		if (kb::GetButton('s') || kb::GetButton(kb::c_Down)) {
			meshRot.y -= meshValue;
		}
		if (kb::GetButton('q')) {
			meshRot.z -= meshValue;
		}
		if (kb::GetButton('e')) {
			meshRot.z += meshValue;
		}
	}
	else if (kb::GetButton(kb::c_Tab)) {
		if (kb::GetButton('a') || kb::GetButton(kb::c_Left)) {

		}
		if (kb::GetButton('d') || kb::GetButton(kb::c_Right)) {

		}
		if (kb::GetButton('w') || kb::GetButton(kb::c_Up)) {

		}
		if (kb::GetButton('s') || kb::GetButton(kb::c_Down)) {

		}
		if (kb::GetButton('q')) {

		}
		if (kb::GetButton('e')) {

		}
	}
	else {
		if (kb::GetButton('a') || kb::GetButton(kb::c_Left)) {
			meshPos.x -= meshValue;
		}
		if (kb::GetButton('d') || kb::GetButton(kb::c_Right)) {
			meshPos.x += meshValue;
		}
		if (kb::GetButton('w') || kb::GetButton(kb::c_Up)) {
			meshPos.y += meshValue;
		}
		if (kb::GetButton('s') || kb::GetButton(kb::c_Down)) {
			meshPos.y -= meshValue;
		}
		if (kb::GetButton('q')) {
			meshPos.z -= meshValue;
		}
		if (kb::GetButton('e')) {
			meshPos.z += meshValue;
		}
	}
	meshTrans->SetPosition(meshPos);
	meshTrans->SetRotation(meshRot);
#endif // 1

#pragma endregion
}

void BillboardSample::Render()
{
#pragma region ビルボードテスト
#ifdef SPRITE_TARGET
	m_pBTSprite->Render();

	m_pSprite->RenderBillboard();
#endif // DEBUG

#ifdef MESH_TARGET
	m_pMesh->Render();
#endif
#pragma endregion

	if (!m_pBillboradTarget)
	{
	//	m_pSprite->Render();
	}
	else
	{
		m_pSprite->RenderBillboard();
	}
}