#include "AnimationSample.h"
#include "../../ResorceLoader.h"
#include <Camera.h>
#include <Keyboard.h>
#include "../Root/SceneRoot.h"
#include "StaticMeshSample.h"

AnimationSample::AnimationSample()
{
}

AnimationSample::~AnimationSample()
{
}

void AnimationSample::Reset()
{
	m_MeshRot = { 0,0,0 };
	m_pMesh->transform->SetScale({ 0.01f });
	m_pMesh->transform->SetPosition({ 0.0f,-0.3f,-8.3 });
	m_pMesh->transform->SetRotation(m_MeshRot);
	animFrame = animFrame = 0;
}

void AnimationSample::Initialize()
{
	API::Camera::GetInstance().SetViewMatrix({ 0,0,-10 }, { 0,0,0 });
	m_pMesh = ResourceLoader::GetInstance().GetMesh();
	autoAnim = true;
	Reset();
}

void AnimationSample::Finalize()
{

}

void AnimationSample::Update()
{
	using kb = Keyboard;

	SetWindowText(GetActiveWindow(), const_cast<LPTSTR>(To_TString("モデルのアニメーションサンプル").c_str()));


#pragma region 自動アニメーション
	if (autoAnim)
	{
		constexpr int c_AnimFrame = 2;
		constexpr int c_AnimUpdateFrame = (278 - 1) *c_AnimFrame;
		if (c_AnimUpdateFrame<animFrame)
		{
			animFrame = 0;
			m_pMesh->AddAnimIndex();
		}
		if (animFrame%c_AnimFrame == 0) 
		m_pMesh->AddFrameIndex();
		animFrame++;
	}
#pragma endregion

#pragma region 手動アニメーション
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
#pragma endregion
	if (kb::GetButtonDown('X')) { autoAnim = !autoAnim; }
	if (kb::GetButtonDown('r')) { Reset(); }
	if (kb::GetButtonDown('n')) { 
		SceneRoot::GetInstance().SetupNextScene(std::make_unique<StaticMeshSample>());
	}
	if (kb::GetButtonDown('m')) {
		SceneRoot::GetInstance().SetupNextScene(std::make_unique<BillboardSample>());
	}

}

void AnimationSample::Render()
{
	m_pMesh->Render();
}