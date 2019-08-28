#include "BillboardSample.h"
#include <SpriteShader.h>
#include <MeshShader.h>
#include <Camera.h>
#include <Keyboard.h>
#include <GeometryFactor.h>
#include "../Root/SceneRoot.h"
#include "AnimationSample.h"
#include "StaticMeshSample.h"
#include <MyGame.h>
using namespace std;
using namespace API;
using namespace D3D11::Graphic;

BillboardSample::BillboardSample()
{
}

BillboardSample::~BillboardSample()
{
}

void BillboardSample::Reset()
{
	switch (m_eEyeType)
	{
	case BillboardSample::FPS:
		break;
	case BillboardSample::TPS:
		m_CameraPos = { 0,0,-10 };
		break;
	default:
		break;
	}

	m_pMesh->transform->SetPosition({ -1,0,0 });
	m_MeshRot = { 0,0,0 };
	elapsedCount = 0;
}

void BillboardSample::Initialize()
{
	m_pTexture = make_shared<Texture>();
	m_pSprite = make_shared<Sprite>();
	m_pShader = make_shared<SpriteShader>();
	m_pMesh = GeometryFactor::GetInstance().CreateCube();
	m_pMeshMaterial = make_shared<Material>();
	m_pMeshShader = make_shared<D3D11::Graphic::MeshShader>();
	m_pMeshShader->Setup();
	m_pMesh->SetupShader(m_pMeshShader.get());
	m_pMeshMaterial->SetupTexture("white.png");
	m_pMesh->SetupMaterial(m_pMeshMaterial.get());
	m_pBillboradTarget = m_pMesh->transform;
	m_pSprite->Initialize(m_pTexture.get());
	m_pShader->Setup();
	m_pTexture->Initialize("ncc.png", { 400,400 });
	m_pSprite->SetupShader(m_pShader.get());
	m_pSprite->SetupBil(m_pBillboradTarget);
	m_pSprite->transform->SetPosition({ 5,0,0 });
	m_eEyeType = EyeType::TPS;
	automatic = false;
	//	座標データの初期化
	Reset();

	Camera::GetInstance().transform->SetPosition(m_CameraPos);
}

void BillboardSample::Finalize()
{
	
}

void BillboardSample::Update()
{
	using kb = Keyboard;
#pragma region ウィンドウ名
	SetWindowText(GetActiveWindow(), const_cast<LPTSTR>(To_TString("ビルボードと画像描画のサンプル").c_str()));
#pragma endregion


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
	if (automatic)
	{
		elapsedCount += 0.05f;
		auto pos = m_pMesh->transform->GetPosition();
		pos.y = sin(elapsedCount);
		m_pMesh->transform->SetPosition(pos);
	}
	else
	{
		auto&meshTrans = m_pMesh->transform;
		auto meshPos = meshTrans->GetPosition();
		float meshValue = 0.1f;
		if (kb::GetButton(kb::c_Shift)) {
			if (kb::GetButton('a') || kb::GetButton(kb::c_Left)) {
				m_MeshRot.x -= meshValue;
			}
			if (kb::GetButton('d') || kb::GetButton(kb::c_Right)) {
				m_MeshRot.x += meshValue;
			}
			if (kb::GetButton('w') || kb::GetButton(kb::c_Up)) {
				m_MeshRot.y += meshValue;
			}
			if (kb::GetButton('s') || kb::GetButton(kb::c_Down)) {
				m_MeshRot.y -= meshValue;
			}
			if (kb::GetButton('q')) {
				m_MeshRot.z -= meshValue;
			}
			if (kb::GetButton('e')) {
				m_MeshRot.z += meshValue;
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
		meshTrans->SetRotation(m_MeshRot);
	}
#endif // 1

#pragma endregion

#pragma region 視点切り替え
#if 1
	switch (m_eEyeType)
	{
	case BillboardSample::FPS:
		DirectX::XMFLOAT3 camPos = m_pMesh->transform->GetPosition();
		{
			//	スプライトとメッシュの方向ベクトル
			DirectX::XMVECTOR dirVec = 
			{
				m_pSprite->transform->GetPosition().x - m_pMesh->transform->GetPosition().x,
				m_pSprite->transform->GetPosition().y - m_pMesh->transform->GetPosition().y,
				m_pSprite->transform->GetPosition().z - m_pMesh->transform->GetPosition().z,
			};
			//	ベクトルを正規化
			dirVec = DirectX::XMVector3Normalize(dirVec);

			//	スカラー値
			float scalr = 0.75f;
			camPos.x += dirVec.m128_f32[0] * scalr;
			camPos.y += dirVec.m128_f32[1] * scalr;
			camPos.z += dirVec.m128_f32[2] * scalr;

			DirectX::XMFLOAT3 lookPt = m_pSprite->transform->GetPosition();
			Camera::GetInstance().SetViewMatrix(camPos, lookPt);
		}

		break;
	case BillboardSample::TPS:
		Camera::GetInstance().SetViewMatrix(m_CameraPos, { 0,0,0 });
		break;
	default:
		break;
	}

#endif // 1
	if (kb::GetButtonDown('X')) {
		m_eEyeType = m_eEyeType == EyeType::FPS ? EyeType::TPS : EyeType::FPS;
	}
#pragma endregion
	if (kb::GetButtonDown('c')) { automatic = !automatic; }
	if (kb::GetButtonDown('R')) { Reset(); }
	if (kb::GetButtonDown('n')) { SceneRoot::GetInstance().SetupNextScene(std::make_unique<AnimationSample>()); }
	if (kb::GetButtonDown('m')) { SceneRoot::GetInstance().SetupNextScene(std::make_unique<StaticMeshSample>()); }
}

void BillboardSample::Render()
{
	m_pMesh->Render();
	if(m_pBillboradTarget)
	{
		m_pSprite->RenderBillboard();
	}
}