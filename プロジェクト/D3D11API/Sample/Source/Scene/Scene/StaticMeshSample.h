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
		@brief	初期化処理
	*/
	void IScene::Initialize();

	/*!
		@fn		Finalize
		@brief	破棄処理
	*/
	void IScene::Finalize();

	/*!
		@fn		Update
		@brief	更新処理
	*/
	void IScene::Update();

	/*!
		@fn		Render
		@brief	描画処理
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