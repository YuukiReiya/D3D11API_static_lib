#include "BillboardSample.h"
#include <SpriteShader.h>
#include <Camera.h>
#include <Keyboard.h>
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
	m_pTexture = make_shared<Texture>();
	m_pSprite = make_shared<Sprite>();
	m_pShader = make_shared<SpriteShader>();

	m_pBillboradTarget = Camera::GetInstance().transform;

	Camera::GetInstance().transform->SetPosition({ 0,0,-10 });

	m_pSprite->Initialize(m_pTexture.get());
	m_pShader->Setup();
	m_pTexture->Initialize("ncc.png", { 400,400 });
	m_pSprite->SetupShader(m_pShader.get());
	m_pSprite->SetupBil(m_pBillboradTarget);
}

void BillboardSample::Finalize()
{
	
}

void BillboardSample::Update()
{
	using kb = Keyboard;
#pragma region カメラ
#if 1
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
	auto&spriteTrans = m_pSprite->transform;
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


}

void BillboardSample::Render()
{
	if (!m_pBillboradTarget)
	{
		m_pSprite->Render();
	}
	else
	{
		m_pSprite->RenderBillboard();
	}
}