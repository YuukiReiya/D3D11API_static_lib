#pragma once
#include "../Interface/IScene.h"
#include <Mesh.h>
#include <Material.h>

class StaticMeshSample
	:public IScene
{
public:
	StaticMeshSample();
	~StaticMeshSample();

	/*!
		@fn		Initialize
		@brief	‰Šú‰»ˆ—
	*/
	void IScene::Initialize();

	/*!
		@fn		Finalize
		@brief	”jŠüˆ—
	*/
	void IScene::Finalize();

	/*!
		@fn		Update
		@brief	XVˆ—
	*/
	void IScene::Update();

	/*!
		@fn		Render
		@brief	•`‰æˆ—
	*/
	void IScene::Render();

	void Reset();
	void SetupMaterial();
private:
	DirectX::XMFLOAT3 m_MeshRot;
	std::vector<std::shared_ptr<API::Mesh>>m_vpMesh;
	std::vector<std::shared_ptr<API::Material>>m_vpMaterial;
	std::shared_ptr<D3D11::Graphic::AbstractShader>m_pShader;
	bool automatic;
};