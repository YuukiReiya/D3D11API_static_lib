#include "StaticMeshSample.h"
#include <Keyboard.h>
#include <MeshShader.h>
#include "../../ResorceLoader.h"
#include "../Root/SceneRoot.h"
#include "BillboardSample.h"
#include "AnimationSample.h"

StaticMeshSample::StaticMeshSample()
{
}

StaticMeshSample::~StaticMeshSample()
{
}

void StaticMeshSample::Reset()
{
	m_MeshRot = { -6.3f,6.3f,3.2f };
	for (auto&it : m_vpMesh)
	{
		it->transform->SetPosition({ 0,-1.80000019f,-3.49999881f });
	}
}

void StaticMeshSample::SetupMaterial()
{
	for (size_t i = 0; i < m_vpMesh.size(); i++)
	{
		m_vpMesh[i]->SetupMaterial(m_vpMaterial[i].get());
	}
}

void StaticMeshSample::Initialize()
{
	m_pShader = std::make_shared<D3D11::Graphic::MeshShader>();
	m_pShader->Setup();
	m_vpMesh = ResourceLoader::GetInstance().GetKouhaiMesh();
	m_vpMaterial = ResourceLoader::GetInstance().GetKouhaiMaterial();

	for (auto&it : m_vpMesh) { it->SetupShader(m_pShader.get()); }
	SetupMaterial();
	Reset();
	automatic = true;
}

void StaticMeshSample::Finalize()
{

}

void StaticMeshSample::Update()
{

	SetWindowText(GetActiveWindow(), const_cast<LPTSTR>(To_TString("静的メッシュの描画サンプル").c_str()));

	using kb = Keyboard;
	if (automatic)
	{
		if (m_MeshRot.z <= -3.1f) { m_MeshRot.z = 3.2f; }
		m_MeshRot.z -= 0.01f;
		for (auto&it : m_vpMesh)
		{
			it->transform->SetRotation(m_MeshRot);
		}
	}
#pragma region メッシュ
#if 1
	else {

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
		for (auto it : m_vpMesh)
		{
			auto&meshTrans = it->transform;
			auto meshPos = meshTrans->GetPosition();
			if (kb::GetButton(kb::c_Shift)) {}
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
	}
#endif // 1
	if (kb::GetButtonDown('x')) { automatic = !automatic; }
	if (kb::GetButtonDown('n')) { SceneRoot::GetInstance().SetupNextScene(std::make_unique<BillboardSample>()); }
	if (kb::GetButtonDown('m')) { SceneRoot::GetInstance().SetupNextScene(std::make_unique<AnimationSample>()); }
	if (kb::GetButtonDown('r')) { Reset(); }
}

void StaticMeshSample::Render()
{
	for (auto it : m_vpMesh)
	{
		it->Render();
	}
}